/**************************************************************************************************

Copyright (c) 2007-2015 Tsunehiko Kato  and  4D2U Project, National Astronomical Observatory of Japan

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
#include  "mtk_places.h"
#include  "mtk_data_file.h"
#include  <map>





//--------------
//  Place Type
//--------------
struct  PlaceType
{
	float sz;		// size
	float R1, R2;	// radius for display criterion
	RGBAf col1, col2;
};




//-- Objects
static std::map<std::string, Places> vPlaces;









//--------
//  Load
//--------
static void
add_place(const std::string& obj_key, const PlaceItem& pli)
{
	if (obj_key == "") {
		return;
	}

	vPlaces[obj_key].add_place(pli);
}


bool
load_places(const char *fn)
{
	std::vector<PlaceType> vPty;	// �n���̕\���^�C�v
	std::string  obj_key;
	FILE *fp;


	fp = fopen(fn, "rt");
	if (fp == NULL)  return false;


	vPty.clear();
	vPlaces.clear();


	while (true) {
		key_data kd;

		kd = read_a_key_data(fp);
		if (kd.key == "") {
			break;
		}

		if (kd.key == "OBJECT") {
			//--  Object  --
			obj_key = kd.data[0];
		}
		else if (kd.key == "TYPE") {
			//--  Type  --
			PlaceType pt;
			int type_no;
			const char *p;

			type_no = stoi(kd.data[0]);		// type no
			pt.sz = stof(kd.data[1]);		// size
			pt.R1 = stof(kd.data[2]);		// fade radius R1
			pt.R2 = stof(kd.data[3]);		// fade radius R2

			// col1
			p = kd.data[4].c_str();
			if (!is_RGBAref(p))  { return false; }
			pt.col1 = get_RGBA_from_RGBAref(p);

			// col2
			p = kd.data[5].c_str();
			if (!is_RGBAref(p))  { return false; }
			pt.col2 = get_RGBA_from_RGBAref(p);

			if (type_no < vPty.size()) {
				vPty[type_no] = pt;
			}
			else {
				while (vPty.size() <= type_no) {
					vPty.push_back(pt);
				}
			}
		}
		else {
			//--  Place data  --
			PlaceItem  pli;
			pli.key = kd.key;			// key
			pli.lon = stof(kd.data[0]);	// lon
			pli.lat = stof(kd.data[1]);	// lat
			pli.ty  = stoi(kd.data[2]);	// type
			pli.align = kd.data[3][0];	// align

			if (pli.ty >= vPty.size()) {
				return false;
			}

			PlaceType& pt = vPty[pli.ty];
			pli.size = pt.sz;
			pli.R1 = pt.R1;
			pli.R2 = pt.R2;
			pli.col1 = pt.col1;
			pli.col2 = pt.col2;
			pli.bDispLabel = true;

			add_place(obj_key, pli);
		}

	}


	return true;
}



Places*
get_places(const std::string& obj_key)
{
	std::map<std::string, Places>::iterator it = vPlaces.find(obj_key);

	if (it != vPlaces.end()) {
		return &it->second;
	}

	return 0;
}
