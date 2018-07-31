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
#include  "avi.h"
#include  <memory.h>


//==================
//  class  AVIFile
//==================
AVIFile::~AVIFile()
{
	close();
}

//---  open AVI file  ---
bool
AVIFile::open(const char* filename, UINT mode)
{
	if ( AVIFileOpen(&pAVI, filename, mode, NULL) != 0) {
		pAVI = 0;
		return  false;
	}
	return true;
}

//---  create VIDEO stream  ---
bool
AVIFile::createVIDEO(UINT width, UINT height, UINT timeScale, UINT rate)
{
	if (!isValid())  return false;
	w = width;
	h = height;
	bits = 24;
	chs = 3;
	unsigned  row_stride = (w * bits + 31)/32 * 4;
	image_size = h * row_stride;

	AVISTREAMINFO  strhdr;
	memset( &strhdr, 0, sizeof(strhdr) );
	strhdr.fccType = streamtypeVIDEO;
	strhdr.fccHandler = 0;
	strhdr.dwScale = timeScale;
	strhdr.dwRate = rate;
	strhdr.dwSuggestedBufferSize = image_size;
	SetRect( &strhdr.rcFrame, 0,0, w,h);
	if (AVIFileCreateStream( pAVI, &ps, &strhdr) != 0)  return  false;

	return  true;
}


//---  select compression type  ---
bool
AVIFile::selectCompression()
{
	if (!isValid())  return  false;
	AVICOMPRESSOPTIONS  opts;
	AVICOMPRESSOPTIONS FAR  *aopts[1] = {&opts};
	memset( &opts, 0, sizeof(opts) );
	if (!AVISaveOptions( NULL, 0, 1, &ps, (LPAVICOMPRESSOPTIONS FAR *) &aopts))  return false;
	if ( AVIMakeCompressedStream( &psCompressed, ps, &opts, NULL ) != AVIERR_OK )  return false;
	return  true;
}


//---  set bitmap format  ---
bool
AVIFile::setFormat(BITMAPINFOHEADER* pBih)
{
	if (!isValid())  return  false;
	if ( AVIStreamSetFormat( psCompressed, 0, pBih, sizeof(BITMAPINFOHEADER) ) !=0)  return false;
	return  true;
}


//---  write one frame  ---
bool
AVIFile::writeFrame(UINT frame, void* pBits)
{
	if (!isValid())  return  false;
	if ( AVIStreamWrite( psCompressed, frame, 1, pBits, image_size, AVIIF_KEYFRAME, NULL, NULL ) != 0)  return  false;
	return  true;
}


//--- close the AVI file ---
void
AVIFile::close()
{
	if (ps)  AVIStreamClose(ps);
	if (psCompressed) AVIStreamClose(psCompressed);
	if (pAVI)  AVIFileClose(pAVI);
	pAVI = 0;
	ps = psCompressed = 0;
}