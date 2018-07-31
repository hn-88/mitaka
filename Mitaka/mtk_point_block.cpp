/**************************************************************************************************

Copyright (c) 2016   Tsunehiko Kato  and  4D2U Project, National Astronomical Observatory of Japan

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
#include  "mtk_point_block.h"





//---------------
//  point_block
//---------------
void
point_block::clear()
{
	bb.clear();
	vPos.clear();
	vIdx.clear();
}


void
point_block::add_point(int idx, const vector3f& pos)
{
	bb.add_point(pos);
	vPos.push_back(pos);
	vIdx.push_back(idx);
}





//----------------
//  point_blocks
//----------------
void
point_blocks::init(int _N, const bbox3f& _bb)
{
	N = _N;
	bb = _bb;

	vBlocks.clear();
}


void
point_blocks::add_point(int idx, const vector3f& pos)
{
	float x, y, z;
	int ix, iy, iz, ary_idx;

	x = pos.x;
	y = pos.y;
	z = pos.z;

	ix = (x - bb.x1) * N / (bb.x2 - bb.x1);
	iy = (y - bb.y1) * N / (bb.y2 - bb.y1);
	iz = (z - bb.z1) * N / (bb.z2 - bb.z1);

	if (ix < 0)  ix = 0;
	if (ix >= N) ix = N - 1;
	if (iy < 0)  iy = 0;
	if (iy >= N) iy = N - 1;
	if (iz < 0)  iz = 0;
	if (iz >= N) iz = N - 1;

	ary_idx = iz * N * N + iy * N + ix;

	vBlocks[ary_idx].add_point(idx, pos);
}
