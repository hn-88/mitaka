/**************************************************************************************************

Written by Tsunehiko Kato
Copyright (c) 2008   Tsunehiko Kato

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
#include  <stdio.h>
#include  <string.h>
#include  "mtk_strings.h"
#include  "../common/text/unicode_string_table.h"
#include  "mtk_font.h"



static unicode_string_table vStrings;





//----------------
//  Clear / Load
//----------------
void
clear_strings()
{
	vStrings.clear();
}


bool
load_strings(FILE *fp, const std::string& lang_id)
{
	return vStrings.load_utf8(fp, lang_id);
}







//-------------
//  Get / Set
//-------------
int
set_string(const std::string& key, const std::string& str)
{
	return vStrings.set_string(key, str);
}


int
get_string_ID(const std::string& key)
{
	return vStrings.get_ID(key);
}


bool
is_defined_key(const std::string& key)
{
	return vStrings.is_defined_key(key);
}


const std::string&
get_string(int ID)
{
	return vStrings.get_string(ID);
}


const std::string&
get_string(const std::string& key)
{
	return vStrings.get_string(key);
}


const std::string&
get_string_key(int ID)
{
	return vStrings.get_key(ID);
}

std::wstring
get_wstring(const std::string& key)
{
	return utf8_to_wstring(get_string(key));
}




//
//
//
std::string
apply_null_character(const std::string& str)
{
	std::string str2;


	for (int i = 0; i < str.size(); i++) {
		char ch;
		ch = str[i];

		if (ch == '\\' && (i < str.size()-1) && str[i+1]=='0') {
			str2 += '\0';
			i++;
		}
		else {
			str2 += ch;
		}
	}

	return str2;
}


