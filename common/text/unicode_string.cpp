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
#include  "unicode_string.h"





//------------------
//  UTF8 <-> UTF32
//------------------
string_utf32
UTF8_to_UTF32_string(const std::string& src8)
{
	const char* p0 = src8.c_str();
	string_utf32 dest32;

	int i = 0;
	while (i < src8.size()) {
		int adv;
		utf32  uc32;

		uc32 = UTF8_to_UTF32(p0 + i, adv);
		i += adv;

		dest32.push_back(uc32);
	}
	dest32.push_back(0);


	return dest32;
}



//-----------------
//  UTF8 <-> UTF16
//-----------------
string_utf16
UTF8_to_UTF16_string(const std::string& src8)
{
	const char* p0 = src8.c_str();
	string_utf16 dest16;

	int i = 0;
	while (i < src8.size()) {
		int adv, count;
		utf16*  p16;

		p16 = UTF8_to_UTF16(p0 + i, adv, count);
		i += adv;

		dest16.push_back(*p16++);
		if (count > 1) {
			dest16.push_back(*p16++);
		}
	}

	dest16.push_back(utf16(0));

	return dest16;
}



std::string
UTF16_to_UTF8_string(const string_utf16& src16)
{
	const utf16* p0 = src16.data();
	std::string dest8;

	int i = 0;
	while (i < src16.size()) {
		int adv, count;
		char* p8;

		p8 = UTF16_to_UTF8(p0 + i, adv, count);
		i += adv;

		while (count != 0) {
			dest8.push_back(*p8++);
			count--;
		}
	}

	return dest8;
}




// wstring version
std::wstring
utf8_to_wstring(const std::string& str)
{
	std::wstring wstr;

	string_utf16 str16 = UTF8_to_UTF16_string(str);

	for (int i = 0; i < str16.size(); i++) {
		wstr += wchar_t(str16[i]);
	}

	return wstr;
}


