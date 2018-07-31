/******************************************************************************

Copyright (c) 2007   Tsunehiko Kato

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

�ȉ��ɒ�߂�����ɏ]���A�{�\�t�g�E�F�A����ъ֘A�����̃t�@�C���i�ȉ��u�\�t�g�E�F
�A�v�j�̕������擾���邷�ׂĂ̐l�ɑ΂��A�\�t�g�E�F�A�𖳐����Ɉ������Ƃ𖳏��ŋ�
���܂��B����ɂ́A�\�t�g�E�F�A�̕������g�p�A���ʁA�ύX�A�����A�f�ځA�Еz�A�T�u
���C�Z���X�A�����/�܂��͔̔����錠���A����у\�t�g�E�F�A��񋟂��鑊��ɓ�����
�Ƃ������錠�����������Ɋ܂܂�܂��B

��L�̒��쌠�\������і{�����\�����A�\�t�g�E�F�A�̂��ׂĂ̕����܂��͏d�v�ȕ�����
�L�ڂ�����̂Ƃ��܂��B

�\�t�g�E�F�A�́u����̂܂܁v�ŁA�����ł��邩�Öقł��邩���킸�A����̕ۏ؂���
���񋟂���܂��B�����ł����ۏ؂Ƃ́A���i���A����̖ړI�ւ̓K�����A����ь�����N
�Q�ɂ��Ă̕ۏ؂��܂݂܂����A����Ɍ��肳�����̂ł͂���܂���B ��҂܂��͒�
�쌠�҂́A�_��s�ׁA�s�@�s�ׁA�܂��͂���ȊO�ł��낤�ƁA�\�t�g�E�F�A�ɋN���܂���
�֘A���A���邢�̓\�t�g�E�F�A�̎g�p�܂��͂��̑��̈����ɂ���Đ������؂̐����A��
�Q�A���̑��̋`���ɂ��ĉ���̐ӔC������Ȃ����̂Ƃ��܂��B
*******************************************************************************/
#include  <windows.h>
#include  "gl_common.h"


// Frustum ���Z�b�g����
void
gl_set_frustum(const frustum&  frs)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum( frs.get_x1(), frs.get_x2(), frs.get_y1(), frs.get_y2(), frs.get_z1(), frs.get_z2() );
}


// �P�ʂ̋���`��
void
gl_draw_sphere(const int d, bool bCelestial, bool bInvNormal)
{
	float _du, _u1;
	if (bCelestial) {
		_du = -d/float(360);
		_u1 = 1.0f;
	} else {
		_du =  d/float(360);
		_u1 = 0.0f;
	}
	const float du = _du;
	const float u1 = _u1;
	const float dv = -d/float(180);
	const float v1 = 1.0f;
	static float z[180+1], rho[180+1];
	static float cs[360+1], sn[360+1];
	static bool bFirst = true;

	// �e�[�u���̏�����
	if (bFirst) {
		for (int iy=0; iy<=180; iy++) {
			const float th = M_PI * float(iy)/180.0f;
			z[iy]   = cosf(th);
			rho[iy] = sinf(th);
		}
		for (int ix=0; ix<=360; ix++) {
			const float phi = 2.0f * M_PI * float(ix)/360.0f;
			cs[ix] = cosf(phi);
			sn[ix] = sinf(phi);
		}
		bFirst = false;
	}

	// �`��
	const float sgn_n = (bInvNormal)  ?  -1  :  1;
	float v = v1;
	for (int iy=0; iy<180; iy+=d) {
		const float z1   = z[iy];
		const float z2   = z[iy+d];
		const float rho1 = rho[iy];
		const float rho2 = rho[iy+d];
		glBegin(GL_TRIANGLE_STRIP);
		float u = u1;
		for (int ix=0; ix<=360; ix+=d) {
			vector3f  nv0, nv;
			nv0.set(rho1*cs[ix], rho1*sn[ix], z1);
			nv = sgn_n * nv0;
			glTexCoord2f(  u,v );
			glNormal3fv( nv.get_pointer() );
			glVertex3fv( nv0.get_pointer() );
			nv0.set(rho2*cs[ix], rho2*sn[ix], z2);
			nv = sgn_n * nv0;
			glTexCoord2f(  u,v+dv );
			glNormal3fv( nv.get_pointer() );
			glVertex3fv( nv0.get_pointer() );
			u += du;
			if (u>1.0f)			u=1.0f;
			else if (u<0.0f)	u=0.0f;
		}
		glEnd();
		v += dv;
	}
}


void
gl_draw_LOD_sphere(bool bCelestial, bool bInvNormal)
{
	const float thr = 45.0f * D2R;		// ����\�����鎋���a
	GLfloat M[16];
	glGetFloatv( GL_MODELVIEW_MATRIX,  M);
	const float L2  = M[12]*M[12] + M[13]*M[13] + M[14]*M[14];	// ���S�܂ł̋���
	const float R2  = M[0] *M[0]  + M[1] *M[1]  + M[2] *M[2];		// ���a
	float L02 = L2 - R2;
	if (L02 <= 0.0f && !bCelestial) {
		return;
	}

	//--- �����ꍇ�́A����\��
	const float th = asinf(sqrt(R2/L2));
	if (th < thr) {
		int d = int(0.25 * thr/th);
		if (d > 85)			{ d = 60; }
		else if (d > 45)	{ d = 45; }
		else if (d > 30)	{ d = 30; }
		else if (d > 20)	{ d = 20; }
		else if (d > 15)	{ d = 15; }
		else if (d > 10)	{ d = 10; }
		else if (d > 5)		{ d = 5; }
		else if (d > 4)		{ d = 4; }
		else				{ d = 3; }
		gl_draw_sphere(d, bCelestial, bInvNormal);
		return;
	}
	gl_draw_sphere(3, bCelestial, bInvNormal);
}
