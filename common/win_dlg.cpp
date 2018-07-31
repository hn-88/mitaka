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
#include  <stdio.h>
#include  "win_dlg.h"
#include  "./text/unicode_string.h"


//----------------------------
//  GetFileName
//----------------------------
//  return value:
//   -1 ... cancel or error
//   otherwise ... filter no
//----------------------------
int
GetFileName(HWND hWnd, char* szFileName, const char* szFilter, const char* szTitle, bool bSave)
{
	OPENFILENAME	ofn;
	char ext[4];

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize	= sizeof(OPENFILENAME);
	ofn.hwndOwner	= hWnd;
	ofn.Flags		= OFN_EXPLORER | OFN_HIDEREADONLY;
	ofn.lpstrFilter	= szFilter;
	ofn.lpstrFile	= szFileName;
	ofn.lpstrTitle	= szTitle;
	ofn.nMaxFile	= 256;
	ext[0] = '\0';
	ofn.lpstrDefExt = ext;	// �������邱�ƂŁA�g���q���t�B���^���玩���I�ɕ₤
	
	BOOL res;
	if (bSave) {
		ofn.Flags |= OFN_OVERWRITEPROMPT;			//  check overwrite
		res = GetSaveFileName(&ofn);
	} else {
		ofn.Flags |= OFN_FILEMUSTEXIST;				//  the file must exist
		res = GetOpenFileName(&ofn);
	}
	if (res != TRUE)  return -1;
	return  ofn.nFilterIndex;	// return current filter index
}


int
GetFileNameW(HWND hWnd, wchar_t* szFileName, const wchar_t* szFilter, const wchar_t* szTitle, bool bSave)
{
	OPENFILENAMEW	ofn;
	wchar_t ext[10];

	ZeroMemory(&ofn, sizeof(OPENFILENAMEW));
	ofn.lStructSize = sizeof(OPENFILENAMEW);
	ofn.hwndOwner = hWnd;
	ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
	ofn.lpstrFilter = szFilter;
	ofn.lpstrFile = (LPWSTR)szFileName;
	ofn.lpstrTitle = szTitle;
	ofn.nMaxFile = 256;
	ext[0] = L'\0';
	ofn.lpstrDefExt = ext;	// �������邱�ƂŁA�g���q���t�B���^���玩���I�ɕ₤

	BOOL res;
	if (bSave) {
		ofn.Flags |= OFN_OVERWRITEPROMPT;			//  check overwrite
		res = GetSaveFileNameW(&ofn);
	}
	else {
		ofn.Flags |= OFN_FILEMUSTEXIST;				//  the file must exist
		res = GetOpenFileNameW(&ofn);
	}
	if (res != TRUE)  return -1;
	return  ofn.nFilterIndex;	// return current filter index
}



//========================
//  formatted MessageBox
//========================
int
MessageBoxf(HWND hWnd, LPCTSTR szCaption, UINT uType, LPCSTR szFormat, ...)
{
	char	szBuffer[256];
	va_list	pArgs;
	va_start( pArgs, szFormat );
		vsprintf( szBuffer, szFormat, pArgs );
	va_end( pArgs );
	return  MessageBox(hWnd, szBuffer, szCaption, uType);
}

int
MessageBoxWf(HWND hWnd, LPCTSTR szCaption, UINT uType, LPCSTR szFormat, ...)
{
	char	szBuffer[256];
	va_list	pArgs;
	va_start(pArgs, szFormat);
		vsprintf(szBuffer, szFormat, pArgs);
	va_end(pArgs);
	return  MessageBoxW(hWnd, utf8_to_wstring(szBuffer).c_str(), utf8_to_wstring(szCaption).c_str(), uType);
}


//-------------------------------------------
//  TextOutf: output a formatted text on DC
//-------------------------------------------
int
TextOutf(HDC hDC, int x, int y, const char* szFormat, ...)
{
	int				length;
	static char		szBuffer[1024];
	va_list			pArgs;
	va_start( pArgs, szFormat );
		length = vsprintf( szBuffer, szFormat, pArgs );
	va_end( pArgs );
	if ( !TextOut(hDC, x,y, szBuffer, length) )
		return 0;

	return length;
}
