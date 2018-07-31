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
#ifndef __MTK_SATELLITE_H__
#define __MTK_SATELLITE_H__
#include  <windows.h>
#include  "../common/math/vector3.h"
#include  "../common/text/unicode.h"
#include  "../common/graphics/gl_common.h"
#include  "../common/win_file.h"
#include  "mtk_topography.h"
#include  "mtk_places.h"
#include  "mtk_planet_eclipse.h"
#include  <string>




//--------------
//  Orbit type
//--------------
enum {
	SAT_ORBIT_MOON,
	SAT_ORBIT_ECLIPTIC,
	SAT_ORBIT_LAPLACE,
	SAT_ORBIT_EQUATORIAL,
	SAT_ORBIT_ICRF
};




//------------------
//  Satellite data
//------------------
class satellite {
	//---  key
	std::string key;

	//---  planet key
	std::string	planet_key;
	int	planet_idx;

	//---  roman number
	std::string roman_no;

	//---  texture
	std::string	texture_name;	// texture filename
	GLuint tex_idx;				// OpenGL textur name

	//---  places
	Places *plc;

	//---  topography
	TopoSphere *topo;

	//---  orientation
	int		ori_idx;

	//---  Eclipse info
	std::vector<eclipse_obj_info>  vEclipse;


	//---  Phys params
	double	Rx, Ry, Rz;			// size(radius, in km): �f�������A���������A���]����

	//---  orbital elements
	int		orbit_type;	// type
	double	epoch;		// epoch
	double	a;			// semi-major axis�i�O�������a�j
	double	e;			// eccentricity�i���S���j
	double	omg;		// argument of periapsis�i�ߓ_�����j
	double	M0;			// mean anomaly�i���ϋߓ_�p�j
	double	inc;		// inclination�ireference plane �ɑ΂���O���X�Ίp�j
	double	Omg;		// longitude of the ascending node�i����_:reference plane��ICRF equator ��node ���瑪�����p�x�j
	double	n;			// mean daily motion�i���ϓ��X�^���j
	double	P;			// sidereal period�i�����i�P���N�j�j
	double	Pw;			// argument of periapsis precession period
	double	Pnode;		// longitude of the ascending node precession period
	double	RA;			// right ascension
	double	DEC;		// declination of the Laplace plane pole with respect to the ICRF
	double  Tilt;

	//
	vector3d pos;		// position
	matrix3d M_orb;		// orbit matrix
	double  orbit_amp_factor;


public:
	bool	load_from_file(FILE *fp);

	GLuint  load_texture(int shrink_rate);
	GLuint	get_texture_index() const;

	const std::string&	get_key() const				{ return key; }
	const std::string&	get_planet_key() const		{ return planet_key; }
	const std::string&  get_roman_number() const	{ return roman_no;  }

	int			get_planet_index() const		{ return planet_idx; }

	void		update_position(const double& t);
	vector3d	get_position() const		{ return pos; }
	void		set_position(const vector3d& _pos)	{ pos = _pos; }
	matrix3d	get_orbit_matrix() const	{ return M_orb; }
	vector3d	get_planetocentric_position() const;
	vector3d	calc_orbit_pos(const double& t, float phi) const;
	int			get_orbit_type() const		{ return orbit_type;  }

	double		get_Rx() const { return Rx; }
	void		get_real_size(double &_Rx, double& _Ry, double& _Rz) const { _Rx = Rx; _Ry = Ry; _Rz = Rz; }

	void		set_orbit_amp_factor(const double& fac) { orbit_amp_factor = fac; }

	void		set_places(Places *p)  { plc = p; }
	Places*		get_places() { return plc; }

	void		set_topography(TopoSphere *tp)  { topo = tp; }
	TopoSphere*	get_topography() { return topo; }

	void		set_orientation_index(int idx) { ori_idx = idx; }
	int			get_orientation_index() const  { return ori_idx; }

	// Eclipse
	void		add_eclipse(const vector3f& col);
	const std::vector<eclipse_obj_info>&  get_eclipse_info() const  { return vEclipse; }




private:
	matrix3d	calc_orbit_matrix(const double& t);
	vector3d	calc_planetocentric_positon(const double& t) const;
	vector3d	calc_position(const double& t) const;
};











#endif
