#include "../inc/swilib.h"
#include "ImageUtils.h"

void FreeIMG(IMGHDR* img)
{
  mfree(img->bitmap);
  mfree(img);
}

void insertIMG(IMGHDR* img, IMGHDR* src, short x, short y)
{
  int cury = 0;
  while(cury < src->h && y < img->h)
  {
    int pos = x*2 + y*img->w*2;
    int ps = cury*src->w*2;
    memcpy(img->bitmap+pos, src->bitmap+ps, src->w*2);
    cury++;
    y++;
  }
}

IMGHDR* CreateImgHdrByAnyFile (WSHDR * path,short width, short height, int rot, RECT clip){
  unsigned int err=0;
  int msz,uid;
  IMGHDR *tmpimg,*myimg=NULL;  
  WSHDR *ext;
  HObj  mypicObj;
  int scaling = 5;
  short pos;
  int len;
  if (!path) {ShowMSG(1,(int)"Zalupa");goto exit0;}
  len=wstrlen(path); 
  pos= wstrrchr(path,len,'.'); 
  if (!pos) {ShowMSG(1,(int)"Zalupa");goto exit0;}

  ext=AllocWS(len-pos+1);
  wstrcpybypos(ext,path,pos+1,len-pos);
  uid=GetExtUid_ws(ext);
  FreeWS(ext);
  mypicObj=Obs_CreateObject(uid,0x2d,0x02,0x80A8,1,1,&err);    
   if (err)  {ShowMSG(1,(int)"Zalupa");return 0;}
  err=Obs_SetInput_File(mypicObj,0,path);
    if (err)  goto exit1;  
  if (width!=0){ 
    unsigned short w=width;  
    unsigned short h=height;      
    err=Obs_SetOutputImageSize(mypicObj,w,h);
    if (err)  goto exit1;     
    if(clip.x2 > 0 && clip.y2 > 0)
    {
      err=Obs_Graphics_SetClipping(mypicObj, clip.x, clip.y, clip.x2, clip.y2);
      scaling = 21;
      if (err)  goto exit1;      
    }
    if (rot){  
      err= Obs_SetRotation(mypicObj,rot);
        if (err)  goto exit1;      
    }
    err=Obs_SetScaling(mypicObj,scaling);
      if (err)  goto exit1;          
  }
  err=Obs_Start(mypicObj);
    if (err)  goto exit1;            
  err=Obs_Output_GetPictstruct(mypicObj,&tmpimg);
    if (err)  goto exit1;             
  msz=CalcBitmapSize (tmpimg->w,tmpimg->h,tmpimg->bpnum);
  /*char d[12];
  sprintf(d,"%d",msz);
  ShowMSG(1,(int)d);*/
  myimg=malloc(sizeof(IMGHDR));
  myimg->w=tmpimg->w;
  myimg->h=tmpimg->h;
  myimg->bpnum=tmpimg->bpnum;
  myimg->bitmap=malloc(msz+1);
  memcpy(myimg->bitmap,tmpimg->bitmap,msz);
exit1:
  Obs_DestroyObject(mypicObj);
exit0:  
  return myimg;
}

void DrwImg(IMGHDR *img, int x, int y)
{
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
  SetPropTo_Obj5(&drwobj,&rc,0,img);
  DrawObject(&drwobj);
}

void DrwCropImg(IMGHDR *img, int x, int y, int deltx, int delty, int w, int h)
{
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc,x,y,w,h);
  SetProp2ImageOrCanvas(&drwobj,&rc,0,img,deltx,delty);
  DrawObject(&drwobj);
}

void DrawDigit(IMGHDR *img, int digit, int x, int y, int w)
{
  DrwCropImg(img, x, y, digit*9, 0, w, img->h);
}
