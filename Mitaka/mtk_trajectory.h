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
#ifndef __MTK_TRAJECTORY_H__
#define __MTK_TRAJECTORY_H__
#include  "../common/math/vector3.h"
#include <vector>




//----------------------------
//  class trajectory_segment
//----------------------------

class trajectory_segment {
	double _t[4], _x[4], _y[4], _z[4];

public:
	bool	read_data(FILE *fp);
	void	scale_pos(const double& fac);

	bool	is_inside(const double& t) const;
	double	get_t_begin() const		{ return _t[0]; }
	double	get_t_end() const		{ return _t[3]; }

	vector3d get_pos(const double& t) const;
};


inline bool
trajectory_segment::is_inside(const double& t) const
{
	return (_t[0] <= t && t <= _t[3]);
}





//------------------------
//  class mtk_trajectory
//------------------------

class mtk_trajectory {
	std::vector<trajectory_segment> vSeg;
	std::vector<int>				vSegTable;
	bool bLoaded;
	double t_begin, t_end;

	int		find_segment(const double& t, int idx1, int idx2) const;
	void	make_index_table(int N);

public:
	mtk_trajectory() : bLoaded(false), t_begin(0), t_end(0) {};

	int		find_segment(const double& t) const;
	bool	load(const char* fn, int Ntbl);

	bool		isLoaded() const		{ return bLoaded; }
	double		get_t_begin() const		{ return t_begin; }
	double		get_t_end() const		{ return t_end; }
	bool		isInRange(const double& t)	{ return (t >= t_begin && t <= t_end); }

	int			get_segment_num() const	{ return vSeg.size(); }
	const trajectory_segment&	get_segment(int idx) const  { return vSeg[idx]; }

	vector3d	get_pos(const double& t) const;
};



#endif
