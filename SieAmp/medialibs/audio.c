#include "../inc/swilib.h"
#include "../inc/playsound.h"
#include "../fmlib.c"
#include "../inc.h"
HObj hobj = NULL;
int played = 0;
int duration = 0;
int MSG_Report = 0xB034;

extern void PlayNext();

KillObj(){
 if (!hobj) return;
  Obs_DestroyObject(hobj);
  hobj=NULL;
  played = 0;
  duration = 0;
}

int isPlaying()
{
  return played;
}

void Pause()
{
  Obs_Pause (hobj);
  played = 2;
}

void Stop()
{
  Obs_Stop (hobj);
  KillObj();
}

void Resume()
{
  Obs_Resume (hobj);
  played = 1;
}

void SetVolume(int volume)
{
  Obs_Sound_SetVolumeEx (hobj, volume, 1);
}

int obError(HObj hobj,int error){
  ShowMSG(1,(int)"Авотхуй!");
  KillObj();
  return 0;
};

int obPrep(HObj hobj,int error){
  Obs_Start(hobj);
  return 0;
};

int obDestroy(HObj hobj,int err)
{
  
 // if (flag_nextsong)CallSomeToPlayNextSong();
 // else ; //nothing when stop
  return 0;
}

int getDuration()
{
  GetPlayObjDuration((void*)hobj, &duration);
  return(duration);
}

int obInfo(HObj hobj,int error){
  return 0;
};

int obResumeCon(HObj hobj,int error){
  return 0;
};

int obParam (HObj hobj,int pl, int error){
    //  if (pl==2)obFrameUpd(hobj);
      if (pl==1)Obs_Resume(hobj);
  return 0;
};

int obSetPause(HObj hobj,int err)
{
  return 0;
}

int obSetStop(HObj hobj,int err)
{
  return 0;
}

int obNext(HObj hobj,int err)
{
  PlayNext();
  return 0;
}

int obPause (HObj hobj,int pl, int error)
{
  return 0;
};

OBSevent ObsEventsHandlers[]={
  OBS_EV_FrameUpdate,NULL,
  OBS_EV_Error,(void*)obError,
  OBS_EV_GetInfoCon,(void*)obInfo,
  OBS_EV_PauseCon,(void*)obPause,
  OBS_EV_ParamChanged,(void*)obParam,
  OBS_EV_ResumeCon,(void*)obResumeCon,
  OBS_EV_PauseInd,(void*)obSetPause,
  OBS_EV_StopInd,(void*)obSetStop,
  OBS_EV_LoopUpdate,(void*)obNext,
  OBS_EV_PrepareCon,(void*)obPrep,
  OBS_EV_ConvDestroyed,(void*)obDestroy,
  OBS_EV_EndList,NULL
};

void ParseMsg(GBS_MSG *msg)
{
  /*GBS_PSOUND_MSG *pmsg=(GBS_PSOUND_MSG *)msg;
  char errc[32];
  sprintf(errc, "Message: %d", pmsg->cmd);
  ShowMSG(1,(int)errc);*/
  Obs_TranslateMessageGBS(msg,ObsEventsHandlers);
}

void SetEq(int type)
{
  int err;
  switch(type)
  {
  case 0:
    err = Obs_Sound_SetEqualizer(hobj, 112, -100, -100, -84);
    break;
  case 1:
    err = Obs_Sound_SetEqualizer(hobj, 0, 0, 100, 45);
    break;
  case 2:
    err = Obs_Sound_SetEqualizer(hobj, 255, -100, -100, 50);
    break;
  }
  if(err) 
  {
    char tmp[128];
    sprintf(tmp, "эквалайзер сказал %d", err);
    ShowMSG(1,(int)tmp);
  }
}

void getHobj()
{
  char tmp[128];
  sprintf(tmp, "hObj = %04X", hobj);
  ShowMSG(1,(int)tmp);
}

void visCb(int one, int two, int three)
{
  char str[128];
  sprintf(str, "cb Value: %d, %d, %d", one, two, three);
  COUTs(str);
}

void setVis()
{
  VIS_FMT* fmt = malloc(sizeof(VIS_FMT));
  fmt->bass=100;
  fmt->treble=15;
  fmt->vis=100;
  int err = Obs_Sound_SetVisualisationsParams(hobj, 1, 1, (int)visCb, fmt);
  if(!err) COUTs("Visual set!");
  else COUTs("Visual set error!");
}

void playSoundFile(char* filename, int vol)
{
  played = 1;
  unsigned int err=0;
  KillObj();
  WSHDR *ext = AllocWS(256);
  char* extt = strrchr (filename,'.');
  extt++;
  wsprintf(ext, "%s", extt);
  int uid = GetExtUid_ws(ext);
  hobj=Obs_CreateObject(uid, 0x34, 2, MSG_Report, 1, 0, &err);
  if(err) 
  {
    ShowMSG(1,(int)"Ашипка CreateObject");
    return;
  }
  //wsprintf(ext, "%s", filename);
  utf8_2ws(ext,filename, strlen(filename));
  err = Obs_SetInput_File(hobj,0,ext);
  if(err) 
  {
    ShowMSG(1,(int)"Файл херовый, да");
    return;
  }
  FreeWS(ext);
  Obs_Sound_SetVolumeEx (hobj, vol, 1);
  Obs_Mam_SetPurpose(hobj,0x16);
  //SetEq(0);
  //setVis();
  err = Obs_Prepare(hobj);
  if(err) 
  {
    //ShowMSG(1,(int)"не Prapare`ваиццо");
    return;
  }
  //Obs_Start(hobj);
}
