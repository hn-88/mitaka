/**************************************************************************************************

Copyright (c) 2015   Tsunehiko Kato  and  4D2U Project, National Astronomical Observatory of Japan

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
#ifndef __MTK_GALACTIC_CENTER_H__
#define __MTK_GALACTIC_CENTER_H__
#include  "mtk_draw_common.h"
#include  "mtk_space_curve.h"
#include  "../common/win_file.h"



//---  background of galactic center
void		load_galactic_center_texture();
void		draw_galactic_center_background(const mtk_draw_struct& ds);



//---  S-stars
bool		load_S_stars(const directory& dir);

void		make_S_stars_orbit_curve(space_curve& sc, int div_N);

bool		is_S_star_selected(int idx);

void		update_S_stars_position(const double& t);
vector3d	get_S_star_position(int idx);

bool		is_S_star_label_visible(const mtk_draw_struct& ds, int idx, bool bTgt);
float		get_S_star_label_alpha(const mtk_draw_struct& ds, int idx);

float		get_S_star_alpha(const mtk_draw_struct& ds, int idx);
float		get_S_star_object_orbit_alpha(const mtk_draw_struct& ds, int idx);

double		get_scale_S_star(int idx);

void		prepare_draw_S_stars_2(const mtk_draw_struct &ds);
bool		prepare_draw_S_star(const mtk_draw_struct& ds, int idx, double* pRb, double* pRh);
void		draw_a_S_star(const mtk_draw_struct& ds, int idx, const vector3d& rpos, const double& scl, const double& Rb, float alp);

void		make_object_info_S_star(const mtk_draw_struct& ds, int idx, std::vector<std::string>& text);


#endif


