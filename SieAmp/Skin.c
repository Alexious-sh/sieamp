#include "../inc/swilib.h"
#include "Skin.h"
#include "file_works.h"
#include "ImageUtils.h"
IMGHDR* BG;
char NormalCOL[4] = {0xFF, 0xFF, 0xFF, 0xFF};
char CurrentCOL[4] = {0x00, 0x00, 0x00, 0x64};
char BGCOL[4] = {0x00, 0x00, 0x00, 0x64};
char SelectBG[4] = {0x00, 0x00, 0xFF, 0x64};

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
  if(!sieamp_fexists(PIC_Numbers))
  {
    ret = 2;
    if(!sieamp_fexists(PIC_NumsEx)) goto err;
  }
  if(!sieamp_fexists(CFG_Pledit)) goto err;
  if(!sieamp_fexists(CFG_Viscolor)) goto err;
  return(ret);
err:
  return(0);
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
  BG->bitmap = malloc(240*320*2);
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

int prepareSkin()
{
  prepareBG();
  extern int col;
  col = parsePLcfg();
  return 1;
}
