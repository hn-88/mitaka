/**************************************************************************************************

Copyright (c) 2007   Tsunehiko Kato  and  4D2U Project, National Astronomical Observatory of Japan

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
#include  <windows.h>
#include  "mtk_image.h"
#include  <stdio.h>


void
mtk_image::set_image(const char* fn)
{
	if (bImageLoaded && stricmp(szFileName, fn)==0)  return;

	strcpy(szFileName, fn);
	bImageLoaded = false;
}

void
mtk_image::load()
{
	if (szFileName[0] == '\0')  return;

	if ( bImageLoaded )  return;

	if (texName!=0) {
		glDeleteTextures(1, &texName);
	}
	glGenTextures(1, &texName);

	image img;
	img.load(szFileName);
	set_texture(texName, img);

	w = img.get_width();
	h = img.get_height();
	bImageLoaded = true;
}

void
mtk_image::draw(float scr_w, float scr_h)
{
	if (szFileName[0] == '\0')  return;

	//--  �ǂݍ���ł��Ȃ����͓ǂݍ���
	if (!bImageLoaded) {
		load();
	}

	//-- �E�B���h�E�Ƀt�B�b�g������i�����ɔz�u�j
	float imgw, imgh;
	float rt_wn = float(scr_h)/float(scr_w);
	float rt_img = float(h)/float(w);
	if (rt_img > rt_wn) {
		// �E�B���h�E�����摜�̕����c��
		imgw = 1.0f/rt_img;
		imgh = 1;
	} else {
		// �摜�����E�B���h�E�̕����c��
		float f = scr_w / scr_h;
		imgw = f;
		imgh = f * rt_img;
	}

	//--  �摜��\��
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, texName);
		glScalef(imgw, imgh, 1.0f);

		glBegin(GL_QUADS);
			glTexCoord2f(1,1);
			glVertex3f( 1, 1,0);
			glTexCoord2f(0,1);
			glVertex3f(-1, 1,0);
			glTexCoord2f(0,0);
			glVertex3f(-1,-1,0);
			glTexCoord2f(1,0);
			glVertex3f( 1,-1,0);
		glEnd();
	glPopMatrix();
}
