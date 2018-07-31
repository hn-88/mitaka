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
#include  "trackball.h"


//-------------
//  trackball
//-------------
void
trackball::set_point1(float x1, float y1)
{
	float x, y;
	x =  (x1-xc) / (0.5f*w); 
	y = -(y1-yc) / (0.5f*h); 
	v1 = direction(x,y);
}

quaternionf
trackball::set_point2(float x2, float y2)
{
	float x, y;
	x =  (x2-xc) / (0.5f*w); 
	y = -(y2-yc) / (0.5f*h); 
	vector3f v2 = direction(x,y);

	vector3f  axis = cross_product(v1, v2);
	float  norm = axis.norm();
	if (norm < 1e-10)  { return quaternionf(1,0,0,0); }
	axis /= norm;

	float phi = (v2-v1).norm() / (2.0 * R);
	return  make_rotation_quaternion(phi*sc, axis);
}

vector3f
trackball::direction(float x, float y)  const
{
	const float a = 1.0 / sqrt(2.0);
	float  d, t, z;

	d = sqrt(x*x + y*y);
	if (d < R * a) {
		z = float(sqrt(R*R - d*d));
	} else {
		t = R * a;
		z = t*t / d;
	}
	return  vector3d(x,y,z);
}



//---------
//  lever
//---------
void
lever::set_point1(float _y1)
{
	y1 = _y1/h;
}

float
lever::set_point2(float _y2)
{
	float y2 = _y2/h;
	float a = (y2-y1);
	return sc*a;
}
