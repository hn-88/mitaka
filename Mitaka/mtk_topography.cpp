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
#include  <Windows.h>
#include  "mtk_topography.h"
#include  <vector>
#include  <map>





static std::map<std::string, TopoSphere>  vTopo;
static char TopoZoomMode = 0;

const float TopoZoomRate[5] = {
	1.0f, 2.0f, 5.0f, 10.0f, 20.0f
};






TopoSphere*
get_topography(const std::string& key)
{
	std::map<std::string, TopoSphere>::iterator it = vTopo.find(key);
	if (it != vTopo.end()) {
		return  &it->second;
	}
	else {
		return 0;
	}

}




void
set_topo_elv_fac(const float& elv_fac)
{
	for (std::map<std::string, TopoSphere>::iterator it = vTopo.begin(); it != vTopo.end(); it++) {
		it->second.set_elv_fac(elv_fac);
	}
}




//------------------
//  Load Topograph
//------------------
void
load_topography(const std::string& key, const char* fn, float shift_tex)
{
	TopoSphere *p;

	p = get_topography(key);
	if (p != 0) {
		p->load(fn);
	}
	else {
		if (vTopo[key].load(fn)) {
			vTopo[key].shift_texture_coord(shift_tex);
		}
		else {
			vTopo.erase(key);
		}
	}
}


void
load_topography_all(directory& dir)
{
	const char *fn[3] = { "earth_topo.dat", "moon_topo.dat", "mars_topo.dat" };
	const char *key[3] = {"PLNT_EARTH", "SAT_E_MOON", "PLNT_MARS"};
	const float shift_tex[3] = {180.0f, 180.0f, 0.0f};

	for (int i = 0; i < 3; i++) {
		load_topography(key[i], dir.get_path(fn[i]), shift_tex[i]);
	}
	set_topo_elv_fac(TopoZoomRate[TopoZoomMode]);
}






//-------------
//  Topo Zoom
//-------------
void
set_topo_zoom_mode(char mode)
{
	TopoZoomMode = mode;
	set_topo_elv_fac(TopoZoomRate[TopoZoomMode]);
}

char
get_topo_zoom_mode()
{
	return TopoZoomMode;
}


float
get_topo_zoom_rate()
{
	return TopoZoomRate[TopoZoomMode];
}