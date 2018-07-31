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
#ifndef  __MTK_SPACECRAFT_MODEL_H__
#define  __MTK_SPACECRAFT_MODEL_H__
#include  <windows.h>
#include  <gl/gl.h>
#include  <vector>
#include  "../common/math/vector3.h"
#include  "../common/graphics/color.h"
#include  "../common/graphics/gl_Model3D.h"
#include  "mtk_trajectory.h"
#include  "mtk_draw_struct.h"
using namespace std;




enum {
	ORIGIN_SS_BC,
	ORIGIN_SUN,
	ORIGIN_EARTH
};




class spacecraft
{
	std::string key;
	mtk_trajectory  trajectory;				// trajectory
	int			ModelIdx;
	bool		bDrawTrajectory;
	int			origin;

public:
	spacecraft() : ModelIdx(-1), bDrawTrajectory(false), origin(ORIGIN_SS_BC)  {};

	bool		is_available_time(const double& t) const;
	double		get_t1() const;
	double		get_t2() const;
	vector3d	get_pos(const double& t) const;

	//-- Trajectory
	bool		load_trajectory(const char *filename, int Ntbl);
	void		draw_trajectory(const mtk_draw_struct& ds, const double& t, const vector3d& target_r, const RGBf& col, float af) const;
	void		set_draw_trajectory_mode(bool bFlg)	{ bDrawTrajectory = bFlg; }
	bool		get_draw_trajectory_mode() const	{ return bDrawTrajectory; }
	const mtk_trajectory&	get_trajectory() const	{ return trajectory; }

	void		set_origin(int org) { origin = org; }
	int			get_oigin() const	{ return origin; }

	//-- Model
	void		set_model(int _ModelIdx) { ModelIdx = _ModelIdx; }
	void		draw_model() const;

	//-- Key
	const std::string&	get_key() { return key; }

	//--
	void		set(const char *_key, int _ModelIdx, int org);
};





#endif
