/**************************************************************************************************

Copyright (c) 2007   Tsunehiko Kato  and  4D2U Project, National Astronomical Observatory of Japan

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
#ifndef __MTK_SPACE_CURVE_H__
#define __MTK_SPACE_CURVE_H__
#include  <vector>
#include  "../common/math/vector3.h"
#include  "mtk_draw_struct.h"
#include  "mtk_object.h"
#include  "mtk_colors.h"


//---------------
//  space curve
//---------------
class space_curve
{
	mtk_object	m_related_object;
	int     m_color_no;
	float	m_line_width;
	std::vector<vector3d>  m_vpos;
public:
	space_curve()
		:  m_related_object(OBJ_TYPE_NONE, 0, 0.0f, false), m_line_width(1.0f), m_color_no(MTK_COLOR_ORBIT_PLANET)  {};
	space_curve(const mtk_object& obj, float width, int color_no)
		: m_related_object(obj), m_line_width(width), m_color_no(color_no) {};

	void		clear()	{ m_vpos.clear(); }
	void		init(const mtk_object& obj, float width, int color_no) {
					clear();
					m_related_object	= obj;
					m_line_width		= width;
					m_color_no			= color_no;
				}

	void		add_point(const vector3d& pos)	{ m_vpos.push_back(pos); }

	void			set_related_object(const mtk_object& obj)	{ m_related_object = obj; }
	void			set_color_no(int col_no)			{ m_color_no = col_no; }
	void			set_line_width(float width)			{ m_line_width = width; }
	void			set_num_points(int num)				{ m_vpos.resize(num); }
	void			set_point(int idx, vector3d& pos)	{ m_vpos[idx] = pos; }

	int				get_num_points() const				{ return m_vpos.size(); }
	const vector3d&	get_point(int idx) const			{ return m_vpos[idx]; }
	const mtk_object&	get_related_object() const		{ return m_related_object; }
	int				get_color_no() const				{ return m_color_no; }
	float			get_line_width() const				{ return m_line_width; }
};

//--  Init / Update
int		register_space_curve(const space_curve&  sc);
void	init_space_curves(const double& t);
void	update_space_curves(const double& t);

//--  Draw
void	draw_space_curves(const mtk_draw_struct& ds);

#endif
