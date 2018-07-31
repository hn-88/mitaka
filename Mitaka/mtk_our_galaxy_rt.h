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
#ifndef __MTK_OUR_GALAXY_RT_H__
#define __MTK_OUR_GALAXY_RT_H__
#include  "../common/win_file.h"
#include  "../common/astro/astrometry.h"
#include  "mtk_draw_struct.h"
#include  "mtk_our_galaxy.h"



//
void	go_to_galactic_coord();


// �ǂݍ���
bool	load_our_galaxy_model(const directory& dir);


// �����̋��x��
const float kpG_kpR = 1.33;
const float kpB_kpR = 1.78;


//---  ��͌n�`��p�̃�����  ---
void	free_memory_for_galaxy();


//---  ��͌n�̕`��  ---
void	draw_our_galaxy_model_rt(const mtk_draw_struct& ds, const vector3d& rpos);


//---  r1 �� r2 �̊Ԃ� optical depth �����߂�ir1g, r2g �͋�͍��W�j  ---
float	calc_galactic_optical_depth_g(const vector3f& r1g, const vector3f& r2g, const float tau_max = 5);


//---  r1 �� r2 �̊Ԃ� optical depth �����߂�ir1, r2 �͉������W�n�j
inline float
calc_galactic_optical_depth(const vector3f& r1, const vector3f& r2, const float tau_max = 5)
{
	//--  ��͍��W�n�ł̈ʒu  --
	const vector3f rg(R_gal, 0, 0);
	vector3f r1g = M_E2G * r1 - rg;
	vector3f r2g = M_E2G * r2 - rg;

	return calc_galactic_optical_depth_g(r1g, r2g, tau_max);
}



#ifndef XP_VERSION
void	init_our_galaxy_threadpool();
void	cleanup_our_galaxy_threadpool();
#endif


#endif
