/**************************************************************************************************

Copyright (c) 2014-2015   Tsunehiko Kato  and  4D2U Project, National Astronomical Observatory of Japan

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
#ifndef __MTK_MOVIE_H__
#define __MTK_MOVIE_H__
#include  <dshow.h>
#include  <vector>
using namespace std;
#include  "mtk_video.h"
#include  "../common/win_file.h"
#include  <string>



enum {
	STATE_NO_MOVIE,
	STATE_PLAYING,
	STATE_PAUSED,
	STATE_STOPPED
};




class mtk_movie
{
public:
	mtk_movie();
	~mtk_movie();

	void	CleanUpGraph();

	HRESULT	Open(const char *fn, HWND hwnd, int x0, int y0, int w, int h, UINT u_WM_NOTIFY, int iRenderer);
	void	Play();
	void	Pause();
	void	Home();
	void	End();
	void	Close();

	LONGLONG	GetPosition() const;
	LONGLONG	GetEndPosition() const;
	void		SetPosition(LONGLONG pos);

	void		GetWindowPosition(int &x, int &y, int &w, int &h) const;

	int	GetState() const;

	HRESULT	OnPaint(HWND hwnd, HDC hdc);
	HRESULT	OnDisplayChange();
	bool	GetEvent(long& evCode, long& prm1, long& prm2);


private:
	IGraphBuilder *m_pGB;
	IMediaControl *m_pMC;
	IMediaEventEx *m_pEv;
	IMediaSeeking *m_pMS;

	mtk_video  *m_pVideo;
	int	m_state;
	LONGLONG m_end_pos;
	int m_w, m_h;	// video size
	int m_x, m_y;	// video position
};





//=======================
//  ���f�B�A�t�@�C���̊Ǘ�
//=======================

//---------------------
// ���f�B�A�t�@�C���̎��
//---------------------
enum {
	MEDIATYPE_IMAGE,
	MEDIATYPE_POPUP_IMAGE,
	MEDIATYPE_MOVIE,
	MEDIATYPE_AUDIO
};



//-----------------------
//  ���f�B�A�t�@�C���̏��
//-----------------------
struct  media_file_info
{
	char			type;					// ���f�B�A�̎��
	std::string		menu_title;				// ���j���[�p�^�C�g���̃L�[
	std::string		disp_title;				// �\���p�^�C�g���̃L�[
	std::string		menu_title_key;			// ���j���[�p�^�C�g���̃L�[
	std::string		disp_title_key;			// �\���p�^�C�g���̃L�[

	std::string file_path;
	std::vector<std::string>	all_file_path;	// ���ׂẴm�[�h�̃��f�B�A�t�@�C���̃p�X�i����pPC�ł̂ݎg�p�j
};




//----------------------------
//  ���f�B�A�t�@�C���������N���X
//----------------------------
class  MediaFiles : public vector<media_file_info>
{
	std::string list_prefix;

public:
	MediaFiles(const std::string& _list_prefix) : list_prefix(_list_prefix) {}
	bool	open_info_file(const directory& dir, const char* fn, const std::string& lang_id);
	void	add_media_file(const media_file_info& mfi);
};




#endif
