/******************************************************************************

Copyright (c) 2007   Tsunehiko Kato

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
#include  "gl_texture.h"
#define GL_CLAMP_TO_EDGE 0x812F


// ���łɓǂݍ��܂�Ă��邩
bool
gl_textures::is_loaded(const char* name) const
{
	map<string, GLuint>::const_iterator  it = textures.find(string(name));
	if (it != textures.end()) {
		return true;		// ���łɓ����t�@�C�����̃e�N�X�`��������ꍇ
	}
	return false;
}


//
GLuint
gl_textures::get_texture_name(const char* name)  const
{
	map<string, GLuint>::const_iterator  it = textures.find(string(name));
	if (it != textures.end()) {
		return it->second;	// ���łɓ����t�@�C�����̃e�N�X�`��������ꍇ
	}
	return 0;
}


GLuint
gl_textures::add_texture(const char* name, int shrink_rate, bool bRepeatY)
{
	GLuint  texname;
	// ���łɃ��[�h����Ă���ꍇ�͂��̒l��Ԃ�
	texname = get_texture_name(name);
	if (texname!=0)  { return texname; }
	// ���[�h����Ă��Ȃ��ꍇ�͐V���Ƀ��[�h
	glGenTextures(1, &texname);
	// �t�@�C���̃��[�h
	image img;
	if (!load(name, img)) {
		// ���[�h�Ɏ��s
		glDeleteTextures(1, &texname);
		return 0;
	}
	// �o�C���h
	GLint RepeatY = (bRepeatY) ? GL_REPEAT : GL_CLAMP_TO_EDGE;
	glBindTexture(GL_TEXTURE_2D, texname);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, RepeatY);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	gl_set_texture_mipmaps(img, shrink_rate);
	img.free();
	if (glIsTexture(texname)==GL_TRUE) {
		// �V�����e�N�X�`���𐶐��ł�����
		pair<string, GLuint>  pr;
		pr.first = string(name);
		pr.second = texname;
		textures.insert(pr);
		return  texname;
	}
	// �e�N�X�`���̐����Ɏ��s
	return  0;
}


void
gl_textures::delete_texture(const char* name)
{
	GLuint  texname;
	// ���[�h����Ă��Ȃ��ꍇ�͉����������^�[��
	texname = get_texture_name(name);
	if (texname==0)		return;
	glDeleteTextures(1, &texname);
}

void
gl_textures::delete_texture_name(GLuint texname)
{
	if (texname == 0)  return;
	for (map<string, GLuint>::iterator  it = textures.begin(); it != textures.end(); it++) {
		if (it->second == texname) {
			glDeleteTextures(1, &it->second);
			textures.erase(it);
			return;
		}
	}
}

bool
gl_textures::load(const char* name, image& img)
{
	if (!img.load( dir.get_path(name)) )	{
		return  false;
	}
	return  true;
}
