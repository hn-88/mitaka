/******************************************************************************

Copyright (c) 2007-2014   Tsunehiko Kato

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
#include  "cele_mech.h"



//-----------------------------
//  �ȉ~�O��(e<1)������
//   Landau1, (15.10),(15.11)
//-----------------------------
vector3d
calc_elliptical_orbit(const double& a, const double& e, const double& _M)
{
	const int iter_max = 100;
	double M, xi, dxi;

	M = _M;
	if (M > M_PI) { M -= 2.0*M_PI; }

	xi = M;	// first guess

	for (int i=0; i<iter_max; ++i) {
		double cs, sn;
		cs = cos(xi);
		sn = sin(xi);

		dxi = (xi - e*sn - M) / (1.0 - e*cs);
		xi -= dxi;
		
		if (fabs(dxi) < 1e-8) {
			// �O�����W�n�ł̍��W�����߂�
			double x,y;
			x = a * (cs - e);
			y = a * sqrt(1.0-e*e) * sn;
			return  vector3d(x, y, 0.0);
		}
	}

	return  vector3d(0,0,0);	// �������Ȃ��ꍇ
}


//-----------------------
//  �o�Ȑ��O��(e>1)������
//   Landau1, (15.12)
//-----------------------
vector3d
calc_hyperbolic_orbit(const double& a, const double& e, const double& _M)
{
	const int iter_max = 1000;
	double M, xi, dxi;
	double xi1, xi2, f1, f2, xim, fm;

	M = _M;
	
	xi1 = -10;
	xi2 = 10;
	f1 = e*sinh(xi1) - xi1 - M;
	f2 = e*sinh(xi2) - xi2 - M;
	for (int i=0; i<10; i++) {
		xim = 0.5 * (xi1 + xi2);
		fm = e*sinh(xim) - xim - M;
		if (f1*fm>0) {
			xi1 = xim;
			f1 = fm;
		} else {
			xi2 = xim;
			f2 = fm;
		}
	}
	xi = 0.5 * (xi1 + xi2);

	//xi = M/e;	// first guess
	for (int i=0; i<iter_max; ++i) {
		double csh, snh;
		csh = cosh(xi);
		snh = sinh(xi);

		dxi = (e*snh - xi - M) / (e*csh - 1.0);
		xi -= dxi;
		
		if (fabs(dxi) < 1e-10) {
			// �O�����W�n�ł̍��W�����߂�
			double x, y;
			x = a * (e - csh);
			y = a * sqrt(e*e - 1.0) * snh;
			return  vector3d(x, y, 0.0);
		}
	}

	return  vector3d(0,0,0);	// �������Ȃ��ꍇ
}


// �˗͂̏ꍇ
vector3d
calc_hyperbolic_orbit2(const double& a, const double& e, const double& _M)
{
	const int iter_max = 1000;
	double M, xi, dxi;
	double xi1, xi2, f1, f2, xim, fm;

	M = _M;
	
	
	xi1 = -10;
	xi2 = 10;
	f1 = e*sinh(xi1) - xi1 - M;
	f2 = e*sinh(xi2) - xi2 - M;
	for (int i=0; i<20; i++) {
		xim = 0.5 * (xi1 + xi2);
		fm = e*sinh(xim) - xim - M;
		if (f1*fm>0) {
			xi1 = xim;
			f1 = fm;
		} else {
			xi2 = xim;
			f2 = fm;
		}
	}
	xi = 0.5 * (xi1 + xi2);

	//xi = M/e;	// first guess
	for (int i=0; i<iter_max; ++i) {
		double csh, snh;
		csh = cosh(xi);
		snh = sinh(xi);

		dxi = (e*snh + xi - M) / (e*csh + 1.0);
		xi -= dxi;
		
		if (fabs(dxi) < 1e-10) {
			// �O�����W�n�ł̍��W�����߂�
			double x, y;
			x = a * (e + csh);
			y = a * sqrt(e*e - 1.0) * snh;
			return  vector3d(x, y, 0.0);
		}
	}

	return  vector3d(0,0,0);	// �������Ȃ��ꍇ
}




//----------------------------------------------
//  �O����̈ʒu�����߂�i�O�����W�n�F�œ_�����_�j
//    a:  �O�������a
//    e:  ���S��
//    M0: ���ϋߓ_���p�i�����A�x�j
//    n:  ���ω^���i�x�A�P�ʎ��Ԃ�����j
//    epoch: ����
//    t:  �ʒu�����߂鎞��
//----------------------------------------------
vector3d
calc_orbital_pos(const double& a, const double& e, const double& M0, const double& n, const JD& epoch, const JD& t)
{
	double M, M_rad;
	M = M0 + n*(t - epoch);		// ���ϋߓ_���p�i���݁j
	M_rad = M*D2R;

	if (0<=e && e<1){ return  calc_elliptical_orbit( a, e, M_rad);  }		// �ȉ~�O��
	else if (e>1)	{ return  calc_hyperbolic_orbit( a, e, M_rad);  }		// �o�ȋO��
	else if (e<-1)	{ return  calc_hyperbolic_orbit2(a,-e, M_rad); }		// �o�ȋO���i�˗́j
	else			{ return  vector3d(0,0,0);					   }		// !!!  �������͖��Ή�  !!!
}





//---------------------------------
//  �O�����W�n�ł̑��x�x�N�g�������߂�
//---------------------------------
vector3d
calc_orbital_velocity(const vector3d& pos, const double& a, const double& e, const double& n)
{
	double x, y, r, lmd, rtGM, vth, vx, vy;

	x = pos.x;
	y = pos.y;
	r = sqrt(x*x + y*y);

	if (fabs(e)>1) {
		lmd = a*(e*e - 1.0);
	} else {
		lmd = a*(1.0 - e*e);
	}

	rtGM = D2R * n * sqrt(a*a*a);
	vth = rtGM * sqrt(lmd) / r;

	vx = vth*y/r*(e*x/lmd - 1.0);
	vy = vth/r*(e*y*y/lmd + x);

	return  vector3d(vx, vy, 0);
}