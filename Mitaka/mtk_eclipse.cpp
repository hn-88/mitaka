/**************************************************************************************************

Copyright (c) 2007, 2015-2016   Tsunehiko Kato  and  4D2U Project, National Astronomical Observatory of Japan

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
#include  "mtk_gl_common.h"
#include  "mtk_eclipse.h"
#include  "mtk_triangle_mesh.h"
#include  "mtk_mode.h"
#include  <vector>
using namespace std;


static eclipse_vertex g_vVtx[TRIANGLE_VTX_MAX];




vector3d
eclipse_get_shadow_center(const vector3d& r1, const double& R1, const vector3d& r2, const vector3d& rs)
{
	vector3d nv = normalize(r2 - rs);
	vector3d _r1 = r1 - r2;

	double b = -dot_product(nv, _r1);
	double c = norm2(_r1) - R1*R1;
	double D = b*b - c;

	if (D <= 0.0)  return vector3d(0,0,0);

	double t = -b - sqrt(D);


	return  t*nv + r2;
}

//********** *********
// r1, R1: �e�𗎂Ƃ����V��
// r2, R2: �e�𗎂Ƃ��V��
// rs, Rs: ���z
eclipse_info
calc_eclipse_info(const vector3d& r1, const double& R1, const vector3d& r2, const double& R2, const vector3d& rs, const double& Rs)
{
	eclipse_info  eci;
	eci.bFlg = false;

	// r2 ���� r1 �̕������z�ɋ߂��ꍇ�̓X�L�b�v
	if (norm2(r1 - rs) <= norm2(r2 - rs))  return eci;

	// r2��ʂ�����̕���
	vector3d nv = normalize(r2 - rs);
	double L2s = norm(r2 - rs);

	// �{�e�R�[���̎n�_�����߂�
	double L2umb = (R2 * L2s) / (Rs - R2);
	vector3d r_umb = L2umb * nv + r2;
	double sinth_umb = R2 / L2umb;
	double costh_umb = sqrt(1.0 - sinth_umb*sinth_umb);

	// ���e�R�[���̎n�_rh�����߂�
	double LB = L2s / (1.0 + R2/Rs);
	double LA = L2s - LB;
	vector3d r_pen = LB * nv + rs;
	double sinth_pen = R2 / LA;
	double costh_pen = sqrt(1.0 - sinth_pen*sinth_pen);

	// r1 �ƍł��߂����~�R�[����̓_�Ƃ̋��� l �����߂�
	double L1h_sqr = norm2( r1 - r_pen );
	double a = dot_product(r1-r_pen, nv);
	double l = sqrt(L1h_sqr - a*a)*costh_pen - a*sinth_pen;
	if (l > R1)  return eci;	// �R�[���̈�̊O��

	// z ���� nv �����Ƃ�����W�n�����
	vector3d n1, n2;
	n1 = get_a_normal_unit_vector(nv);
	n2 = cross_product(nv, n1);
	eci.bFlg = true;
	eci.r_umb = r_umb;
	eci.r_pen = r_pen;
	eci.nv = nv;
	eci.n1 = n1;
	eci.n2 = n2;
	eci.sinth_umb = sinth_umb;
	eci.costh_umb = costh_umb;
	eci.sinth_pen = sinth_pen;
	eci.costh_pen = costh_pen;

	return  eci;
}

// ���z���B����Ă��銄���i���S�ɉB����Ă���Ƃ��ɂP�j
double
calc_eclipse_fraction(double R, double Dlt)
{
	if (R > 1.0) {
		// �B���V�̂̎����a�̂ق������z�̎����a�����傫���ꍇ
		if (Dlt < R - 1.0)	return  1.0;	// �{�e
		if (Dlt > R + 1.0)	return  0.0;	// �͈͊O
	} else {
		// �B���V�̂̎����a�̂ق������z�̎����a�����������ꍇ
		if (Dlt > R + 1.0)	return 0.0;		// �d�Ȃ�Ȃ�
		if (Dlt < 1.0 - R)	return R*R;		// �ő�̉�����
	}

	// �����I�ɉB���i���e�j�̏ꍇ�iR > 1, R < 1 �ŋ��ʁj
	double a, b, th1, th2;
	a = (Dlt*Dlt + R*R - 1.0) / (2.0 * Dlt);
	b = (Dlt*Dlt - R*R + 1.0) / (2.0 * Dlt);
	th1 = atan2( sqrt(R*R - a*a), a );
	th2 = atan2( sqrt(1.0 - b*b), b );

	return (R*R*th1 - a* sqrt(R*R - a*a) + th2 - b * sqrt(1.0 - b*b)) / M_PI;
}


// �n�\�ɗ������e�p�̃|���S�������
int
make_eclipse_shadow_polygon(const vector3d& r1, const double& R1, const vector3d& nz, const float f,
							const vector3d& r2, const double& R2,
							const vector3d& rs, const double& Rs,
							const int N1, const int N2, eclipse_vertex* vVtx)
{
	int vn = 0;
	clear_triangle_vtx();

	eclipse_info  eci;
	eci = calc_eclipse_info(r1, R1, r2, R2, rs, Rs);
	if (!eci.bFlg)  return 0;

	double tn_umb, tn_pen;
	vector3d r_umb, r_pen, nv, n1, n2, r1pen, _r1pen;

	tn_umb = eci.sinth_umb / eci.costh_umb; 
	tn_pen = eci.sinth_pen / eci.costh_pen;
	r_umb = eci.r_umb;
	r_pen = eci.r_pen;
	nv = eci.nv;
	n1 = eci.n1;
	n2 = eci.n2;
	r1pen = r_pen - r1;

	double alp = 1.0 / (1.0 - f);
	double az = (1.0 - alp);
	_r1pen = r1pen - az*dot_product(nz, r1pen) * nz;	// z �����ɏk���������e�R�[���̎n�_�̈ʒu

	for (int i=0; i<N1; i++) {
		double tn;
		double u = float(i) / (N1-1);
		tn = tn_pen * (u*u);
		
		for (int j=0; j<N2; j++) {
			if (i==0 && j!=0)  break;

			double th_j = j*2.0*M_PI/N2 + 0.00001;
			double cs = cos(th_j);
			double sn = sin(th_j);
			double x, y;
			x = tn * cs;
			y = tn * sn;

			// �R�[����̐ڐ��̕����x�N�g��
			vector3d nt  = normalize( nv + x*n1 + y*n2 );
			vector3d _nt = nt - az*dot_product(nz, nt) * nz;

			// �R�[����̐ڐ��Ƌ��Ƃ̌�_�����߂�
			double _a = norm2(_nt);
			double b = dot_product(_nt, _r1pen) / _a;
			double c = (norm2(_r1pen) - R1*R1) / _a;
			double D = b*b - c;
			if (D <= 0.0)  continue;
			double t = -b - sqrt(D);
			vector3d  pos = t*nt + r_pen;			// �������W�n�ł̈ʒu

			// �����a
			double thm, ths, R;
			thm = asin( R2 / norm(pos - r2) );
			ths = asin( Rs / norm(pos - rs) );
			R = thm/ths;

			// ���S�Ԃ̋���
			double Dlt;
			vector3d nm = normalize( r2 - pos );
			vector3d ns = normalize( rs - pos );
			Dlt = acos( dot_product(nm, ns) ) / ths;

			// ���������v�Z����
			double f = calc_eclipse_fraction(R, Dlt);
			if (f <= 0.0)  continue;
			vVtx[vn].pos = pos;
			vVtx[vn].f   = f;
			add_triangle_xy(x, y);

			vn++;
		}
	}

	// z ���� n1s �����Ƃ�����W�n�����
	vector3d n1s, n1s1, n1s2;
	n1s  = normalize(rs - r1);
	n1s1 = get_a_normal_unit_vector(n1s);
	n1s2 = cross_product(n1s, n1s1);
	double L1s = norm(r1 - rs);
	double th_max = acos(R1 / L1s);

	const int N3 = 20;
	const int N4 = 3*N3;
	for (int i=0; i<N3; i++) {
		double u, th, rho, z;
		u = float(i) / (N3-1);
		th = th_max * u;
		rho = R1 * sin(th);
		z   = R1 * cos(th);

		for (int j=0; j<N4; j++) {
			if (i==0 && j!=0)  break;
			double th_j = j*2.0*M_PI/N4 + 0.00001;
			double cs = cos(th_j);
			double sn = sin(th_j);
			double x, y;
			x = rho * cs;
			y = rho * sn;
			vector3d  pos = x*n1s1 + y*n1s2 + z*n1s;

			// ���]��������Η��ɉ����ďk������
			pos = pos - f * dot_product(nz, pos) * nz;
			pos = pos + r1;

			// �����a
			double thm, ths, R;
			thm = asin( R2 / norm(pos - r2) );
			ths = asin( Rs / norm(pos - rs) );
			R = thm/ths;

			// ���S�Ԃ̋���
			double Dlt;
			vector3d nm = normalize( r2 - pos );
			vector3d ns = normalize( rs - pos );
			Dlt = acos( dot_product(nm, ns) ) / ths;

			// ���������v�Z����
			double frac = calc_eclipse_fraction(R, Dlt);
			if (frac <= 0.0)  continue;
			vVtx[vn].pos = pos;
			vVtx[vn].f   = frac;

			// �R�[�����W�ɕϊ�
			float _x, _y;
			vector3d _pos = pos - r_pen;
			_x = dot_product(_pos, n1) / dot_product(_pos, nv);
			_y = dot_product(_pos, n2) / dot_product(_pos, nv);
			add_triangle_xy(_x, _y);

			vn++;
		}
	}

	if (vn < 3) return 0;
	int nTri = triangulation();
	if (nTri <= 0)  return 0;

	return  nTri;
}



//--------------------------
//  �f���E�q���̉e��`��
//--------------------------
// r1, R1: �e�𗎂Ƃ����V��
// r2, R2: �e�𗎂Ƃ��V��
// rs, Rs: ���z
//--------------------------
void
draw_eclipse_shadow(const mtk_draw_struct& ds,
					const vector3d& r1, const double& R1,
					const vector3d& nz, const float f,		// �e�𗎂Ƃ����V�̂̎������ƕΗ�
					const vector3d& r2, const double& R2,
					const vector3d& rs, const double& Rs,
					const matrix3d& M,						// �������W�n����f�����W�n�ւ̕ϊ��s��
					const float th_c,						// �e���ȗ�����p�x(rad)
					const int N1, const int N2,				//
					bool bShadow, bool bBorder,
					const vector3f& shd_col,				// �{�e���̌��̐F�i���݈̂Â��ԁj
					const double& Rc_pl)					// �v���l�^���E�����[�h���̓��e���a
{
	vector3d eF, eU, eR;
	quaterniond ori0 = ds.camera_base.get_ori() * get_base_camera_ori();
	quaternion_to_axes(ori0, eF, eU, eR);
	vector3d r_cam = get_local_camera_pos();
	const vector3d re = ds.camera.get_pos() + ( r_cam.x*eR + r_cam.y*eU - r_cam.z*eF)/ ds.camera.get_scale_factor();	// ���_�̈ʒu

	const double   R = R1;					// �f�����a
	const double   L = norm(re - r1);		// ���_�Ƙf�����S�̋���
	if (R >= L)			return;

	const double th_view = asin(R / L);
	if (th_view < th_c)  return;

	eclipse_info  eci;
	eci = calc_eclipse_info(r1, R1, r2, R2, rs, Rs);
	if (!eci.bFlg)  return;

	double costh_umb, sinth_umb, costh_pen, sinth_pen;
	vector3d r_umb, r_pen, nv, n1, n2;
	
	r_umb = eci.r_umb;
	r_pen = eci.r_pen;
	costh_umb = eci.costh_umb;
	sinth_umb = eci.sinth_umb;
	costh_pen = eci.costh_pen;
	sinth_pen = eci.sinth_pen;
	nv = eci.nv;
	n1 = eci.n1;
	n2 = eci.n2;
	vector3d _r1  = r1 - r2;


	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glPushMatrix();
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	//---  �e�̕\��  ---
	if (bShadow) {
		int nTri = make_eclipse_shadow_polygon(r1, R1, nz, f, r2, R2, rs, Rs, N1, N2, g_vVtx);
		if (nTri > 0) {
			Triangle* vTri = get_triangle_list();
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glBlendFunc(GL_ZERO, GL_SRC_COLOR);

			float fR, fG, fB;
			fR = 1.0f - shd_col.r;
			fG = 1.0f - shd_col.g;
			fB = 1.0f - shd_col.b;
			
			glBegin(GL_TRIANGLES);
			for (int i=0; i<nTri; i++) {
				const Triangle& tri = vTri[i];
				for (int j=0; j<3; j++) {
					int idx = tri.p[2-j];

					vector3d pos3;
					if (ds.tyMode == MODE_3D) {
						vector3d  pos2 = (g_vVtx[idx].pos - r1) / R1;	// �e�𗎂Ƃ����V�̂̒��S�����_�Ƃ����ʒu
						pos3 = M * pos2;
					} else {
						pos3 = Rc_pl * (g_vVtx[idx].pos - ds.camera.get_pos()).normalized();
					}

					double alp = g_vVtx[idx].f;

					float Rs, Gs, Bs;
					Rs = 1.0f - fR*alp;
					Gs = 1.0f - fG*alp;
					Bs = 1.0f - fB*alp;



					glColor3f(Rs,Gs,Bs);
					glVertex3dv( pos3.get_pointer() );
				}
			}
			glEnd();
		}
	}

	//---  ���E���̕\��  ---
	if (bBorder) {
		const double fac = 1.004;
		const int Nb_fac = (R1 > R2)  ?  1  :  int(R2/R1);

		vector3d r1umb, _r1umb;
		vector3d r1pen, _r1pen;
		r1umb = r_umb - r1;
		r1pen = r_pen - r1;
		double alp = 1.0 / (1.0 - f);
		double az = (alp - 1.0);
		_r1umb = r1umb + az*dot_product(nz, r1umb) * nz;	// z �����ɏk�������{�e�R�[���̎n�_�̈ʒu
		_r1pen = r1pen + az*dot_product(nz, r1pen) * nz;	// z �����ɏk���������e�R�[���̎n�_�̈ʒu

		// �{�e�̈�̋��E��
		bool bTotalEclipse = (-dot_product(r1umb, nv) < R1);
		const int Nb1 = 100 * Nb_fac;
		const double c0 = norm2(_r1umb) - R1*R1;
		double tn_umb = sinth_umb / costh_umb;
		glColor3f(1,0.5,0);
		glLineWidth(1.5);

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);


		if (bTotalEclipse)	{ glBegin(GL_LINE_STRIP); }
		else				{ glBegin(GL_LINES); }
		for (int i=0; i<=Nb1; i++) {
			double cs = cos(i*2*3.14159/Nb1);
			double sn = sin(i*2*3.14159/Nb1);

			// �R�[����̐ڐ��̕����x�N�g��
			vector3d nt = normalize( -nv + tn_umb*cs*n1 + tn_umb*sn*n2 );

			// �R�[����̐ڐ��Ƌ��Ƃ̌�_�����߂�
			vector3d _nt = nt + az*dot_product(nz, nt) * nz;
			double _a = norm2(_nt);
			double b = dot_product(_nt, _r1umb) / _a;
			double c = c0 / _a;
			double D = b*b - c;
			if (D <= 0.0)  continue;
			double t = -b + sqrt(D);

			vector3d  pos1 = t*nt + r_umb;				// �������W�n�ł̈ʒu

			vector3d pos3;
			if (ds.tyMode == MODE_3D) {
				vector3d  pos2 = fac * (pos1 - r1)/R1;		// �e�𗎂Ƃ����V�̂̒��S�����_�Ƃ����ʒu
				pos3 = M * pos2;
			}
			else {
				pos3 = Rc_pl * (pos1 - ds.camera.get_pos()).normalized();
			}




			glVertex3dv( pos3.get_pointer() );
		}
		glEnd();


		// ���e�̈�̋��E��
		const int Nb2 = 200 * Nb_fac;
		double tn_pen = sinth_pen / costh_pen;
		glColor3f(1,0.5,0);
		glLineWidth(1.5);
		glBegin(GL_LINES);
		for (int i = 0; i <= Nb2; i++) {
			double cs = cos(i*2*3.14159/Nb2);
			double sn = sin(i*2*3.14159/Nb2);

			// �R�[����̐ڐ��̕����x�N�g��
			vector3d nt = normalize( nv + tn_pen*cs*n1 + tn_pen*sn*n2 );

			// �R�[����̐ڐ��Ƌ��Ƃ̌�_�����߂�
			vector3d _nt = nt + az*dot_product(nz, nt) * nz;
			double _a = norm2(_nt);
			double b = dot_product(_nt, _r1pen) / _a;
			double c = (norm2(_r1pen) - R1*R1) / _a;
			double D = b*b - c;
			if (D <= 0.0)  continue;
			double t = -b - sqrt(D);
			vector3d  pos1 = t*nt + r_pen;				// �������W�n�ł̈ʒu

			vector3d pos3;
			if (ds.tyMode == MODE_3D) {
				vector3d  pos2 = fac * (pos1 - r1) / R1;		// �e�𗎂Ƃ����V�̂̒��S�����_�Ƃ����ʒu
				pos3 = M * pos2;
			}
			else {
				pos3 = Rc_pl * (pos1 - ds.camera.get_pos()).normalized();
			}

			glVertex3dv( pos3.get_pointer() );
		}
		glEnd();


		// ���S�_
		{
			// �R�[����̐ڐ��Ƌ��Ƃ̌�_�����߂�
			vector3d _nt = nv + az*dot_product(nz, nv) * nz;
			double _a = norm2(_nt);
			double b = dot_product(_nt, _r1pen) / _a;
			double c = (norm2(_r1pen) - R1*R1) / _a;
			double D = b*b - c;

			if (D > 0.0) {
				double t = -b - sqrt(D);
				vector3d  pos1 = t*nv + r_pen;					// �������W�n�ł̈ʒu

				vector3d pos3;
				if (ds.tyMode == MODE_3D) {
					vector3d  pos2 = fac * (pos1 - r1) / R1;		// �e�𗎂Ƃ����V�̂̒��S�����_�Ƃ����ʒu
					pos3 = M * pos2;
				}
				else {
					pos3 = Rc_pl * (pos1 - ds.camera.get_pos()).normalized();
				}


				glPointSize(3.0);
				glColor3f(1, 0, 0);
				glBegin(GL_POINTS);
					glVertex3dv( pos3.get_pointer() );
				glEnd();
			}
		}
	}

	glPopMatrix();
	glPopAttrib();
}
