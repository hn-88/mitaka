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
#ifndef __MTK_GL_COMMON_H__
#define __MTK_GL_COMMON_H__
#include  "mitaka.h"
#include  "../common/graphics/gl_common.h"
#include  "../common/math/vector3.h"
#include  "../common/graphics/gl_image.h"
#include  "../common/astro/astro_const.h"
#include  "mtk_camera_setting.h"
#include  "mtk_screen.h"


extern float eye_offset_x;

//-------------------------------------
//  ���̓_���`��̈�ɓ����Ă��邩���肷��
//-------------------------------------
inline bool
PointIsInside(const frustum& frs, const vector3f& r, const GLdouble* MM)
{
	// ���_���W�n�ł̈ʒu�x�N�g��
	vector3f  rv;
	rv.x = MM[0]*r.x + MM[4]*r.y + MM[8] *r.z + MM[12];
	rv.y = MM[1]*r.x + MM[5]*r.y + MM[9] *r.z + MM[13];
	rv.z = MM[2]*r.x + MM[6]*r.y + MM[10]*r.z + MM[14];

	// �X�N���[���̊p�x�ɂ��J�����O
	char bf = frs.cull_test(rv.x, rv.y, -rv.z);
	return  (bf == 0);
}

// �I�u�W�F�N�g�����݂̎��_�̕����Ɍ�������
inline float
toward_pos(const vector3d& r0)
{
	quaterniond camera_ori;
	double M[16];
	vector3d  r, eU, eF2, eU2, eR2;
	double R, R0;

	// �J�������W�n�ł̈ʒu���W���擾
	glGetDoublev( GL_MODELVIEW_MATRIX, M);
	r.x = M[12];
	r.y = M[13];
	r.z = M[14];
	r -= get_base_origin_on_local_camera();	// ���_�̂��炵�����ǂ�
	R0 = r.norm();
	r -= r0;
	eU = get_eU_of_base_on_local_camera();	// �J�������W�n�ł̊���W�n�̏����
	R   = r.norm();
	
	eF2 = (r/R);			// r0 ����I�u�W�F�N�g�֌������P�ʃx�N�g��
	eR2 = normalize( cross_product(  eU, -eF2 ) );
	eU2 = cross_product( -eF2, eR2 );
	M[0]  = eR2.x;
	M[1]  = eR2.y;
	M[2]  = eR2.z;
	M[3]  = 0;
	M[4]  = eU2.x;
	M[5]  = eU2.y;
	M[6]  = eU2.z;
	M[7]  = 0;
	M[8]  = -eF2.x;
	M[9]  = -eF2.y;
	M[10] = -eF2.z;
	M[11] = 0;
	glLoadMatrixd(M);

	return  R0;
}



inline float
toward_eye_pos()
{
	quaterniond camera_ori;
	double M[16];
	vector3d  r, eU, eF2, eU2, eR2;
	double R;

	// �J�������W�n�ł̈ʒu���W���擾
	glGetDoublev( GL_MODELVIEW_MATRIX, M);
	r.x = M[12];
	r.y = M[13];
	r.z = M[14];

	r += get_base_origin_on_local_camera();	// ���_�̂��炵�����ǂ�
	eU = get_eU_of_base_on_local_camera();	// �J�������W�n�ł̊���W�n�̎�

	R   = r.norm();
	eF2 = (r/R);			// ���_�i���ڂ̒��ԁj����I�u�W�F�N�g�֌������x�N�g��
	eR2 = normalize( cross_product(  eU, -eF2 ) );
	eU2 = cross_product( -eF2, eR2 );
	M[0]  = eR2.x;
	M[1]  = eR2.y;
	M[2]  = eR2.z;
	M[3]  = 0;
	M[4]  = eU2.x;
	M[5]  = eU2.y;
	M[6]  = eU2.z;
	M[7]  = 0;
	M[8]  = -eF2.x;
	M[9]  = -eF2.y;
	M[10] = -eF2.z;
	M[11] = 0;
	glLoadMatrixd(M);
	return  R;
}




//--  �I�u�W�F�N�g�����݂̎��_�̕����Ɍ�������
//    �X�P�[���͂��̂܂܂̃o�[�W����
inline void
toward_eye_pos2()
{
	quaterniond camera_ori;
	double M[16];
	vector3d  r, eU, eF2, eU2, eR2;
	double R;

	// �J�������W�n�ł̈ʒu���W���擾
	glGetDoublev( GL_MODELVIEW_MATRIX, M);
	r.x = M[12];
	r.y = M[13];
	r.z = M[14];

	r += get_base_origin_on_local_camera();	// ���_�̂��炵�����ǂ�
	eU = get_eU_of_base_on_local_camera();	// �J�������W�n�ł̊���W�n�̎�

	R   = r.norm();
	eF2 = (r/R);			// ���_�i���ڂ̒��ԁj����I�u�W�F�N�g�֌������x�N�g��
	eR2 = normalize( cross_product(  eU, -eF2 ) );
	eU2 = cross_product( -eF2, eR2 );

	float scl = sqrt(M[0]*M[0] + M[1]*M[1] + M[2]*M[2]);
	eF2 *= scl;
	eR2 *= scl;
	eU2 *= scl;
	M[0]  = eR2.x;
	M[1]  = eR2.y;
	M[2]  = eR2.z;
	M[3]  = 0;
	M[4]  = eU2.x;
	M[5]  = eU2.y;
	M[6]  = eU2.z;
	M[7]  = 0;
	M[8]  = -eF2.x;
	M[9]  = -eF2.y;
	M[10] = -eF2.z;
	M[11] = 0;
	glLoadMatrixd(M);
}

//  �����̃X�N���[����Ɉړ�����
inline void
goto_center_screen(float rot_ang)
{
	const double* Mbc;
	Mbc = get_local_camera_matrix_GL();

	glLoadIdentity();
	glMultMatrixd(Mbc);
	double M[16];
	quaternion_to_matrix_GL_tr( get_base_camera_ori(), M );
	glMultMatrixd(M);

	glRotatef(rot_ang, 1, 0, 0);

	glTranslatef(0,0,-get_screen_z());
}

inline void
goto_each_screen()
{
	glLoadIdentity();
	glTranslatef(0,0,-get_screen_z());
}

//-----------------------------
//  �ʂ����_�������悤�ɐݒ肷��B
//  �܂��A�����ɂ�炸�����T�C�Y��
//  �Ȃ�悤�ɕ����T�C�Y����������
//-----------------------------
inline void
prepare_3Dtext_1(const float R, const float th_deg, const float sz, const float scl_fac, bool bConstSize)
{
	// �ʂ̌�����ݒ肷��
	float ra;
	ra = toward_eye_pos();
	float fac0 = ra/get_screen_z();

	// �T�C�Y�̕��������炷
	float d = R;
	d *= scl_fac;
	double th = pi/180.0*th_deg;
	if (bConstSize) { d *= fac0; }
	glTranslatef(d*cos(th), d*sin(th), 0);

	float scl = sz*fac0 * get_screen_w()/180.0;
	glScalef(scl,scl,scl);
}

inline void
prepare_3Dtext_2(const vector3f& rc, const float R, const float sz, const float screen_w, const float screen_z, bool bConstSize, bool bRev)
{
	float ra = toward_pos(rc);
	if (bRev) {
		glRotatef(180, 0,1,0);
	}

	// �T�C�Y�̕��������炷
	float fac0 = ra/screen_z;
	float d = R / 1.4142f;
	if (bConstSize) { d *= fac0; }
	glTranslatef(d, d, 0);
	float scl = sz*fac0 * screen_w / 180.0;
	glScalef(scl,scl,scl);
}


//---------------------
//  �n���ԓ��ʂɈړ�����
//---------------------
inline void
go_to_equ_coord()
{
	glRotated(-epsJ2000, 1,0,0);	// �ԓ����W�n��
}


//-----------
//  Texture
//-----------
void	set_texture(GLuint& texName, image& img);
void	set_texture_alpha(GLuint& texName, image& img);

#endif
