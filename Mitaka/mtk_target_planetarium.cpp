/**************************************************************************************************

Copyright (c) 2016   Tsunehiko Kato  and  4D2U Project, National Astronomical Observatory of Japan

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
#include  "mtk_target_planetarium.h"
#include  "mtk_planetarium.h"
#include  "mtk_sun.h"
#include  "mtk_planet.h"
#include  "mtk_planet_position.h"
#include  "mtk_planet_orientation.h"
#include  "mtk_satellites.h"



//***
extern float  landing_lon, landing_lat;


static int target_planetarium = TARGET_PLNT_NONE;





void
set_planetarium_target(int tgt)
{
	target_planetarium = tgt;
}



int
get_planetarium_target()
{
	return target_planetarium;
}



void
follow_planetarium_target_sub(const vector3d& tgt_pos, const vector3d& cam_pos)
{
	vector3d r, nv;

	r = tgt_pos - cam_pos;										// �f���n�\�������W�ł̃^�[�Q�b�g�̈ʒu
	r = get_planet_orientation(PLNT_EARTH, true) * r;			// ec -> pl 
	r = Ry3deg(-90 + landing_lat) * Rz3deg(-landing_lon) * r;	// pl -> �n�����W
	nv = r.normalized();


	double rho, lon, lat;
	rho = sqrt(nv.x*nv.x + nv.y*nv.y);
	lat = atan2(nv.z, rho) / M_PI * 180.0;
	lon = atan2(nv.y, nv.x) / M_PI * 180.0;
	if (lat < 0) {
		lat = 0;
	}


	set_plnt_long(lon);
	set_plnt_lati(lat);
}



void
follow_planetarium_target(const vector3d& cam_pos)
{
	static bool first = true;
	static int idxMoon;

	if (first) {
		first = false;
		idxMoon = get_satellite_index("SAT_E_MOON");
	}


	vector3d tgt_pos;
	switch (target_planetarium) {
	case TARGET_PLNT_SUN:
		tgt_pos = get_sun_position();
		break;
	case TARGET_PLNT_MOON:
		tgt_pos = get_satellite_position(idxMoon);
		break;
	case TARGET_PLNT_JUPITER:
		tgt_pos = get_planet_position(PLNT_JUPITER);
		break;
	case TARGET_PLNT_NONE:
	default:
		return;
	}

	follow_planetarium_target_sub(tgt_pos, cam_pos);
}

