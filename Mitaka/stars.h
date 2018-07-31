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
#ifndef  __STARS_H__
#define  __STARS_H__
#include  "../common/graphics/color.h"
#include  "../common/graphics/image.h"
#include  "../common/astro/astrometry.h"


struct RGB2 {
	RGBf  ncr, cr;
};


//----------------------
//  class star_picture
//----------------------
class star_picture
{
	int		T_N, Lum_N;
	float	T_min, T_max;
	vector<float>	Temp, TempBin;
	vector<vector<RGB2> >	vvCol;
public:
	star_picture(float TempMin, float TempMax, int TempN, int LumN);
	int		get_TempN() const				{ return T_N; }
	int		get_TempIdx(float Tc) const;
	void	create_texture_image(image& img, int img_size, const float& SizeIdx, const float& ShapeIdx);
	const RGBf&		get_correct_RGB(int T_idx, float lv) const;
	int				get_Lum_rank(float lv) const;
};


//-------------------
//  star draw cache
//-------------------
struct star_draw_cache {
	int			TempIdx;		// ���x�̃C���f�b�N�X
	int			LumRank;		// ���邳�̃����N
	RGBf		RGBcr;			// �K���}�␳�����F
	float		sz;				// �\���T�C�Y
	bool		bPoint;			// �_�ŕ\�����邩�ǂ���
};


//--------------
//  class star
//--------------
class star
{
	float		Tc;
	star_draw_cache		sdc;
	float		highest_comp, lowest_comp;		// �P�x���ő�̐����ƍŏ��̐��� (0-1)
public:
	void	set_Tc(const float _Tc);
	float	get_Tc() const						{ return Tc; }
	void	set_B_V(const float B_V)			{ set_Tc( B_V_to_Tc(B_V) ); }
	void	make_picture_cache(float lv, const star_picture& sp);
	const star_draw_cache&	get_picture_cache() const	{ return sdc; }
};



#endif