/**************************************************************************************************

Written by Tsunehiko Kato
Copyright (c) 2008-2015   Tsunehiko Kato

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
#ifndef __GL_BMP_FONT_H__
#define __GL_BMP_FONT_H__
#include  <ft2build.h>
#include  FT_FREETYPE_H
#include  "../text/unicode.h"
#include  <string>
#include  "./bmp_font.h"






struct gl_font_info {
	int num_chars;
	GLuint texname;
};



class  gl_bmp_font : public bmp_font
{
	std::vector<gl_font_info>  vGLTextures;

	float	draw_char_utf32(const utf32& uc32, GLuint tex_prev);
public:
	int		get_text_size(const std::string& str, float& w, float& h);
	int		get_text_size_raw(const std::string& str, float& w, float& h);

	void	draw_text_raw(const std::string& text, const float& D);
	void	draw_text(const std::string& text, bool bDrawRuby);
	void	draw_text_centering(const std::string& text, bool bDrawRuby);
	void	draw_text_right(const std::string& text, bool bDrawRuby);
private:
	float	get_sub_text_block_size(const char* p, bool bKern, int& adv);
	float	get_superscript_text_size(const char* p, bool bKern, int& adv);
	float	get_subscript_text_size(const char* p, bool bKern, int& adv);
	float	draw_sub_text_block(const char *p, bool bKern, int& adv);
	void	draw_superscript(const char *p, bool bKern, int& adv);
	void	draw_subscript(const char *p, bool bKern, int& adv);
	void	draw_ruby(const char* p0, bool bKern, int& adv, float scl, float w);
	void	skip_ruby(const char* p0, int& adv) const;
};






#endif
