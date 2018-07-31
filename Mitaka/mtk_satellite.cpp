/**************************************************************************************************

Copyright (c) 2007-2015   Tsunehiko Kato

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
#include  "mtk_satellite.h"
#include  "../common/astro/astrometry.h"
#include  "../common/astro/cele_mech.h"
#include  "mtk_objects.h"
#include  "mtk_data_file.h"
#include  "mtk_OpenGL.h"
#include  "mtk_orbital_element.h"
#include  "mtk_planets.h"
#include  "mtk_planet_orientation.h"
#include  "mtk_planet_position.h"
#include  "mtk_lunar_pos.h"









bool
satellite::load_from_file(FILE *fp)
{
	key_data kd;


	kd = read_a_key_data(fp);
	if (kd.key.size() == 0) {
		return false;
	}


	// Key / names
	key = kd.key;
	planet_key = kd.data[0];
	roman_no = kd.data[1];
	texture_name = kd.data[2];


	//*** �f����񂪐�ɓǂݍ��܂�Ă���K�v������
	planet_idx = get_object(planet_key).get_index();


	// places and topography
	plc = 0;
	topo = 0;

	tex_idx = 0;	// invalid texture name
	ori_idx = -1;


	// Radius
	Rx = stod(kd.data[3]);
	Ry = stod(kd.data[4]);
	Rz = stod(kd.data[5]);



	// Orbital plane type
	const string& obt_plane = kd.data[6];
	if (obt_plane == "ORBIT_MOON") {
		orbit_type = SAT_ORBIT_MOON;
	}
	else if (obt_plane == "ORBIT_ECLIPTIC") {
		orbit_type = SAT_ORBIT_ECLIPTIC;
	}
	else if (obt_plane == "ORBIT_LAPLACE") {
		orbit_type = SAT_ORBIT_LAPLACE;
	}
	else if (obt_plane == "ORBIT_EQUATORIAL") {
		orbit_type = SAT_ORBIT_EQUATORIAL;
	}
	else if (obt_plane == "ORBIT_ICRF") {
		orbit_type = SAT_ORBIT_ICRF;
	}
	else {
		// error
	}



	// Orbital elements
	epoch = stod(kd.data[7]);

	a   = stod(kd.data[8]);
	e   = stod(kd.data[9]);
	omg = stod(kd.data[10]);
	M0  = stod(kd.data[11]);
	inc = stod(kd.data[12]);
	Omg = stod(kd.data[13]);
	n   = stod(kd.data[14]);
	P   = stod(kd.data[15]);
	Pw  = stod(kd.data[16]);
	Pnode = stod(kd.data[17]);


	if (orbit_type == SAT_ORBIT_LAPLACE || orbit_type == SAT_ORBIT_ICRF) {
		RA   = stod(kd.data[18]);
		DEC  = stod(kd.data[19]);
		Tilt = stod(kd.data[20]);
	}


	// zoom rate
	//zoom_rate = 1.0;
	orbit_amp_factor = 1.0;


	return true;
}



//---  Textures
GLuint
satellite::load_texture(int shrink_rate)
{
	if (texture_name != "") {
		tex_idx = LoadTexture(texture_name.c_str(), shrink_rate, false);
	}
	else {
		tex_idx = 0;
	}

	return tex_idx;
}

GLuint
satellite::get_texture_index() const
{
	return  tex_idx;
}





//---  Eclipse
void
satellite::add_eclipse(const vector3f& col)
{
	eclipse_obj_info eci;

	eci.obj_idx = planet_idx;
	eci.col = col;
	vEclipse.push_back(eci);
}







//---  Position
//--------------------------------------
//  �q�����W���物�����W�ւ̕ϊ��s������߂�
//--------------------------------------
//******** ���̕ϊ��s��͖�����  ***************
matrix3d
satellite::calc_orbit_matrix(const double& t)
{
	if (orbit_type == SAT_ORBIT_MOON) {
		return  matrix3d(1, 0, 0, 0, 1, 0, 0, 0, 1);
	}


	double domg, dOmg;
	double dt, _omg, _Omg;
	domg = (Pw != 0.0) ? 360.0 / Pw / 365.25 : 0.0;
	dOmg = (Pnode != 0.0) ? 360.0 / Pnode / 365.25 : 0.0;
	dt = t - epoch;
	_omg = omg + domg*dt;
	_Omg = Omg + dOmg*dt;

	switch (orbit_type) {
	case SAT_ORBIT_LAPLACE:		// ���v���X�ʂ��
		return  M_R2E * Rzxz3deg(90 + RA, 90 - DEC, 0.0) * Rzxz3deg(_Omg, inc, _omg);
	case SAT_ORBIT_ICRF:		// ICRF���
		return  M_R2E * Rzxz3deg(90 + RA, 90 - DEC, 0.0) * Rzxz3deg(_Omg, inc, _omg);
	case SAT_ORBIT_EQUATORIAL:	// �f���ԓ��ʂ��
		return  get_planet_orientation(planet_idx, false) * Rzxz3deg(_Omg, inc, _omg);
	case SAT_ORBIT_ECLIPTIC:	// �����ʂ��
	default:
		return  Rzxz3deg(_Omg, inc, _omg);
	}
}


//---------------------------------
//  �q���̈ʒu�����߂�(�f�����S���W)
//---------------------------------
vector3d
satellite::calc_planetocentric_positon(const double& t)  const
{
	//--  Moon  --
	if (orbit_type == SAT_ORBIT_MOON) {
		// Moon
		return  double(orbit_amp_factor * _1km_pc) * calc_lunar_pos(t);
	}



	//--  Other satellites  --
	vector3d  ro, rL, rICRF, rE;

	// omg, node �� precession
	double domg, dOmg, _n;
	domg = (Pw != 0.0) ? 360.0 / Pw / 365.25 : 0.0;
	dOmg = (Pnode != 0.0) ? 360.0 / Pnode / 365.25 : 0.0;
	_n = n - domg - dOmg;

	// �O���ʏ�̈ʒu
	ro = calc_orbital_pos(a*_1km_AU, e, M0, _n, epoch, t);
	rE = M_orb * ro;

	return  double(orbit_amp_factor * _1AU_pc) * rE;
}


//---------------------------------
//  �q���̈ʒu�����߂�(���S�������W)
//  x ���t���_�Axy-���ʂ�������
//---------------------------------
vector3d
satellite::calc_position(const double& t) const
{
	return  calc_planetocentric_positon(t) + get_planet_position(planet_idx);
}


void
satellite::update_position(const double& t)
{
	M_orb = calc_orbit_matrix(t);
	pos = calc_position(t);
}


vector3d
satellite::get_planetocentric_position() const
{
	return  pos - get_planet_position(planet_idx);
}



vector3d
satellite::calc_orbit_pos(const double& t, float phi) const
{
	//--  Position in planetocentric coord in km
	vector3d pos0;
	if (orbit_type == SAT_ORBIT_MOON) {
		// Moon
		pos0 = calc_lunar_pos(t - 27.3*((phi - M_PI) / (2 * M_PI)));
	}
	else {
		// Other satellites
		pos0 = M_orb * calc_orbital_position(a, e, phi);
	}

	return  (_1km_pc * orbit_amp_factor) * pos0 + get_planet_position(planet_idx);
}
