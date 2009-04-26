#include "../inc/swilib.h"
#include "fmlib.c"
#include "menu_module.h"
#include "Skin.h"
#include "ImageUtils.h"
#include "unpack.h"
#include "file_works.h"
#define COLOR(x) (char *)(&(x)) // Приведение цвета из char[4]
extern int ScrW,
           ScrH,
           CurItem;
const char* skin = "4:\\zbin\\sieamp\\skins\\default.wsz";
extern char *percent_t;
extern char *percent_s;
extern char *percent_d;
extern char *percent_ts;
extern char *percent_td;
extern int MAINGUI_ID;
extern int maincsm_id;
int paused = 0;
int loaded = 0, col = 0;
DRWOBJ drwobj;
unsigned int* lcd_ptr;
unsigned int ovr_layer_ptr;
int dir = 90;
GBSTMR MyTimer;

HObj gObj=NULL;
int MSG_Report = 0xB034;
int vol = 7;

typedef struct
{
  GUI gui;
  WSHDR *ws;
  WSHDR *ws1;
  int i1;
}MAIN_GUI;



void GUIOnRedraw(MAIN_GUI *data)
{
  DrawRoundedFrame(0,0,240,320,0,0,0,GetPaletteAdrByColorIndex(1),GetPaletteAdrByColorIndex(1));
  if(loaded) 
  {
    DrwImg(BG, 0, 0);
    if(col)DrawRoundedFrame(15,118,224,286,0,0,0,COLOR(BGCOL),COLOR(BGCOL));
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
  if(!isSkinExist()) 
  {
    char* imgpath = getSymbolicPath("skins\\img\\");
    UnPack(skin, imgpath);
    mfree(imgpath);
  }
  loaded = prepareSkin();
  REDRAW();
}

void GUIOnCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
  DisableIconBar(1);
  data->ws1=AllocWS(256);
  data->ws=AllocWS(32);
  data->gui.state=1;
  SUBPROC((void *)initSkin);
}

void GUIOnClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  DisableIconBar(0); 
  GBS_DelTimer(&MyTimer);
  data->gui.state=0;
  if(BG) FreeIMG(BG);
  extern void kill_data(void *p, void (*func_p)(void *));
  kill_data(data,mfree_adr);
}

void GUIOnFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
  DisableIconBar(1); 
  data->gui.state=2;
}

void GUIOnUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  DisableIconBar(0); 
  if (data->gui.state!=2) return;
  data->gui.state=1;
}

int GUIOnKey(MAIN_GUI *data, GUI_MSG *msg)
{ 
  if (msg->gbsmsg->msg==KEY_DOWN)// Если клавиша нажата или зажата
  {
    if (msg->gbsmsg->submess==RED_BUTTON || msg->gbsmsg->submess==RIGHT_SOFT)
       return (1);
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

void Light()
{
  SetIllumination(0, 1, 80, 15);
  GBS_StartTimerProc(&MyTimer,100,Light);
}

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
