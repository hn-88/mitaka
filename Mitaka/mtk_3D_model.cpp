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
#include  "mtk_3D_model.h"
#include  "../common/graphics/3ds_loader.h"
#include  "../common/graphics/obj_loader.h"
#include  "mtk_directory.h"
#include  <string>
#include  <vector>
#include  <map>




struct model_info {
	int			dir_idx;
	glModel3D	mdl;
};

//---
static std::map<std::string, int> ModelIdx;
static std::vector<model_info> v3DModels;




//------------------------
//  Load a 3D model file
//------------------------
bool
load_3D_model_file_sub(const char* filename, Model3D& mdl)
{
	//-- �t�@�C���̎��
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	_splitpath(filename, drive, dir, fname, ext);

	if (stricmp(ext, ".3ds") == 0) {
		return load_3DS_file(filename, mdl);
	}
	else if (stricmp(ext, ".obj") == 0) {
		return load_OBJ_file(filename, mdl);
	}

	return false;
}



int
load_3D_model_file(int dir_idx, const char* fn)
{
	// fn �̓t�@�C�����̂�
	const directory& dir = get_directory(dir_idx);
	const char *filename = dir.get_path(fn);		// �t���p�X�̃t�@�C����



	const map<string, int>::const_iterator it = ModelIdx.find(string(filename));
	int idx;

	if (it == ModelIdx.end()) {	// �܂������̃��f�����ǂݍ��܂�Ă��Ȃ���
		model_info mi;
		idx = -1;

		if (load_3D_model_file_sub(filename, mi.mdl)) {
			mi.dir_idx = dir_idx;
			mi.mdl.fit_size(1.0);	//***
			v3DModels.push_back(mi);
			idx = v3DModels.size() - 1;
		}

		ModelIdx.insert(pair<string, int>(string(filename), idx));
	}
	else {
		idx = it->second;
	}

	return idx;
}



void
bind_3D_model_textures()
{
	for (int i = 0; i < v3DModels.size(); i++) {
		model_info &mi = v3DModels[i];

		mi.mdl.bind_textures(get_directory(mi.dir_idx));
	}
}


glModel3D*
get_3DModel(int idx)
{
	if (idx < 0) return 0;

	return &v3DModels[idx].mdl;
}
