/**************************************************************************************************

Copyright (c) 2012   Tsunehiko Kato
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
#include  "mtk_scale_line.h"
#include  "../common/astro/astro_const.h"
#include  "../common/astro/astrometry.h"
#include  "mitaka.h"
#include  "mtk_switches.h"
#include  "mtk_gl_common.h"
#include  "mtk_colors.h"
#include  "mtk_font.h"
#include  "mtk_label.h"
#include  "mtk_calc_alpha.h"
#include  "mtk_objects.h"
#include  "mtk_our_galaxy.h"
#include  "mtk_target.h"




//*****
float	calc_alpha(const float r1, const float r2, const float r, const float fac);

float ScaleTextSize, ScaleWidth;



//----------------------
//  �X�P�[�� (�P�ʂ� pc)
//----------------------
struct  tagScale {
	double		divs;			// �X�P�[���������l
	const char* string_key;		//
	char		plane;			// �~����������
	float		angle;			// ���x���������ʒu
};

enum {
	SCALE_NO_CIRCLE,		// �~��`���Ȃ�
	SCALE_ON_GALACTIC,		// ��͖ʏ�
	SCALE_ON_ECLIPTIC		// �����ʏ�
};


const int scale_N = 21;
const tagScale dist_scale[scale_N] = {
	{1 * _1AU_pc, "SCL_1AU", SCALE_ON_ECLIPTIC, 230},
	{10 * _1AU_pc, "SCL_10AU", SCALE_ON_ECLIPTIC, 45},
	{100 * _1AU_pc, "SCL_100AU", SCALE_ON_ECLIPTIC, 45},
	{1000 * _1AU_pc, "SCL_1KAU", SCALE_ON_ECLIPTIC, 45},
	{10000 * _1AU_pc, "SCL_10KAU", SCALE_ON_ECLIPTIC, 45},
	{1 * _1ly_pc, "SCL_1LY", SCALE_ON_GALACTIC, 45},
	{3 * _1ly_pc, "SCL_3LY", SCALE_ON_GALACTIC, 45},
	{10 * _1ly_pc, "SCL_10LY", SCALE_ON_GALACTIC, 45},
	{30 * _1ly_pc, "SCL_30LY", SCALE_ON_GALACTIC, 45},
	{100 * _1ly_pc, "SCL_100LY", SCALE_ON_GALACTIC, 45},
	{300 * _1ly_pc, "SCL_300LY", SCALE_ON_GALACTIC, 45},
	{1000 * _1ly_pc, "SCL_1KLY", SCALE_ON_GALACTIC, 45},
	{3000 * _1ly_pc, "SCL_3KLY", SCALE_ON_GALACTIC, 45},
	{1e4*_1ly_pc, "SCL_10KLY", SCALE_ON_GALACTIC, 45},
	{1e5*_1ly_pc, "SCL_100KLY", SCALE_ON_GALACTIC, 45},
	{1e6*_1ly_pc, "SCL_1MLY", SCALE_ON_GALACTIC, 45},
	{1e7*_1ly_pc, "SCL_10MLY", SCALE_ON_GALACTIC, 45},
	{1e8*_1ly_pc, "SCL_100MLY", SCALE_ON_GALACTIC, 45},
	{1e9*_1ly_pc, "SCL_1GLY", SCALE_ON_GALACTIC, 45},
	{1e10*_1ly_pc, "SCL_10GLY", SCALE_ON_GALACTIC, 45},
	{1.38e10*_1ly_pc, "SCL_13GLY", SCALE_ON_GALACTIC, 300}
};






//----------------
//  Scale circle
//----------------
void
register_scale_circle_curves_0(const char *key0, int obj_type)
{
	for (int i = 0; i<scale_N; i++) {
		char key[256];

		//--  Objects
		sprintf(key, "%s%d", key0, i);
		int obj_idx = register_object(mtk_object(obj_type, i, 0.0f, false), key);

		//--  Labels
		const tagScale&  scl = dist_scale[i];
		float R = 0;
		float thc = 10 * D2R;
		float tscl = ScaleTextSize;

		add_label(scl.string_key, obj_idx, MTK_COLOR_SCALE_CIRCLE_TEXT, 1.0f, tscl, R, thc, false);

		//--  Space curves
		space_curve  sc(mtk_object(obj_type, i, 0.0f, false), ScaleWidth, MTK_COLOR_SCALE_CIRCLE);
		register_space_curve(sc);
	}
}

void
register_scale_circle_curves()
{
	register_scale_circle_curves_0("ScaleLine", OBJ_TYPE_SCALE_CIRCLE);
	register_scale_circle_curves_0("ScaleLineG", OBJ_TYPE_SCALE_CIRCLE_G);
}



void
make_scale_circle_curve(space_curve& sc, int div_N, bool bGalacticCenter)
{
	const int idx = sc.get_related_object().get_index();
	const double r   = dist_scale[idx].divs;
	const char plane = dist_scale[idx].plane;

	sc.clear();

	if (!bGalacticCenter) {
		for (int j = 0; j < div_N; j++) {
			double phi = 2.0 * M_PI * double(j) / div_N;
			vector3d  pos = r * vector3d(cos(phi), sin(phi), 0);
			if (plane == SCALE_ON_GALACTIC)  {
				pos = M_G2E * pos;
			}
			sc.add_point(pos);
		}
	}
	else {
		const vector3d pos0 = get_our_galaxy_position();
		for (int j = 0; j < div_N; j++) {
			double phi = 2.0 * M_PI * double(j) / div_N;
			vector3d  pos = r * vector3d(cos(phi), sin(phi), 0);
			pos = M_G2E * pos + pos0;
			sc.add_point(pos);
		}
	}
}

float
get_scale_circle_alpha(const mtk_draw_struct& ds, int idx)
{
	double scale = ds.scale;

	const mtk_object& tgt = get_target();
	const int type = tgt.get_type();
	if (type == OBJ_TYPE_OUR_GALAXY || type == OBJ_TYPE_S_STAR || type == OBJ_TYPE_BLACKHOLE) {
		return 0;
	}

	float dscl = dist_scale[idx].divs;

	float alp = calc_alpha_by_disapear_scale_inside( scale, dscl );
	alp *= calc_alpha_by_disapear_scale_outside( scale, 70*dscl );

	return  alp;
}

float
get_scale_circle_g_alpha(const mtk_draw_struct& ds, int idx)
{
	double scale = ds.scale;

	const mtk_object& tgt = get_target();
	const int type = tgt.get_type();
	if (type == OBJ_TYPE_OUR_GALAXY || type == OBJ_TYPE_S_STAR || type == OBJ_TYPE_BLACKHOLE) {
	}
	else {
		return 0;
	}

	float dscl = dist_scale[idx].divs;

	float alp = calc_alpha_by_disapear_scale_inside(scale, dscl);
	alp *= calc_alpha_by_disapear_scale_outside(scale, 70 * dscl);

	return  alp;
}



//------------
//  Position
//------------
vector3d
get_scale_circle_position(int idx)
{
	const tagScale&  dsc = dist_scale[idx];

	double phi = D2R * dsc.angle;
	vector3d  pos = dsc.divs * vector3d(cos(phi), sin(phi), 0);
	if (dsc.plane == SCALE_ON_GALACTIC)  {
		pos = M_G2E * pos;
	}
	return  pos;
}

vector3d
get_scale_circle_g_position(int idx)
{
	const tagScale&  dsc = dist_scale[idx];
	const vector3d pos0 = get_our_galaxy_position();

	double phi = D2R * dsc.angle;
	vector3d  pos = dsc.divs * vector3d(cos(phi), sin(phi), 0);
	pos = M_G2E * pos + pos0;
	return  pos;
}







//---------
//  Label
//---------
bool
is_scale_circle_label_visible(int idx)
{
	if (swScaleLine & SCALE_SQUARE && idx == 20) {
		return  true;
	}
	return  swScaleLine & SCALE_CIRCLE;
}



//---------------------
//  �X�P�[���̎l�p������
//---------------------
void
draw_scale_square(const mtk_draw_struct& ds)
{
	const vector3d target_pos = ds.tgt_pos;
	const vector3d camera_pos = ds.camera.get_pos();
	const vector3d dr = target_pos - camera_pos;
	const float scale = ds.scale;
	const float lw = ScaleWidth * ds.line_width_fac / (ds.rad_per_pixel / 0.001);

	glLineWidth(lw);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();
		glTranslatef(dr.x, dr.y, dr.z);

		toward_eye_pos();
		const float sc = ds.scale_factor;
		glScalef(sc, sc, sc);

		for (int i=0; i<28; ++i) {
			const float scl = pow(10, double(i)) / _1pc_meter;
			const float d   = 0.5 * scl;

			// �����x
			float l = scl / norm(dr);
			float alp = calc_alpha(0.01, 5.0, l, 2);
			if (alp <= 0.0f)  continue;

			// �l�p���g������
			const RGBf& col = get_color(MTK_COLOR_SCALE_SQUARE);
			glColor4f(col.r, col.g, col.b, alp);

			glBegin(GL_LINE_LOOP);
				glVertex3f(-d, -d, 0);
				glVertex3f( d, -d, 0);
				glVertex3f( d,  d, 0);
				glVertex3f(-d,  d, 0);
			glEnd();

			// ���x��������
			char str[20];
			if ( test_disp_switch(DISPSW_PLANET, DISPM_NAME) ) {
				glPushMatrix();
					// ���炷
					glTranslatef(d, d, 0);
					prepare_3Dtext_1(0, 45, ScaleTextSize*ds.ltf, ds.scale_factor, true);	// �e�L�X�g�̖ʂ̌����ƃT�C�Y��ݒ肷��

					const RGBf& col = get_color(MTK_COLOR_SCALE_SQUARE_TEXT);
					glColor4f(col.r, col.g, col.b, alp);
					draw_text("10");
					
					// �w��
					glScalef(0.5, 0.5, 0.5);
					glTranslatef(0,0.6,0);
					sprintf(str, "%d", i);
					draw_text(str);

					glTranslatef(0.0, -0.6, 0);
					glScalef(2,2,2);
					draw_text("m");

				glPopMatrix();
			}
		}
	glPopMatrix();
}
