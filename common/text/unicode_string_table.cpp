/**************************************************************************************************

Written by Tsunehiko Kato
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
#include  "unicode_string_table.h"
#include  <tchar.h>
#include  "string_util.h"
#include  "key_value_file.h"




//--------
//  Init
//--------
void
unicode_string_table::clear()
{
	key_ID_map.clear();
	vStrings.clear();
	vKey.clear();
}



int
unicode_string_table::set_string(const std::string& key, const std::string& str)
{
	int ID;


	if ( !is_defined_key(key) ) {
		// �V�K�o�^
		ID = vStrings.size();
		vStrings.push_back( str );
		key_ID_map[key] = ID;
		vKey.push_back(key);
	} else {
		// �㏑��
		ID = key_ID_map[key];
		vStrings[ID] = str;
	}

	return  ID;
}




//-------------
//  Get / Set
//-------------
int
unicode_string_table::get_ID(const std::string& key)  const
{
	std::map<std::string, int>::const_iterator  it = key_ID_map.find(key);

	// ����`�̃L�[
	if (it == key_ID_map.end())  {
		return -1;
	}

	return  it->second;
}


const std::string&
unicode_string_table::get_string(const std::string& key)  const
{
	int ID = get_ID(key);

	// ����`�̃L�[
	if (ID < 0)  {
		return key;
	}
	
	return  vStrings[ID];
}





//--------
//  Load
//--------
bool
unicode_string_table::load_utf8(FILE* fp, const std::string& lang_id)
{
	std::vector<key_value> vKeyValue;

	if (load_key_value_file(fp, lang_id, vKeyValue)) {

		//--  Register key and value to string table
		for (int i = 0; i < vKeyValue.size(); i++) {
			const key_value kv = vKeyValue[i];
			set_string(kv.key, kv.value);
		}
	}
	else {
		return false;
	}

	return  true;
}

