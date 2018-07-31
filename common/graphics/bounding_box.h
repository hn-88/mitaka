/******************************************************************************

Copyright (c) 2007,2016   Tsunehiko Kato

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
#ifndef  __BOUNDING_BOX_H__
#define  __BOUNDING_BOX_H__
#include  "../math/vector3.h"





//-----------------------
//  bounding box for 2D
//-----------------------
template  <class T>
class bbox2
{
	bool  bFirst;
public:
	T x1, y1, x2, y2;
public:
	bbox2() : bFirst(true), x1(0), y1(0), x2(0), y2(0) {};
	bbox2(const T& _x1, const T& _x2, const T& _y1, const T& _y2) : bFirst(false),
		x1(_x1), x2(_x2), y1(_y1), y2(_y2)
	{
	}
	void	set(const T& _x1, const T& _x2, const T& _y1, const T& _y2) {
		x1 = _x1; x2 = _x2;
		y1 = _y1; y2 = _y2;
	}
	bool	isValid() const { return !bFirst; }
	void	add_point(const vector2<T>& pt);
	void	merge(const bbox2<T>& bb);
	void	clear() { x1 = y1 = x2 = y2 = T(0); bFirst = true; }

	bool	contains(const vector2<T>& pos) const;
	bool	contains(const bbox2<T>& bb) const;
};

typedef	bbox2<float>	bbox2f;
typedef	bbox2<double>	bbox2d;



template  <class T>
void
bbox2<T>::add_point(const vector2<T>& pt)
{
	if (bFirst) {
		bFirst = false;
		x1 = x2 = pt.x;
		y1 = y2 = pt.y;
		z1 = z2 = pt.z;
	}
	else {
		if (pt.x < x1)		x1 = pt.x;
		else if (pt.x > x2)	x2 = pt.x;

		if (pt.y < y1)		y1 = pt.y;
		else if (pt.y > y2)	y2 = pt.y;

		if (pt.z < z1)		z1 = pt.z;
		else if (pt.z > z2)	z2 = pt.z;
	}
}


template  <class T>
void
bbox2<T>::merge(const bbox2<T>& bb)
{
	if (!bb.isValid())	return;
	if (!isValid()) {
		*this = bb;
	}
	else {
		if (bb.x1 < x1)		x1 = bb.x1;
		if (bb.x2 > x2)		x2 = bb.x2;
		if (bb.y1 < y1)		y1 = bb.y1;
		if (bb.y2 > y2)		y2 = bb.y2;
		if (bb.z1 < z1)		z1 = bb.z1;
		if (bb.z2 > z2)		z2 = bb.z2;
	}
}


template  <class T>
bool
bbox2<T>::contains(const vector2<T>& pos) const
{
	return  (x1 < pos.x && x2 > pos.x && y1 < pos.y && y2 > pos.y);
}


template  <class T>
bool
bbox2<T>::contains(const bbox2<T>& bb) const
{
	return  (x1 < bb.x1 && x2 > bb.x2 && y1 < bb.y1 && y2 > bb.y2);
}






//-----------------------
//  bounding box for 3D
//-----------------------
template  <class T>
class bbox3
{
	bool  bFirst;
public:
	T x1,y1,z1, x2,y2,z2;
public:
	bbox3() : bFirst(true), x1(0),y1(0),z1(0), x2(0),y2(0),z2(0) {};
	bbox3(const T& _x1, const T& _x2, const T& _y1, const T& _y2, const T& _z1, const T& _z2) : bFirst(false),
		x1(_x1), x2(_x2), y1(_y1), y2(_y2), z1(_z1), z2(_z2)
	{
	}
	void	set(const T& _x1, const T& _x2, const T& _y1, const T& _y2, const T& _z1, const T& _z2) {
		x1 = _x1; x2 = _x2;
		y1 = _y1; y2 = _y2;
		z1 = _z1; z2 = _z2;
	}
	bool	isValid() const  { return !bFirst; }
	void	add_point(const vector3<T>& pt);
	void	merge(const bbox3<T>& bb);
	void	clear()	{ x1=y1=z1=x2=y2=z2=T(0); bFirst=true; }

	bool	contains(const vector3<T>& pos) const;
	bool	contains(const bbox3<T>& bb) const;
};

typedef	bbox3<float>	bbox3f;
typedef	bbox3<double>	bbox3d;


template  <class T>
void
bbox3<T>::add_point(const vector3<T>& pt)
{
	if (bFirst) {
		bFirst = false;
		x1 = x2 = pt.x;
		y1 = y2 = pt.y;
		z1 = z2 = pt.z;
	} else {
		if (pt.x < x1)		x1=pt.x;
		else if (pt.x > x2)	x2=pt.x;

		if (pt.y < y1)		y1=pt.y;
		else if (pt.y > y2)	y2=pt.y;

		if (pt.z < z1)		z1=pt.z;
		else if (pt.z > z2)	z2=pt.z;
	}
}


template  <class T>
void
bbox3<T>::merge(const bbox3<T>& bb)
{
	if (!bb.isValid())	return;
	if (!isValid()) {
		*this = bb;
	} else {
		if (bb.x1 < x1)		x1 = bb.x1;
		if (bb.x2 > x2)		x2 = bb.x2;
		if (bb.y1 < y1)		y1 = bb.y1;
		if (bb.y2 > y2)		y2 = bb.y2;
		if (bb.z1 < z1)		z1 = bb.z1;
		if (bb.z2 > z2)		z2 = bb.z2;
	}
}


template  <class T>
bool
bbox3<T>::contains(const vector3<T>& pos) const
{
	return  (x1 < pos.x && x2 > pos.x && y1 < pos.y && y2 > pos.y && z1 < pos.z && z2 > pos.z);
}


template  <class T>
bool
bbox3<T>::contains(const bbox3<T>& bb) const
{
	return  (x1 < bb.x1 && x2 > bb.x2 && y1 < bb.y1 && y2 > bb.y2 && z1 < bb.z1 && z2 > bb.z2);
}






template  <class T>
bool
is_visible(const vector3<T>* nv, const T *D, const bbox3<T>& bb, bool bCullFar)
{
	const int N = (bCullFar) ? 6 : 5;


	T A[6], B[6], C[6];
	for (int face = 0; face < N; face++) {
		vector3<T>  _nv = T(0.5) * nv[face];

		A[face] = fabs(_nv.x);
		B[face] = fabs(_nv.y);
		C[face] = fabs(_nv.z);
	}

	T Lx, Ly, Lz, xc, yc, zc;
	Lx = bb.x2 - bb.x1;
	Ly = bb.y2 - bb.y1;
	Lz = bb.z2 - bb.z1;

	xc = T(0.5) * (bb.x1 + bb.x2);
	yc = T(0.5) * (bb.y1 + bb.y2);
	zc = T(0.5) * (bb.z1 + bb.z2);
	vector3<T> rc = vector3<T>(xc, yc, zc);


	int count = 0;
	for (int face = 0; face < N; face++) {
		float r_eff, d;

		r_eff = Lx*A[face] + Ly*B[face] + Lz*C[face];
		d = dot_product(rc, nv[face]) + D[face];

		// completely outside
		if (d > r_eff) {
			//return OCTREE_OUTSIDE;
			return false;
		}

		// completely inside
		if (d < -r_eff) {
			count++;
		}
	}

	return true;
}




#endif
