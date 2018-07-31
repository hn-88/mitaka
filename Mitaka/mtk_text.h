/**************************************************************************************************

Copyright (c) 2008   Tsunehiko Kato
Copyright (c) 2015   Tsunehiko Kato  and  4D2U Project, National Astronomical Observatory of Japan

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
#ifndef __MTK_TEXT_H__
#define __MTK_TEXT_H__
#include  "../common/ctext.h"
#include  "../common/graphics/color.h"


struct mtk_text_block_size {
	float w, hu, hd;
};


//---------------------
//  mtk_text_block_nifo
//---------------------
struct mtk_text_block_info {
	float	x1, x2, y;
	mtk_text_block_size  bsz, esz;
	float	size;
	RGBAf	color;
	float   D;
	char	align_mode;

	vector<float> vTabStop;
	int		tab_count;
};


//============
//  mtk_text
//============
class  mtk_text : public  ctext
{
	vector<mtk_text_block_info>	vBlock;
	mtk_text_block_info	cur_blk;
	float cx, cy;
	float bx1, bx2;
	float	sze;	// ���݂̃G�������g���̑��ΓI�T�C�Y
public:
	mtk_text() {};
	mtk_text(const char* p) : ctext(p)  {}
	void	draw();
	void	draw_center(float w);
	void	draw_right(float w);
	void	draw_kintou(float w);
	mtk_text_block_size	 get_total_size()  {
		int icode = 0;

		sze = 1.0;

		mtk_text_block_size  bsz;
		get_block_size(icode, bsz);
		return  bsz;
	}
private:
	//-- �T�C�Y����їv�f���̌v�Z
	void	set_element(int icode);
	int		get_string_size(const char* p, mtk_text_block_size& sz);
	int		get_command_size(const ctext_command& cmd, mtk_text_block_size& sz);
	int		get_element_size(int& icode, mtk_text_block_size& sz);
	int		get_block_size(int& icode, mtk_text_block_size& sz);
	//-- �`��
	void	draw_string(const char* p);
	void	draw0(const mtk_text_block_info& root_blk);
	void	process_command(int idx);
	//-- �X�̃R�}���h
	void	set_color(const RGBAf& col);
	void	set_size(float size);
	void	set_relative_size(float rsize);
};





#endif