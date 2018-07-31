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
#include  "mtk_object_orientation.h"
#include  "../common/astro/astrometry.h"
#include  "mtk_planet_orientation.h"
#include  "mtk_satellite_orientation.h"
#include  "mtk_minor_planet_orientation.h"
#include  "mtk_OpenGL.h"
#include  <vector>
#include  <map>





struct orientation {
	int obj_type;
	int ori_id;
	matrix3d  M;	// EC to OBJ matrix
	matrix3d  M0;	// without rotation
};



//---  Objects
static std::vector<orientation> vOri;
static std::map<std::string, int> mOri;





int
add_orientation(const std::string& key, int obj_type, int ori_id)
{
	orientation ori;
	ori.obj_type = obj_type;
	ori.ori_id = ori_id;
	vOri.push_back(ori);

	int idx;
	idx = vOri.size() - 1;
	mOri[key] = idx;

	return idx;
}


int
get_orientation_index(const std::string& key)
{
	std::map<std::string, int>::const_iterator it = mOri.find(key);

	if (it == mOri.end()) {
		return -1;
	}

	return it->second;
}






void
update_orientation(int ori_idx, const double& t)
{
	if (ori_idx < 0 || ori_idx >= vOri.size()) {
		return;
	}



	orientation& ori = vOri[ori_idx];

	switch (ori.obj_type) {
	case ORIENTATION_PLANET:
		calc_planet_ec_to_pl_matrix(ori.ori_id, t, ori.M0, ori.M);
		break;
	case ORIENTATION_SATELLITE:
		calc_satellite_ec_to_sat_matrix(ori.ori_id, t, ori.M0, ori.M);
		break;
	case ORIENTATION_SMALL_OBJECT:
		calc_minor_planet_ec_to_mp_matrix(ori.ori_id, t, ori.M0, ori.M);
		break;
	default:
		break;
	}
}


void
update_all_object_orientation(const double& t)
{
	const double DT_MIN = 0.01 / (24.0*60.0*60.0);
	static bool first = true;
	static double t_last;

	if (first)	{ first = false; }
	else { if (fabs(t - t_last) < DT_MIN)  return; }


	for (int i = 0; i < vOri.size(); i++) {
		update_orientation(i, t);
	}

	t_last = t;
}



const matrix3d&
get_orientation(int ori_idx, bool bRotation)
{
	static const matrix3d I(1, 0, 0, 0, 1, 0, 0, 0, 1);

	if (ori_idx < 0 || ori_idx >= vOri.size()) {
		return I;
	}

	if (bRotation) {
		return vOri[ori_idx].M;
	}
	else {
		return vOri[ori_idx].M0;
	}
}





//---------------
//  Orientation
//---------------

//-- �������W�n����V�̍��W�n�ֈڂ�
void
set_object_orientation(const matrix3d& MT)
{
	float M[16];

	M[0] = MT.m11;
	M[1] = MT.m21;
	M[2] = MT.m31;
	M[3] = 0.0;

	M[4] = MT.m12;
	M[5] = MT.m22;
	M[6] = MT.m32;
	M[7] = 0.0;

	M[8] = MT.m13;
	M[9] = MT.m23;
	M[10] = MT.m33;
	M[11] = 0.0;

	M[12] = 0.0;
	M[13] = 0.0;
	M[14] = 0.0;
	M[15] = 1.0;

	glMultMatrixf(M);
}


// ���]���̌����ɍ��킹��
void
set_object_orientation_GL(int ori_idx)
{
	if (ori_idx >= 0) {
		set_object_orientation(get_orientation(ori_idx, true).transposed());
	}
}

