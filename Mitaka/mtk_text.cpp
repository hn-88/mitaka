/******************************************************************************

Written by Tsunehiko Kato
Copyright (c) 2008-2009   Tsunehiko Kato
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
#include  "mtk_text.h"
#ifdef WIN32
#include  <windows.h>
#endif
#include  <gl/gl.h>
#include  "mtk_font.h"



//--- ������̃T�C�Y�𓾂� ---
int
mtk_text::get_string_size(const char* p, mtk_text_block_size& sz)
{
	float w, h;
	int n;	// ������
	n = 0;
	w = h = 0;

	n = get_text_size_raw(p, w, h);

	sz.w = sze * w;
	sz.hu = sze * h;
	sz.hd = 0.0;
	return n;
}


//--- �R�}���h�̃T�C�Y�𓾂� ---
int
mtk_text::get_command_size(const ctext_command& cmd, mtk_text_block_size& sz)
{
	float w, h;
	int n;	// ������
	n = 0;
	w = h = 0.0;
	const tagCTEXT_CTRL_CODE& cc = CTEXT_CTRL_CODE[cmd.idx];
	int ID = cc.ID;

	switch (ID) {
	case CCMD_SIZE:
		if (cmd.optf.size() > 0) {
			sze = cmd.optf[0];
		}
		break;
	case CCMD_RSIZE:
		if (cmd.optf.size() > 0) {
			sze *= cmd.optf[0];
		}
		break;
	case CCMD_HSPACE:
		if (cmd.optf.size() > 0) {
			w = cmd.optf[0];
		}
		break;
	case CCMD_VSPACE:
		if (cmd.optf.size() > 0) {
			sz.w = 0;
			sz.hu = 0;
			sz.hd = -cmd.optf[0];
			return 0;
		}
		break;
	default:
		break;
	}
	sz.w = sze*w;
	sz.hu = sze*h;
	sz.hd = 0.0;
	return n;
}


//--- �G�������g�̃T�C�Y�𓾂� ---
int
mtk_text::get_element_size(int& icode, mtk_text_block_size& elsz)
{
	int n;
	n = 0;
	elsz.w = elsz.hu = elsz.hd = 0.0;

	bool bTable = false;
	while (icode < vCode.size()) {
		const ctext_code& code = vCode[icode];
		mtk_text_block_size sz;
		sz.w = sz.hu = sz.hd = 0.0;
		switch (code.type) {
		case CTEXTCODE_STRING:
			n += get_string_size(vString[code.idx].c_str(), sz);
			break;
		case CTEXTCODE_COMMAND:
		{
			const ctext_command& cmd = vCommand[code.idx];
			const tagCTEXT_CTRL_CODE& cc = CTEXT_CTRL_CODE[cmd.idx];

			if (cc.ID == CCMD_TABLE) {
				float w = 0;
				for (int i = 0; i<cmd.optf.size(); i++) {
					w += sze*cmd.optf[i];
				}
				if (elsz.w < w)  elsz.w = w;

				bTable = true;
				icode++;
				continue;
			}
			n += get_command_size(cmd, sz);
		}
		break;
		case CTEXTCODE_ENTER_BLOCK:
		{
			float sze0 = sze;
			icode++;
			get_block_size(icode, sz);
			n++;	// �T�u�E�u���b�N��1�����Ɠ�������
			sze = sze0;
		}
		break;
		case CTEXTCODE_NEW_LINE:
		case CTEXTCODE_UPWARD_NEW_LINE:
			//-- through down --
		case CTEXTCODE_LEAVE_BLOCK:
			return n;
		}
		if (!bTable) elsz.w += sz.w;
		elsz.hu = max(elsz.hu, sz.hu);
		elsz.hd = min(elsz.hd, sz.hd);
		icode++;
	}
	return n;
}


//--- �u���b�N�̃T�C�Y�𓾂� ---
int
mtk_text::get_block_size(int& icode, mtk_text_block_size& bsz)
{
	int n;
	n = 0;
	bsz.w = bsz.hu = bsz.hd = 0.0;
	bool bUpward = true;
	while (icode < vCode.size()) {
		const ctext_code& code = vCode[icode];
		switch (code.type) {
		case CTEXTCODE_LEAVE_BLOCK:
			return n;
		case CTEXTCODE_NEW_LINE:
			bUpward = false;
			icode++;
			continue;
		case CTEXTCODE_UPWARD_NEW_LINE:
			bUpward = true;
			icode++;
			continue;
		default:
			break;
		}
		mtk_text_block_size  elsz;
		get_element_size(icode, elsz);
		bsz.w = max(bsz.w, elsz.w);
		if (bUpward)	{ bsz.hu += (elsz.hu - elsz.hd); }
		else			{ bsz.hd -= (elsz.hu - elsz.hd); }
		n++;
	}
	return n;
}


//---
void
mtk_text::set_element(int icode)
{
	//
	int align_mode = cur_blk.align_mode;
	if (vCode[icode].type == CTEXTCODE_COMMAND) {
		const ctext_code& code = vCode[icode];
		const tagCTEXT_CTRL_CODE& cc = CTEXT_CTRL_CODE[vCommand[code.idx].idx];
		switch (cc.ID) {
		case CCMD_ALIGN_LEFT:
			align_mode = CTEXT_ALIGN_LEFT;
			break;
		case CCMD_ALIGN_CENTER:
			align_mode = CTEXT_ALIGN_CENTER;
			break;
		case CCMD_ALIGN_RIGHT:
			align_mode = CTEXT_ALIGN_RIGHT;
			break;
		case CCMD_ALIGN_KINTOU:
			align_mode = CTEXT_ALIGN_KINTOU;
			break;
		default:
			break;
		}
	}

	int n;
	sze = cur_blk.size;
	n = get_element_size(icode, cur_blk.esz);

	//--- �����̑��蕝��ݒ肷�� ---
	if (align_mode == CTEXT_ALIGN_LEFT) {		//-- ����
		cur_blk.D = 0.0;
	}
	else if (align_mode == CTEXT_ALIGN_CENTER) {	//-- ������
		cur_blk.D = 0.0;
		float skip_x = 0.5 * cur_blk.size * (cur_blk.bsz.w - cur_blk.esz.w);
		glTranslatef(skip_x, 0, 0);
		cx += skip_x;
		if (cx > cur_blk.x2)  { cur_blk.x2 = cx; }
	}
	else if (align_mode == CTEXT_ALIGN_RIGHT)	{
		cur_blk.D = 0.0;
		float skip_x = cur_blk.size * (cur_blk.bsz.w - cur_blk.esz.w);
		glTranslatef(skip_x, 0, 0);
		cx += skip_x;
		if (cx > cur_blk.x2)  { cur_blk.x2 = cx; }
	}
	else {									//-- �ϓ�����t��
		if (n <= 1) {
			cur_blk.D = 0.0;
			float skip_x = sze * (cur_blk.bsz.w - cur_blk.esz.w);
			glTranslatef(skip_x, 0, 0);
			cx += skip_x;
			if (cx > cur_blk.x2)  { cur_blk.x2 = cx; }
		}
		else {
			cur_blk.D = (cur_blk.bsz.w - cur_blk.esz.w) / float(n - 1);
		}
	}
	sze = cur_blk.size;
}


//--- ������̕\�� ---
void
mtk_text::draw_string(const char* p)
{
	float dw, dh, w, h;


	glPushMatrix();
	const float& sz = cur_blk.size;
	const float  D2 = cur_blk.D / sz;
	glScalef(sz, sz, sz);

	get_text_size_raw(p, w, h);
	draw_text_raw(p, D2);

	glPopMatrix();
	glTranslatef(sz*w, 0, 0);
	cx += sz*w;
	if (cx > cur_blk.x2)  { cur_blk.x2 = cx; }
}




//--- �R�}���h�̏��� ---
void
mtk_text::process_command(int idx)
{
	const ctext_command& cmd = vCommand[idx];
	const tagCTEXT_CTRL_CODE& cc = CTEXT_CTRL_CODE[cmd.idx];
	switch (cc.ID) {
	case CCMD_ALIGN_LEFT:
		cur_blk.align_mode = CTEXT_ALIGN_LEFT;
		break;
	case CCMD_ALIGN_KINTOU:
		cur_blk.align_mode = CTEXT_ALIGN_KINTOU;
		break;
	case CCMD_COLOR:		//--- �F�̐ݒ�
	{
		RGBAf col;
		for (int i = 0; i<3; i++) {
			col.rgba[i] = (i < cmd.optf.size()) ? cmd.optf[i] : 0.0;
		}
		col.rgba[3] = (cmd.optf.size() >= 4) ? cmd.optf[3] : 1.0;
		set_color(col);
	}
	break;
	case CCMD_SIZE:
		if (cmd.optf.size() < 1) break;
		set_size(cmd.optf[0]);
		break;
	case CCMD_RSIZE:
		if (cmd.optf.size() < 1) break;
		set_relative_size(cmd.optf[0]);
		break;
	case CCMD_VSPACE:
		if (cmd.optf.size() >= 1) {
			float sz = cur_blk.size;
			glTranslatef(0, -sz * cmd.optf[0], 0);
		}
		break;
	case CCMD_HSPACE:
		if (cmd.optf.size() >= 1) {
			float sz = cur_blk.size;
			glTranslatef(sz * cmd.optf[0], 0, 0);
			cx += sz * cmd.optf[0];
			if (cx > cur_blk.x2) { cur_blk.x2 = cx; }
			if (cx < cur_blk.x1) { cur_blk.x1 = cx; }
		}
		break;
	case CCMD_TABLE:
	{
		const float sz = cur_blk.size;
		cur_blk.vTabStop.clear();
		cur_blk.tab_count = 0;
		float tab_pos = 0.0;
		for (int i = 0; i<cmd.optf.size(); i++) {
			tab_pos += sz*cmd.optf[i];
			cur_blk.vTabStop.push_back(tab_pos);
		}
	}
	break;
	case CCMD_DELIMITER:
	{	if (cur_blk.tab_count + 1 >= cur_blk.vTabStop.size()) break;
	float tab_pos = cur_blk.vTabStop[cur_blk.tab_count++];
	glTranslatef(-(cx - cur_blk.x1) + tab_pos, 0, 0);
	cx = cur_blk.x1 + tab_pos;
	}
	break;
	default:
		break;
	}
}


//--- �`�� ---
void
mtk_text::draw0(const mtk_text_block_info& root_blk)
{
	cur_blk = root_blk;
	// ���݂̐F���擾
	glGetFloatv(GL_CURRENT_COLOR, cur_blk.color.get_pointer());
	set_color(cur_blk.color);
	//��グ����
	glTranslatef(0, -cur_blk.bsz.hd, 0);

	int icode0;
	icode0 = 0;
	set_element(icode0);
	for (int icode = 0; icode<vCode.size(); icode++) {
		const ctext_code& code = vCode[icode];
		switch (code.type) {
		case CTEXTCODE_STRING:
			draw_string(vString[code.idx].c_str());
			break;
		case CTEXTCODE_COMMAND:
			process_command(code.idx);
			break;
		case CTEXTCODE_ENTER_BLOCK:
		{
			vBlock.push_back(cur_blk);
			int elems;
			int icode1 = icode + 1;
			sze = cur_blk.size;
			elems = get_block_size(icode1, cur_blk.bsz);
			cur_blk.x1 = cx;
			cur_blk.x2 = cur_blk.x1 + cur_blk.bsz.w;
			cur_blk.vTabStop.clear();
			cur_blk.tab_count = 0;
			if (icode + 1 < vCode.size()) {
				// �ŏ��̃G�������g�̃T�C�Y
				set_element(icode + 1);
			}
		}
		break;
		case CTEXTCODE_LEAVE_BLOCK:
		{
			// ���̃u���b�N�̉E�[�܂Ői�߂�
			glTranslatef(cur_blk.x2 - cx, -cy, 0);
			cx = cur_blk.x2;
			cur_blk = vBlock.back();
			vBlock.pop_back();
			if (cx > cur_blk.x2)  { cur_blk.x2 = cx; }
			sze = cur_blk.size;
			cy = 0;
			set_color(cur_blk.color);
		}
		break;
		case CTEXTCODE_NEW_LINE:
		{
			sze = cur_blk.size;
			glTranslatef(-(cx - cur_blk.x1), 0, 0);
			cx = cur_blk.x1;
			cy = cur_blk.size;
			cur_blk.tab_count = 0;
			if (icode + 1 < vCode.size()) {
				// �G�������g�̃T�C�Y
				set_element(icode + 1);
				glTranslatef(0, -cur_blk.esz.hu, 0);
			}
		}
		break;
		case CTEXTCODE_UPWARD_NEW_LINE:
		{
			sze = cur_blk.size;
			glTranslatef(-(cx - cur_blk.x1), 0, 0);
			cx = cur_blk.x1;
			glTranslatef(0, cur_blk.esz.hu, 0);
			cy = cur_blk.size;
			cur_blk.tab_count = 0;
			if (icode + 1 < vCode.size()) {
				// ���̃G�������g�̃T�C�Y
				set_element(icode + 1);
			}
		}
		break;
		case CTEXTCODE_NULL:
		default:
			break;
		}
	}
}


void
mtk_text::draw()
{
	mtk_text_block_info  blk;
	blk.size = 1.0;
	blk.align_mode = CTEXT_ALIGN_LEFT;
	int icode0 = 0;
	sze = blk.size;
	get_block_size(icode0, blk.bsz);
	blk.x1 = 0.0;
	blk.x2 = 0.0;
	cx = 0;
	cy = 0;
	cur_blk.x1 = 0;
	cur_blk.x2 = cur_blk.x1 + cur_blk.bsz.w;
	draw0(blk);
}


void
mtk_text::draw_center(float w)
{
	mtk_text_block_info  blk;
	blk.size = 1.0;
	blk.align_mode = CTEXT_ALIGN_CENTER;
	int icode0 = 0;
	sze = blk.size;
	get_block_size(icode0, blk.bsz);
	blk.x1 = 0.0;
	blk.x2 = w;
	blk.bsz.w = w;
	cx = 0;
	cy = 0;
	cur_blk.x1 = 0;
	cur_blk.x2 = cur_blk.x1 + cur_blk.bsz.w;
	draw0(blk);
}


void
mtk_text::draw_right(float w)
{
	mtk_text_block_info  blk;
	blk.size = 1.0;
	blk.align_mode = CTEXT_ALIGN_RIGHT;
	int icode0 = 0;
	sze = blk.size;
	get_block_size(icode0, blk.bsz);
	blk.x1 = 0.0;
	blk.x2 = w;
	blk.bsz.w = w;
	cx = 0;
	cy = 0;
	cur_blk.x1 = 0;
	cur_blk.x2 = cur_blk.x1 + cur_blk.bsz.w;
	draw0(blk);
}


void
mtk_text::draw_kintou(float w)
{
	mtk_text_block_info  blk;
	blk.size = 1.0;
	blk.align_mode = CTEXT_ALIGN_KINTOU;
	int icode0 = 0;
	sze = blk.size;
	get_block_size(icode0, blk.bsz);
	blk.x1 = 0.0;
	blk.x2 = w;
	blk.bsz.w = w;
	cx = 0;
	cy = 0;
	cur_blk.x1 = 0;
	cur_blk.x2 = cur_blk.x1 + cur_blk.bsz.w;
	draw0(blk);
}


//----------------
//  �X�̃R�}���h
//----------------

//--- �F�̐ݒ� ---
void
mtk_text::set_color(const RGBAf& col)
{
	glColor4fv(col.get_pointer());
	cur_blk.color = col;
}

//--- �T�C�Y�̐ݒ� ---
void
mtk_text::set_size(float size)
{
	cur_blk.size = size;
}

void
mtk_text::set_relative_size(float rsize)
{
	cur_blk.size *= rsize;
}





