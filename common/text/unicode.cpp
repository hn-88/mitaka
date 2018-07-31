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
#include  "unicode.h"





//------------------
//  UTF8 <-> UTF32
//------------------
utf32
UTF8_to_UTF32(const char* str8, int &adv)
{
	unsigned char* p = (unsigned char*)str8;
	unsigned char c1, c2, c3, c4, c5, c6;
	adv = 1;


	c1 = *p++;
	if (c1 < 0x80) {
		adv = 1;
		return (utf32)(c1);
	}
	
	if ((c1 & 0xc0) != 0xc0) {
		return 0;	// error
	}
	

	c2 = *p++;
	if (!(c1 & 0x20)) {
		adv = 2;
		// 110yyyyx 10xxxxxx
		return (utf32)(c1 & 0x1f) << 6 | (utf32)(c2 & 0x3f);
	}

	c3 = *p++;
	if (!(c1 & 0x10)) {
		adv = 3;
		// 1110yyyy 10yxxxxx 10xxxxxx
		return (utf32)(c1 & 0x0f) << 12 | (utf32)(c2 & 0x3f) << 6 | (utf32)(c3 & 0x3f);
	}

	c4 = *p++;
	if (!(c1 & 0x08)) {
		adv = 4;
		// 11110yyy 10yyxxxx 10xxxxxx 10xxxxxx
		return (utf32)(c1 & 0x07) << 18 | (utf32)(c2 & 0x3f) << 12 | (utf32)(c3 & 0x3f) << 6 | (utf32)(c4 & 0x3f);
	}

	c5 = *p++;
	if (!(c1 & 0x04)) {
		adv = 5;
		// 111110yy 10yyyxxx 10xxxxxx 10xxxxxx 10xxxxxx
		return (utf32)(c1 & 0x03) << 24 | (utf32)(c2 & 0x3f) << 18 | (utf32)(c3 & 0x3f) << 12 | (utf32)(c4 & 0x3f) << 6 | (utf32)(c5 & 0x3f);
	}

	c6 = *p++;
	if (!(c1 & 0x02)) {
		adv = 6;
		// 1111110y 10yyyyxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
		return (utf32)(c1 & 0x01) << 30 | (utf32)(c2 & 0x3f) << 24 | (utf32)(c3 & 0x3f) << 18 | (utf32)(c4 & 0x3f) << 12 | (utf32)(c5 & 0x3f) << 6 | (utf32)(c6 & 0x3f);
	}

	// error
	//adv = 0;
	adv = 1;
	return 0;
}


char*
UTF32_to_UTF8(utf32 u, int& count)
{
	static char  uc8[7];

	if (u < 0x80) {
		count = 1;
		uc8[0] = (char)u;
		uc8[count] = '\0';
		return  uc8;
	}

	if (u < 0x800) {
		count = 2;
		uc8[0] = char(0xc0 | ((u >> 5) & 0x1f));
		uc8[1] = char(0x80 | (u & 0x3f));
		uc8[count] = '\0';
		return  uc8;
	}

	if (u < 0X10000) {
		count = 3;
		uc8[0] = char(0xe0 | ((u >> 12) & 0xff));
		uc8[1] = char(0x80 | ((u >> 6) & 0x3f));
		uc8[2] = char(0x80 | (u & 0x3f));
		uc8[count] = '\0';
		return  uc8;
	}

	if (u < 0X200000) {
		count = 4;
		uc8[0] = char(0xf0 | ((u >> 18) & 0x07));
		uc8[1] = char(0x80 | ((u >> 12) & 0x3f));
		uc8[2] = char(0x80 | ((u >> 6) & 0x3f));
		uc8[3] = char(0x80 | (u & 0x3f));
		uc8[count] = '\0';
		return  uc8;
	}

	if (u < 0X4000000) {
		count = 5;
		uc8[0] = char(0xf8 | ((u >> 24) & 0x03));
		uc8[1] = char(0x80 | ((u >> 18) & 0x3f));
		uc8[2] = char(0x80 | ((u >> 12) & 0x3f));
		uc8[3] = char(0x80 | ((u >> 6) & 0x3f));
		uc8[4] = char(0x80 | (u & 0x3f));
		uc8[count] = '\0';
		return  uc8;
	}

	count = 6;
	uc8[0] = char(0xfc | ((u >> 30) & 0x01));
	uc8[1] = char(0x80 | ((u >> 24) & 0x3f));
	uc8[2] = char(0x80 | ((u >> 18) & 0x3f));
	uc8[3] = char(0x80 | ((u >> 12) & 0x3f));
	uc8[4] = char(0x80 | ((u >> 6) & 0x3f));
	uc8[5] = char(0x80 | (u & 0x3f));
	uc8[count] = '\0';
	return  uc8;
}




//-------------------
//  UTF16 <-> UTF32
//-------------------
utf32
UTF16_to_UTF32(const utf16* str, int& adv)
{
	const utf16* p = str;
	utf16  us, us2;

	us = *p++;
	adv = 1;

	if (us <= 0xd7ff || us >= 0xe00) {
		adv = 1;
		return (utf32)us;
	}


	// Surrogate pair
	if ((us & 0xfc00) != 0xd800)  return 0;   // error
	us2 = *p++;
	if ((us2 & 0xfc00) != 0xdc00)  return 0;  // error

	utf16  hw, lw;
	hw = ((us >> 6) & 0x000f) + 1;
	lw = (us << 10) | (us2 & 0x03ff);

	adv = 2;
	return  (((utf32)hw) << 16) | lw;
}



utf16*
UTF32_to_UTF16(utf32 u, int& count)
{
	static utf16  uc16[2];

	count = 1;

	if (u<=0xd7ff || (u>=0xe000 && u<=0xffff)) {
		uc16[0] = (utf16)u;
		return uc16;
	}



	// Surrogate pair
	unsigned short us1, us2;

	u -= 0x00010000;
	us1 = ((utf16)(u >> 10)) & 0x03ff | 0xd800;
	us2 = ((utf16)u) & 0x03ff | 0xdc00;
	uc16[0] = us1;
	uc16[1] = us2;
	count = 2;
	return  uc16;
}





//-----------------
//  UTF8 <-> UTF16
//-----------------
utf16*
UTF8_to_UTF16(const char* str8, int& adv, int& count)
{
	utf32  u;
	u = UTF8_to_UTF32(str8, adv);

	return UTF32_to_UTF16(u, count);
}



char*
UTF16_to_UTF8(const utf16* str16, int& adv, int& count)
{
	utf32  u;
	u = UTF16_to_UTF32(str16, adv);

	return UTF32_to_UTF8(u, count);
}





//-----------
//  Utility
//-----------
const static char GreekLetter_UTF8[][3] = {
	"\xCE\xB1", "\xCE\xB2", "\xCE\xB3", "\xCE\xB4",
	"\xCE\xB5", "\xCE\xB6", "\xCE\xB7", "\xCE\xB8",
	"\xCE\xB9", "\xCE\xBA", "\xCE\xBB", "\xCE\xBC",
	"\xCE\xBD", "\xCE\xBE", "\xCE\xBF", "\xCF\x80",
	"\xCF\x81", "\xCF\x83", "\xCF\x84", "\xCF\x85",
	"\xCF\x86", "\xCF\x87", "\xCF\x88", "\xCF\x89"
};


const char*
get_Greek_letter_UTF8(const char GreekID) {
	if (GreekID < 0 || GreekID >= 24)  return 0;
	return  GreekLetter_UTF8[GreekID];
}
