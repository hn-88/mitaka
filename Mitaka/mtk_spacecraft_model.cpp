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
#include  "mtk_3D_model.h"
#include  "mtk_spacecraft_model.h"
#include  <windows.h>
#include  <stdio.h>
#include  "../common/graphics/gl_common.h"
#include  "../common/astro/astro_const.h"
#include  "mtk_spacecraft.h"
#include  "mtk_sun.h"




bool
spacecraft::load_trajectory(const char* filename, int Ntbl)
{
	return  trajectory.load(filename, Ntbl);
}


double
spacecraft::get_t1() const
{
	return  trajectory.get_t_begin();
}

double
spacecraft::get_t2() const
{
	return  trajectory.get_t_end();
}


// �w�莞���̈ʒu�𓾂�
vector3d
spacecraft::get_pos(const double& t)  const
{
	vector3d ro;

	switch (origin) {
	case ORIGIN_SUN:
		ro = get_sun_position();
		break;
	case ORIGIN_SS_BC:
	default:
		ro = vector3d(0, 0, 0);
		break;
	}

	return  trajectory.get_pos(t) + ro;
}



// �O���̕\��
void
spacecraft::draw_trajectory(const mtk_draw_struct& ds, const double& t, const vector3d& camera_pos, const RGBf& col, float af) const
{
	if (!bDrawTrajectory)  return;
	if (!trajectory.isLoaded()) return;


	int idx = 0;
	vector3d pos;

	int Nseg, Ndiv;
	Nseg = trajectory.get_segment_num();
	Ndiv = 10;


	vector3d ro;
	switch (origin) {
	case ORIGIN_SUN:
		ro = get_sun_position();
		break;
	case ORIGIN_SS_BC:
	default:
		ro = vector3d(0, 0, 0);
		break;
	}


	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glColor4f(col.r, col.g, col.b, af);
	
	const float lw = 1.5 * ds.line_width_fac / (ds.rad_per_pixel / 0.001);
	glLineWidth(lw);

	glBegin(GL_LINE_STRIP);


	double _t;
	for (int i = 0; i < Nseg; i++) {
		double t1, t2;

		const trajectory_segment &ts = trajectory.get_segment(i);
		t1 = ts.get_t_begin();
		t2 = ts.get_t_end();
		
		if (t < t1) break;

		
		double dt = (t2 - t1) / Ndiv;
		for (int j = 0; j < Ndiv; j++) {
			vector3d pos;

			_t = j * dt + t1;
			if (_t > t) {
				break;
			}
			pos = ts.get_pos(_t) - camera_pos + ro;

			glVertex3dv(pos.get_pointer());
		}
	}

	pos = get_pos(t) - camera_pos;
	glVertex3dv(pos.get_pointer());

	glEnd();
	glPopAttrib();
}




// ���f���̕\��
void
spacecraft::draw_model() const
{
	if ( ModelIdx < 0 )  return;

	glModel3D *pModel = get_3DModel(ModelIdx);
	if (pModel == 0) return;

	glPushMatrix();
		const float		scl = 3 * pModel->get_scale();
		const vector3f	r0  = pModel->get_r0();
		glScalef(scl,scl,scl);
		glTranslatef(r0.x, r0.y, r0.z);

		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
		pModel->draw();
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glPopMatrix();
}



//
void
spacecraft::set(const char *_key, int _ModelIdx, int org)
{
	key = _key;
	set_model(_ModelIdx);
	set_origin(org);
}

