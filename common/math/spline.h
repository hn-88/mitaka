/******************************************************************************

Copyright (c) 2007   Tsunehiko Kato

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
#ifndef __SPLINE_H__
#define __SPLINE_H__
#include  <vector>
#include  "vector3.h"
using namespace std;



//=========
//  NURBS
//=========
const int NURBS2_MAX_ORDER = 10;

template<class T>
class NURBS2
{
	int K;							// �K��
	vector<T>				knot;	// �m�b�g
	vector<vector3<T> >		cp;		// ����_�{�d��

public:
	NURBS2() : K(0), knot(0), cp(0) {}

	void		set_K(const int _K)		{ K=_K; }
	void		set_knots(const int n, const T* kt);
	void		set_control_points(const int n, const vector2<T>* vcp2);
	
	float		t_ini() const	{ return knot[K-1]; }
	float		t_fin() const	{ return knot[knot.size() - K]; }

	vector2<T>	eval(const T& t) const;
};


template <class T>
void
NURBS2<T>::set_knots(const int n, const T* kt)
{
	knot.clear();
	for (int i=0; i<n; ++i) {
		knot.push_back(kt[i]);
	}
}

template <class T>
void
NURBS2<T>::set_control_points(const int n, const vector2<T>* vcp2)
{
	cp.clear();
	for (int i=0; i<n; ++i) {
		const vector2<T>& cp2 = vcp2[i];
		vector3<T> cp3(cp2.x, cp2.y, 1);
		cp.push_back(cp3);
	}
}

template<class T>
vector2<T>
NURBS2<T>::eval(const T& t)  const
{
	//------------------------------------------------
	//  de Boor Cox �̃A���S���Y���ɂ��A���֐������߂�
	//------------------------------------------------

	//-- ������
	T  b[NURBS2_MAX_ORDER][NURBS2_MAX_ORDER+1];
	for (int j=0; j<K; ++j) {
		for (int i=0; i<K+1; ++i) {
			b[j][i] = 0.0;
		}
	}
	b[0][K-1] = T(1.0);	// �O���̊֐��l


	//-- �p�����[�^t�̋�Ԃ�T��
	int s, n;
	n = knot.size();
	for (s=K-1; s<n-K; s++) {
		if (knot[s] <= t && t < knot[s+1])  break;
	}

	//-- �P������(K-1)���܂Ōv�Z����
	for (int j=0; j<K; ++j) {
		for (int i=s-j, x=K-1-j; i<=s; ++i, ++x) {
			if (knot[i+j] != knot[i]) {
				b[j][x] = (t - knot[i]) / (knot[i+j] - knot[i]) * b[j-1][x];
			}
			if (knot[i+j+1] != knot[i+1]) {
				b[j][x] += (knot[i+j+1] - t) / (knot[i+j+1] - knot[i+1]) * b[j-1][x+1];
			}
		}
	}

	// ���ʂ��i�[����
	T  B[NURBS2_MAX_ORDER];
	for (int i=0; i<K; ++i) {
		B[i] = b[K-1][i];
	}

	//-------------------
	//  t�ł̒l��]������
	//-------------------

	// ���͈̔͂ŗL���Ȋe����_�ɂ��đ������킹��
	vector3<T>  v(T(0),T(0),T(0));
	for (int i=0; i<K; ++i) {
		v += B[i] * cp[s-K+1 + i];
	}

	T  w = v.z;	// �E�F�C�g
	return  vector2<T>(v.x/w, v.y/w);
}


//-------------------------------
//  �R���X�v���C�����
//  From Numerical Recipes in C
//-------------------------------
template  <class T>
void
spline(const vector<T>& xa, const vector<T>& ya, vector<T>& y2a)
{
	const int n = xa.size();
	y2a.resize(n);

	double qn,un;
	vector<double> u(n-1);

	// Set to be natural spline
	y2a[0] = u[0] = qn = un = 0.0;

	// Decomposition loop of the trigonal algorithm
	for (int i=1; i<n-1; i++) {
		double p, sig;
		sig = (xa[i]-xa[i-1]) / (xa[i+1]-xa[i-1]);
		p = sig * y2a[i-1] + 2.0;
		y2a[i] = (sig-1.0) / p;
		u[i] = (ya[i+1]-ya[i]) / (xa[i+1]-xa[i]) - (ya[i]-ya[i-1])/(xa[i]-xa[i-1]);
		u[i] = (6.0*u[i]/(xa[i+1]-xa[i-1]) - sig*u[i-1])/p;
	}
	y2a[n-1] = (un-qn*u[n-2]) / (qn*y2a[n-2]+1.0);

	// Backsubstitution loop of the tridiagonal algorithm
	for (int k=n-2; k>=0; k--) {
		y2a[k] = y2a[k]*y2a[k+1] + u[k];
	}
}


template  <class T>
T
splint(const vector<T>& xa, const vector<T>& ya, const vector<T>& y2a, const T x)
{
	const int n = xa.size();
	int k;
	int klo = 0;
	int khi = n-1;
	while (khi-klo > 1) {
		k = (khi+klo) >> 1;
		if (xa[k] > x)	khi=k;
		else			klo=k;
	}

	double h,a,b,y;
	h = xa[khi]-xa[klo];
	if (h==0.0)  return 0.0;	// error. xa must be distinct

	a = (xa[khi]-x)/h;
	b = (x-xa[klo])/h;
	y = a*ya[klo] + b*ya[khi]
		+ ((a*a*a-a)*y2a[klo] + (b*b*b-b)*y2a[khi]) * (h*h) / 6.0;

	return y;
}


#endif