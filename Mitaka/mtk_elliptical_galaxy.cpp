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
#include  "mtk_elliptical_galaxy.h"
#include  <windows.h>
#include  <gl/gl.h>
#include  "../common/graphics/color.h"
#include  "../common/astro/astrometry.h"
#include  "../common/math/random.h"
#include  "mtk_target.h"
#include  "../common/win_dlg.h"

const int IMAX = 150;
const int JMAX = 40;

// ���󐯒c�̐�
const int N_blue = 500;
const int N_red  = 500;
vector3d GCB[N_blue];
vector3d GCR[N_red];
RGBf GCBcol, GCRcol;
const double _pi = 3.14159265358979;

void	draw_M87_GC(const mtk_draw_struct& ds, const vector3d& rpos, const float fac);


//----------
//  �ȉ~���
//----------
inline float
calc_elliptical_galaxy_ray(const vector3f& r0, const vector3f& ev)
{
	const int N = 40;

	//---  �\�����Ƃ̌�_�����߂�  ---
	const float b = dot_product(r0, ev);
	const float c = r0.norm2() - 1.0f;
	float D = b*b - c;
	if (D <= 0.0f)  return 0;
	const float smax = -b + sqrt(D);
	if (smax <= 0) return 0;
	const float ds = smax/N;
	// �R�A���a
	const float rb = 0.05;
	const float rb2 = rb*rb;

	//---  �ϕ�  ---
	vector3f  r = r0;
	float rho = 0.0;
	const float rth = 0.01 * rb;
	const float rth2 = rth*rth;
	for (int i=0; i<N; i++) {
		const float r2 = r.norm2()/rb2;

		float _r2 = r2;
		if (_r2 < rth2) _r2 = rth2;
		rho += 1.0f/(_r2*(1.0f+_r2));
		r += ds * ev;
	}
	rho *= ds;

	// �W��
	rho *= 60;
	if (rho > 100) rho=100;

	return rho;
}


//-------
//  �O��
//-------
void
calc_elliptical_galaxy_outer(const float& L, const float& R, const vector3f& rc,
							 const RGBf& col0, RGBf F_RGB[IMAX+1][JMAX+1], vector3f r_pt [IMAX+1][JMAX+1], int istep, int jstep)
{
	static bool first = true;
	static float cosphi[JMAX+1], sinphi[JMAX+1];
	if (first) {
		first = false;
		for (int j=0; j<=JMAX; j++) {
			float phi = float(j)/JMAX * 2 * 3.1415926535;
			cosphi[j] = cos(phi);
			sinphi[j] = sin(phi);
		}
	}

	//----------------
	//  �P�x���z�̌v�Z
	//----------------
	const float Th = 3.14159265 - acos(R/L);	// �I�u�W�F�N�g���S���猩���\���̈�̊p�x�i�̔����j
	for (int i=0; i<=IMAX; i+=istep) {
		float f, th, z, rho;
		f   = float(i)/IMAX;

		th  = f * Th;
		z   = -cos(th);
		rho = sin(th);
		for (int j=0; j<=JMAX; j+=jstep) {
			float x,y;
			x   = rho * cosphi[j];
			y   = rho * sinphi[j];
			const vector3f n(x,y,z);
			const vector3f r  = n;							// �v�Z�J�n�_�̈ʒu�i�\������j
			const vector3f ev = -normalize( r + rc );		// ��������� r �ւ̕����x�N�g���i�����̕����j
			float I = calc_elliptical_galaxy_ray(r, ev);
			RGBf col = I*col0;

			r_pt[i][j] = R*n;
			F_RGB[i][j] = correct_RGB( col );
		}
	}
}



//-------
//  ����
//-------
void
calc_elliptical_galaxy_inner(const float& L, const float& R, const vector3f& rc,
							 const RGBf& col0, RGBf F_RGB[IMAX+1][JMAX+1], vector3f r_pt [IMAX+1][JMAX+1], int istep, int jstep)
{
	static bool first = true;
	static float cosphi[JMAX+1], sinphi[JMAX+1];
	if (first) {
		first = false;
		for (int j=0; j<=JMAX; j++) {
			float phi = float(j)/JMAX * 2 * 3.1415926535;
			cosphi[j] = cos(phi);
			sinphi[j] = sin(phi);
		}
	}


	//----------------
	//  �P�x���z�̌v�Z
	//----------------
	float rt = L/R;
	if (rt > 1.0)  { rt = 1.0; }
	const float Th = 3.14159265 - acos(rt);		// �I�u�W�F�N�g���S���猩���\���̈�̊p�x�i�̔����j
	for (int i=0; i<=IMAX; i+=istep) {
		float f, th, z, rho;
		f   = float(i)/IMAX;
		th  = f * Th;
		z   = -cos(th);
		rho =  sin(th);
		for (int j=0; j<=JMAX; j+=jstep) {
			float x,y;
			x   = rho * cosphi[j];
			y   = rho * sinphi[j];
			const vector3f n(x,y,z);
			const vector3f r  = n;							// �v�Z�J�n�_�̈ʒu�i�\������j
			const vector3f ev = normalize( r + rc );		// ��������� r �ւ̕����x�N�g���i�����̕����j

			float I = calc_elliptical_galaxy_ray(-rc, ev);
			RGBf col = I*col0;

			r_pt[i][j] = R*n;
			F_RGB[i][j] = correct_RGB( col );
		}
	}
}




//====================================
//  draw_elliptical_galaxy
//------------------------------------
//   �\�������a�i�R�A���a�̐��{�j��
//   �X�P�[�����O������Ăяo��
//====================================
void
draw_elliptical_galaxy(const mtk_draw_struct& ds, const vector3d& rpos)
{
	const float Re = 102*1000.0; // 5.1 kpc

	//---  �\���p�v�Z�̈�  ---
	RGBf		F_RGB[IMAX+1][JMAX+1];
	vector3f	r_pt [IMAX+1][JMAX+1];

	//---  �O�p�֐��e�[�u���ƐF�̏�����(����̂݁j ---
	static RGBf col0;
	static bool first = true;
	static float cosphi[JMAX+1], sinphi[JMAX+1];
	if (first) {
		first = false;
		for (int j=0; j<=JMAX; j++) {
			float phi = float(j)/JMAX * 2 * 3.1415926535;
			cosphi[j] = cos(phi);
			sinphi[j] = sin(phi);
		}
        col0 = scale_by_max(blackbody_color(B_V_to_Tc(0.92)) , true);
	}

	//---  ���_���猩���p�x�����v�Z����  ---
	GLfloat M[16];

	glPushMatrix();
		glScalef(Re,Re,Re);
	    glGetFloatv( GL_MODELVIEW_MATRIX,  M);
	glPopMatrix();

	const float LSQR  = M[12]*M[12] + M[13]*M[13] + M[14]*M[14];		// ���S�܂ł̋���
	const float RSQR  = M[0] *M[0]  + M[1] *M[1]  + M[2] *M[2];			// ���a
	const float _L = sqrt(LSQR);			// ���_�܂ł̋���
	const float _R = sqrt(RSQR);			// �\�����̔��a
	const float th  = _R/_L;
	const float thc  = 0.01;
	const float thc2 = 0.25;
	
	if (th < thc)	return;		// �����ފp������������ꍇ�͕`�悵�Ȃ�

	int istep, jstep;
	istep = jstep = thc2 / th;
	if (istep < 1) istep = 1;
	if (istep > 10) istep = 10;
	if (jstep < 1) jstep = 1;
	if (jstep > 3) jstep = 3;


	//---  �I�u�W�F�N�g���������������W�n��ݒ肷��  ---
	vector3f n, nx, ny;
	n = normalize( -vector3f(M[12], M[13], M[14]) );	// �I�u�W�F�N�g���S���猩�����_�̕���
	if (fabs(n.x) < fabs(n.y)) {
		ny = normalize(cross_product(n, vector3f(1,0,0)));
		nx = normalize(cross_product(ny, n));
	} else {
		nx = normalize(cross_product(vector3f(0,1,0), n));
		ny = normalize(cross_product(n, nx));
	}
	M[0] = nx.x;	M[1] = nx.y;	M[2]  = nx.z;	M[3]  = 0.0;
	M[4] = ny.x;	M[5] = ny.y;	M[6]  = ny.z;	M[7]  = 0.0;
	M[8] = n.x;		M[9] = n.y;		M[10] = n.z;	M[11] = 0.0;

	glPushMatrix();
	glLoadMatrixf(M);

	const vector3f rc = vector3f(0,0,-_L)/_R;


	//----------------
	//  �P�x���z�̌v�Z
	//----------------
	if (_L > 1.05 * _R)	{ calc_elliptical_galaxy_outer(_L, _R, rc, col0, F_RGB, r_pt, istep, jstep); }
	else				{ calc_elliptical_galaxy_inner(_L, _R, rc, col0, F_RGB, r_pt, istep, jstep); }


	//-------
	//  �`��
	//-------
	glDepthMask(GL_FALSE);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glBlendFunc(GL_ONE, GL_ONE);


	//---  �\�����̕`��  ---
	
	//-- ����
	glBegin(GL_TRIANGLE_FAN);
		{
			const RGBf&  F = F_RGB[0][0];
			glColor3f(F.r,F.g,F.b);
			glVertex3fv( r_pt[0][0].get_pointer() );
		}

		for (int j=0; j<=JMAX-jstep; j+=jstep) {
			const RGBf&  F = F_RGB[istep][j];
			glColor3f(F.r,F.g,F.b);
			glVertex3fv( r_pt[istep][j].get_pointer() );
		}
		{
			const RGBf&  F = F_RGB[istep][0];
			glColor3f(F.r,F.g,F.b);
			glVertex3fv( r_pt[istep][0].get_pointer() );
		}
	glEnd();
	

	//-- �����ȊO
	for (int i=istep; i<=IMAX-istep; i+=istep) {
		glBegin(GL_TRIANGLE_STRIP);
			for (int j=0; j<=JMAX-jstep; j+=jstep) {
				const RGBf&  F = F_RGB[i][j];
				glColor3f(F.r,F.g,F.b);
				glVertex3fv( r_pt[i][j].get_pointer() );

				const RGBf&  F2 = F_RGB[i+istep][j];
				glColor3f(F2.r,F2.g,F2.b);
				glVertex3fv( r_pt[i+istep][j].get_pointer() );
			}
			{
				const RGBf&  F = F_RGB[i][0];
				glColor3f(F.r,F.g,F.b);
				glVertex3fv( r_pt[i][0].get_pointer() );

				const RGBf&  F2 = F_RGB[i+istep][0];
				glColor3f(F2.r,F2.g,F2.b);
				glVertex3fv( r_pt[i+istep][0].get_pointer() );
			}
		glEnd();
	}
	
	glPopMatrix();

	/***** �ꎞ�I�ɖ�����
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	draw_M87_GC(ds, rpos, _R/_L);
	*****/

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_TRUE);
}



//-------------
//  Find Root
//-------------
inline  double
rtbis(double (*func)(double), double x1, double x2, double xacc)
{
	const int JMAX = 40;
	int j;
	double dx, f, fmid, xmid, rtb;

	f = (*func)(x1);
	fmid = (*func)(x2);
	if (f*fmid >= 0.0) return 0;
	rtb = f < 0.0  ?  (dx=x2-x1,x1)  :  (dx=x1-x2,x2);
	for (j=1; j<=JMAX; j++) {
		fmid = (*func)(xmid=rtb+(dx *= 0.5));
		if (fmid <= 0.0) rtb=xmid;
		if (fabs(dx)<xacc || fmid==0.0) return rtb;
	}
	return 0.0;
}


//---------------------
//  M87 �̎���̋��󐯒c
//---------------------
double g_A, g_y, g_I0;

double
fGC(double x)
{
	return g_y - g_A * ((1/x + log(x)) - g_I0);
}

vector3d make_a_GC(const double& r1, const double& r2, const double& rt, const double& rmax,
				   const double& p1,const double& C1, const double& C2)
{
	float phi = rndUni(0,2*_pi);
	float mu  = rndUni(-1,1);
	float x,y,z,rho;


	double r;
	if (rnd() < p1) {
		g_A = 1/C1;
		g_I0 = 1.0;
		g_y = rnd();
		r = r1 * rtbis(fGC, 0, rt/r1, 1e-8);
	} else {
		double zt = rt/r2;
		g_A = 1/C2;
		g_I0 = 1.0/(1+zt) + log(1+zt);
		g_y = rnd();
		r = r2 * rtbis(fGC, zt, rmax/r2, 1e-8);
	}

	z   = r*mu;
	rho = r*sqrt(1-mu*mu);
	x   = rho*cos(phi);
	y   = rho*sin(phi);

	return  vector3d(x,y,z);
}



void
make_M87_GC(const double& r1, const double& r2, const double& rt, const double& rmax, vector3d* p, int N)
{
	const double xt = rt/r1;
	const double zt = rt/r2;
	const double zmax = rmax/r2;
	const double C1 = 1/(1+xt) + log(1+xt) - 1;
	const double C2 = (1/(1+zmax) + log(1+zmax)) - (1/(1+zt) + log(1+zt));
	const double r12 = r1/r2;
	const double R  = r12*(1+zt)/(1+xt);
	const double p1 = 1/(1 + R/(r12*r12*r12)*C2/C1);

	for (int i=0; i<N; i++) {
		p[i] = 1000.0 * make_a_GC(r1, r2, rt, rmax, p1, C1, C2);
	}
}


void
init_M87_GC()
{
	const double rb1 = 20.5;
	const double rb2 = 560.0;
	const double rbt = 95;
	const double rr1 = 3.3;
	const double rr2 = 560;
	const double rrt = 125;
	const double rmax = 130;
	rndInit(1234);
	// Blue
	GCBcol = scale_by_max(blackbody_color(B_V_to_Tc(0.80)) , true);
	make_M87_GC(rb1, rb2, rbt, rmax, GCB, N_blue);
	// Red
	GCRcol = scale_by_max(blackbody_color(B_V_to_Tc(1.20)) , true);
	make_M87_GC(rr1, rr2, rrt, rmax, GCR, N_red);
}



void
draw_M87_GC(const mtk_draw_struct& ds, const vector3d& rpos, const float fac)
{
	float sz = fac / 1.1;
	float lv = 1.0;

	sz *= 1.0 / (ds.rad_per_pixel / 0.001);
	if (sz < 1) {
		lv = sz*sz;
		sz = 1.0f;
	}
	if (sz > 2) {
		sz = 2;
	}
	if (lv < 0.01) return;

	glPointSize(sz);



	//---  �J�������W�n�̎�
	vector3d  eF, eU, eR;
	ds.camera.get_axes(eF, eU, eR);
	const float a = 1.0 / get_screen_z() * get_screen_w() / 180;
	const vector3f eUf = vector3f( eU );


	//--------
	//  Blue
	//--------
	RGBf col = correct_RGB(lv*GCBcol);
	glColor3fv(col.get_pointer());


	if (true) {
		// QUAD

		glBegin(GL_QUADS);
		for (int i=0; i<N_blue; i++) {
			vector3f  rr = GCB[i];

			//---  �l�p���W�n�̎�
			vector3f  eF2, eU2, eR2;
			float R2, R;
			R2 = norm2(rr);
			if (R2 < 1e-100)  continue;
			
			R = sqrt(R2);
			eF2 = rr / R;
			eR2 = normalize( cross_product(eF2, eUf) );
			eU2 = cross_product( eR2, eF2 );

			float sz;
			sz = a * R;

			vector3f  r = rr - 0.5f*sz*eR2 - 0.5f*sz*eU2;
			glVertex3fv( r.get_pointer() );
			r += sz * eR2;
			glVertex3fv( r.get_pointer() );
			r += sz * eU2;
			glVertex3fv( r.get_pointer() );
			r -= sz * eR2;
			glVertex3fv( r.get_pointer() );
		}
		glEnd();
	} else {
		glBegin(GL_POINTS);
		for (int i=0; i<N_blue; i++) {
			glVertex3dv(GCB[i].get_pointer());
		}
		glEnd();
	}


	//-------
	//  Red
	//-------
	col = correct_RGB(lv*GCRcol);
	glColor3fv(col.get_pointer());


	if (true) {
		// QUAD

		glBegin(GL_QUADS);
		for (int i=0; i<N_red; i++) {
			vector3f  rr = GCR[i] + rpos;

			//---  �l�p���W�n�̎�
			vector3f  eF2, eU2, eR2;
			float R2, R;
			R2 = norm2(rr);
			if (R2 < 1e-100)  continue;
			
			R = sqrt(R2);
			eF2 = rr / R;
			eR2 = normalize( cross_product(eF2, eUf) );
			eU2 = cross_product( eR2, eF2 );

			float sz;
			sz = a * R;

			vector3f  r = rr - 0.5f*sz*eR2 - 0.5f*sz*eU2;
			glVertex3fv( r.get_pointer() );
			r += sz * eR2;
			glVertex3fv( r.get_pointer() );
			r += sz * eU2;
			glVertex3fv( r.get_pointer() );
			r -= sz * eR2;
			glVertex3fv( r.get_pointer() );
		}
		glEnd();
	} else {
		glBegin(GL_POINTS);
		for (int i=0; i<N_red; i++) {
			glVertex3dv(GCR[i].get_pointer());
		}
		glEnd();
	}
}
