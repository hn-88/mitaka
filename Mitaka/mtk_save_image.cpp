/**************************************************************************************************

Copyright (c) 2007-2015   Tsunehiko Kato  and  4D2U Project, National Astronomical Observatory of Japan

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
#include  <gl/gl.h>
#include  "mtk_save_image.h"
#include  "../common/graphics/gl_image.h"
#include  "../common/win_dlg.h"
#include  "../common/text/unicode_string.h"
#include  "mtk_draw_struct.h"
#include  "mtk_pbuffer.h"
#include  "mtk_strings.h"


//  externs
void				draw_scene_all(const mtk_draw_struct& ds, bool bClear);
mtk_draw_struct calc_draw_struct(const mtk_camera& camera_base, float aspc_rt, int _vp_x1, int _vp_x2, int _vp_y1, int _vp_y2, vector3f dr, bool bRotBB, float line_w);


//
static wchar_t szFileName[MAX_PATH] = L"\0";
static int ImageType;
static int SaveImageWidth, SaveImageHeight;



const wchar_t*
get_save_image_high_res_filename()
{
	return  szFileName;
}



bool
save_image_high_resolution_get_filename(int w, int h)
{
	static char  fn[MAX_PATH];
	static char  filename16[MAX_PATH], filter16[256], title16[256];
	fn[0] = '\0';


	std::wstring filter, title;
	filter = utf8_to_wstring(apply_null_character(get_string("SAVEIMAGE_FILTER")));
	title  = get_wstring("SAVEIMAGE_TITLE");


	ImageType = GetFileNameW(NULL, szFileName, filter.c_str(), title.c_str(), true);


	if (ImageType <= 0)  {
		szFileName[0] = '\0';
		return false;
	}

	SaveImageWidth = w;
	SaveImageHeight = h;

	return  true;
}




bool
save_image_high_resolution(const mtk_camera camera_base)
{
	if (szFileName[0] == '\0')  return false;

	int w = SaveImageWidth;
	int h = SaveImageHeight;
	int wmax = get_pbuffer_width_max();
	int hmax = get_pbuffer_height_max();
	if (w > wmax)  w = wmax;
	if (h > hmax)  h = hmax;


	if ( !create_pbuffer(w, h) )  return false;
	draw_scene_all(calc_draw_struct(camera_base, float(h)/float(w), 0, w, 0, h, vector3f(0,0,0), true, 1.0), true);
	image  img(0,0,3);
	gl_read_pixels(img, 0,0, w, h);
	destroy_pbuffer();


	switch (ImageType) {
		case 1:	img.saveJPEG(szFileName);	break;
		case 2:	img.savePNG(szFileName);	break;
		case 3:	img.saveBMP(szFileName);	break;
		default:							break;
	}

	return  true;
}