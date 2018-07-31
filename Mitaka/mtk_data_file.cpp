/**************************************************************************************************

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
#include  "mtk_data_file.h"
#include  "../common//text/string_util.h"




static std::vector<std::string>
get_token(FILE *fp, const std::string& delim, const std::string& trim_chars)
{
	const int buf_size = 1024;
	static char buf[buf_size];

	while (true) {
		std::vector<std::string> elems;
		char *p;

		p = fgets(buf, buf_size, fp);
		if (p == NULL) {
			// EOF
			return std::vector<std::string>();
		}

		elems = split2_trim(remove_cpp_comment(buf), delim, trim_chars);

		if (elems.size() > 1) {
			return elems;
		}
	}
}



key_data
read_a_key_data(FILE *fp)
{
	const std::string trim_chars = " \t\n";
	key_data kd;
	std::string buf;
	std::vector<std::string> elems;



	//--  key
	elems = get_token(fp, ":", trim_chars);

	if (elems.size() != 2) {
		// size < 2: EOF
		// size > 2: multiple keys in one line
		kd.key = std::string("");
		return kd;
	}

	kd.key = elems[0];
	buf = elems[1];



	//--  data
	if (buf.size() != 0) {
		// data exist in the first line
		kd.data = split2_trim(buf, ",", trim_chars);
		if (kd.data.back().size() == 0) {
			// continue data
			kd.data.pop_back();
		}
		else {
			// end data line
			return kd;
		}
	}

	while (true) {
		int sz;
		bool bContinue;

		elems = get_token(fp, ",", trim_chars);
		sz = elems.size();
		if (sz == 0)  break;



		if (elems.back().size() == 0) {
			bContinue = true;
			elems.pop_back();
		}
		else {
			bContinue = false;
		}

		// add data
		kd.data.insert(kd.data.end(), elems.begin(), elems.end());
		if (!bContinue) {
			break;
		}
	}


	return kd;
}
