/******************************************************************************

Copyright (c) 2007-2015   Tsunehiko Kato

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
#ifndef __IMAGE_H__
#define __IMAGE_H__
#include  <stdio.h>

typedef unsigned char  byte;


//-------------------------------
//  error object  for exception
//-------------------------------
class  img_err
{
	const char* err_str;
public:
	img_err(const char* error_string) : err_str(error_string) {};

	const char* get_err_str() { return  err_str; }
};


//======================================
//  class  gl_image
//--------------------------------------
//		JPEG	: libjpeg version 6b
//		PNG		: libpng version 1.2.2
//======================================
class  image
{
	int  width, height, channels;
	byte* pBits;
	int size;
public:
	image() : width(0), height(0), pBits(0), channels(0) {};
	image(int w, int h, int chs) : pBits(0)		{ alloc(w, h, chs); }
	~image()									{ free(); }
	void	alloc(int w, int h, int chs);
	void	free();
	int		get_width() const		{ return  width; }
	int		get_height() const		{ return  height; }
	int		get_channels() const	{ return  channels; }
	int		get_size() const		{ return  size; }
	int		get_stride() const		{ return  width * channels; }
	byte*	get_pointer() const		{ return  pBits; }

	//-- pixel
	byte	getPixel(int x, int y, int ch) const;
	void	setPixel(int x, int y, int ch, byte val);
	float	getPixelf(int x, int y, int ch) const;
	void	setPixelf(int x, int y, int ch, float val);
	float	getPixelf_ave(float x1, float y1, float x2, float y2, int ch) const;

	byte	getR(int x, int y) const	{ return getPixel(x,y,0); }
	byte	getG(int x, int y) const	{ return getPixel(x,y,1); }
	byte	getB(int x, int y) const	{ return getPixel(x,y,2); }
	byte	getA(int x, int y) const	{ return getPixel(x,y,3); }
	float	getRf(int x, int y) const	{ return getPixelf(x,y,0); }
	float	getGf(int x, int y) const	{ return getPixelf(x,y,1); }
	float	getBf(int x, int y) const	{ return getPixelf(x,y,2); }
	float	getAf(int x, int y) const	{ return getPixelf(x,y,3); }
	void	getRGB(int x, int y, byte& R, byte& G, byte& B) const;
	void	getRGBf(int x, int y, float& Rf, float& Gf, float& Bf) const;

	void	setRGB(int x, int y, byte R, byte G, byte B);
	void	setRGBA(int x, int y, byte R, byte G, byte B, byte A);
	void	setRGBf(int x, int y, float R, float G, float B);
	void	setRGBAf(int x, int y, float R, float G, float B, float A);
	void	setA(int x, int y, byte A);
	void	setAf(int x, int y, float A);

	//-- scaling
	void		scale_image(const image& srg_img, float src_x1, float src_y1, float src_x2, float src_y2);

	//-- load/save image file
	bool		loadBMP(FILE* fp);
	bool		saveBMP(FILE* fp);
	bool		loadJPEG(FILE* fp);
	bool		saveJPEG(FILE* fp);
	bool		loadPNG(FILE* fp);
	bool		savePNG(FILE* fp);
	bool		loadTGA(FILE* fp);
	bool		saveTGA(FILE* fp);
	//  by filename
	bool		loadBMP(const char* filename);
	bool		saveBMP(const char* filename);
	bool		loadJPEG(const char* filename);
	bool		saveJPEG(const char* filename);
	bool		loadPNG(const char* filename);
	bool		savePNG(const char* filename);
	bool		loadTGA(const char* filename);
	bool		saveTGA(const char* filename);
	bool		load(const char* filepath);
	bool		save(const char* filepath);
	//  by filename (wchar_t)
	bool		loadBMP(const wchar_t* filename);
	bool		saveBMP(const wchar_t* filename);
	bool		loadJPEG(const wchar_t* filename);
	bool		saveJPEG(const wchar_t* filename);
	bool		loadPNG(const wchar_t* filename);
	bool		savePNG(const wchar_t* filename);
	bool		loadTGA(const wchar_t* filename);
	bool		saveTGA(const wchar_t* filename);
	bool		load(const wchar_t* filepath);
	bool		save(const wchar_t* filepath);

private:
	unsigned long	get_index(int x, int y, int ch)  const;
	float		pixel_ave_xloop(const float&x1, const float& x2, const int& iy, const int& ch) const;
};


//--------
inline  unsigned long
image::get_index(int x, int y, int ch)  const
{
	return  (y*width + x)*channels + ch;
}

inline  byte
image::getPixel(int x, int y, int ch)  const
{
	return  pBits[get_index(x,y,ch)];
}

inline  void
image::setPixel(int x, int y, int ch, byte val)
{
	pBits[get_index(x,y,ch)] = val;
}

inline  float
image::getPixelf(int x, int y, int ch)  const
{
	return  float(getPixel(x,y,ch)) / 255;
}

inline  void
image::setPixelf(int x, int y, int ch, float val)
{
	pBits[get_index(x,y,ch)] = byte(255 * val);
}

inline	void
image::getRGB(int x, int y, byte& R, byte& G, byte& B) const
{
	unsigned long  idx = get_index(x,y,0);
	R = pBits[idx];
	G = pBits[idx+1];
	B = pBits[idx+2];
}

inline  void
image::getRGBf(int x, int y, float& Rf, float& Gf, float& Bf) const
{
	byte  R, G, B;
	getRGB(x,y,R,G,B);
	Rf = float(R)/255;
	Gf = float(G)/255;
	Bf = float(B)/255;
}

inline  void
image::setRGB(int x, int y, byte R, byte G, byte B)
{
	unsigned long  idx = get_index(x,y,0);
	pBits[idx+0] = R;
	pBits[idx+1] = G;
	pBits[idx+2] = B;
}

inline  void
image::setRGBf(int x, int y, float Rf, float Gf, float Bf)
{
	setRGB(x,y, byte(255*Rf), byte(255*Gf), byte(255*Bf));
}

inline  void
image::setRGBA(int x, int y, byte R, byte G, byte B, byte A)
{
	unsigned long  idx = get_index(x,y,0);
	pBits[idx+0] = R;
	pBits[idx+1] = G;
	pBits[idx+2] = B;
	pBits[idx+3] = A;
}

inline  void
image::setRGBAf(int x, int y, float R, float G, float B, float A)
{
	setRGBA(x,y, byte(255*R), byte(255*G), byte(255*B), byte(255*A));
}

inline  void
image::setA(int x, int y, byte A)
{
	pBits[get_index(x,y,3)] = A;
}

inline  void
image::setAf(int x, int y, float A)
{
	setA(x,y, byte(255*A));
}


#endif