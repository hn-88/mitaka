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
#include  "mtk_planet_orientation.h"
#include  "../common/astro/astrometry.h"
#include  "mtk_planet.h"
#include  "mtk_planets.h"
#include  "mtk_planet_position.h"
#include  "mtk_satellites.h"
#include  "mtk_object_orientation.h"





const matrix3d&
get_planet_orientation(int idx, bool bRotation)
{
	const planet& pnt = get_planet(idx);
	return get_orientation(pnt.get_orientation_index(), bRotation);
}






//----------------------------------------
//  �f���̎��]�������߂�i�Ԍo�E�Ԉ܁j Ref[2]
//----------------------------------------
void
calc_planet_orientation_angle(int idx, const double& t, double& alp, double& dlt, double& W)
{
	const double  JD0 = 2451545.0;	// ���� 2000/1/1.5
	const double  d = t - JD0;
	const double  T = d/36525.0;	// in Julian centuries

	switch (idx) {
		case 0:	// ����
			double M1, M2, M3, M4, M5;
			M1 = 174.791086 + 4.092335 * d;
			M2 = 349.582171 + 8.184670 * d;
			M3 = 164.373257 + 12.277005 * d;
			M4 = 339.164343 + 16.369340 * d;
			M5 = 153.955429 + 20.461675 * d;
			alp = 281.0097 - 0.0328*T;
			dlt = 61.4143  - 0.0049*T;
			W   = 329.5469	+ 6.1385025*d
							+ 0.00993822 * sin(M1 * D2R)
							- 0.00104581 * sin(M2 * D2R)
							- 0.00010280 * sin(M3 * D2R)
							- 0.00002364 * sin(M4 * D2R)
							- 0.00000532 * sin(M5 * D2R);
			break;
		case 1:	// ����
			alp = 272.76;
			dlt = 67.16;
			W   = 160.20 - 1.4813688*d;
			break;
		case 2:	// �n��
			alp = 0.00 - 0.641*T;
			dlt = 90.00 - 0.557*T;
			W   = 190.147 + 360.9856235*d;
			break;
		case 3:	// �ΐ�
			alp = 317.68143 - 0.1061*T;
			dlt = 52.88650  - 0.0609*T;
			W   = 176.630   + 350.89198226*d;
			break;
		case 4:	// �ؐ�
			double Ja, Jb, Jc, Jd, Je;
			Ja =  99.360714 + 4850.4046 * T;
			Jb = 175.895369 + 1191.9605 * T;
			Jc = 300.323162 + 262.5475 * T;
			Jd = 114.012305 + 6070.2476 * T;
			Je =  49.511251 + 64.3000 * T;

			alp = 268.056595 - 0.006499 * T + 0.000117 * sin(Ja * D2R)
											+ 0.000938 * sin(Jb * D2R)
											+ 0.001432 * sin(Jc * D2R)
											+ 0.000030 * sin(Jd * D2R)
											+ 0.002150 * sin(Je * D2R);
			dlt = 64.495303 + 0.002413 * T	+ 0.000050 * cos(Ja * D2R)
											+ 0.000404 * cos(Jb * D2R)
											+ 0.000617 * cos(Jc * D2R)
											- 0.000013 * cos(Jd * D2R)
											+ 0.000926 * cos(Je * D2R);
			W   = 284.95 + 870.5360000*d;
			break;
		case 5:	// �y��
			alp = 40.589 - 0.036*T;
			dlt = 83.537 - 0.004*T;
			W   = 38.90  + 810.7939024*d;
			break;
		case 6:	// �V����
			alp = 257.311;
			dlt = -15.175;
			W   = 203.81 - 501.1600928*d;
			break;
		case 7:	// �C����
			{
				double N, th, sinN;
				N = 357.85 + 52.316*T;
				th = N*D2R;
				sinN = sin(th);
				alp = 299.36 + 0.70*sinN;
				dlt = 43.46 - 0.51*cos(th);
				W   = 253.18 + 536.3128492*d - 0.48*sinN;
			}
			break;
		case 8:	// ������ [2]+[3]
			alp = 132.993;
			dlt = -6.163;
			W   = 302.695 + 56.3625225*d;
			break;
		default:
			alp = dlt = W = 0;
			break;
	}
}


//------------------------------------------
//  �������W�n����f�����W�n�֕ϊ�����s��𓾂�
//------------------------------------------

//---  Lieske 1977  ---
void
calc_Earth_ec_to_pl_matrix_Lieske1977(const double& t, matrix3d& M0, matrix3d& M)
{
	const double JD0 = 2451545.0;	// ���� 2000/1/1.5
	const double a1 = 2306.2181; 
	const double a2 = 0.30188;
	const double a3 = 0.017998;
	const double b1 = 2306.2181;
	const double b2 = 1.09468;
	const double b3 = 0.018203;
	const double c1 = 2004.3109;
	const double c2 = -0.42665;
	const double c3 = -0.041833;
	const double d1 = -46.8150;
	const double d2 = -0.00059;
	const double d3 = 0.001813;
	double d, T, T2, T3, zetaA, zA, thA, epsA, GMST;

	d  = t - JD0;
	T  = d/36525.0;		// time in Julian centuries
	T2 = T*T;
	T3 = T*T2;

	zetaA = (a1*T + a2*T2 + a3*T3)/3600.0;
	zA    = (b1*T + b2*T2 + b3*T3)/3600.0;
	thA   = (c1*T + c2*T2 + c3*T3)/3600.0;
	epsA  = epsJ2000 + (d1*T + d2*T2 + d3*T3)/3600.0;
	
	GMST = 360.0*(0.7790572732640 + 1.00273781191135448*d);
	//GMST   = 190.147 +90 + 360.9856235*d;

	M0 = Rzxz3deg((90.0+zA),   -thA, -(90.0-zetaA));
	M  = Rz3deg(-GMST) * M0;
}


//---  Captine et al. 2003  ---
void
calc_Earth_ec_to_pl_matrix_Captine2003(const double& t, matrix3d& M0, matrix3d& M)
{
	const double JD0 = 2451545.0;	// ���� 2000/1/1.5
	const double a1 = 5038.481507; 
	const double a2 = -1.0790069;
	const double a3 = -0.00114045;
	const double a4 = 0.000132851;
	const double a5 = -0.0000000951;
	const double b1 = -0.025754;
	const double b2 =  0.0512623;
	const double b3 = -0.00772503;
	const double b4 = -0.000000467;
	const double b5 =  0.0000003337;
	const double c1 = 10.556403;
	const double c2 = -2.3814292;
	const double c3 = -0.00121197;
	const double c4 =  0.000170663;
	const double c5 = -0.0000000560;
	const double eps0 = 84381.406 / 3600.0;
	double PhiA, omgA, chiA;
	double d, T, T2, T3, T4, T5, GMST;

	d  = t - JD0;
	T  = d/36525.0;		// time in Julian centuries
	T2 = T*T;
	T3 = T*T2;
	T4 = T2*T2;
	T5 = T4*T;

	PhiA = (a1*T + a2*T2 + a3*T3 + a4*T4 + a5*T5) / 3600.0;
	omgA = eps0 + (b1*T + b2*T2 + b3*T3 + b4*T4 + b5*T5) / 3600.0;
	chiA = (c1*T + c2*T2 + c3*T3 + c4*T4 + c5*T5) / 3600.0;

	GMST = 360.0*(0.7790572732640 + 1.00273781191135448*d);	// Captine (2000)
	//GMST = 190.147 + 90 + 360.9856235*d;						// Seidelmann (2002)
	//GMST = 360.0*(0.7790572732640 + 1.002737851*d);				// ���S�ɑ��z���Ɉ�v����ꍇ

	M0 = Rzxz3deg(-chiA,   omgA, PhiA) * Rx3deg(-eps0);
	M  = Rz3deg(-GMST) * M0;
}


void
calc_planet_ec_to_pl_matrix(int idx, const double& t, matrix3d& M0, matrix3d& M)
{
	static matrix3d  M_J2000;
	static bool first = true;
	if (first) {
		first = false;
		M_J2000 = Rx3deg(epsJ2000);	// �������W����ԓ����W(J2000)�֕ϊ�����s��
	}

	
	if (idx==2) {
		//calc_Earth_ec_to_pl_matrix_Lieske1977(t, M0, M);
		calc_Earth_ec_to_pl_matrix_Captine2003(t, M0, M);
		M  =  M * M_J2000;
		M0 = M0 * M_J2000;
		return;
	}
	

	double alp, dlt, W;
	calc_planet_orientation_angle(idx, t, alp, dlt, W);

	M0 = Rzxz3deg(0.0, -90+dlt, -90-alp) * M_J2000;
	M  = Rz3deg(-W) * M0;
}




// ���]���̌����ɍ��킹��
void
set_planet_orientation_GL(int idx)
{
	const planet& pnt = get_planet(idx);
	int ori_idx = pnt.get_orientation_index();

	set_object_orientation_GL(ori_idx);
}




//----------------------------------------
//  �������W�n�̈ʒu�x�N�g����f�����W�n�ɕϊ�����
//----------------------------------------
vector3d
ec_to_pl(int idx, const vector3d&  pos_ec)
{
	vector3d  pos_pl = pos_ec - get_planet_position(idx);	// �f�����S�ł̈ʒu

	return get_planet_orientation(idx, true) * pos_pl;
}


//------------------------------------------------
//  �f�����S����x�N�g�� nv �̕����̌o�x�E�ܓx�����߂�
//  nv �̐����͉������W(J2000)�ł̒l�Ƃ���
//------------------------------------------------
vector2d
get_planet_lon_lat_ec(int idx, const vector3d& nv)
{
	//--  �f�����W�n�̃x�N�g��
	vector3d  ex, ey, ez;
	get_planet_bases(idx, ex, ey, ez);

	//--  lon, lat �����߂�
	double x, y, z, rho;
	double th, phi;
	double lon, lat;
	x = dot_product(nv, ex);
	y = dot_product(nv, ey);
	z = dot_product(nv, ez);
	rho = sqrt(x*x + y*y);

	th  = atan2(rho, z);
	phi = atan2(y, x);

	lon = R2D * phi;
	lat = 90.0 - R2D * th;
	if (lon > 180.0) lon -= 360.0;

	return  vector2d(lon, lat);
}


//---------------------------------------------------------
//  �ܓx�E�o�x����A�f�����W�n�ł̉���������̒P�ʃx�N�g�������߂�
//---------------------------------------------------------
vector3d
get_planet_nv_pl(const double& lon, const double& lat)
{
	const double th  = D2R * (90.0 - lat);
	const double phi = D2R * lon;
	const double rho = sin(th);

	return  vector3d(rho*cos(phi), rho*sin(phi), cos(th));
}

//---------------------------------------------------------------
//  �o�x�E�ܓx����A����������̒P�ʃx�N�g�������߂�i�������W�n(J2000)�j
//---------------------------------------------------------------
vector3d
get_planet_nv_ec(int idx, const double& lon, const double& lat)
{
	return  get_planet_orientation(idx, true).transposed() * get_planet_nv_pl(lon, lat);
}


//---------------------------------------------------------
//  �v���l�^���E�����[�h�̉������W�n�ł̊���W�n�𓾂�
//---------------------------------------------------------
quaterniond
get_planetarium_ori(int idx, const double& lon, const double& lat, const double& azim, const double& alti)
{
	vector3d  nv  = get_planet_nv_ec(idx, lon, lat);
	vector3d  nnv = get_planet_nv_ec(idx, 0,   90.0);
	vector3d  ex = normalize( cross_product(nnv, nv) );	// ������
	vector3d  ey = normalize( cross_product(ex,  nv) );	// �����

	// ���ʊp
	const double  phi = D2R * azim;
	vector3d _ex, _ey;
	_ex = cos(phi) * ex - sin(phi) * ey;
	_ey = cos(phi) * ey + sin(phi) * ex;

	// ���x
	const double  th = D2R * alti;
	vector3d eF, eU;
	eF = cos(th) * _ey + sin(th) *  nv;
	eU = cos(th) *  nv - sin(th) * _ey;

	return  axes_to_quaternion(eF, eU);
}

//---------------------------------------------------------
//  �v���l�^���E�����[�h�ł̐��ʕ�������A�������W�n�ł̎p���𓾂�
//---------------------------------------------------------
quaterniond
get_ori_from_planetarium_dirc(int idx, const double& lon, const double& lat, const double& thd)
{
	vector3d  nv  = get_planet_nv_ec(idx, lon, lat);
	vector3d  nnv = get_planet_nv_ec(idx, 0, 90.0);
	vector3d  ex = normalize( cross_product(nnv, nv) );	// ������
	vector3d  ey = normalize( cross_product(ex,  nv) );	// �����

	const double  th = D2R * thd;
	vector3d  eF = cos(th) * ey + sin(th) * ex;
	quaterniond  ori = axes_to_quaternion(-nv, eF);

	return  ori;
}


//---------------------------------------------------------
//  �RD���[�h�ł̎p������A�v���l�^���E���E���[�h�̐��ʕ����𓾂�
//---------------------------------------------------------
double
get_dirc_from_3D_ori(int idx, const quaterniond& ori)
{
	vector3d  eF, eU, eR;
	quaternion_to_axes(ori, eF, eU, eR);

	vector3d  nnv = get_planet_nv_ec(idx, 0, 90.0);
	double x, y, th;
	x = dot_product(-eR, nnv);
	y = dot_product(-eU, nnv);
	th = atan2(y, x);

	return  90.0 - R2D * th;
}

