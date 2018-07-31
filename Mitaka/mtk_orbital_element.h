/**************************************************************************************************

Copyright (c) 2007   Tsunehiko Kato  and  4D2U Project, National Astronomical Observatory of Japan
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
#ifndef __MTK_ORBITAL_ELEMENT_H__
#define __MTK_ORBITAL_ELEMENT_H__
#include  "../common/math/vector3.h"




//  �O����̈ʒu
inline  vector3d
calc_orbital_position(const double& a, const double& e, const double& phi)
{
	double x, y, b;
	x = cos(phi);
	y = sin(phi);
	b = a * sqrt(1.0-e*e);

	double xx, yy;
	xx = a*x - a*e;
	yy = b*y;

	return  vector3d(xx, yy, 0);
}



//--------------------
//  orbital elements
//--------------------
class orbital_elements
{
	double	m_epoch;	// epoch (JD)

    double	m_a;		// �O�������a(AU)
	double	m_e;		// �O�����S��
	double	m_M0;		// Mean anomaly; ���ϋߓ_�p
	double	m_n;		// mean daily motion�G���ϓ��X�^��

	matrix3d	m_MT;	// �O�����W�������W�ւ̕ϊ��s��
	vector3d	m_pos;
	vector3d	m_vel;

public:
	orbital_elements() {}
	orbital_elements(const double& epoch, const double& a, const double& e, const double& M0, const double& n,
					 const double& node,  const double& i, const double& omg);

	void	set_parameters(const double& epoch, const double& a, const double& e, const double& M0, const double& n,
						   const double& node,  const double& i, const double& omg);

	vector3d	calc_pos(const double& t);
	void		calc_pos_and_velocity(const double& t, vector3d &pos, vector3d &vel);
	vector3d	calc_velocity(const double& t);
	void		update_pos_and_velocity(const double& t);

	vector3d	calc_orbit_pos(const double& phi) const;

	// accessor
	double		get_epoch() const	{ return m_epoch; }
	double		get_a() const		{ return m_a; }
	double		get_e() const		{ return m_e; }
	double		get_M0() const		{ return m_M0; }
	double		get_n() const		{ return m_n; }
	matrix3d	get_MT() const		{ return m_MT; }
	vector3d	get_pos() const			{ return m_pos; }
	vector3d	get_velocity() const	{ return m_vel; }

	void		mult_MT(const matrix3d& M) { m_MT = M * m_MT; }
};



//
//
//
void	set_orbital_elements(vector3d x, vector3d v, double t0, double mu, orbital_elements &oe);


#endif

