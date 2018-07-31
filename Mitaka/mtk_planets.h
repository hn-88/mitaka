/**************************************************************************************************

Copyright (c) 2007-2015   Tsunehiko Kato  and  4D2U Project, National Astronomical Observatory of Japan

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
#ifndef __MTK_PLANETS_H__
#define __MTK_PLANETS_H__
#include  "../common/win_file.h"
#include  "mtk_planet.h"
#include  "mtk_places.h"
#include  "mtk_draw_struct.h"
#include  "../common/graphics/color.h"
#include  "mtk_space_curve.h"
#include  <vector>
#include  <string>



//-------------
//  Functions
//-------------


planet&	get_planet(int idx);

//--  Register  --
void	register_object_planets();
//--  Radius
double	get_planet_Re(int mode, int idx);
//--  Position  --
//--  Scale  --
double	get_scale_planet(int idx);
//--  Prepare draw
bool	prepare_draw_planet(const mtk_draw_struct& ds, int idx, double* pRb, double* pRh);
//--  Draw  --
void	draw_a_planet(const mtk_draw_struct& ds, int idx, const vector3d& rpos, const double& scl);
//--  Label  --
bool	is_planet_label_visible(int idx);

//--  Landing radius
double	get_landing_radius_planet(int idx);

//--  Atmospehre
void	init_atmospehre();
//--  Places and topography
void	init_planet_places_and_topography();



//--  Orbit  --
void	register_planets_orbit_curves();
void	make_planets_orbit_curve(space_curve& sc, int div_N);
float	get_planet_label_alpha(const mtk_draw_struct& ds, int idx);
float	get_planet_orbit_alpha(const mtk_draw_struct& ds, int idx);

double	calc_visible_radius_planet(int idx);

void	draw_planet_axis(const float Rz);

void	set_topo_zoom_mode(char mode);
char	get_topo_zoom_mode();

void	load_planets_textures();


void	set_planet_light(int idx);

void	draw_planet2(int idx);

bool	hit_test_planet(const mtk_draw_struct& ds, int idx, const vector3d& rpos, const vector3d& nv);

void	make_object_info_planet(const mtk_draw_struct& ds, int idx, std::vector<std::string>& text);


//*******
void	draw_planets_planetarium(const mtk_draw_struct& ds);


#endif
