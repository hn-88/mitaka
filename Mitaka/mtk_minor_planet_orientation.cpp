/**************************************************************************************************

Copyright (c) 2015   Tsunehiko Kato  and  4D2U Project, National Astronomical Observatory of Japan

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
#include  "mtk_minor_planet_orientation.h"
#include  "../common/math/vector3.h"
#include  "../common/astro/astrometry.h"
#include  "mtk_minor_planets.h"
#include  "mtk_object_orientation.h"


enum {
	MP_ORI_CERES,
	MP_ORI_ITOKAWA,
	MP_ORI_CHARIKLO,
	MP_ORI_NUM			// Number
};



struct mp_ori_info {
	const char*  obj_key;
	int ori_idx;
};



const mp_ori_info moi[MP_ORI_NUM] = {
	{"MP_1", MP_ORI_CERES},
	{"MP_25143", MP_ORI_ITOKAWA},
	{"MP_10199", MP_ORI_CHARIKLO}
};




void
init_minor_planet_orientation()
{
	for (int ori_id = 0; ori_id < MP_ORI_NUM; ori_id++) {
		const mp_ori_info& mo = moi[ori_id];
		int mp_idx;

		const std::string& key = mo.obj_key;
		mp_idx = get_small_object_index(key);

		if (mp_idx != -1) {
			small_object& so = get_small_object(mp_idx);
			int ori_idx;

			so.ori_idx = add_orientation(key, ORIENTATION_SMALL_OBJECT, ori_id);
		}
	}
}





//-------------------
//  ���]��� Ref.[1],[2]
//-------------------
void
calc_minor_planet_orientation_angle(int ori_id, const double& t, double& alp, double& dlt, double& W)
{
	const double  JD0 = 2451545.0;	// ���� 2000/1/1.5
	const double  d = t - JD0;

	switch (ori_id) {
	case MP_ORI_CERES:	// Ceres [1]
	{
		alp = 291.0;
		dlt = 59.0;
		W = 170.90 + 952.1532 * d;
	}
	break;

	case MP_ORI_ITOKAWA:	// Itokawa [1]
	{
		alp = 90.53;
		dlt = -66.30;	// retrograde
		W = 0.0 + 712.143 * d;
	}
	break;

	case MP_ORI_CHARIKLO:	// Chariklo [2], [3]
	{
		alp = 151.25;
		dlt = 41.4833;
		W = 0.0 + 1233.5808 * d;	// P = 7.004h
	}
	break;

	default:
		alp = W = 0.0;
		dlt = 90.0;
		break;
	}
}



void
calc_minor_planet_ec_to_mp_matrix(int ori_id, const double& t, matrix3d& M0, matrix3d& M)
{
	static matrix3d  M_J2000;
	static bool first = true;
	if (first) {
		first = false;
		M_J2000 = Rx3deg(epsJ2000);
	}

	// ���f���̎��]
	double alp, dlt, W;
	calc_minor_planet_orientation_angle(ori_id, t, alp, dlt, W);
	M0 = Rzxz3deg(0.0, -90 + dlt, -90 - alp) * M_J2000;
	M = Rz3deg(-W) * M0;
}



const matrix3d&
get_minor_planet_orientation(int idx, bool bRotation)
{
	const small_object& smo = get_small_object(idx);
	return get_orientation(smo.ori_idx, bRotation);
}






void
set_minor_planet_orientation_GL(int idx, const double& W_shift)
{
	const small_object& smo = get_small_object(idx);
	int ori_idx = smo.ori_idx;

	set_object_orientation_GL(ori_idx);

	glRotated(W_shift, 0, 0, 1);
}





////////////////////////////////////////
/// References
//
// [1] Archinal et al., 2011, Celestial Mechanics and Dynamical Astronomy, 109, 101-135
// [2] Braga-Ribas et al., 2014, Nature, 508, 72-75
// [3] JPL Small-Body Database Browser (http://ssd.jpl.nasa.gov/sbdb.cgi?sstr=2010199)


