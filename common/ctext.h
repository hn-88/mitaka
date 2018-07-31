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
#ifndef  __CTEXT_H__
#define  __CTEXT_H__
#include  <vector>
#include  <string>
using namespace std;



//==============
//  ����R�}���h
//==============
struct tagCTEXT_CTRL_CODE {
	int			ID;
	char		optn_type;
	const char	*name;
};

extern tagCTEXT_CTRL_CODE  CTEXT_CTRL_CODE[];


enum {
	CCMD_NULL,
	CCMD_ALIGN_LEFT,
	CCMD_ALIGN_CENTER,
	CCMD_ALIGN_RIGHT,
	CCMD_ALIGN_KINTOU,
	CCMD_COLOR,
	CCMD_SIZE,
	CCMD_RSIZE,			// ���΃T�C�Y
	CCMD_TABLE,
	CCMD_DELIMITER,
	CCMD_HSPACE,
	CCMD_VSPACE,
	CCMD_SUBSCRIPT,
	CCMD_SUPERSCRIPT,
	CCMD_CDOT,
	CCMD_COPYRIGHT
};


//-----------
//  ��������
//-----------
enum {
	CTEXT_ALIGN_LEFT,
	CTEXT_ALIGN_CENTER,
	CTEXT_ALIGN_RIGHT,
	CTEXT_ALIGN_KINTOU
};


//==============
//  ctext_code
//==============
enum
{
	CTEXTCODE_NULL,				// �����Ȃ�
	CTEXTCODE_COMMAND,			// �R�}���h
	CTEXTCODE_STRING,			// ������
	CTEXTCODE_ENTER_BLOCK,		// �u���b�N�ɓ���
	CTEXTCODE_LEAVE_BLOCK,		// �u���b�N����o��
	CTEXTCODE_NEW_LINE,			// ���̍s�ֈڂ�
	CTEXTCODE_UPWARD_NEW_LINE	// ���̍s�ֈڂ�i�㑤�ɉ��s�j
};


struct ctext_code {
	char	type;		// �R�}���h�܂��͕�����
	int		idx;		// �z��ԍ�
};


//=================
//  ctext_command
//=================
class  ctext_command
{
public:
	int				idx;		// �R�}���h index
	vector<int>		opti;		// �I�v�V�����̐����l
	vector<float>	optf;		// �I�v�V�����̕��������_���l
public:
	ctext_command()  {};
	void	clear()	{ idx = -1; opti.clear(); optf.clear(); }
	bool	parse_command(const char*& p);
private:
	bool	parse_command_options(const char*& p);
	bool	parse_command1(const char*& p);
	bool	parse_command2(const char*& p);
};



//=========
//  ctext
//=========
class ctext
{
private:
	int		block_lv;
	int		elem_no;
public:
	vector<ctext_code>		vCode;
	vector<ctext_command>	vCommand;
	vector<string>			vString;
	const char*	p_last;
public:
	ctext()	{};
	ctext(const char*p)	{ set_text(p); }
	void	clear()		{ vCode.clear(); vString.clear(); vCommand.clear(); }
	bool	append_text(const char* p);
	bool	set_text(const char* p)		{ clear(); return append_text(p); }
private:
	void	push_command(const ctext_command& cmd);
	bool	parse_command(const char*& p);
	bool	parse_string(const char*& p);
	bool	parse_token(const char*& p);
	bool	parse_element(const char*& p);
	bool	parse_block(const char*& p);
	void	skip_comment(const char*& p);
};



#endif