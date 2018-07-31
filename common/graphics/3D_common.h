/******************************************************************************

Copyright (c) 2007, 2015-2016   Tsunehiko Kato

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

�ȉ��ɒ�߂�����ɏ]���A�{�\�t�g�E�F�A����ъ֘A�����̃t�@�C���i�ȉ��u�\�t�g�E�F
�A�v�j�̕������擾���邷�ׂĂ̐l�ɑ΂��A�\�t�g�E�F�A�𖳐����Ɉ������Ƃ𖳏��ŋ�
���܂��B����ɂ́A�\�t�g�E�F�A�̕������g�p�A���ʁA�ύX�A�����A�f�ځA�Еz�A�T�u
���C�Z���X�A�����/�܂��͔̔����錠���A����у\�t�g�E�F�A��񋟂��鑊��ɓ�����
�Ƃ������錠�����������Ɋ܂܂�܂��B

��L�̒��쌠�\������і{�����\�����A�\�t�g�E�F�A�̂��ׂĂ̕����܂��͏d�v�ȕ�����
�L�ڂ�����̂Ƃ��܂��B

�\�t�g�E�F�A�́u����̂܂܁v�ŁA�����ł��邩�Öقł��邩���킸�A����̕ۏ؂���
���񋟂���܂��B�����ł����ۏ؂Ƃ́A���i���A����̖ړI�ւ̓K�����A����ь�����N
�Q�ɂ��Ă̕ۏ؂��܂݂܂����A����Ɍ��肳�����̂ł͂���܂���B ��҂܂��͒�
�쌠�҂́A�_��s�ׁA�s�@�s�ׁA�܂��͂���ȊO�ł��낤�ƁA�\�t�g�E�F�A�ɋN���܂���
�֘A���A���邢�̓\�t�g�E�F�A�̎g�p�܂��͂��̑��̈����ɂ���Đ������؂̐����A��
�Q�A���̑��̋`���ɂ��ĉ���̐ӔC������Ȃ����̂Ƃ��܂��B
*******************************************************************************/
#ifndef __3D_COMMON_H__
#define __3D_COMMON_H__
#include  "../math/vector3.h"


//-----------
//  Frustum
//-----------
class frustum
{
private:
	float x1, y1, x2, y2, z1, z2;
public:
	frustum() {};
	~frustum() {};
	void	set(float _x1, float _x2, float _y1, float _y2, float _z1, float _z2);
	void	set2(float aspc_rt, float screen_z, float screen_w, float near_z, float far_z, float offset_x);
	float	get_x1() const	{ return x1; }
	float	get_x2() const	{ return x2; }
	float	get_y1() const	{ return y1; }
	float	get_y2() const	{ return y2; }
	float	get_z1() const	{ return z1; }
	float	get_z2() const	{ return z2; }

	void	get_planes(vector3f *nv, float *D) const;

	void	calc_cull_vectors(vector3d *nv);
	char	cull_test(const float& x, const float& y, const float& z) const;
	char	cull_test(const vector3f& r) const { return cull_test(r.x, r.y, r.z); }
	bool	is_outside(int face_no, const float& x, const float& y, const float& z) const;
	bool	is_outside(int face_no, const vector3f& r) const { return is_outside(face_no, r.x, r.y, r.z); }

	vector3f	get_intersection(int face_no, const vector3f& r1, const vector3f& r2) const;
};


//--
const char FRUSTUM_CULL_X1 = 0x01;
const char FRUSTUM_CULL_X2 = 0x02;
const char FRUSTUM_CULL_Y1 = 0x04;
const char FRUSTUM_CULL_Y2 = 0x08;
const char FRUSTUM_CULL_Z1 = 0x10;
const char FRUSTUM_CULL_Z2 = 0x20;
const char FRUSTUM_CULL_X  = FRUSTUM_CULL_X1 | FRUSTUM_CULL_X2;
const char FRUSTUM_CULL_Y  = FRUSTUM_CULL_Y1 | FRUSTUM_CULL_Y2;
const char FRUSTUM_CULL_XY = FRUSTUM_CULL_X  | FRUSTUM_CULL_Y;

//
enum {
	FRUSTUM_FACE_LEFT,
	FRUSTUM_FACE_RIGHT,
	FRUSTUM_FACE_TOP,
	FRUSTUM_FACE_BOTTOM,
	FRUSTUM_FACE_NEAR,
	FRUSTUM_FACE_FAR
};



// �O�����@���Az�������ʕ���
inline  void
frustum::get_planes(vector3f *nv, float *D) const
{
	
	//*** Test
	/*
	float a = 0.8;
	nv[0] = normalize(vector3f(-z1, 0, -x1*a));
	nv[1] = normalize(vector3f(z1, 0, x2*a));
	nv[2] = normalize(vector3f(0, -z1, -y1*a));
	nv[3] = normalize(vector3f(0, z1, y2*a));
	*/

	
	nv[0] = normalize(vector3f(-z1,   0, -x1));
	nv[1] = normalize(vector3f( z1,   0,  x2));
	nv[2] = normalize(vector3f(  0, -z1, -y1));
	nv[3] = normalize(vector3f(  0,  z1,  y2));
	

	nv[4].set(0, 0, 1);
	nv[5].set(0, 0, -1);

	D[0] = D[1] = D[2] = D[3] = 0;
	D[4] = z1;
	D[5] = -z2;
}



//*** to be deleted
inline void
frustum::calc_cull_vectors(vector3d *nv) {
	float _z1;
	_z1 = -z1;

	vector3f v11, v12, v21, v22;

	//***  Test
	/*
	float a = 0.9;
	v11.set(x1*a, y1*a, _z1);
	v12.set(x2*a, y1*a, _z1);
	v21.set(x1*a, y2*a, _z1);
	v22.set(x2*a, y2*a, _z1);
	*/
	//***


	
	v11.set(x1, y1, _z1);
	v12.set(x2, y1, _z1);
	v21.set(x1, y2, _z1);
	v22.set(x2, y2, _z1);
	


	// �e���ʂ̊O�����@���x�N�g�������߂�
	nv[0] = normalize(cross_product(v21, v11));	//  Left
	nv[1] = normalize(cross_product(v12, v22));	//  Right
	nv[2] = normalize(cross_product(v22, v21));	//  Top
	nv[3] = normalize(cross_product(v11, v12));	//  Bottom
}



// �w�肵���_���A�O���ɂ��邩���ׂ�
inline char
frustum::cull_test(const float& x, const float& y, const float& z) const
{
	char bf = 0;
	const float xz = x*z1;
	const float yz = y*z1;
	if (xz < x1*z)		{ bf |= FRUSTUM_CULL_X1; }
	if (xz > x2*z)		{ bf |= FRUSTUM_CULL_X2; }
	if (yz < y1*z)		{ bf |= FRUSTUM_CULL_Y1; }
	if (yz > y2*z)		{ bf |= FRUSTUM_CULL_Y2; }
	if (z < z1)			{ bf |= FRUSTUM_CULL_Z1; }
	if (z > z2)			{ bf |= FRUSTUM_CULL_Z2; }
	return  bf;
}


// �w�肵���_���w�肵���ʂ̊O���ɂ��邩���ׂ�
inline bool
frustum::is_outside(int face_no, const float& x, const float& y, const float& z) const
{
	switch (face_no) {
	case FRUSTUM_FACE_LEFT:		return (x*z1 < x1*z);
	case FRUSTUM_FACE_RIGHT:	return (x*z1 > x2*z);
	case FRUSTUM_FACE_TOP:		return (y*z1 < y1*z);
	case FRUSTUM_FACE_BOTTOM:	return (y*z1 > y2*z);
	case FRUSTUM_FACE_NEAR:		return (z < z1);
	case FRUSTUM_FACE_FAR:		return (z > z2);
	default:					return false;
	}
}


// 2�_�Ǝw�肵���ʂƂ̌�_�����߂�
inline vector3f
frustum::get_intersection(int face_no, const vector3f& r1, const vector3f& r2) const
{
	vector3f dr;
	float dx, dy, dz, t;

	dr = r2 - r1;
	dx = dr.x;
	dy = dr.y;
	dz = dr.z;

	switch (face_no) {
	case FRUSTUM_FACE_LEFT:
		t = (x1*r1.z - z1*r1.x) / (dx*z1 - dz*x1);
		break;
	case FRUSTUM_FACE_RIGHT:
		t = (x2*r1.z - z1*r1.x) / (dx*z1 - dz*x2);
		break;
	case FRUSTUM_FACE_TOP:
		t = (y1*r1.z - z1*r1.y) / (dy*z1 - dz*y1);
		break;
	case FRUSTUM_FACE_BOTTOM:
		t = (y2*r1.z - z1*r1.y) / (dy*z1 - dz*y2);
		break;
	case FRUSTUM_FACE_NEAR:
		t = (z1 - r1.z) / dz;
		break;
	case FRUSTUM_FACE_FAR:
		t = (z2 - r1.z) / dz;
		break;
	default:
		t = 0;
		break;
	}

	return dr*t + r1;
}


#endif
