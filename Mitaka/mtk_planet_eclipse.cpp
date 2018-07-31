/**************************************************************************************************

Copyright (c) 2007-2016   Tsunehiko Kato  and  4D2U Project, National Astronomical Observatory of Japan

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
#include  "mtk_planet_eclipse.h"
#include  "mitaka.h"
#include  "mtk_mode.h"
#include  "../common/astro/astro_const.h"
#include  "mtk_planet_position.h"
#include  "mtk_planet_orientation.h"
#include  "mtk_satellite_orientation.h"
#include  "mtk_sun.h"
#include  "mtk_eclipse.h"
#include  "mtk_planetarium.h"
#include  "mtk_switches.h"
#include  "mtk_objects.h"
#include  "mtk_planet.h"
#include  "mtk_planets.h"
#include  "mtk_satellites.h"



static float MoonEclipseR = 0.3;
static float MoonEclipseG = 0.0;

void
set_moon_eclipse_shadow_color(float R, float G)
{
	if (R < 0.0f) { R = 0.0f; }
	if (R > 1.0f) { R = 1.0f; }
	if (G < 0.0f) { G = 0.0f; }
	if (G > 1.0f) { G = 1.0f; }

	MoonEclipseR = R;
	MoonEclipseG = G;
}

vector3f
get_moon_eclipse_shadow_color()
{
	return  vector3f(MoonEclipseR, MoonEclipseG, 0);
}



//-----------
//  �e�̕`��
//-----------
void
draw_object_eclipse_sub(const mtk_draw_struct& ds, const vector3d& r1, const double& R1, const vector3d& r2, const double& R2, float f, const matrix3d& M, const vector3f& shd_col, const double& Rc_pl)
{
	double Rs;
	vector3d rs, ex, ey, ez;


	rs = get_sun_position();
	if (ds.tyMode == MODE_PLANETARIUM) {
		Rs = R_sun_km * _1km_pc;
	}
	else {
		Rs = get_sun_radius(ds.tyMode);
	}
	M.get_bases_tr(ex, ey, ez);

	float th_c = ds.rad_per_pixel;
	if (ds.tyMode == MODE_PLANETARIUM) {
		th_c /= get_planetarium_zooming_factor();
	}

	draw_eclipse_shadow(ds,
		r1, R1, ez, f,
		r2, R2, rs, Rs, M, th_c, 30, 30,
		(swEclipse & ECLIPSE_DISP), (swEclipse & ECLIPSE_BORDER),
		shd_col, Rc_pl);
}




//-------------
//  ���H���̉e
//-------------
void
draw_planet_eclipse(const mtk_draw_struct& ds, int plnt_idx, const double& Rc_pl)
{
	const planet& pnt = get_planet(plnt_idx);
	const std::vector<eclipse_obj_info>&  vEcl = pnt.get_eclipse_info();

	for (int i = 0; i < vEcl.size(); i++) {
		const eclipse_obj_info& ecl = vEcl[i];
		double R1, R2;
		vector3d r1, r2;

		double Rx, Ry, Rz;
		get_satellite_size(ds.tyMode, ecl.obj_idx, Rx, Ry, Rz);


		r1 = get_planet_position(plnt_idx);
		R1 = get_planet_Re(ds.tyMode, plnt_idx) * _1km_pc;
		r2 = get_satellite_position(ecl.obj_idx);
		R2 = Rx * _1km_pc;

		const matrix3d& M = get_planet_orientation(plnt_idx, true);
		draw_object_eclipse_sub(ds, r1, R1, r2, R2, pnt.get_f(), M, ecl.col, Rc_pl);
	}
}



//---------------
//  �q���H���̉e
//---------------
void
draw_satellite_eclipse(const mtk_draw_struct& ds, int sat_idx, const double& Rc_pl)
{
	const satellite& st = get_satellite(sat_idx);
	const int plnt_idx = st.get_planet_index();
	const std::vector<eclipse_obj_info>&  vEcl = st.get_eclipse_info();
	const int idx_Moon = get_satellite_index("SAT_E_MOON");



	for (int i = 0; i < vEcl.size(); i++) {
		const eclipse_obj_info& ecl = vEcl[i];
		const planet& pnt = get_planet(ecl.obj_idx);
		double R1, R2;
		double Rx, Ry, Rz;
		vector3d r1, r2;



		if (ds.tyMode == MODE_PLANETARIUM) {
			get_satellite_size(ds.tyMode, sat_idx, Rx, Ry, Rz);
			R2 = pnt.get_Re() * _1km_pc;
			if (sat_idx == idx_Moon) {
				r1 = calc_lunar_real_position(ds.JD);
			}
			else {
				r1 = get_satellite_position(sat_idx);
			}
		}
		else {
			get_satellite_size(ds.tyMode, sat_idx, Rx, Ry, Rz);
			R2 = get_planet_Re(ds.tyMode, plnt_idx)*_1km_pc;
			r1 = get_satellite_position(sat_idx);
		}
		R1 = Rx * _1km_pc;
		r2 = get_planet_position(plnt_idx);


		const matrix3d& M = get_satellite_orientation(sat_idx, true);
		draw_object_eclipse_sub(ds, r1, R1, r2, R2, 0, M, ecl.col, Rc_pl);
	}
}
