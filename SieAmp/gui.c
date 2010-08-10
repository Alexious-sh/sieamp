#include "../inc/swilib.h"
#include "fmlib.c"
#include "menu_module.h"
#include "Skin.h"
#include "medialibs\ImageUtils.h"
#include "medialibs\m3uparse.h"
#include "medialibs\id3lib.h"
#include "medialibs\audio.h"
#include "unpack.h"
#include "file_works.h"
#include "font.h"
#include "random.h"
#define COLOR(x) (char *)(&(x)) // Приведение цвета из char[4]
#define TMR_SECOND(A) (1300*A/6)
extern int ScrW,
           ScrH,
           CurItem;
extern char *percent_t;
extern char *percent_s;
extern char *percent_d;
extern char *percent_ts;
extern char *percent_td;
extern int MAINGUI_ID;
extern int maincsm_id;
int is_focus = 1;
int curtime = 0;
int paused = 0;
int state = 0, col = 0;
int plitems = 0;
int plSelect = 0;
int plFirst = 0;
char currFreq[4];
M3U* nowplayed = NULL;
DRWOBJ drwobj;
GBSTMR MyTimer;
GBSTMR SecondTimer;

int vol = 7;

typedef struct
{
  GUI gui;
  WSHDR *ws;
  WSHDR *ws1;
  int i1;
}MAIN_GUI;

/*void DrwStr(WSHDR* ws, int x, int y, int w, int h, int font, int flag, char* pen, char* brush)
{
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc, x, y, w, h);
  SetPropTo_Obj1(&drwobj, &rc, 0, ws, font, flag);
  SetColor(&drwobj, pen, brush);
  DrawObject(&drwobj);
}*/

void RedrawPlaylist(MAIN_GUI *data)
{
  RECT *tmptrk = malloc(sizeof(RECT));
  memcpy(tmptrk, &SKIN.plTrack, sizeof(RECT));
  int curtrk = plFirst;
  M3U* curitem = GetM3UByItem(plFirst);
  if(col)DrawRoundedFrame(SKIN.plFrame.x,SKIN.plFrame.y,SKIN.plFrame.x2,SKIN.plFrame.y2,0,0,0,COLOR(BGCOL),COLOR(BGCOL));
  while(curtrk < SKIN.plItemsNum + plFirst && curtrk < plitems)
  {
    if(plSelect == curtrk) 
      DrawRoundedFrame(tmptrk->x-2, tmptrk->y, SKIN.plTime.x2+2, tmptrk->y2,0,0,0,COLOR(SelectBG),COLOR(SelectBG));
    char color[4];
    if(isPlaying()) memcpy(color, (nowplayed==curitem)?CurrentCOL:NormalCOL, 4);
    else memcpy(color, NormalCOL, 4);
    DrawString(curitem->track, tmptrk->x, tmptrk->y, tmptrk->x2, tmptrk->y2, FONT_SMALL, TEXT_NOFORMAT, 
               COLOR(color), GetPaletteAdrByColorIndex( 23 ) );
    wsprintf(data->ws1, "%d:%02d", curitem->time.min, curitem->time.sec);
    DrawString(data->ws1, SKIN.plTime.x, tmptrk->y+1, SKIN.plTime.x2, tmptrk->y2-1, FONT_SMALL, TEXT_ALIGNRIGHT, 
               COLOR(color), GetPaletteAdrByColorIndex( 23 ) );
    tmptrk->y += SKIN.plItemH;
    tmptrk->y2 += SKIN.plItemH;
    curtrk++;
    curitem = curitem->next;
  }
  mfree(tmptrk);  
}

int getFreq()
{
  switch (*RamFreq())
    {
      case 0x4: return 22;
      case 0x5: return 32;
      case 0x7: return 44;
      case 0x8: return 48;
    }
  return 0;
}

void DrawInfo()
{
  char* tmp = malloc(128);
  sprintf(tmp, "%d", 320);
  DrawStr(tmp ,97,38,NormalCOL);
  DrawStr(currFreq ,136,38,NormalCOL);
  if(!nowplayed) sprintf(tmp, "%s", "SieAmp PreAlpha 0.000001 :)");
  else 
  {
    ws16_2str8(nowplayed->track, tmp, 29);
  }
  DrawStr(tmp ,97,23,NormalCOL);
  mfree(tmp);
}

void RedrawTime()
{
  int x = 43;
  int all = curtime/600;
  DrawDigit(DIGITS, all, x, 22, 10);
  x+=10;
  all = curtime%600;
  DrawDigit(DIGITS, all/60, x, 22, 10);
  x+=14;
  all = curtime%60;
  DrawDigit(DIGITS, all/10, x, 22, 10);
  x+=10;
  all = curtime%10;
  DrawDigit(DIGITS, all, x, 22, 10);
}

void GUIOnRedraw(MAIN_GUI *data)
{
  DrawRoundedFrame(0,0,240,320,0,0,0,GetPaletteAdrByColorIndex(1),GetPaletteAdrByColorIndex(1));
  if(state) 
  {
    DrwImg(BG, 0, 0);
    if(plitems) RedrawPlaylist(data);
    DrawInfo();
    RedrawTime();
    
    int crop = 0;
    if(!isPlaying()) crop = 18;
    DrwCropImg(STATE, 22, 24, crop, 0, 9, 9);
  }
  else
  {
    wsprintf(data->ws1, percent_t, "Инициализация скина");
    DrawString(data->ws1, 0, 120, 240, 320, FONT_MEDIUM, TEXT_ALIGNMIDDLE, 
               GetPaletteAdrByColorIndex( 0 ), GetPaletteAdrByColorIndex( 23 ) );
  }
}

int isVolume()
{
  return(1);
}

void initSkin()
{
  if(isSkinCached()) state = initCache();
  else
  {
    if(!isSkinExist()) 
    {
      char* imgpath = getSymbolicPath("skins\\img\\");
      char* skin = getSymbolicPath("skins\\default.wsz");
      UnPack(skin, imgpath);
      mfree(imgpath);
      mfree(skin);
    }
    state = prepareSkin();
  }
}

void initPL() // Для SUBPROC
{
  char* filename = getSymbolicPath("playlists\\default.m3u");
  plitems = ParseM3UFile(filename);
  mfree(filename);
  state = 2;
  REDRAW();
}

void init()
{
  initSkin();
  initPL();
  /*extern void init_random();
  Randomize();*/
}

void tmrRedraw()
{
  if(isPlaying() && is_focus)
  {
    curtime++;
    RedrawTime();
  }
  GBS_StartTimerProc(&SecondTimer,TMR_SECOND(1),tmrRedraw);
}

void GUIOnCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
  DisableIconBar(1);
  data->ws1=AllocWS(256);
  data->ws=AllocWS(32);
  data->gui.state=1;
  getParams(data->ws1);
#ifdef LOG
  init();
#else
  SUBPROC((void *)init);
#endif
  GBS_StartTimerProc(&SecondTimer,TMR_SECOND(1),tmrRedraw);
  //REDRAW();
}

void GUIOnClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  GBS_DelTimer(&MyTimer);
  GBS_DelTimer(&SecondTimer);
  data->gui.state=0;
  if(BG) FreeIMG(BG);
  if(DIGITS) FreeIMG(DIGITS);
  if(STATE) FreeIMG(STATE);
  if(plitems) FreeM3UList();
  Stop();
  DisableIconBar(0); 
  extern void kill_data(void *p, void (*func_p)(void *));
  kill_data(data,mfree_adr);
}

void GUIOnFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
  is_focus = 1;
  DisableIconBar(1); 
  data->gui.state=2;
}

void GUIOnUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  is_focus = 0;
  DisableIconBar(0); 
  if (data->gui.state!=2) return;
  data->gui.state=1;
}

void test()
{
  int duration = getDuration();
  nowplayed->time.min = duration/60000;
  nowplayed->time.sec = (duration%60000) / 1000;
  if( (duration%60000 - (nowplayed->time.sec * 1000)) >= 500 ) nowplayed->time.sec++;
  sprintf(currFreq, percent_d, getFreq());
  REDRAW();
  //char tt[32];
  //sprintf(tt, "Длительность: %d", duration);
  //ShowMSG(1,(int)tt);
}

void readInfo()
{
  WSHDR* ws = AllocWS(256);
  if(getMP3Label(nowplayed->path, ws)) wstrcpy(nowplayed->track, ws);
  FreeWS(ws);
  GBS_StartTimerProc(&MyTimer,100,test);
  //REDRAW();
}

int GUIOnKey(MAIN_GUI *data, GUI_MSG *msg)
{ 
  if(state == 2)
  {
    if(msg->gbsmsg->msg==KEY_DOWN || msg->gbsmsg->msg==LONG_PRESS)
    {
      switch(msg->gbsmsg->submess)
      {
        case DOWN_BUTTON:
         if(plSelect < plitems-1) 
         {
           plSelect++;
           if(plSelect > plFirst + SKIN.plItemsNum - 1) plFirst++;
         }
         else {plSelect = plFirst = 0;}
         RedrawPlaylist(data);
         break;
        case UP_BUTTON:
         if(plSelect > 0)
         {
           plSelect--;
           if(plSelect < plFirst) plFirst = plSelect;
         }
         else 
         {
           plSelect = plitems - 1;
           plFirst = (plitems - SKIN.plItemsNum) > 0 ? plitems - SKIN.plItemsNum : 0;
         }
         RedrawPlaylist(data);
         break;
        case VOL_UP_BUTTON:
         if(vol < 15) vol++;
         SetVolume(vol);
         break;
        case VOL_DOWN_BUTTON:
         if(vol > 0) vol--;
         SetVolume(vol);
         break; 
      }
    }
    if(msg->gbsmsg->msg==KEY_DOWN)
    {
      switch(msg->gbsmsg->submess)
      {
        case ENTER_BUTTON:
          Stop();
          nowplayed = GetM3UByItem(plSelect);
          curtime = 0;
          playSoundFile(nowplayed->path, vol);
          SUBPROC((void *)readInfo);
          //DrawInfo();
          REDRAW();
          break;
        case '1':
          //SetEq(1);
          break;
        case '5':
          if(isPlaying()) Pause();
          else Resume();
          break;
        case '3':
          //getHobj();
          break;
      }
    }
  }
  if (msg->gbsmsg->msg==KEY_DOWN)// Если клавиша нажата
  {
    if (msg->gbsmsg->submess==RED_BUTTON || msg->gbsmsg->submess==RIGHT_SOFT)
    {
      if(isPlaying())
      {
        Stop();
        curtime = 0;
        REDRAW();
      }
      else return (1);
    }
  }
 return (0);
}

void GUIOnDestroy(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  
}

int method8(void){return(0);}
int method9(void){return(0);}

const void * const IdleGuiMethods[11]={
  (void *)GUIOnRedraw,  //Redraw
  (void *)GUIOnCreate,  //Create
  (void *)GUIOnClose,	  //Close
  (void *)GUIOnFocus,	  //Focus
  (void *)GUIOnUnfocus, //Unfocus
  (void *)GUIOnKey,     //OnKey
  0,
  (void *)GUIOnDestroy, //Destroy
  (void *)method8,
  (void *)method9,
  0
};

const RECT Canvas={0,0,0,0};

int ShowGUI()
{
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  zeromem(main_gui,sizeof(MAIN_GUI));
  main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.flag30=2;
  main_gui->gui.methods=(void *)IdleGuiMethods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  patch_rect((RECT*)&Canvas,0,YDISP,ScrW-1,ScrH-1);
  return CreateGUI(main_gui);
}

M3U* getRandom()
{
  M3U* item = GetM3UByItem(randint(plitems-1));
  if(item->played)
  {
    int i = 0;
    while(i <= plitems)
    {
      item = GetM3UByItem(randint(plitems-1));
      if(!item->played) break;
      i++;
      if(i >= plitems)
      {
        Randomize();
        item = GetM3UByItem(randint(plitems-1));
      }
    }
  }
  return(item);
}

void PlayNext()
{
  Stop();
  curtime = 0;
  nowplayed = nowplayed->next;
  if(!nowplayed) nowplayed = GetM3UByItem(0);
  //nowplayed = getRandom();
  playSoundFile(nowplayed->path, vol);
  SUBPROC((void *)readInfo);
  LockSched();
  plSelect = nowplayed->index;
  M3U* item = nowplayed->prev;
  plFirst = (item)?item->index:nowplayed->index;
  UnlockSched();
  REDRAW();
}
