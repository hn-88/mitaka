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
#ifndef __MTK_COMMAND_H__
#define __MTK_COMMAND_H__
#include  "../common/math/vector3.h"
#include  <string>


void	OnCommandDraw();
void	OnUpdateViewCtrl();
void	OnUpdateViewCtrl2();



//---------------
//  Display Mode
//---------------
void	OnDispMode(char ty);


//----------
//  TARGET
//----------
void	OnTargetMoveControler();
void	OnTargetByKey(const char* target_key);


//--------
//  VIEW
//--------


//-- DispSwitch
void	OnDispSwitch(char idx, char sw);
void	OnDispSwitchSetBit(char idx, char mask, bool bFlg);
void	OnDispSwitchToggle(char idx, char mask);

//-- Planet
void	OnEclipse(char sw);
void	OnEclipseSet(char mask, bool bFlg);
void	OnPlanetAtmosphere(char val);
void	OnZoomMode(char val);
void	OnSetZoom(double val);
void	OnTopoZoomMode(char val);
void	OnSurfacePointer(bool bFlg);
void	OnLandingMode(char val);	
void	OnOortCloud(bool bFlg);

//-- Star
void	OnStar(char sw);
void	OnStarSet(char mask, bool bFlg);
void	OnStarMark(char idx);
void	OnSetLRef(float val);
void	OnLRef(float dL);

//-- Constellation
void	OnConstellation(char sw);
void	OnCnstSet(char mask, bool bFlg);

//-- Extragalactic Object
void	OnGalaxy(char sw);
void	OnGalaxySet(char mask, bool bFlg);
void	OnGalCenterBG(bool bFlg);

void	OnMarkVirgoCluster(bool bFlg);
void	OnCMB(bool bFlg);
void	OnNGC(bool bFlg);

//-- Spacecraft
void	OnSpacecraft(bool bFlg);

//-- Spacecraft Path
void	OnSpacecraftPath(char idx, bool bFlg);

//-- Lighting
void	OnSideLight(bool bFlg);


//-- Milkyway
void	OnMilkyWay(char type_MilkyWay);

//-- Guide Line
void	OnCelestialGrid(bool bFlg);
void	OnEcliptic(bool bFlg);
void	OnGalacticGrid(bool bFlg);
void	OnHorizonCoord(bool bFlg);
void	OnDistScale(char sw);

//-- Object Info
void	OnObjectInfo(bool bFlg);

//-- View
void	OnViewInfo(bool bFlg);
void	OnResetViewangle();

//-- TIME
void	OnDate(bool bFlg);
void	OnTimeStep(char type_timestep);
void	OnPresentTime();
void	OnRealTimeMode(bool bFlg);

//-- Scale
void	OnPresetScale(char ty);

//-- Scale line
void	OnDistScaleSet(char mask, bool bFlg);

//-- FontSize
void	OnFontSize(const char idx);

//-- Language
void	OnChangeLanguage(char idx);

//-- Title
void	OnTitle(bool bFlg);

//-- About
void	OnCheckAbout(bool bFlg);
void	OnAbout();

//-- Debug
void	OnDebug(bool bFlg);

//-- Single Eye
void	OnSingleEye(bool bFlg);

//-- Path info
void	OnPathInfo(bool bFlg);

//-- Full Screen Mode
void	OnFullScreen(bool bFlg);

//-- Speed
void	OnSetSpeedFactor(float val);





//--------
//  Time
//--------
void	OnInitTime();
void	OnChangeTime(const double& JD);
void	OnTimeSetting();


//----------
//  Target
//----------
void	OnChangeTargetControler(int tgt_ID);
void	OnPlanetariumTarget(int tgt_ID);



//---------------
//  Planetarium
//---------------
void	OnPlanetariumAngle(float val);


//-----------
//  Landing
//-----------
void	OnLanding();
void	OnLandingInit();
void	OnSetLandingLon(float lon);
void	OnSetLandingLat(float lat);
void	OnTakeoff();



//------------
//  3D chart
//------------
void	OnOpen3DChart(char val);
void	OnClose3DChart();



//---------
//  Movie
//---------
void	OnMovieOpenControler(int idx);
void	OnMovieOpen(int idx);


//---------
//  Image
//---------
void	OnImageOpenControler(int idx);
void	OnImageOpen(int idx);

//---------
//  Audio
//---------
void	OnAudioOpenControler(int idx);
void	OnAudioOpen(int idx);



//-----------
//  Program
//-----------
void	OnProgram(char program_no);




//----------
//  Preset
//----------
void	OnPreset(int idx);



//------------
//  Subtitle
//------------
void	OnSubtitles(bool bFlg);



//---------
//  Popup
//---------
void	OnPopup(int popup_idx);


//--------
//  Test
//--------
void	OnTest(bool bFlg);



/*
//--------
//  Path
//--------
void	OnPlayFrame(bool bFlg);
void	OnSavePath();
void	OnLoadPath();
void	OnClearPath();
*/




//-------------
//  Subtitles
//-------------
void	OnChangeSubtitlesIndex(int idx);




//---------
//  Other
//---------
void	OnSaveImage();
void	OnSaveImageHighRes();
//void	OnSaveCubicImage();


//--------
//  Exit
//--------
void	OnExitAll();



#endif
