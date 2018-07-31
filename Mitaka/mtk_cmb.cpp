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
#include  "mtk_cmb.h"
#include  "mtk_OpenGL.h"
#include  "mtk_switches.h"
#include  "mtk_dialog_box.h"
#include  "mtk_our_galaxy_rt.h"




static GLuint texCMB;
const char *fn_tex_CMB = "CMB.jpg";



const float CMB_R = 138.0e8 * _1ly_pc;
static float CMB_R1 = 0.85 * CMB_R;
static float CMB_R2 = 1.15 * CMB_R;
static float CMB_Alpha = 0.4;



void
load_CMB_texture()
{
	SetSplashText("LOADING_CMB");
	texCMB = LoadTexture(fn_tex_CMB, TexShrinkRate, false);
}


void
set_CMB_Alpha(const float alp)
{
	CMB_Alpha = alp;
}


void
set_CMB_fade_scales(const float _R1, const float _R2)
{
	CMB_R1 = _R1 * CMB_R;
	CMB_R2 = _R2 * CMB_R;
}


void
draw_CMB_map(const mtk_draw_struct& ds, bool bFarSide)
{
	if (!bCMB) return;


	const float dist = norm(ds.camera_base.get_pos());
	float af, af1, af2;
	af = CMB_Alpha;

	if (dist < CMB_R2) {
		float t = (dist - CMB_R1) / (CMB_R2 - CMB_R1);
		af *= t;
	}
	if (af <= 0.0) return;


	af1 = af;
	af2 = af*0.5;


	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, texCMB);
	glEnable(GL_CULL_FACE);


	vector3f rpos = -ds.camera.get_pos();


	glPushMatrix();

		glTranslatef(rpos.x, rpos.y, rpos.z);

		go_to_galactic_coord();

		glScalef(CMB_R, CMB_R, CMB_R);
	
		glRotatef(180, 0, 0, 1);
		if (bFarSide) {
			glColor4f(1, 1, 1, af1);
			glFrontFace(GL_CW);
			gl_draw_sphere(5, true);
		}
		else {
			glColor4f(1, 1, 1, af2);
			glFrontFace(GL_CCW);
			gl_draw_sphere(5, true);
		}
		glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
