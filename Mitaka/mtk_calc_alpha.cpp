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
#include  <windows.h>
#include  "mtk_calc_alpha.h"
#include  "mtk_screen.h"


float
calc_alpha_by_disapear_scale_inside(float current_scale, float disapear_scale)
{
	const float d_in  = 1.0;
	const float d_out = 5.0;
	const float d = current_scale / disapear_scale;
	if (d < d_in)	return 0.0f;
	if (d > d_out)	return 1.0f;

	float t = (d_out - d) / (d_out - d_in);
	return  (1.0f - t);
}


float
calc_alpha_by_disapear_scale_outside(float current_scale, float disapear_scale)
{
	const float d_in  = 0.2;
	const float d_out = 1.0;
	const float d = current_scale / disapear_scale;
	if (d > d_out)	return 0.0f;
	if (d < d_in)	return 1.0f;

	float t = (d - d_in) / (d_out - d_in);
	return  (1.0f - t);
}



float
calc_alpha_by_apparent_diameter(float Dth)
{
	float thc = 7.0f * D2R * get_fade_angle_factor();

	if (Dth >= thc)  return  1.0f;

	float lv = Dth / thc;
	return  lv*lv*lv*lv;
}


//---  Old
float
calc_alpha(const float r1, const float r2, const float r, const float fac)
{
	if (r<r1 || r>r2)  return 0;

    const float dr1= fac*0.02f;
	const float dr2= fac*0.3f;
	float alp=1;
	if (r<r1+dr1) { alp = fabs(r-r1)/dr1; }
	if (r>r2-dr2) { alp = fabs(r-r2)/dr2; }
	alp *= 0.9f;

	return alp;
}
