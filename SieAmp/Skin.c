#include "../inc/swilib.h"
#include "Skin.h"
#include "file_works.h"
#include "medialibs\ImageUtils.h"
#include "medialibs\m3uparse.h"
//char* skinbuf;
IMGHDR* BG;
IMGHDR* DIGITS;
IMGHDR* STATE;
char NormalCOL[4] = {0xFF, 0xFF, 0xFF, 0xFF};
char CurrentCOL[4] = {0x00, 0x00, 0x00, 0x64};
char BGCOL[4] = {0x00, 0x00, 0x00, 0x64};
char SelectBG[4] = {0x00, 0x00, 0xFF, 0x64};

SKIN_COORDS SKIN =
{
  16,
  20,
  {12,120,222,120},
  {190,120,222,120},
  {10,118,224,286},
  {93,51,150,60},
  {9,61,232,72},
  {227,115,234,290},
  {97,18,230,32},
  {47,21,87,38},
  {95,36,110,43},
  {134,36,147,43},
  {183,35},
  {32,75},
  {12,75},
  {52,75},
  {72,75},
  {92,75},
  {142,75},
  {182,75}
};

int isSkinExist()
{
  int ret = 1;
  if(!sieamp_fexists(PIC_Main)) goto err;
  if(!sieamp_fexists(PIC_Cbuttons)) goto err;
  if(!sieamp_fexists(PIC_Volume)) goto err;
  if(!sieamp_fexists(PIC_PleditPic)) goto err;
  if(!sieamp_fexists(PIC_MonoSter)) goto err;
  if(!sieamp_fexists(PIC_Playpaus)) goto err;
  if(!sieamp_fexists(PIC_Posbar)) goto err;
  if(!sieamp_fexists(PIC_Shufrep)) goto err;
  if(!sieamp_fexists(PIC_Titlebar)) goto err;
  //if(!sieamp_fexists(PIC_Balance)) goto err;
  if(!sieamp_fexists(PIC_Numbers) || !sieamp_fexists(PIC_NumsEx)) goto err;
  if(!sieamp_fexists(CFG_Pledit)) goto err;
  if(!sieamp_fexists(CFG_Viscolor)) goto err;
  return(ret);
err:
  return(0);
}

int isSkinCached()
{
  if(!sieamp_fexists("skins\\cache\\BG.smp") ||
     !sieamp_fexists("skins\\cache\\DIGITS.smp") ||
     !sieamp_fexists("skins\\cache\\STATE.smp") ||
     !sieamp_fexists("skins\\cache\\pledit.txt")) return 0;
  return 1;
}

void prepareBG()
{
  IMGHDR* tmpimg;
  char* path;
  RECT clip;
  WSHDR* wst = AllocWS(256);
  path = getSymbolicPath(PIC_Main);
  wsprintf(wst, percent_s, path);
  mfree(path);
  StoreXYXYtoRECT (&clip,0,0,0,0);
  tmpimg = CreateImgHdrByAnyFile (wst, 240, 232, 0, clip);
  BG=malloc(sizeof(IMGHDR));
  BG->w = 240;
  BG->h = 320;
  BG->bpnum = tmpimg->bpnum;
  BG->bitmap = malloc(240*320*2+1);
  insertIMG(BG, tmpimg, 0, 0);
  FreeIMG(tmpimg);
  
  path = getSymbolicPath(PIC_Titlebar);
  wsprintf(wst, percent_s, path);
  mfree(path);
  StoreXYXYtoRECT (&clip,27,0,275,15);
  tmpimg = CreateImgHdrByAnyFile (wst, 240, 32, 0, clip);
  insertIMG(BG, tmpimg, 0, 0);
  FreeIMG(tmpimg);
  
  path = getSymbolicPath(PIC_Cbuttons);
  wsprintf(wst, percent_s, path);
  mfree(path);
  StoreXYXYtoRECT (&clip,0,0,114,18);
  tmpimg = CreateImgHdrByAnyFile (wst, 100, 32, 0, clip);
  insertIMG(BG, tmpimg, 13, 77);
  FreeIMG(tmpimg);
  
  path = getSymbolicPath(PIC_PleditPic);
  wsprintf(wst, percent_s, path);
  StoreXYXYtoRECT (&clip,127,0,25,20);
  tmpimg = CreateImgHdrByAnyFile (wst, 22, 18, 0, clip);
  insertIMG(BG, tmpimg, 22, 101);
  insertIMG(BG, tmpimg, 44, 101);
  insertIMG(BG, tmpimg, 66, 101);
  insertIMG(BG, tmpimg, 160, 101);
  insertIMG(BG, tmpimg, 182, 101);
  insertIMG(BG, tmpimg, 204, 101);
  FreeIMG(tmpimg);
  
  StoreXYXYtoRECT (&clip,0,0,25,20);
  tmpimg = CreateImgHdrByAnyFile (wst, 22, 19, 0, clip);
  insertIMG(BG, tmpimg, 0, 101);
  FreeIMG(tmpimg);
  
  StoreXYXYtoRECT (&clip,153,0,25,20);
  tmpimg = CreateImgHdrByAnyFile (wst, 22, 19, 0, clip);
  insertIMG(BG, tmpimg, 219, 101);
  FreeIMG(tmpimg);
  
  StoreXYXYtoRECT (&clip,26,0,100,20);
  tmpimg = CreateImgHdrByAnyFile (wst, 88, 19, 0, clip);
  insertIMG(BG, tmpimg, 76, 101);
  FreeIMG(tmpimg);
  
  StoreXYXYtoRECT (&clip,0,42,25,29);
  tmpimg = CreateImgHdrByAnyFile (wst, 22, 29, 0, clip);
  insertIMG(BG, tmpimg, 0, 119);
  insertIMG(BG, tmpimg, 0, 143);
  insertIMG(BG, tmpimg, 0, 168);
  insertIMG(BG, tmpimg, 0, 118);
  insertIMG(BG, tmpimg, 0, 193);
  insertIMG(BG, tmpimg, 0, 218);
  insertIMG(BG, tmpimg, 0, 243);
  insertIMG(BG, tmpimg, 0, 268);
  FreeIMG(tmpimg);
  
  StoreXYXYtoRECT (&clip,26,42,25,29);
  tmpimg = CreateImgHdrByAnyFile (wst, 22, 29, 0, clip);
  insertIMG(BG, tmpimg, 219, 119);
  insertIMG(BG, tmpimg, 219, 143);
  insertIMG(BG, tmpimg, 219, 168);
  insertIMG(BG, tmpimg, 219, 118);
  insertIMG(BG, tmpimg, 219, 193);
  insertIMG(BG, tmpimg, 219, 218);
  insertIMG(BG, tmpimg, 219, 243);
  insertIMG(BG, tmpimg, 219, 268);
  FreeIMG(tmpimg);
  
  StoreXYXYtoRECT (&clip,0,72,275,38);
  tmpimg = CreateImgHdrByAnyFile (wst, 240, 36, 0, clip);
  insertIMG(BG, tmpimg, 0, 287);
  FreeIMG(tmpimg);
  
  mfree(path);
  
  FreeWS(wst);
}

void saveIMGHDR(char* filename, IMGHDR* img)
{
  int f = -1;
  unsigned int err;
  unlink(filename, &err);
  f = fopen(filename, A_ReadWrite+A_BIN+A_Create+A_Truncate,P_READ+P_WRITE, &err);
  if(f == -1) return;
  fwrite(f,img, sizeof(IMGHDR), & err );
  fwrite(f,img->bitmap, img->w*img->h*2, & err );
  fclose(f,&err);
}

IMGHDR* loadIMGHDR(char* filename)
{
  int f;
  unsigned int err;
  
  int len = get_file_size(filename);
  if (!len) return 0;
  char* skinbuf = malloc(len+1);
  f = fopen(filename,A_ReadOnly+A_BIN,P_READ,&err);
  fread(f,skinbuf,len,&err);
  fclose(f,&err);
  
  IMGHDR* img=malloc(sizeof(IMGHDR));
  memcpy(img, skinbuf, sizeof(IMGHDR));
  img->bitmap = malloc(img->w*img->h*2+1);
  memcpy(img->bitmap, skinbuf+sizeof(IMGHDR), img->w*img->h*2);
  img->bpnum = 8;
  mfree(skinbuf);
  return img;
}

int saveCache()
{
  unsigned int err;
  char* cfile = getSymbolicPath("skins\\cache\\BG.smp");
  saveIMGHDR(cfile, BG);
  mfree(cfile);
  
  cfile = getSymbolicPath("skins\\cache\\DIGITS.smp");
  saveIMGHDR(cfile, DIGITS);
  mfree(cfile);
  
  cfile = getSymbolicPath("skins\\cache\\STATE.smp");
  saveIMGHDR(cfile, STATE);
  mfree(cfile);
  
  cfile = getSymbolicPath("skins\\cache\\pledit.txt");
  char* cffile = getSymbolicPath(CFG_Pledit);
  fmove(cffile, cfile, &err);
  mfree(cfile);
  mfree(cffile);
  return 1;
}

int prepareSkin()
{
  prepareBG();
  
  RECT clip;
  char* numbers;
  WSHDR* ws_nums = AllocWS(256);
  
  if(sieamp_fexists(PIC_Numbers)) numbers = getSymbolicPath(PIC_Numbers);
  else numbers = getSymbolicPath(PIC_NumsEx);
  wsprintf(ws_nums, percent_s, numbers);
  StoreXYXYtoRECT (&clip,0,0,0,0);
  DIGITS = CreateImgHdrByAnyFile (ws_nums, 0, 0, 0, clip);
  mfree(numbers);
  
  numbers = getSymbolicPath(PIC_Playpaus);
  CutWSTR(ws_nums, 0);
  wsprintf(ws_nums, percent_s, numbers);
  //StoreXYXYtoRECT (&clip,0,0,0,0);
  STATE = CreateImgHdrByAnyFile (ws_nums, 0, 0, 0, clip);
  mfree(numbers);
  
  FreeWS(ws_nums);
  
  extern int col;
  char* cfgpath = getSymbolicPath(CFG_Pledit);
  col = parsePLcfg(cfgpath);
  mfree(cfgpath);
  
  saveCache();
  return 1;
}

void getParams(WSHDR* ws)
{
  SKIN.plItemH = GetFontYSIZE(FONT_SMALL)+2;
  SKIN.plItemsNum = (SKIN.plFrame.y2 - SKIN.plFrame.y)/SKIN.plItemH;
  wsprintf(ws, "%s", "99:99");
  SKIN.plTime.x = SKIN.plTime.x2 - Get_WS_width(ws, FONT_SMALL);
  SKIN.plTrack.y2 += SKIN.plItemH;
  SKIN.plTime.y2 += SKIN.plItemH;
  SKIN.plTrack.x2 = SKIN.plTime.x - 2;
}

int initCache()
{
  extern int col;
  char* cskin = getSymbolicPath("skins\\cache\\BG.smp");
  BG = loadIMGHDR(cskin);
  mfree(cskin);
  
  cskin = getSymbolicPath("skins\\cache\\DIGITS.smp");
  DIGITS = loadIMGHDR(cskin);
  mfree(cskin);
  
  cskin = getSymbolicPath("skins\\cache\\STATE.smp");
  STATE = loadIMGHDR(cskin);
  mfree(cskin);
  
  char* cfgpath = getSymbolicPath("skins\\cache\\pledit.txt");
  col = parsePLcfg(cfgpath);
  mfree(cfgpath);
  return 1;
}
