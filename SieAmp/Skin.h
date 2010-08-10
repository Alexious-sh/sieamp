#ifndef _SKIN_H_
#define _SKIN_H_

extern char *percent_t;
extern char *percent_s;
extern char *percent_d;
extern char *percent_ts;
extern char *percent_td;

#define PIC_Main          "skins\\img\\main.bmp"
#define PIC_Cbuttons      "skins\\img\\cbuttons.bmp"
#define PIC_Volume        "skins\\img\\volume.bmp"
#define PIC_PleditPic     "skins\\img\\pledit.bmp"
#define PIC_MonoSter      "skins\\img\\monoster.bmp"
#define PIC_Playpaus      "skins\\img\\playpaus.bmp"
#define PIC_Posbar        "skins\\img\\posbar.bmp"
#define PIC_Shufrep       "skins\\img\\shufrep.bmp"
#define PIC_Titlebar      "skins\\img\\titlebar.bmp"
#define PIC_Balance       "skins\\img\\balance.bmp"
#define PIC_Numbers       "skins\\img\\numbers.bmp"
#define PIC_NumsEx        "skins\\img\\nums_ex.bmp"
  
#define CFG_Pledit        "skins\\img\\pledit.txt"
#define CFG_Viscolor      "skins\\img\\viscolor.txt"

typedef struct // Структура с координатами скина(после ресайза)
{
  int plItemH;
  int plItemsNum;
  RECT plTrack;
  RECT plTime;
  RECT plFrame;
  RECT vol;
  RECT progress;
  RECT scroll;
  RECT curtrack;
  RECT time;
  RECT kbps;
  RECT kHz;
  short monoster[2];
  short playbutt[2];
  short prevbutt[2];
  short pausebutt[2];
  short stopbutt[2];
  short nextbutt[2];
  short shuffle[2];
  short repeat[2];
}SKIN_COORDS;
extern SKIN_COORDS SKIN;

//extern char* skinbuf;
extern IMGHDR* BG;
extern IMGHDR* DIGITS;
extern IMGHDR* STATE;

extern char NormalCOL[4];
extern char CurrentCOL[4];
extern char BGCOL[4];
extern char SelectBG[4];

int isSkinCached();
int isSkinExist();
int initCache();
int prepareSkin();
void getParams(WSHDR* ws);

#endif /* _SKIN_H_ */
