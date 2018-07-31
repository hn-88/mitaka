/******************************************************************************

Copyright (c) 2007-2015   Tsunehiko Kato

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
#include  "3D_common.h"




//-----------
//  frustum
//-----------


// frustum ��ݒ肷��
void
frustum::set(float _x1, float _x2, float _y1, float _y2, float _z1, float _z2)
{
	x1 = _x1;
	x2 = _x2;
	y1 = _y1;
	y2 = _y2;
	z1 = _z1;
	z2 = _z2;
}


// frustum ���v�Z����
void
frustum::set2(	float aspc_rt,		// �E�B���h�E�̃A�X�y�N�g��(h/w)
				float screen_z,		// �X�N���[���܂ł̋���
				float screen_w,		// �X�N���[���̕�
				float near_z,		// �߂��N���b�s���O�ʂ܂ł̋���
				float far_z,		// �����N���b�s���O�ʂ܂ł̋���
				float offset_x		// �J�����ʒu�� x �����̂���i�ڂ̂���{�X�N���[���j
			 )
{
	float  dd = screen_w / 2.0f;
	if (aspc_rt >= 1.0f) {
		x1 = -dd;
		y1 = -aspc_rt * dd;
	}
	else {
		x1 = -(1.0f/aspc_rt) * dd;
		y1 = -dd;
	}
	x2 = -x1;
	y2 = -y1;
	x1 -= offset_x;
	x2 -= offset_x;

	float scl = near_z / screen_z;
	x1 *= scl;
	x2 *= scl;
	y1 *= scl;
	y2 *= scl;
	z1 = near_z;
	z2 = far_z;
}
