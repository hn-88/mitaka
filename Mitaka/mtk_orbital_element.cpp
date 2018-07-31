/**************************************************************************************************

Copyright (c) 2007, 2014   Tsunehiko Kato  and  4D2U Project, National Astronomical Observatory of Japan
Copyright (c) 2013   Tsunehiko Kato

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
#include  "mtk_orbital_element.h"
#include  "../common/astro/cele_mech.h"
#include  "../common/astro/astro_const.h"






orbital_elements::orbital_elements(const double& epoch, const double& a, const double& e, const double& M0, const double& n,
					 const double& node,  const double& i, const double& omg)
{
	set_parameters(epoch, a, e, M0, n, node, i, omg);
}


void
orbital_elements::set_parameters(const double& epoch, const double& a, const double& e, const double& _M0, const double& n,
								 const double& node,  const double& i, const double& omg)
{
	double M0;
	
	M0 = _M0;
	if (fabs(e) < 1 && M0 > 180.0) { M0 -= 360.0; }

	m_epoch	= epoch;
	m_a		= a;
	m_e		= e;
	m_M0	= M0;
	m_n		= n;
	
	// �ϊ��s��
	m_MT = Rzxz3(D2R*node, D2R*i, D2R*omg);
}


// �w�肵�������ł̈ʒu
vector3d
orbital_elements::calc_pos(const double& t)
{
	return  m_MT * calc_orbital_pos(m_a, m_e, m_M0, m_n, m_epoch, t);
}


// �w�肵�������ł̈ʒu�Ƒ��x
void
orbital_elements::calc_pos_and_velocity(const double& t, vector3d &pos, vector3d &vel)
{
	vector3d x, v;

	x = calc_orbital_pos(m_a, m_e, m_M0, m_n, m_epoch, t);
	v = calc_orbital_velocity(x, m_a, m_e, m_n);

	pos = m_MT * x;
	vel = m_MT * v;
}


// �w�肵�������ł̑��x
vector3d
orbital_elements::calc_velocity(const double& t)
{
	vector3d x;

	x = calc_orbital_pos(m_a, m_e, m_M0, m_n, m_epoch, t);

	return  m_MT * calc_orbital_velocity(x, m_a, m_e, m_n);
}


void
orbital_elements::update_pos_and_velocity(const double& t)
{
	calc_pos_and_velocity(t, m_pos, m_vel);
}



// �O����̈ʒu
vector3d
orbital_elements::calc_orbit_pos(const double& phi) const
{
	return  m_MT * ::calc_orbital_position(m_a, m_e, phi);
}



// �ʒu�Ƒ��x����O���v�f���v�Z����
void
set_orbital_elements(vector3d x, vector3d v, double t0, double mu, orbital_elements &oe)
{
	vector3d  er, eth, vperp;
	double    r, vr, vth, lmd, alp, bet, e, phi, a, n, u, M0;
	double GM;
	matrix3d A;

	GM = 2.95912208e-4 * mu;

	er = normalize(x);
	vr = dot_product(v, er);
	vperp = v - vr*er;
	vth = norm(vperp);
	eth = normalize(vperp);

	r = norm(x) / _1AU_pc;	// in AU
	vr /= _1AU_pc;
	vth /= _1AU_pc;

	lmd = r*r*vth*vth / fabs(GM);
	alp = lmd / r;
	bet = vr / vth;


	if (GM >= 0) {
		e = sqrt(alp*alp*bet*bet + (alp - 1)*(alp - 1));
		phi = atan2(alp*bet, alp - 1);
	}
	else {
		e = sqrt(alp*alp*bet*bet + (alp + 1)*(alp + 1));
		phi = atan2(alp*bet, alp + 1);
	}


	if (e > 1.0) {
		double z;
		a = lmd / (e*e - 1.0);
		z = r*sin(phi) / (a*sqrt(e*e - 1));
		u = log(z + sqrt(z*z + 1));
		if (GM >= 0) {
			M0 = e*sinh(u) - u;
		}
		else {
			M0 = e*sinh(u) + u;
		}
	}
	else {
		a = lmd / (1.0 - e*e);
		u = atan2(r / (a*sqrt(1 - e*e)) * sin(phi), r*cos(phi) / a + e);
		M0 = u - e*sin(u);
	}
	n = sqrt(fabs(GM) / (a*a*a)) * 180 / M_PI;

	if (GM<0) { e = -e; }
	oe.set_parameters(t0, a, e, M0*R2D, n, 0, 0, -phi*R2D);

	A.set(er.x, eth.x, 0, er.y, eth.y, 0, er.z, eth.z, 0);
	oe.mult_MT(A);
}
