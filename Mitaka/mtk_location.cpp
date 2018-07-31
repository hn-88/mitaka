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
#include  "mtk_location.h"
#include  <stdio.h>
#include  "../common/win_file.h"
#include  "../common/text/string_util.h"
#include  "../common/text/key_value_file.h"
#include  "mtk_strings.h"
#include  "mtk_directory.h"




static location initital_location;
const location default_location = { 139.54125, 35.6725, "Mitaka" };



void
set_initial_location(const location& loc)
{
	initital_location = loc;

	// 
	set_string("LANDING_POS_NAME", initital_location.name);
	

	// Set menu strings
	char str[512];

	// Screen menu
	_snprintf(str, 512, get_string("MENU_LANDING_POS_NAME_FMT").c_str(), get_string("LANDING_POS_NAME").c_str());
	set_string("MENU_LANDING_POS_NAME", str);
	// Menu bar
	_snprintf(str, 512, get_string("MENUB_LANDING_POS_NAME_FMT").c_str(), get_string("LANDING_POS_NAME").c_str());
	set_string("MENUB_LANDING_POS_NAME", str);
}



bool
load_initial_location(const char *fn, const std::string& lang_id)
{
	directory  dir = get_directory(DIR_LOCALE);

	FILE* fp = dir.fopen_utf8(fn, "rt");
	if (fp == NULL)  return false;


	std::vector<key_value> vKeyValue;
	bool res = load_key_value_file(fp, lang_id, vKeyValue);
	fclose(fp);

	if (!res) {
		set_initial_location(default_location);
		return false;
	}



	location loc;
	bool bLon, bLat, bName;

	bLon = bLat = bName = false;
	for (int i = 0; i < vKeyValue.size(); i++) {
		const key_value& kv = vKeyValue[i];


		if (stricmp(kv.key, "LONGITUDE") == 0) {
			int count = sscanf(kv.value.c_str(), "%e", &loc.lon);
			if (count == 1) {
				bLon = true;
			}
		}
		else if (stricmp(kv.key, "LATITUDE") == 0) {
			int count = sscanf(kv.value.c_str(), "%e", &loc.lat);
			if (count == 1) {
				bLat = true;
			}
		}
		else if (stricmp(kv.key, "NAME") == 0) {
			loc.name = kv.value;
			if (loc.name != "") {
				bName = true;
			}
		}
		else {
			continue;
		}
	}

	if (!bLon || !bLat || !bName) {
		set_initial_location(default_location);
		return false;
	}


	set_initial_location(loc);
	

	return true;
}


const location&
get_initial_location()
{
	return initital_location;
}