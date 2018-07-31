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
#ifndef __MTK_OBJECT_H__
#define __MTK_OBJECT_H__
#include  "../common/math/vector3.h"
#include  "mtk_draw_struct.h"
#include  <vector>
#include  <string>




//---------------
//  Object Type
//---------------
enum {
	OBJ_TYPE_NONE,
	OBJ_TYPE_FLOATING_OBJECT,
	OBJ_TYPE_SUN,
	OBJ_TYPE_PLANET,
	OBJ_TYPE_SATELLITE,
	OBJ_TYPE_SMALL_OBJECT,
	//OBJ_TYPE_COMET,
	OBJ_TYPE_OORT_CLOUD,
	OBJ_TYPE_STAR,
	OBJ_TYPE_S_STAR,
	OBJ_TYPE_BLACKHOLE,
	OBJ_TYPE_GLOBULAR_CLUSTER,
	OBJ_TYPE_VERA_OBJECT,
	OBJ_TYPE_OUR_GALAXY,
	OBJ_TYPE_NEARBY_GALAXY,
	OBJ_TYPE_DISTANT_GALAXY,
	OBJ_TYPE_SPACECRAFT,

	OBJ_TYPE_FIXED_OBJECT,
	OBJ_TYPE_SCALE_CIRCLE,
	OBJ_TYPE_SCALE_CIRCLE_G,
	OBJ_TYPE_LABEL
};



//----------------------
//  Object Draw Region
//----------------------
enum {
	OBJ_DRAW_NONE,
	OBJ_DRAW_NEAR,
	OBJ_DRAW_FAR,
	OBJ_DRAW_BOTH
};



//----------------------
//  Object Draw Status
//----------------------
enum {
	OBJ_DRAW_STAT_REALSIZE,
	OBJ_DRAW_STAT_POINT
};



//--------------------------
//  Object Hit-test Status
//--------------------------
enum {
	OBJ_HITTEST_STAT_NONE,
	OBJ_HITTEST_STAT_REALSIZE,
	OBJ_HITTEST_STAT_POINT
};



//--------------------
//  class mtk_object
//--------------------
class  mtk_object
{
	int		m_type, m_idx;
	float	m_dz;	// ���_���W�n�ł̂��炵
	bool	m_bTgt;

	//--- �`��p�ꎞ���
	double		m_Rb;			//  Bounding R
	double		m_Rh;			//  Hit-test R
	vector3d	rpos;
	double		scl;
	float		draw_z;
	float		alpha;

	int	draw_status;
	int	hit_test_status;

public:
	mtk_object() : m_dz(0.0f), m_bTgt(false) {};
	mtk_object(int type, int idx, float dz, bool bTargetable) { init(type, idx, dz, bTargetable); }

	void		init(int type, int idx, float dz, bool bTargetable);

	int			get_type() const  { return  m_type; }
	int			get_index() const { return  m_idx; }
	vector3d	get_pos() const;
	float		get_dz() const		{ return m_dz; }

	double		get_landing_radius() const;

	vector3d	get_parent_pos() const;
	
	double		get_scale() const;

	bool		is_targetable() const  { return m_bTgt; }

	int			prepare_draw(const mtk_draw_struct *v_ds, int Nds, const float z_near, const float z_mid, const float z_far);
	float		get_draw_z() const			{ return  draw_z; }
	int			get_draw_status() const		{ return  draw_status; }
	int			get_hit_test_status() const	{ return hit_test_status; }
	float		get_alpha() const			{ return  alpha; }

	double		get_R() const { return m_Rh; }

	bool		hit_test(const mtk_draw_struct& ds, int mx, int my, const vector3d& r0, const vector3d& nv) const;

	void		draw(const mtk_draw_struct& ds, bool bSelect, bool bChangeObjType) const;

	void		make_object_info(const mtk_draw_struct& ds, std::vector< std::string >& text)  const;
};





//
//
//
float	get_label_alpha(const mtk_draw_struct& ds, const mtk_object& obj);
float	get_orbit_alpha(const mtk_draw_struct& ds, const mtk_object& obj);
bool	is_object_label_visible(const mtk_draw_struct& ds, const mtk_object& obj);
bool	is_object_orbit_visible(const mtk_object& obj);





#endif
