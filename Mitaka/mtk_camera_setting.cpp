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
#include  <windows.h>
#include  "mtk_screen.h"

#include  "mtk_camera_setting.h"







static  quaterniond  base_ori(1,0,0,0);		// ����W�n

static  quaterniond  camera_ori;	// ����W�n�ł̃J�����̎p��
static  vector3d     camera_pos;	// ��n�ŕ\�����J�����̈ʒu

static float g_local_camera_angle = 0;	// �J�����̉�]�p�i3�ʃV�A�^�[�j

// GL �p�ϊ��s��
static double Mbc_GL[16];		// ��n����J�������W�n�ւ̕ϊ��s�� 
static double Mcb_GL[16];		// �J�������W�n�����n�ւ̕ϊ��s��


// frustum
static frustum  frs_near, frs_far;
static bool bLoadFrustum = false;


//  ������ς���Ƃ��́i�E�ڂ́j�ړ�����(�f�t�H���g�� x ����j
static vector3d  eye_shift_dirc(1,0,0);



void
set_local_camera(const quaterniond& ori, const vector3d& r0)
{
	double M[16];

	camera_ori = ori;
	camera_pos = r0;


	//--- �J�������W�n�������W�n�ւ̕ϊ��s�� ---
	quaternion_to_matrix_GL(camera_ori, M);
	for (int i=0; i<3; i++) {
		int idx = 4*i;
		Mcb_GL[idx  ] = M[idx  ];
		Mcb_GL[idx+1] = M[idx+1];
		Mcb_GL[idx+2] = M[idx+2];
		Mcb_GL[idx+3] = 0.0;
	}
	Mcb_GL[12] = r0.x;
	Mcb_GL[13] = r0.y;
	Mcb_GL[14] = r0.z;
	Mcb_GL[15] = 1.0;


	//--- ����W�n����J�������W�n�ւ̕ϊ��s�� ---
	quaternion_to_matrix_GL(camera_ori.inv(), M);
	for (int i=0; i<3; i++) {
		int idx = 4*i;
		Mbc_GL[idx  ] = M[idx  ];
		Mbc_GL[idx+1] = M[idx+1];
		Mbc_GL[idx+2] = M[idx+2];
		Mbc_GL[idx+3] = 0.0;
	}
	// �J�������W�n�ł̊���W�n�̌��_�̈ʒu
	for (int i=0; i<3; i++) {
		double val = 0.0;
		for (int j=0; j<3; j++) {
			val += -M[4*j+i] * Mcb_GL[12+j];
		}
		Mbc_GL[12+i] = val;
	}
	Mbc_GL[15] = 1.0;
}



// �R�ʃV�A�^�[�̏ꍇ
void
set_local_camera_sub(const float eye_offset)
{
	quaterniond  ori;

	ori = make_rotation_quaternion(double(-g_local_camera_angle*D2R), vector3d(0,1,0));
	
	vector3d eF, eU, eR;
	quaternion_to_axes(ori, eF, eU, eR);

	eye_shift_dirc = eR;	// �ڂ̂���̕���

	set_local_camera(ori, double(eye_offset) * eye_shift_dirc );
}

void
init_local_camera_angle(const float angle, const float eye_offset)
{
	g_local_camera_angle = angle;

	set_local_camera_sub(eye_offset);
}

void
reset_local_camera_angle(const float eye_offset)
{
	set_local_camera_sub(eye_offset);
}


void
set_local_camera_pos(const vector3d& r0)
{
	camera_pos = r0;

	//--- �J�������W�n�������W�n�ւ̕ϊ��s�� �i�ʒu�̂ݍX�V�j---
	Mcb_GL[12] = r0.x;
	Mcb_GL[13] = r0.y;
	Mcb_GL[14] = r0.z;
	Mcb_GL[15] = 1.0;

	//--- ����W�n����J�������W�n�ւ̕ϊ��s�� �i�ʒu�̂ݍX�V�j---
	// �J�������W�n�ł̊���W�n�̌��_�̈ʒu
	for (int i=0; i<3; i++) {
		double val = 0.0;
		for (int j=0; j<3; j++) {
			val += -Mbc_GL[4*j+i] * Mcb_GL[12+j];
		}
		Mbc_GL[12+i] = val;
	}
	Mbc_GL[15] = 1.0;
}


void
set_local_camera_pos(const float eye_offset)
{
	set_local_camera_pos( double(eye_offset) * eye_shift_dirc );
}




quaterniond
get_local_camera_orientation()
{
	return  camera_ori;
}

vector3d
get_local_camera_pos()
{
	return  camera_pos;
}



const double*
get_local_camera_matrix_GL()
{
	return  Mbc_GL;
}

const double*
get_local_camera_matrix_GL_inv()
{
	return  Mcb_GL;
}

vector3d
get_eU_of_base_on_local_camera()
{
	return  vector3d(Mbc_GL[4], Mbc_GL[5], Mbc_GL[6]);
}

vector3d
get_base_origin_on_local_camera()
{
	return  vector3d(-Mbc_GL[12], -Mbc_GL[13], -Mbc_GL[14]);
}




bool
load_local_camera_setting(FILE* fp)
{
	int count;
	float a, b, c, d;
	float x, y, z;

	//  �ʒu�Ǝp���́A����W�n�ł̒l
	count = fscanf(fp, "%f %f %f %f", &a, &b, &c, &d);		// �p��
	if (count != 4)  return false;
	count = fscanf(fp, "%f %f %f", &x, &y, &z);				// �ʒu
	if (count != 3)  return false;

	quaterniond ori(a, b, c, d);
	ori.normalize();
	vector3d r0(x,y,z);

	set_local_camera(ori, r0);

	return  true;
}



void
set_base_camera(const quaterniond& ori)
{
	base_ori = ori;
}

quaterniond
get_base_camera_ori()
{
	return  base_ori;
}


bool
load_base_camera_setting(FILE* fp)
{
	int count;
	float a, b, c, d;

	//  ����W�n�̎p����ݒ�
	count = fscanf(fp, "%f %f %f %f", &a, &b, &c, &d);
	if (count != 4)  return false;

	quaterniond ori(a, b, c, d);
	ori.normalize();

	set_base_camera(ori);

	return  true;
}




void
set_frustum_sub(frustum& frs, float l, float r, float b, float t, float n, float f, float z1, float z2)
{
	float x1, x2, y1, y2, a;
	a  = z1 / n;
	x1 = a * l;
	x2 = a * r;
	y1 = a * b;
	y2 = a * t;
	frs.set(x1, x2, y1, y2, z1, z2);
}


bool
load_frustum(FILE* fp, float z_near, float z_mid, float z_far)
{
	int count;
	float l, r, b, t, n, f;

	// 
	count = fscanf(fp, "%f %f %f %f %f %f", &l, &r, &b, &t, &n, &f);
	if (count != 6)  return false;

	//--- near
	set_frustum_sub(frs_near, l, r, b, t, n, f, z_near, z_mid);

	//--- far
	set_frustum_sub(frs_far, l, r, b, t, n, f, z_mid, z_far);


	bLoadFrustum = true;
	return  true;
}


bool
load_glrc_cfg(FILE* fp, float z_near, float z_mid, float z_far)
{
	int count;
    char token[1024];

	quaterniond  ori(1,0,0,0);
	vector3d  r(0,0,0);

    while (fscanf(fp, "%s", token)==1)
    {
        if (strcmp(token, "Offset")==0) {
        }
        else if (strcmp(token, "Translate")==0) {
            float x, y, z;
            count = fscanf(fp, "%f %f %f", &x, &y, &z);
			if (count != 3)  continue;
			r.set(100*x, 100*y, 100*z);
        }
        else if (strcmp(token, "Rotate")==0) {
            float angle, x, y, z;
            count = fscanf(fp, "%f %f %f %f", &angle, &x, &y, &z);
			if (count != 4)  continue;
			ori = make_rotation_quaternion(double(angle)*3.14159265/180.0, vector3d(x, y, z)) * ori;
        }
        else if (strcmp(token, "Scale")==0) {
        }
        else if (strcmp(token, "Lens")==0) {
        }
        else if (strcmp(token, "Frustum")==0) {
            float l, r, b, t, n, f;
            count = fscanf(fp, "%f %f %f %f %f %f", &l, &r, &b, &t, &n, &f);
			if (count != 6)  continue;

			set_frustum_sub(frs_near, l, r, b, t, n, f, z_near, z_mid);
			set_frustum_sub(frs_far,  l, r, b, t, n, f, z_mid,  z_far);
			bLoadFrustum = true;
        }
    }


	ori.normalize();
	set_local_camera(ori, r);




    return true;
}




bool
isFrustumLoaded()
{
	return bLoadFrustum;
}

frustum
get_loaded_frustum_near()
{
	return  frs_near;
}


frustum
get_loaded_frustum_far()
{
	return  frs_far;
}






//--- ���� ---
static float eye_distance_2 = 6.4;

float
get_eye_distance_2()
{
	return eye_distance_2;
}


void
set_eye_distance_2(float d, char CLR)
{
	eye_distance_2 = d;
	
	float offset;
	switch ( CLR ) {
		case 'L':	offset = -0.5*d;	break;
		case 'R':	offset =  0.5*d;	break;
		default:
		case 'C':	offset =  0.0;		break;
	}

	set_local_camera_pos( offset );
	
	set_eye_offset_x( offset );
}
