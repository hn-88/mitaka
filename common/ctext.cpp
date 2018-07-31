/******************************************************************************

Copyright (c) 2007,2014   Tsunehiko Kato

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
#include  "ctext.h"
#include  <ctype.h>


//==============
//  ����R�}���h
//==============
// ���l�I�v�V�����̎��:  [ , , ]
enum {
	CMDOPTNUM_NONE,			// �I�v�V�����Ȃ�
	CMDOPTNUM_INT,			// �����i�����j
	CMDOPTNUM_FLOAT			// ���������_���i�����j
};




struct tagCTEXT_CTRL_CODE1 {
	const char* str;
	int			ID;
};

struct tagCTEXT_CTRL_CODE2 {
	char		ch;
	int			ID;
};



tagCTEXT_CTRL_CODE  CTEXT_CTRL_CODE[] = {
	{CCMD_ALIGN_LEFT, CMDOPTNUM_NONE, "Left"},
	{CCMD_ALIGN_CENTER, CMDOPTNUM_NONE, "Center"},
	{CCMD_ALIGN_RIGHT, CMDOPTNUM_NONE, "Right"},
	{CCMD_ALIGN_KINTOU, CMDOPTNUM_NONE, "Kintou"},
	{CCMD_COLOR, CMDOPTNUM_FLOAT, "Color"},
	{CCMD_SIZE, CMDOPTNUM_FLOAT, "Size"},
	{CCMD_RSIZE, CMDOPTNUM_FLOAT, "Rsize"},
	{CCMD_TABLE, CMDOPTNUM_FLOAT, "Table"},
	{CCMD_DELIMITER, CMDOPTNUM_NONE, "Delimiter"},
	{CCMD_HSPACE, CMDOPTNUM_FLOAT, "Hspace"},
	{CCMD_VSPACE, CMDOPTNUM_FLOAT, "Vspace"},
	{CCMD_SUBSCRIPT, CMDOPTNUM_NONE, "Subscript"},
	{CCMD_SUPERSCRIPT, CMDOPTNUM_NONE, "Superscript"}
};


// '\' �Ŏn�܂鐧��R�[�h�i�Q�����ڈȍ~�́A�A���t�@�x�b�g�݂̂̕����񂩁A����ȊO�̂P�����j
tagCTEXT_CTRL_CODE1  CTEXT_CTRL_CODE1[] = {
	{"Left", CCMD_ALIGN_LEFT},
	{"Center", CCMD_ALIGN_CENTER},
	{"Right", CCMD_ALIGN_RIGHT},
	{"Kintou", CCMD_ALIGN_KINTOU},
	{"Color", CCMD_COLOR},
	{"Size", CCMD_SIZE},
	{"Rsize", CCMD_RSIZE},
	{"Hspace", CCMD_HSPACE},
	{"Vspace", CCMD_VSPACE},
	{"Table", CCMD_TABLE}
};

// '\' �ȊO�Ŏn�܂鐧��R�[�h�i�K���A���t�@�x�b�g�ȊO�̂P�����j
tagCTEXT_CTRL_CODE2  CTEXT_CTRL_CODE2[] = {
	{'&', CCMD_DELIMITER},
	{'_', CCMD_SUBSCRIPT},
	{'^', CCMD_SUPERSCRIPT}
};



// �R�}���h�Q�̋L���������ǂ����𒲂ׂ�
bool
is_char_for_command2(char ch)
{
	const int n = sizeof(CTEXT_CTRL_CODE2) / sizeof(tagCTEXT_CTRL_CODE2);
	for (int i = 0; i<n; i++) {
		if (ch == CTEXT_CTRL_CODE2[i].ch)  return true;
	}

	return  false;
}

// ������ȊO�Ɏg������ꕶ�����ǂ���
bool
is_special_char(char ch)
{
	if (ch == '\\')		return  true;
	return  is_char_for_command2(ch);
}


// �R�}���h�̔z��ԍ��𓾂�
int
get_command_index1(const char* str)
{
	const int n0 = sizeof(CTEXT_CTRL_CODE) / sizeof(tagCTEXT_CTRL_CODE);
	const int n1 = sizeof(CTEXT_CTRL_CODE1) / sizeof(tagCTEXT_CTRL_CODE1);

	for (int i = 0; i<n1; i++) {
		if (stricmp(str, CTEXT_CTRL_CODE1[i].str) == 0)  {
			int ID = CTEXT_CTRL_CODE1[i].ID;
			for (int j = 0; j<n0; j++) {
				if (ID == CTEXT_CTRL_CODE[j].ID)		return j;
			}
			return -1;
		}
	}

	return  -1;
}


// �R�}���h�Q�̔z��ԍ��𓾂�
int
get_command_index2(char ch)
{
	const int n0 = sizeof(CTEXT_CTRL_CODE) / sizeof(tagCTEXT_CTRL_CODE);
	const int n2 = sizeof(CTEXT_CTRL_CODE2) / sizeof(tagCTEXT_CTRL_CODE2);

	for (int i = 0; i<n2; i++) {
		if (ch == CTEXT_CTRL_CODE2[i].ch)  {
			int ID = CTEXT_CTRL_CODE2[i].ID;
			for (int j = 0; j<n0; j++) {
				if (ID == CTEXT_CTRL_CODE[j].ID)		return j;
			}
			return -1;
		}
	}

	return  -1;
}



//=================
//  ctext_command
//=================

//--- �R�}���h�̃I�v�V������ǂݍ���
bool
ctext_command::parse_command_options(const char*& p)
{
	const char optn_type = CTEXT_CTRL_CODE[idx].optn_type;

	if (*p != '[' || optn_type == CMDOPTNUM_NONE)  {	// �I�v�V�����Ȃ�
		return true;
	}

	// �I�v�V����������𕪉�
	while (*p != ']') {
		if (*p == '\0')  {
			return false;	// �\�����Ȃ��I��
		}

		p++;

		string elem;
		while (*p != ',' && *p != ']') {
			if (*p == ' ')  { p++; continue; }

			elem.push_back(*p++);
		}
		if (elem.size() == 0)  continue;

		int		count;
		switch (optn_type) {
		case CMDOPTNUM_INT:
		{
			int		value_int;
			count = sscanf(elem.c_str(), "%d", &value_int);
			if (count == 1)	opti.push_back(value_int);
		}
		break;
		case CMDOPTNUM_FLOAT:
		{
			float	value_float;

			const char* pp = elem.c_str();

			count = sscanf(elem.c_str(), "%f", &value_float);
			if (count == 1)  optf.push_back(value_float);
		}
		break;
		default:	count = 0;	break;
		}

		if (count != 1) {
			return 0;
		}
	}
	p++;

	return  p;
}


//--- �R�}���h���P�ǂݎ��i�ʏ�R�}���h�j
bool
ctext_command::parse_command1(const char*& p)
{
	p++;

	// �R�}���h������̓ǂݍ���
	string  cmd_str;
	if (isalpha(int(*p)) || *p == '_') {
		do {
			cmd_str.push_back(*p++);
		} while (isalpha(int(*p)) || *p == '_');
	}
	else {
		cmd_str.push_back(*p++);
	}

	// �R�}���h index �̎擾
	idx = get_command_index1(cmd_str.c_str());
	if (idx < 0)	return  false;

	return  parse_command_options(p);
}



//--- �R�}���h���P�ǂݎ��i1�����R�}���h�j
bool
ctext_command::parse_command2(const char*& p)
{
	idx = get_command_index2(*p);
	if (idx < 0)	return false;

	p++;
	return  parse_command_options(p);
}



//--- �R�}���h���P�ǂݎ��
bool
ctext_command::parse_command(const char*& p)
{
	bool res = false;
	if (*p == '\\')						{ res = parse_command1(p); }
	else if (is_char_for_command2(*p))	{ res = parse_command2(p); }

	if (!res)	return false;

	// �I�v�V�������Ȃ��A����ɋ󔒂�����Ƃ��͂�����폜����
	if (*p == ' ' && opti.size() == 0 && optf.size() == 0) {
		p++;
	}

	return  true;
}




//=========
//  ctext
//=========

void
ctext::push_command(const ctext_command& cmd)
{
	int cmd_idx = vCommand.size();
	vCommand.push_back(cmd);

	ctext_code  code;
	code.type = CTEXTCODE_COMMAND;
	code.idx = cmd_idx;
	vCode.push_back(code);
}


//--- �P�̃R�}���h��ǂݎ��
bool
ctext::parse_command(const char*& p)
{
	ctext_command  cmd;
	if (!cmd.parse_command(p))	return  false;
	push_command(cmd);
	return  true;
}


//--- �R�����g�F'\n' �܂��� '\0' �������܂ŃX�L�b�v����
void
ctext::skip_comment(const char*& p)
{
	p++;
	while (*p != '\n' && *p != '\0') {
		p++;
	}
	if (*p == '\n')  p++;
}


//--- �P�̕������ǂݎ��
bool
ctext::parse_string(const char*& p)
{
	string  str;

	while (true) {
		//-- ������̏I��
		if (*p == '}' || *p == '{' || *p == '|' || *p == '\0')  break;
		if (*p == '\\' && *(p + 1) == '\\')  break;
		if (is_special_char(*p))	break;

		//-- �R�����g���X�L�b�v����
		if (*p == '%')	{
			skip_comment(p);
			continue;
		}

		//-- ���s�͖�������
		if (*p == '\n')  {
			if (*(p + 1) == '\n')  break;	// �A�������Q�ȏ�̉��s�͉��s�R�[�h
			p++;
			continue;
		}

		// �������X�g�A
		str.push_back(*p++);
	};

	// ������̑傫�����O�łȂ��Ƃ��o�^����
	if (str.size() > 0) {
		int str_idx = vString.size();
		vString.push_back(str);

		ctext_code  code;
		code.type = CTEXTCODE_STRING;
		code.idx = str_idx;
		vCode.push_back(code);
	}
	return  true;
}


//--- �P�g�[�N����ǂݎ��
bool
ctext::parse_token(const char*& p)
{
	//--- �R�}���h�̏���
	if (*p == '\\' || is_char_for_command2(*p)) {
		return  parse_command(p);
	}

	//--- ����ȊO�͕�����Ƃ��ď�������
	return  parse_string(p);
}


//--- �P�G�������g��ǂݎ��
bool
ctext::parse_element(const char*& p)
{
	while (true) {
		//--- �V�����u���b�N�̊J�n
		if (*p == '{') {
			ctext_code  code;
			code.idx = 0;
			code.type = CTEXTCODE_ENTER_BLOCK;
			vCode.push_back(code);

			block_lv++;
			p++;
			if (!parse_block(p))  return false;
			continue;
		}

		//--- �s�̏I��
		if (*p == '|' || *p == '}') {
			return  true;
		}
		if (*p == '\\' && *(p + 1) == '\\') {
			return  true;
		}

		if (*p == '\n' && *(p + 1) == '\n') {
			while (*(p + 1) == '\n') {
				p++;
			}
			return  true;
		}

		//--- �I�[����
		if (*p == '\0') {
			return  true;
		}

		//--- �R�}���h��������
		if (!parse_token(p))	return false;
	}
}


//--- �P�u���b�N��ǂݎ��
bool
ctext::parse_block(const char*& p)
{
	elem_no = 0;

	while (true) {
		if (!parse_element(p))	return false;

		//-- next line
		if (*p == '\n' || (*p == '\\' && *(p + 1) == '\\')) {
			ctext_code  code;
			code.idx = 0;
			code.type = CTEXTCODE_NEW_LINE;
			vCode.push_back(code);

			elem_no++;

			if (*p == '\n')	p++;
			else			p += 2;
			continue;
		}

		//-- next line (upward newline)
		if (*p == '|') {
			ctext_code  code;
			code.idx = 0;
			code.type = CTEXTCODE_UPWARD_NEW_LINE;
			vCode.push_back(code);

			ctext_command cmd;
			///----- align ----
			const char* szKintou = "\\Kintou";
			cmd.parse_command(szKintou);
			push_command(cmd);
			///----- size -----
			cmd.clear();
			const char* szSize = "\\Rsize[0.5]";
			cmd.parse_command(szSize);
			push_command(cmd);

			elem_no++;
			p++;
			continue;
		}

		//-- end of the block
		if (*p == '}') {
			if (block_lv == 0)  return  false;
			ctext_code  code;
			code.idx = 0;
			code.type = CTEXTCODE_LEAVE_BLOCK;
			vCode.push_back(code);
			block_lv--;
			p++;
			return  true;
		}

		//-- other
		if (*p == '\0' && block_lv == 0) {
			return  true;
		}
		return  false;
	}
}


//---
bool
ctext::append_text(const char* p)
{
	block_lv = 0;
	bool res;
	res = parse_block(p);
	p_last = p;
	return  res;
}


