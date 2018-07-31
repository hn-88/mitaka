/**************************************************************************************************

Copyright (c) 2007   Tsunehiko Kato  and  4D2U Project, National Astronomical Observatory of Japan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

�ȉ��ɒ�߂�����ɏ]���A�{�\�t�g�E�F�A����ъ֘A�����̃t�@�C���i�ȉ��u�\�t�g�E�F�A�v�j�̕������擾��
�邷�ׂĂ̐l�ɑ΂��A�\�t�g�E�F�A�𖳐����Ɉ������Ƃ𖳏��ŋ����܂��B����ɂ́A�\�t�g�E�F�A�̕�����
�g�p�A���ʁA�ύX�A�����A�f�ځA�Еz�A�T�u���C�Z���X�A�����/�܂��͔̔����錠���A����у\�t�g�E�F�A��
�񋟂��鑊��ɓ������Ƃ������錠�����������Ɋ܂܂�܂��B

��L�̒��쌠�\������і{�����\�����A�\�t�g�E�F�A�̂��ׂĂ̕����܂��͏d�v�ȕ����ɋL�ڂ�����̂Ƃ���
���B

�\�t�g�E�F�A�́u����̂܂܁v�ŁA�����ł��邩�Öقł��邩���킸�A����̕ۏ؂��Ȃ��񋟂���܂��B����
�ł����ۏ؂Ƃ́A���i���A����̖ړI�ւ̓K�����A����ь�����N�Q�ɂ��Ă̕ۏ؂��܂݂܂����A����Ɍ���
�������̂ł͂���܂���B ��҂܂��͒��쌠�҂́A�_��s�ׁA�s�@�s�ׁA�܂��͂���ȊO�ł��낤�ƁA�\�t
�g�E�F�A�ɋN���܂��͊֘A���A���邢�̓\�t�g�E�F�A�̎g�p�܂��͂��̑��̈����ɂ���Đ������؂̐����A��
�Q�A���̑��̋`���ɂ��ĉ���̐ӔC������Ȃ����̂Ƃ��܂��B
***************************************************************************************************/
#include  "stars.h"


//----------------------------
//  ���̃e�N�X�`���ƐF�̃e�[�u��
//----------------------------
star_picture::star_picture(float TempMin, float TempMax, int TempN, int LumN)
{
	T_N   = TempN;
	T_min = TempMin;
	T_max = TempMax;
	Lum_N = LumN;

	//  �x�N�^�̊m��
	Temp.resize(T_N);
	TempBin.resize(T_N-1);
	vvCol.resize(T_N);

	//  ���x�e�[�u���̍쐬
	const double logT_min = log10(T_min);
	const double logT_max = log10(T_max);
	const double dlogT    = (logT_max - logT_min)/(T_N-1);

	for (int i=0; i<T_N; ++i) {
		const double logT = logT_min + i*dlogT;
		Temp[i]    = pow(10.0, logT);
		if (i<T_N-1)	TempBin[i] = pow(10.0, logT + 0.5*dlogT);
	}

	//  �F�e�[�u���̍쐬
	for (int i=0; i<T_N; ++i) {
		RGBf rgb0 = blackbody_color( Temp[i] );
		rgb0 /= min_level(rgb0);					// ��ԃ��x�����Ⴂ�����ŋK�i������
		vector<RGB2>&  vCol = vvCol[i];
		vCol.resize(Lum_N+1);
		for (int j=0; j<=Lum_N; ++j) {
			const float lv = float(j)/Lum_N;
			RGBf rgb = lv * rgb0;
			clamp(rgb);
			vCol[j].ncr	= rgb;
			vCol[j].cr	= correct_RGB( rgb );
		}
	}
}


void
star_picture::create_texture_image(image& img, int img_size, const float& SizeIdx, const float& ShapeIdx)
{
	const int sz(img_size);
	img.alloc(sz*T_N, sz, 4);

	for (int i=0; i<T_N; ++i) {
		const int ix0 = i * sz;
		RGBf rgb = blackbody_color( Temp[i] );

		//  �e�N�X�`���̍쐬
		double fR, fB, fG;
		rgb /= max_level(rgb);
		fR = pow(rgb.r, SizeIdx);		// �␳�Ȃ��̏ꍇ SizeIdx=0.5
		fG = pow(rgb.g, SizeIdx);
		fB = pow(rgb.b, SizeIdx);
		const float r_star2 = 0.50;
		float r0 = sz/2.0;
		float r2 = r0 * r_star2;
		for (int iy=0; iy<sz; ++iy) {
			for (int ix=0; ix<sz; ++ix) {
				float x, y, r;
				x = double(ix) - double(sz)/2.0;
				y = double(iy) - double(sz)/2.0;
				r = sqrt(x*x+y*y);

				double R,G,B,A, rr0, rr2;
				float val;
				//-- Red
				rr0 = r0*fR;
				rr2 = r2*fR;
				if (r > rr2) {
					val = 1.0 - (r-rr2)/(rr0-rr2);
					if (val<0)	{ R = 0; }
					else		{ R = correct_gamma(pow(val, ShapeIdx)); }
				} else {
					R = 1.0;
				}
				//-- Green
				rr0 = r0*fG;
				rr2 = r2*fG;
				if (r > rr2) {
					val = 1.0 - (r-rr2)/(rr0-rr2);
					if (val<0)	{ G = 0; }
					else		{ G = correct_gamma(pow(val, ShapeIdx)); }
				} else {
					G = 1.0;
				}
				//-- Blue
				rr0 = r0*fB;
				rr2 = r2*fB;
				if (r > rr2) {
					val = 1.0 - (r-rr2)/(rr0-rr2);
					if (val<0)	{ B = 0; }
					else		{ B = correct_gamma(pow(val, ShapeIdx)); }
				} else {
					B = 1.0;
				}
				//-- Alpha
				A = R+G+B;
				if (A > 1.0)  A=1.0;
				//-- Set pixel
				img.setRGBAf(ix0+ix,iy, R,G,B,A);
			}
		}
	}
}


int
star_picture::get_TempIdx(float Tc) const
{
	int idx = 0;
	for (int i=0; i<T_N; ++i) {
		if (TempBin[i] > Tc) break;
		idx++;
	}
	return  idx;
}


const RGBf&
star_picture::get_correct_RGB(int T_Idx, float lv) const
{
	if (lv > 1.0f) lv=1.0f;
	int LumIdx = int(lv*Lum_N);
	return  vvCol[T_Idx][LumIdx].cr;
}

int
star_picture::get_Lum_rank(float lv) const
{
	if (lv > 1.0f) lv=1.0f;
	if (lv < 0.0f) lv=0.0f;
	return  int(lv*100);
}


//--------------
//  class star
//--------------
void
star::set_Tc(float _Tc)
{
	Tc = _Tc;
	RGBf rgb = blackbody_color(Tc);
	highest_comp = max_level(rgb);
	lowest_comp  = min_level(rgb);
}


void
star::make_picture_cache(float lv, const star_picture& sp)
{
	// ���x�̃C���f�b�N�X
	sdc.TempIdx = sp.get_TempIdx(Tc);
	sdc.LumRank = sp.get_Lum_rank(lv);
	sdc.RGBcr   = sp.get_correct_RGB(sdc.TempIdx, lv);
    // ���̃T�C�Y
	const float sz_max = 50;
	float sz, hcm;
	hcm = highest_comp * lv;
	sz = (hcm > 1.0)  ?  sqrt(hcm)  :  1.0;
	if (sz > sz_max) sz = sz_max;
	sdc.sz = sz;
    sdc.bPoint = (sz <= 1.0);
}

