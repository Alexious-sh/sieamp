#include "../inc/swilib.h"
extern int Obs_MM_SetInput_ProxyURI(HObj hObj, int unk_zero, WSHDR *addr);
extern int Obs_SoundNotify_SendEventAUMA(int messageid, int payload, int offset, HObj* user_data);
//extern Obs_Sound_Set3dAudio
//extern int Obs_Sound_SetVolumeEx(HObj hobj,int sobj,int vol);

extern void fmdl_set_mute(int flag);
extern int fmdl_get_mute();
//extern int fmdl_set_freq(int wfreq, char* level, int freq); // freq format - 105300(105.3MHz), level - 0xADC3 ?
extern int fmdl_set_freq(int freq,int *level, int adr_cb);
extern int fmdl_get_current_PLL_freq();
extern void fmdl_af_jump(int freq);
extern void fmdl_set_local_band(int freq);
extern int fmdl_get_local_band();
extern int fmdl_auto_search(int start_freq, int *level, int adr_cb, int Direction);
extern void fmdl_auto_search_stop();
extern void fmdl_set_power(int flag);
extern int fmdl_get_stereo_status(int adr_cb);
extern int fmdl_set_snc(int flag);
extern int fmdl_get_current_level();
extern int fmdl_set_forced_mono(int flag);
extern int fmdl_get_forced_mono();
extern int fmdl_get_snc();
extern void Obs_Sound_SetLogSourceEx(HObj hobj, int src);
extern int Obs_Snd_SetPurpose (HObj hObj,int purpose);
extern int Obs_Sound_SetHandsFree (HObj hObj,char hf);
extern int Obs_Sound_Set3dAudio (HObj hObj, char aud);
extern int Obs_Sound_SetEqualizer(HObj hObj, int bass, int mid, int treble, int balance);
extern int Obs_Sound_SetPWM(HObj hObj, int fIsPWM);
extern int Obs_SetOutput_Uid(HObj hObj, int OutUID);
extern int Obs_Sound_SetPlayModePartial(HObj hObj, int mode);
extern int Obs_Sound_SetMute(HObj hObj, int mute);

//Recording
extern int Obs_Sound_SetFIsRecording(HObj hobj, int fIsRecording);
extern int Obs_Sound_SetAMRDTX(HObj hobj4, int dtx);
extern int Obs_Sound_SetNofChannels(HObj hobj, int num_channels);
extern int Obs_Sound_SetMaxFileSize(HObj hobj, int max_file_size);
extern int Obs_Sound_SetBitrate(HObj hobj, int bitRate);
extern int Obs_Sound_SetAMRMode(HObj hobj, int data_rate);
extern int Obs_Sound_SetAMRFormat(HObj hobj, int format);
extern int Obs_Sound_SetAMRFrmPrBuf(HObj hobj, int nof_frames);
extern int Obs_Sound_SetRecordingMode(HObj hobj, int RecordingMode);
extern int Obs_SetOutput_File (HObj hObj/*, int unk_zero*/, WSHDR *path);
extern int Obs_Sound_SetSampleRate(HObj hobj, int SampleRate);
extern int Obs_SetOutput_Push(HObj hobj, int hz);

#pragma swi_number=0x80F5
__swi __arm unsigned int Ram_LCD_Overlay_Layer(void);

#pragma swi_number=0x7A
__swi __arm void UpdateDisplayByLayerPtr(unsigned int);

#pragma swi_number=0x80F6
__swi __arm unsigned int* Ram_Layer_Base_Ptr(void);

#pragma swi_number=0x79
__swi __arm void DrawObject2Layer(unsigned int lcd_ptr, DRWOBJ* drwobj);

extern void TempLightOn();
