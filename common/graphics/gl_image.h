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
//  2002/5/17, 7/9
#ifndef __GL_IMAGE_H__
#define __GL_IMAGE_H__
#include  <stdio.h>
#include  "image.h"
#include  <windows.h>
#include  <gl/gl.h>
#include  <gl/glu.h>


void		set_glColor3f(const image &img, int x, int y); 
void		gl_set_texture(GLenum target, const image& img);
void		gl_set_texture_mipmaps(GLenum target, GLenum fmt, const image& img, int shrink_rate);
void		gl_set_texture_mipmaps(GLenum target, const image& img, int shrink_rate);
void		gl_read_pixels(image& img, int x, int y, int w, int h);
void		gl_capture_viewport(image& img, GLenum read_buf);


//--- inline
inline  bool
is2n(int a)
{
	return  (a>0) && (0 == (a & (a - 1)));
}


inline  int
to2n(int a)
{
	if (a == 0 || is2n(a))  return a;

	int b = 1;
	while (b<a) { b <<= 1; }

	return b;
}

inline  GLenum
get_GLformat(int channels)
{
	switch (channels) {
	case 1:	 return  GL_LUMINANCE;
	case 2:  return  GL_LUMINANCE_ALPHA;
	case 3:	 return  GL_RGB;
	case 4:  return  GL_RGBA;
	default: return  GL_RGB;		// error!
	}
}


inline void
gl_set_texture(const image& img)
{
	gl_set_texture(GL_TEXTURE_2D, img);
}

inline void
gl_set_texture_mipmaps(const image& img, int shrink_rate)
{
	gl_set_texture_mipmaps(GL_TEXTURE_2D, img, shrink_rate);
}


//----
inline  void
set_glColor3f(const image& img, int x, int y)
{
	byte R, G, B;
	img.getRGB(x,y, R,G,B);
	glColor3ub( R, G, B );
}


#endif