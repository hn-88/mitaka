/******************************************************************************

Copyright (c) 2015   Tsunehiko Kato

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
#include  "fast_pow.h"
#include  <math.h>
#include  <float.h>
#include  <stdint.h>


const int FLT_BITS = 32;
const int FLT_MNT_BITS = FLT_MANT_DIG - 1;
const int FLT_IDX_BITS = FLT_BITS - FLT_MNT_BITS - 1;
const int FLT_IDX_OFFSET = FLT_MAX_EXP - 1;
const int32_t FLT_MNT_MASK = (1 << FLT_MNT_BITS) - 1;

const int TBL_MNT_BITS = 10;

const int tblIdxN = 1 << FLT_IDX_BITS;
const int tblMntN = 1 << TBL_MNT_BITS;


fast_pow::fast_pow(float a)
{
	init(a);
}


void
fast_pow::init(float a)
{
	if (tblIdx == 0) {
		tblIdx = new float[tblIdxN];
	}
	if (tblMnt == 0) {
		tblMnt = new float[tblMntN];
	}

	// table of 2^s
	for (int i = 0; i < tblIdxN; i++) {
		tblIdx[i] = powf(2.0f, float(i - FLT_IDX_OFFSET)*a);
	}

	// table of mantissa
	for (int i = 0; i < tblMntN; i++) {
		tblMnt[i] = powf(1.0f + i / float(tblMntN), a);
	}
}


float
fast_pow::eval(float x) const
{
	if (tblIdx == 0 || tblMnt == 0 || x < 0.0f)  return 0.0f;

	union {
		float f;
		int32_t i;
	} xx;
	xx.f = x;

	int32_t idx, mnt;
	idx = xx.i >> FLT_MNT_BITS;
	mnt = (xx.i & FLT_MNT_MASK) >> (FLT_MNT_BITS - TBL_MNT_BITS);

	return tblIdx[idx] * tblMnt[mnt];
}



// Refs:
// Original algorthm from Shigeo Mitsunari
// http://homepage1.nifty.com/herumi/adv/adv43.html
