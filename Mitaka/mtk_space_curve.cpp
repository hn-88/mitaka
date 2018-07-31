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
#include  "mtk_space_curve.h"
#include  "../common/astro/astro_const.h"
#include  "mtk_colors.h"
#include  "mtk_object_position.h"
#include  "mtk_planets.h"
#include  "mtk_satellites.h"
#include  "mtk_minor_planets.h"
#include  "mtk_galactic_center.h"
#include  "mtk_our_galaxy.h"
#include  "mtk_target.h"
#include  "mtk_scale_line.h"
#include  "mtk_thread.h"
#include  <vector>

#include  "mtk_switches.h"





static  std::vector<space_curve>  vSpaceCurves;
static CS cs;	// Grobal only




//------------
//  Register
//------------
int
register_space_curve(const space_curve&  sc)
{
	vSpaceCurves.push_back(sc);

	return  vSpaceCurves.size() - 1;
}

//--------------
//  Initialize
//--------------
void
init_space_curves(const double& t)
{
	for (std::vector<space_curve>::iterator it=vSpaceCurves.begin(); it!=vSpaceCurves.end(); it++) {
		const mtk_object&  obj = it->get_related_object();
		switch (obj.get_type()) {
			case OBJ_TYPE_PLANET:			make_planets_orbit_curve(*it, 2000);		break;
			case OBJ_TYPE_SATELLITE:		make_satellites_orbit_curve(*it, t, 200);	break;
			case OBJ_TYPE_SMALL_OBJECT:		make_small_objects_orbit_curve(*it, 200);	break;
			case OBJ_TYPE_S_STAR:			make_S_stars_orbit_curve(*it, 1000);		break;
			case OBJ_TYPE_SCALE_CIRCLE:		make_scale_circle_curve(*it, 200, false);	break;
			case OBJ_TYPE_SCALE_CIRCLE_G:	make_scale_circle_curve(*it, 200, true);	break;
			default:	break;
		}
	}
}

//----------
//  Update
//----------
void
update_space_curves(const double& t)
{
	cs.Enter();

	for (std::vector<space_curve>::iterator it=vSpaceCurves.begin(); it!=vSpaceCurves.end(); it++) {
		const mtk_object&  obj = it->get_related_object();
		switch (obj.get_type()) {
			case OBJ_TYPE_PLANET:		make_planets_orbit_curve(*it, 2000);	break;	// �O���v�f�̓A�b�v�f�[�g����Ă���Ɖ���
			case OBJ_TYPE_SATELLITE:	set_satellites_orbit_curve(*it, t);		break;
			default:	break;
		}
	}

	cs.Leave();
}


//--------
//  Draw
//--------
void
draw_a_space_curve(const mtk_draw_struct& ds, const space_curve& sc)
{
	const vector3d camera_pos	= ds.camera.get_pos();
	const float scale			= ds.scale;

	//--  �����x�̐ݒ�
	const mtk_object&  obj = sc.get_related_object();
	float lv = get_orbit_alpha( ds, obj );
	if (lv < 0.05f)  return;

	float af = lv;

	//--  �F
	const RGBf& col = get_color( sc.get_color_no() );
	glColor4f(col.r, col.g, col.b ,af);

	//--  ���̑���
	const float lw = sc.get_line_width() * ds.line_width_fac / (ds.rad_per_pixel/0.0015);
	glLineWidth(lw);



	//--  �`��
	glBegin(GL_LINE_STRIP);		//*** GL_LINE_LOOP �͎g��Ȃ�
	const int N = sc.get_num_points();
	vector3d pos;
	for (int i=0; i<N; i++) {
		pos = sc.get_point(i) - camera_pos;
		glVertex3dv( pos.get_pointer() );
	}
	pos = sc.get_point(0) - camera_pos;
	glVertex3dv( pos.get_pointer() );
	glEnd();
}

void
draw_space_curves(const mtk_draw_struct& ds)
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);

	for (std::vector<space_curve>::const_iterator  it=vSpaceCurves.begin(); it!=vSpaceCurves.end(); it++) {
		const mtk_object&  obj = it->get_related_object();

		if ( !is_object_orbit_visible(obj) )  continue;


		draw_a_space_curve( ds, *it );
	}
}
