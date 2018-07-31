/**************************************************************************************************

Copyright (c) 2007, 2016   Tsunehiko Kato  and  4D2U Project, National Astronomical Observatory of Japan

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
#ifndef __MTK_ECLIPSE_H__
#define __MTK_ECLIPSE_H__
#include  "../common/math/vector3.h"
#include  "../common/math/Triangulation.h"
#include  "mtk_draw_struct.h"


struct eclipse_info {
	bool		bFlg;
	vector3d	nv, n1, n2;
	vector3d	r_umb;	// �{�e�R�[���̎n�_
	vector3d	r_pen;	// ���e�R�[���̎n�_
	double		sinth_umb, costh_umb;	// �{�e�R�[���̊p�x
	double		sinth_pen, costh_pen;	// ���e�R�[���̊p�x
};

struct eclipse_vertex {
	vector3d	pos;
	float		f;
};

vector3d	eclipse_get_shadow_center(const vector3d& r1, const double& R1, const vector3d& r2, const vector3d& rs);
eclipse_info	calc_eclipse_info(const vector3d& r1, const double& R1, const vector3d& r2, const double& R2, const vector3d& rs, const double& Rs);
double			calc_eclipse_fraction(double R, double dlt);

int
make_eclipse_shadow_polygon(const vector3d& r1, const double& R1, const vector3d& nz, const float f, 
							const vector3d& r2, const double& R2,
							const vector3d& rs, const double& Rs,
							const int N1, const int N2, eclipse_vertex* vVtx);

void
draw_eclipse_shadow(const mtk_draw_struct& ds,
					const vector3d& r1, const double& R1, const vector3d& nz, const float f,
					const vector3d& r2, const double& R2,
					const vector3d& rs, const double& Rs,
					const matrix3d& M,
					const float thc,
					const int N1, const int N2,
					bool bShadow, bool bBorder,
					const vector3f& shd_col,
					const double& Rc_pl);


#endif
