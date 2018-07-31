/**************************************************************************************************

Copyright (c) 2007, 2015   Tsunehiko Kato  and  4D2U Project, National Astronomical Observatory of Japan

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
#include  "mtk_planetary_ring.h"
#include  "../common/astro/cele_mech.h"
#include  "mtk_planets.h"
#include  "mtk_planet_position.h"
#include  "mtk_planet_orientation.h"
#include  "mtk_solar_system.h"
#include  "mtk_switches.h"
#include  "mtk_OpenGL.h"



// ���x
static float frac[particle_ring_RN][particle_ring_PHIN + 1];




// ���_
enum {
	PT1,
	PT2,
	PT3,
	PT4,
	PTC1,
	PTC2
};


// ��_�̃p�^�[��
enum {
	CODE_12_13 = 0x3,
	CODE_12_24 = 0x5,
	CODE_13_34 = 0xA,
	CODE_24_34 = 0xC,
	CODE_12_34 = 0x9,
	CODE_13_24 = 0x6
};

enum {
	CP_12_13,
	CP_12_24,
	CP_13_34,
	CP_24_34,
	CP_12_34,
	CP_13_24
};


// �`��p�̒��_���
struct tri_index {
	int N1;
	int idx1[5];
	int N2;
	int idx2[5];
};

const tri_index tri_idx[6] = {
	{3, {PT1, PTC1, PTC2, 0, 0}, 5, {PTC1, PT2, PTC2, PT4, PT3}},
	{5, {PTC1, PT1, PTC2, PT3, PT4}, 3, {PT2, PTC1, PTC2, 0, 0}},
	{5, {PT1, PT2, PTC1, PT4, PTC2}, 3, {PTC1, PTC2, PT3, 0, 0}},
	{5, {PT2, PTC1, PT1, PTC2, PT3}, 3, {PTC1, PTC2, PT4, 0, 0}},
	{4, {PT1, PTC1, PT3, PTC2, 0}, 4, {PTC1, PT2, PTC2, PT4, 0}},
	{4, {PT1, PT2, PTC1, PTC2, 0}, 4, {PTC1, PTC2, PT3, PT4, 0}}
};



//---------------
//  Ring object
//---------------
static std::vector<particle_ring> vRings;







//-------------------
//  Planetary Rings
//-------------------
// ���a�͘f���̔��a�ŋK�i����������
particle_ring::particle_ring(float _R1, float _R2, float _alp, const std::string& tex_fn) : R1(_R1), R2(_R2), alp(_alp), last_mode(-1), JD(0.0), bLastSideLight(bSideLight)
{
	tex_file_name = tex_fn;

	for (int i=0; i<particle_ring_RN; i++) {
		s[i] = float(i)/(particle_ring_RN-1);
		const float r = (R2-R1) * float(i) / (particle_ring_RN-1) + R1;

		for (int j=0; j<=particle_ring_PHIN; j++) {
			const float phi  = 2*M_PI * float(j)/particle_ring_PHIN;

			float x, y;
			x = r *cosf(phi);
			y = r *sinf(phi);

			pos[i][j].set(x,y,0.0f);
		}
	}

}



//------------
//  Textures
//------------
GLuint
particle_ring::load_texture(int shrink_rate)
{
	tex_idx = LoadTexture(tex_file_name.c_str(), shrink_rate, false);

	return tex_idx;
}






//--------------
//  �e���̐���
//--------------
void
particle_ring::calc_shadow_factor(const mtk_draw_struct& ds, const vector3d& r0, const double& R, const double& f, const vector3d& ex, const vector3d& ey, const vector3d& ez, const vector3d& _lv)
{
	// �ȑO�̌v�Z���玞���ƃ��[�h���ς���Ă��Ȃ��Ƃ��͍Čv�Z���Ȃ�
	if (!bSideLight) {
		if (last_mode != -1) {
			if (last_mode == ds.tyMode && bLastSideLight == bSideLight && JD == ds.JD) {
				return;
			}
		}
	}
	last_mode = ds.tyMode;
	bLastSideLight = bSideLight;
	JD = ds.JD;

	vector3d _rs, nv;
	double a, _x, _y, _z;



	// �f�������ɂȂ�悤�Ɏ������ɃX�P�[���������W�n�ł̌����̈ʒu�����߂�
	a = 1.0 / (1.0 - f); // ����-�Z����(>1)

	_x = dot_product(ex, _lv);
	_y = dot_product(ey, _lv);
	_z = dot_product(ez, _lv);
	_rs.set(_x, _y, a*_z);	// �f�����W�n�ł̌����̈ʒu�i�ȉ��A�f�����W�n�Ōv�Z����j
	nv = normalize(-_rs);	// �����x�N�g��
	

	double sinth, costh;
	sinth = R / norm(_rs);
	costh = sqrt(1.0 - sinth*sinth);


	for (int i = 0; i<particle_ring_RN; i++) {
		for (int j = 0; j <= particle_ring_PHIN; j++) {

			vector3d dr = R * vector3d(pos[i][j]);
			if (dot_product(dr, nv) < 0.0) {
				shadow_fac[i][j] = -1.0;
				continue;
			}

			vector3d dr_perp = dr - dot_product(dr, nv)*nv;
			
			shadow_fac[i][j] = 1.0 - norm(dr_perp) / R;
		}
	}

	for (int i = 0; i<particle_ring_RN-1; i++) {
		for (int j = 0; j < particle_ring_PHIN; j++) {
			bool b1, b2, b3, b4;

			b1 = (shadow_fac[i][j] > 0.0);
			b2 = (shadow_fac[i+1][j] > 0.0);
			b3 = (shadow_fac[i][j+1] > 0.0);
			b4 = (shadow_fac[i+1][j+1] > 0.0);

			if ((b1 && b2 && b3 && b4) || (!b1 && !b2 && !b3 && !b4)) {
				b_shadow_boundary[i][j] = false;
			}
			else {
				b_shadow_boundary[i][j] = true;
			}
		}
	}
}





//--------
//  �`��
//--------
void
particle_ring::draw_triangles(const vector2f* pt, const float *u, const float *f, const int *pt_idx, const int N, bool bShadow) const
{
	glBegin(GL_TRIANGLE_STRIP);


	const float amb = get_lighting_ambient();
	for (int i = 0; i < N; i++) {
		int idx;
		float lv;

		idx = pt_idx[i];
		lv = amb;
		if (!bShadow) {
			lv += f[idx];
		}
		glColor3f(lv, lv, lv);
		glTexCoord2f(u[idx], 0);
		glVertex2fv(pt[idx].get_pointer());
	}

	glEnd();
}


void
particle_ring::draw_boundary_mesh(int i, int j) const
{
	// �_�̈ʒu
	vector2f pt[6];
	pt[PT1] = vector2d(pos[i][j].x, pos[i][j].y);
	pt[PT2] = vector2d(pos[i + 1][j].x, pos[i + 1][j].y);
	pt[PT3] = vector2d(pos[i][j + 1].x, pos[i][j + 1].y);
	pt[PT4] = vector2d(pos[i + 1][j + 1].x, pos[i + 1][j + 1].y);

	// �e�N�X�`�����W
	float u[6];
	u[PT1] = u[PT3] = s[i];
	u[PT2] = u[PT4] = s[i + 1];

	// ���x
	float f[6];
	f[PT1] = frac[i][j];
	f[PT2] = frac[i + 1][j];
	f[PT3] = frac[i][j + 1];
	f[PT4] = frac[i + 1][j + 1];
	
	// �e���q
	float sf[4];
	sf[PT1] = shadow_fac[i][j];
	sf[PT2] = shadow_fac[i + 1][j];
	sf[PT3] = shadow_fac[i][j + 1];
	sf[PT4] = shadow_fac[i + 1][j + 1];


	//---  ��_�̌v�Z
	int idx = 0;
	int code = 0;
	int flg = 1;

	for (int k = 0; k < 4; k++) {
		const int edge_idx[4][2] = {{PT1, PT2}, {PT1, PT3}, {PT2, PT4}, {PT3, PT4}};	// 1-2, 1-3, 2-4, 3-4
		int i1, i2;
		i1 = edge_idx[k][0];
		i2 = edge_idx[k][1];

		// ��_�̌v�Z
		if (sf[i1] * sf[i2] < 0.0) {
			code |= flg;

			// �e�ʂ̕ω���
			vector2f edge = pt[i2] - pt[i1];
			float du = u[i2] - u[i1];
			float df = f[i2] - f[i1];

			// ���`��Ԃ̈��q
			float a = -sf[i1] / (sf[i2] - sf[i1]);

			pt[idx + 4] = a*edge + pt[i1];
			u[idx + 4] = a*du + u[i1];
			f[idx + 4] = a*df + f[i1];

			idx++;
		}

		flg <<= 1;
	}

	int cp_idx;
	switch (code) {
		case CODE_12_13:	cp_idx = CP_12_13;	break;
		case CODE_12_24:	cp_idx = CP_12_24;	break;
		case CODE_13_34:	cp_idx = CP_13_34;	break;
		case CODE_24_34:	cp_idx = CP_24_34;	break;
		case CODE_12_34:	cp_idx = CP_12_34;	break;
		case CODE_13_24:	cp_idx = CP_13_24;	break;
		default:	return;	// incorrect pattern
	}


	//---  �`��
	bool bShadow = (sf[PT1] > 0.0);
	const tri_index& ti = tri_idx[cp_idx];
	draw_triangles(pt, u, f, ti.idx1, ti.N1, bShadow);
	draw_triangles(pt, u, f, ti.idx2, ti.N2, !bShadow);
}



void
particle_ring::draw(const mtk_draw_struct& ds, const vector3d& r0, const double& R, const vector3d& ex, const vector3d& ey, const vector3d& lv)  const
{
	vector3d cpos;
	cpos = ds.camera.get_pos();


	// ���x�̌v�Z
	for (int i = 0; i<particle_ring_RN; i++) {
		for (int j = 0; j <= particle_ring_PHIN; j++) {
			const vector3f&  pos_pl = pos[i][j];

			double x, y;
			x = R * pos_pl.x;
			y = R * pos_pl.y;

			vector3d pos;
			pos = x*ex + y*ey + r0;


			// ���x���v�Z
			const vector3d nv = normalize(pos - cpos);
			const vector3d lv_e = -lv;
			float dp = dot_product(lv_e, nv);
			float th = acosf(dp);

			frac[i][j] = 1.0 - th / M_PI;
		}
	}


	// �`��
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, tex_idx);


	const float amb = get_lighting_ambient();
	for (int i = 0; i<particle_ring_RN - 1; i++) {
		for (int j = 0; j<particle_ring_PHIN; j++) {
			float f;

			if (b_shadow_boundary[i][j]) {
				draw_boundary_mesh(i, j);
				continue;
			}

			glBegin(GL_TRIANGLE_STRIP);



			float fac;
			if (shadow_fac[i][j] > 0.0) {
				// �e�̒�
				fac = amb;
			}
			else {
				// �e�̊O
				fac = 1.0f;
			}

			f = amb;
			if (shadow_fac[i][j] <= 0.0) {
				f += frac[i][j];
			}
			glColor3f(f, f, f);
			glTexCoord2f(s[i], 0);
			glVertex3fv(pos[i][j].get_pointer());


			f = amb;
			if (shadow_fac[i+1][j] <= 0.0) {
				f += frac[i+1][j];
			}
			glColor3f(f, f, f);
			glTexCoord2f(s[i + 1], 0);
			glVertex3fv(pos[i + 1][j].get_pointer());

			f = amb;
			if (shadow_fac[i][j+1] <= 0.0) {
				f += frac[i][j+1];
			}
			glColor3f(f, f, f);
			glTexCoord2f(s[i], 0);
			glVertex3fv(pos[i][j + 1].get_pointer());

			f = amb;
			if (shadow_fac[i+1][j + 1] <= 0.0) {
				f += frac[i+1][j + 1];
			}
			glColor3f(f, f, f);
			glTexCoord2f(s[i + 1], 0);
			glVertex3fv(pos[i + 1][j + 1].get_pointer());
			glEnd();
		}
	}
	glPopAttrib();
}



// �f�����S�Ɉړ��B�f���̔��a�ŃX�P�[�����O�B�f���̐ԓ����W
void
particle_ring::draw_3Dchart()  const
{
	float lp[4];
	glGetLightfv(GL_LIGHT0, GL_POSITION, lp);	// ���_���W�n�ł̌����̈ʒu
	const vector3f lv_e = normalize(vector3f(lp[0], lp[1], lp[2]));	// ���s����������



	float M[16];
	glGetFloatv(GL_MODELVIEW_MATRIX,  M);


    // ���x�̌v�Z
	for (int i=0; i<particle_ring_RN; i++) {
		for (int j=0; j<=particle_ring_PHIN; j++) {
			const vector3f&  pos_pl = pos[i][j];

			float x, y;
			x = pos_pl.x;
			y = pos_pl.y;

			// ���_���W�n�ł̗v�f�̈ʒu�x�N�g��
			vector3f  pos_e;
			pos_e.x = M[0]*x + M[4]*y + M[12];
			pos_e.y = M[1]*x + M[5]*y + M[13];
			pos_e.z = M[2]*x + M[6]*y + M[14];

			// ���x���v�Z
			const vector3f nv = normalize(-pos_e);
			float dp = dot_product(lv_e, nv);		// ���s����������
			float th = acosf(dp);

			frac[i][j] = 1.0 - th / M_PI;
		}
	}


	// �`��
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_2D, tex_idx);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	for (int i=0; i<particle_ring_RN-1; i++) {
		glBegin(GL_TRIANGLE_STRIP);
		for (int j=0; j<particle_ring_PHIN; j++) {
			float f;

			f = frac[i][j];
			glColor3f(f,f,f);
			glTexCoord2f(s[i], 0);
			glVertex3fv(pos[i][j].get_pointer());

			f = frac[i+1][j];
			glColor3f(f,f,f);
			glTexCoord2f(s[i+1], 0);
			glVertex3fv(pos[i+1][j].get_pointer());

			f = frac[i][j+1];
			glColor3f(f,f,f);
			glTexCoord2f(s[i], 0);
			glVertex3fv(pos[i][j+1].get_pointer());

			f = frac[i+1][j+1];
			glColor3f(f,f,f);
			glTexCoord2f(s[i+1], 0);
			glVertex3fv(pos[i+1][j+1].get_pointer());
		}
		glEnd();
	}
	glPopAttrib();
}





//---------------
//  Ring object
//---------------
int
add_particle_ring(float R1, float R2, float alp, const std::string& tex_fn)
{
	particle_ring pr(R1, R2, alp, tex_fn);
	vRings.push_back(pr);

	return vRings.size() - 1;
}

particle_ring&
get_particle_ring(int idx)
{
	return  vRings[idx];
}



void
load_ring_textures()
{
	for (int i = 0; i<vRings.size(); ++i) {
		particle_ring& pr = vRings[i];
		pr.load_texture(TexShrinkRate);
	}

}






//--------
//  Draw
//--------

// �P�ʂ̋���`��
// ---> gl_draw_sphere �ɓ����H
void
draw_ring_shaded_sphere(const vector3d& r0, const double& R, const double& f, const vector3d& ex, const vector3d& ey, const vector3d& ez, const vector3d &_lv, const particle_ring& pr, const GLuint texPlanet, const RGBf& mat_col)
{
	const GLuint texRing = pr.get_texture_index();

	static float z0[180 + 1], rho[180 + 1];
	static float cs[360 + 1], sn[360 + 1];
	static bool bFirst = true;

	const float a = 1.0f - f;
	const float a2 = a*a;

	const float R1 = pr.get_R1();
	const float R2 = pr.get_R2();
	const float af = pr.get_alpha();


	// �e�[�u���̏�����
	if (bFirst) {
		for (int iy = 0; iy <= 180; iy++) {
			const float th = M_PI * float(iy) / 180.0f;
			z0[iy]  = cosf(th);
			rho[iy] = sinf(th);
		}
		for (int ix = 0; ix <= 360; ix++) {
			const float phi = 2.0f * M_PI * float(ix) / 360.0f;
			cs[ix] = cosf(phi);
			sn[ix] = sinf(phi);
		}
		bFirst = false;
	}



	// �����a�̌v�Z
	const float thr = 45.0f * D2R;		// ����\�����鎋���a
	GLfloat M[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, M);
	const float _L2 = M[12] * M[12] + M[13] * M[13] + M[14] * M[14];	// ���S�܂ł̋���
	const float _R2 = M[0] * M[0] + M[1] * M[1] + M[2] * M[2];		// ���a
	float L02 = _L2 - _R2;
	if (L02 <= 0.0f) {
		return;
	}

	// �����a�ɂ���ċ��̕�������ς���
	int d;
	const float th = asinf(sqrt(_R2 / _L2));
	if (th < thr) {
		d = int(0.25 * thr / th);
		if (d > 85)			{ d = 60; }
		else if (d > 45)	{ d = 45; }
		else if (d > 30)	{ d = 30; }
		else if (d > 20)	{ d = 20; }
		else if (d > 15)	{ d = 15; }
		else if (d > 10)	{ d = 10; }
		else if (d > 5)		{ d = 5; }
		else if (d > 4)		{ d = 4; }
		else				{ d = 3; }
	}
	else {
		d = 2;
	}





	// �f���{�̂̕`��
	float _du, _u1;
	_du = d / float(360);
	_u1 = 0.0f;
	const float du = _du;
	const float u1 = _u1;
	const float dv = -d / float(180);
	const float v1 = 1.0f;

	// �A���r�G���g���������ŕ`��
	if (texPlanet != 0) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texPlanet);
	}
	else {
		glDisable(GL_TEXTURE_2D);
	}
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glDisable(GL_BLEND);


	GLfloat ambient_def[] = {0.1, 0.1, 0.1, 1.0};
	GLfloat ambient0[] = {0.0, 0.0, 0.0, 1.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);


	float mat_d[3], mat_a[3];
	if (texPlanet != 0) {
		mat_d[0] = mat_d[1] = mat_d[2] = 1;
	}
	else {
		mat_d[0] = mat_col.r;
		mat_d[1] = mat_col.g;
		mat_d[2] = mat_col.b;
	}

	mat_a[0] = mat_a[1] = mat_a[2] = 0;
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);


	float v = v1;
	for (int iy = 0; iy<180; iy += d) {
		const float z1 = a * z0[iy];
		const float z2 = a * z0[iy + d];
		const float rho1 = rho[iy];
		const float rho2 = rho[iy + d];
		glBegin(GL_TRIANGLE_STRIP);
		float u = u1;
		for (int ix = 0; ix <= 360; ix += d) {
			float x, y, z;
			vector3f  r, nv;

			x = rho1*cs[ix];
			y = rho1*sn[ix];
			z = z1;
			r.set(x, y, z);
			nv = normalize(vector3f(x,y,z/a2));
			glTexCoord2f(u, v);
			glNormal3fv(nv.get_pointer());
			glVertex3fv(r.get_pointer());

			x = rho2*cs[ix];
			y = rho2*sn[ix];
			z = z2;
			r.set(x, y, z);
			nv = normalize(vector3f(x, y, z / a2));
			glTexCoord2f(u, v + dv);
			glNormal3fv(nv.get_pointer());
			glVertex3fv(r.get_pointer());

			u += du;
			if (u>1.0f)			u = 1.0f;
			else if (u<0.0f)	u = 0.0f;
		}
		glEnd();
		v += dv;
	}




	// �f�����W�n�ł̌����x�N�g��
	float _x, _y, _z;
	vector3f lv;

	_x = dot_product(ex, _lv);
	_y = dot_product(ey, _lv);
	_z = dot_product(ez, _lv);
	lv = normalize( vector3f(_x, _y, _z) );



	//
	const int Ne = 4;
	const int eidx[Ne][2] {{0, 1}, {1, 3}, {3, 2}, {2, 0}};


	// �y����̊̉e�̕`��
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(-1, -1);
	glDepthMask(GL_FALSE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texRing);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glColor4f(0, 0, 0, af);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	



	for (int iy = 0; iy<180; iy += d) {
		const float z1   = a * z0[iy];
		const float z2   = a * z0[iy + d];
		const float rho1 = rho[iy];
		const float rho2 = rho[iy + d];
		
		for (int ix = 0; ix <= 360-d; ix += d) {
			vector3f  r[4], nv[4];
			float t[4], u[4];
			bool bn[4];
			int ix2, cu1, cu2;

			ix2 = ix + d;


			// ���_�P
			r[0].set(rho1*cs[ix], rho1*sn[ix], z1);
			t[0] = -r[0].z / lv.z;
			// ���_2
			r[1].set(rho2*cs[ix], rho2*sn[ix], z2);
			t[1] = -r[1].z / lv.z;
			// ���_3
			r[2].set(rho1*cs[ix2], rho1*sn[ix2], z1);
			t[2] = -r[2].z / lv.z;
			// ���_4
			r[3].set(rho2*cs[ix2], rho2*sn[ix2], z2);
			t[3] = -r[3].z / lv.z;

			if (t[0] < 0 && t[1] < 0 && t[2] < 0 && t[3] < 0) {
				continue;
			}
			

			
			for (int i = 0; i < Ne; i++) {
				int i1, i2;
				i1 = eidx[i][0];
				i2 = eidx[i][1];

				if (t[i1] * t[i2] < 0) {
					int itmp;
					float w;

					if (t[i1] > 0) { itmp = i1; i1 = i2; i2 = itmp; }

					w = -t[i1] / (t[i2] - t[i1]);
					t[i1] = 0;
					r[i1] = (1 - w)*r[i1] + w*r[i2];
				}
			}
			


			// �e�N�X�`�����W
			cu1 = cu2 = 0;
			for (int i = 0; i < 4; i++) {
				vector3f _r;
				float R;

				_r = r[i] + t[i] * lv;			// �����O���ʏ�̓_
				R = norm(_r);					// ���S����̋���
				u[i] = (R - R1) / (R2 - R1);	// 1�����e�N�X�`�����W

				if (u[i] < 0.0) cu1++;
				if (u[i] > 1.0) cu2++;
			}

			if (cu1 == 4 || cu2 == 4) {
				continue;
			}


			
			for (int i = 0; i < Ne; i++) {
				int i1, i2;
				i1 = eidx[i][0];
				i2 = eidx[i][1];
				const float um = 1.1;

				if ((u[i1] > um && u[i2] < 1.0) || (u[i2] > um && u[i1] < 1.0)) {
					int itmp;
					float w;

					if (u[i1] > um) { itmp = i1; i1 = i2; i2 = itmp; }

					w = (1-u[i1]) / (u[i2] - u[i1]);
					u[i2] = 1;
					r[i2] = (1 - w)*r[i1] + w*r[i2];
				}
			}
			



			// �@��
			for (int i = 0; i < 4; i++) {
				nv[i] = normalize(vector3f(r[i].x, r[i].y, r[i].z / a2));
				float dp = dot_product(nv[i], lv);
				bn[i] = (dp >= 0.0);
			}

			if (!bn[0] && !bn[1] && !bn[2] && !bn[3]) {
				continue;
			}


			// �`��
			glBegin(GL_TRIANGLE_STRIP);
			for (int i = 0; i < 4; i++) {
				glTexCoord2f(u[i], 0);
				glVertex3fv(r[i].get_pointer());
			}
			glEnd();
		}
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);




	// �A���r�G���g�����݂̂ŉ��Z�`��
	float amb = get_lighting_ambient();
	if (texPlanet != 0) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texPlanet);
		glColor3f(amb, amb, amb);
	}
	else {
		glDisable(GL_TEXTURE_2D);
		RGBf col = amb * mat_col;
		glColor3fv(col.get_pointer());
	}




	glBlendFunc(GL_ONE, GL_ONE);


	v = v1;
	for (int iy = 0; iy<180; iy += d) {
		const float z1 = a * z0[iy];
		const float z2 = a * z0[iy + d];
		const float rho1 = rho[iy];
		const float rho2 = rho[iy + d];
		glBegin(GL_TRIANGLE_STRIP);
		float u = u1;
		for (int ix = 0; ix <= 360; ix += d) {
			float x, y, z;
			vector3f  r, nv;

			x = rho1*cs[ix];
			y = rho1*sn[ix];
			z = z1;
			r.set(x, y, z);
			nv = normalize(vector3f(x, y, z / a2));
			glTexCoord2f(u, v);
			glNormal3fv(nv.get_pointer());
			glVertex3fv(r.get_pointer());

			x = rho2*cs[ix];
			y = rho2*sn[ix];
			z = z2;
			r.set(x, y, z);
			nv = normalize(vector3f(x, y, z / a2));
			glTexCoord2f(u, v + dv);
			glNormal3fv(nv.get_pointer());
			glVertex3fv(r.get_pointer());

			u += du;
			if (u>1.0f)			u = 1.0f;
			else if (u<0.0f)	u = 0.0f;
		}
		glEnd();
		v += dv;
	}



	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_def);
	glEnable(GL_LIGHTING);
	glDepthMask(GL_TRUE);
	glDisable(GL_POLYGON_OFFSET_FILL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
