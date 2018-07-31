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
#ifndef __MTK_ATMOSPHERE_H__
#define __MTK_ATMOSPHERE_H__
#include  "../common/graphics/color.h"
#include  "mtk_draw_struct.h"



struct  atmosphere
{
	double R;		// Radius (in meter)
	double H;		// Scale height (in meter)
	double n0;		// Number density of scatterer at surface
	double dp;		// �`�悷���C�̌���(in H)
	double I0;		// Intensity
};




// Object
void	add_atmosphere(int plnt_idx, const atmosphere& atm);
bool	has_atmosphere(int plnt_idx);
const atmosphere&	get_atmosphere(int plnt_idx);





// Draw
void	draw_atmosphere(const atmosphere& atm, const double& zoom_rate, bool bMultipleScattering);
void	draw_planet_highlight(const mtk_draw_struct& ds, int pl_idx, const RGBf col);


//
RGBf	calc_scattered_light_intensity(float R1, float R2, float n0, float H, float I0,
								const vector3f& r0, const vector3f& ev, const vector3f& lv,
								const int imax);


void	init_multiple_scattering(const char* filename);
void	free_multiple_scattering_work();
RGBf	calc_multiple_scattered_light_intensity(const vector3d& r0, const vector3d& ev, const vector3d& lv);



RGBf	calc_attenuated_light_intensity(float R1, float R2, float n0, float H, float I0,
								const vector3f& r0, const vector3f& lv, const int imax);


vector3d	calc_highlight_pos(const vector3d& re, const vector3d& rp, const double& R, const vector3d& rs);

RGBf		calc_intensity_of_highlight(const vector3d& re, const vector3d& rp, const vector3d& rs, 
										const double& R, const double& R2_H, const double& H, const double& n0, const double& I0, int imax);


#ifndef XP_VERSION
void	init_atmosphere_threadpool();
void	cleanup_atmosphere_threadpool();
#endif


#endif
