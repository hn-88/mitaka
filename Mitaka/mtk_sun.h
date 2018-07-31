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
#ifndef __MTK_SUN_H__
#define __MTK_SUN_H__
#include  "../common/math/vector3.h"
#include  "mtk_draw_struct.h"
#include  "mtk_colors.h"
#include  <vector>
#include  <string>




//-------------
//  Constants
//-------------
const float R_sun_km  = 696000;





//-------------
//  Functions
//-------------

//--  Register  --
void	register_object_sun();

//--  Position  --
bool		load_Sun_position(const char *filename);
void		update_Sun_position(const double& t);
vector3d	get_sun_position();


//--  Scale  --
double	get_scale_sun();

//--  Textures  --
void	load_sun_textures();


//--  radius  --
double	get_sun_radius(int mode);
double	get_sun_glare_radius(int mode);

double	get_landing_radius_sun();



//--  Label  --
bool	is_sun_label_visible();
float	get_sun_label_alpha(const mtk_draw_struct& ds);
float	get_sun_label_size(const mtk_draw_struct& ds);


//--  Glare  --
//void	set_sun_glare_size(float sz);
void	set_sun_glare_color(const RGBf& col);


//--  Draw  --
bool	prepare_draw_sun(const mtk_draw_struct& ds, double* pRb, double* pRh);
void	draw_sun0(const float& R, const RGBf& col);
void	draw_sun(const mtk_draw_struct& ds, int idx, const vector3d& rpos, const double& scl);
void	draw_sun_label(const mtk_draw_struct& ds, const RGBf& col, float af);

void	draw_sun_label_planetarium(const mtk_draw_struct& ds, const double& Rc);
void	draw_sun_planetarium_2(const mtk_draw_struct& ds, const double& Rc, const double& I0);




void	set_light_from_sun(const vector3d& pos);


//--  Info  --
void	make_object_info_sun(const mtk_draw_struct& ds, std::vector<std::string>& text);




#endif
