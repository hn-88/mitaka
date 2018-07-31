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
#ifndef __MTK_SATELLITES_H__
#define __MTK_SATELLITES_H__
#include  "../common/math/vector3.h"
#include  "../common/win_file.h"
#include  "mtk_satellite.h"
#include  "mtk_draw_struct.h"
#include  "mtk_space_curve.h"


//-------------
//  Constants
//-------------
const float R_moon_km = 1738.092;
const float SatelliteNameSizeDef = 2.5f;



//-------------
//  Functions
//-------------

//--  Scale  --
double	get_scale_satellite(int idx);
//--  Label  --
bool	is_satellite_label_visible(int idx, bool bTgt);
float	get_satellite_label_alpha(const mtk_draw_struct& ds, int idx);

//--
double	get_landing_radius_satellite(int idx);


//--  Load Textures  --
void	load_satellites_textures();


//--  Text size  --//
void	set_satellites_name_size(float sz);
float	get_satellites_name_size();



//--  Orbits  --
void	register_satellites_orbit_curves();
void	make_satellites_orbit_curve(space_curve& sc, double t, int div_N);
void	set_satellites_orbit_curve(space_curve& sc, double t);
float	get_satellite_orbit_alpha(const mtk_draw_struct& ds, int idx);



//--  Draw  --
bool	prepare_draw_satellite(const mtk_draw_struct& ds, int idx, double* pRb, double* pRh);
void	draw_a_satellite(const mtk_draw_struct& ds, int idx, const vector3d& rpos, const double& scl);
void	draw_moon_planetarium(const mtk_draw_struct& ds, const double& Rc, char atm_mode, float I0);
void	draw_moon_label_planetarium(const mtk_draw_struct& ds, const double& Rc);


//--  Hit-test
bool	hit_test_satellite(const mtk_draw_struct& ds, int idx, const vector3d& rpos, const vector3d& nv);

//--  Info
void	make_object_info_satellite(const mtk_draw_struct& ds, int idx, std::vector<std::string>& text);


//****
void	draw_satellites_planetarium(const mtk_draw_struct& ds, const double& Rc_pl);




//-------------
//  Functions
//-------------


//--  Number
int			get_satellites_num();

//--  Satellite
int			get_satellite_index(const std::string& key);
satellite&	get_satellite(int idx);



//---  Planet ID
int		get_satellite_planet_index(int idx);


//--  key
const std::string&	get_satellite_key(int idx);





//--  Size
void		get_satellite_size(int mode, int idx, double& Rx, double& Ry, double& Rz);


//--  Places and topography
void	init_satellite_places_and_topography();


//--  Position
vector3d	get_satellite_position(int idx);
void		set_satellite_position(int idx, const vector3d& pos);

vector3d	calc_satellite_orbit_pos(int idx, const double& t, float phi);
vector3d	get_planetocentric_satellite_position(int idx);
vector3d	calc_lunar_real_position(const double& t);


//--  Zoom rate
void	set_satellites_zoom_rate(int mode);



//--  Load
bool	load_satellites(const directory& dir);


//--  Selection
bool	is_satellite_selected(int idx);

#endif
