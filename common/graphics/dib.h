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
//  2002/8/9
#ifndef __DIB_H__
#define __DIB_H__
#include  <windows.h>
#include  "image.h"


//=============
//  class dib
//=============
class
dib
{
	HDC  hDC;
	HBITMAP  hBmp, hBmpOld;
	BITMAPINFOHEADER  bmih;
	BYTE*  pBits;
	unsigned  RowLength;
public:
	dib();
	dib(const BITMAPINFOHEADER& _bmih);
	dib(int w, int h, int bitcount);
	~dib();
	void  alloc(const BITMAPINFOHEADER& _bmih);
	void  alloc(int w, int h, int bitcount);
	void  free();
	int		get_width() const	{ return  bmih.biWidth; }
	int		get_height() const	{ return  bmih.biHeight; }
	int		get_row_length() const { return RowLength; }
	int		get_image_size() const { return bmih.biSizeImage; }
	BYTE*	get_pBits()		{ return pBits; }
	BITMAPINFOHEADER*	get_pbmih() { return &bmih; }
	HDC		getDC() const  { return hDC; }
	bool	set_pixelformat(const PIXELFORMATDESCRIPTOR& pfd);
	bool	set_pixelformat(DWORD dwFlags, int nDepth, int nStencil);
	void	getRGB(int ix, int iy, BYTE& R, BYTE& G, BYTE& B) const;
	void	setRGB(int ix, int iy, const BYTE& R, const BYTE& G, const BYTE& B);
	void	fill( DWORD dwRop )	 { ::PatBlt( hDC, 0,0, bmih.biWidth, bmih.biHeight, dwRop ); }
	bool	load(FILE* fp);
	bool	save(FILE* fp) const;
	bool	load(const char* filename);
	bool	save(const char* filename) const;
};

//===  functions  ===
void	image_to_dib(const image& img, dib& bmp);


#endif