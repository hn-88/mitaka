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
#ifndef __FRAME_H__
#define __FRAME_H__
#include  "../math/vector3.h"
#include  "../math/quaternion.h"


//---------
//  Frame
//---------
template  <class T>
class frame
{
public:
	vector3<T>		r;		// position of origin
	quaternion<T>	ori;	// orientation
public:
	frame() : r(0,0,0), ori(1,0,0,0) {};
	frame(const vector3<T>& r0, const quaternion<T>& ori0) : r(r0), ori(ori0) {};
	frame(const vector3<T>& r0, const vector3<T>& eF, const vector3<T>& eU);

	void	reset() { r.set(0,0,0); ori.set(1,0,0,0); }
	void	set_axes(const vector3<T>& eF, const vector3<T>& eU);
	void	get_axes(vector3<T>& eF, vector3<T>& eU, vector3<T>& eR) const;
};

template  <class T>
frame<T>::frame(const vector3<T>& r0, const vector3<T>& eF, const vector3<T>& eU) : r(r0)
{
	ori = axes_to_quaternion(eF, eU);
}

template  <class T>
void
frame<T>::set_axes(const vector3<T>& eF, const vector3<T>& eU)
{
	ori = axes_to_quaternion<T>(eF, eU);
}

template  <class T>
void
frame<T>::get_axes(vector3<T>& eF, vector3<T>& eU, vector3<T>& eR) const
{
	quaternion_to_axes<T>(ori, eF, eU, eR);
}

//-- special version
typedef		frame<double>	framed;
typedef		frame<float>	framef;


#endif
