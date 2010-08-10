	PUBLIC	ELF_BEGIN
	RSEG	ELFBEGIN:DATA
ELF_BEGIN

	RSEG	CODE:CODE
	PUBLIC	kill_data
	CODE32
kill_data
	BX	R1
        PUBLIC  seqkill
seqkill
        BX      R3

;
defadr	MACRO	a,b
	PUBLIC	a
a	EQU	b
	ENDM

;        defadr  GBS_WaitForMsg,0xA00925C4
;        defadr  RegisterCepidForSocketWork,0xA09915E5
;        defadr  RequestLMANConnect,0xA09926F5
;	defadr	PrepareEditControl,0xA0A0B3F4+1
;	defadr	AllocEQueue,0xA0A0BFB6+1
;	defadr	ConstructEditControl,0xA0A0B704+1
;	defadr	AddEditControlToEditQend,0xA0A0C152+1
;	defadr	CreateInputTextDialog,0xA0A08D7C+1
;	defadr	SetSoftKey,0xA0A1205A+1
;	defadr	ExtractEditControl,0xA0A11FF0+1
;	defadr	StoreEditControl,0xA0A11FFC+1
;	defadr	unlink,0xA02510B8
        defadr  TempLightOn,0xA08CCB2+1
//------------------------------ FM-Чип (fmdl) ---------------------------------
        defadr	fmdl_set_mute,0xA05783F0
	defadr	fmdl_get_mute,0xA0578508
        defadr	fmdl_set_freq,0xA0578A3C
        defadr	fmdl_get_current_PLL_freq,0xA0579050
        defadr	fmdl_af_jump,0xA0578D4C
        defadr	fmdl_set_local_band,0xA0577CA0
        defadr	fmdl_get_local_band,0xA0577BA0
        defadr	fmdl_auto_search,0xA0578BD4
        defadr	fmdl_auto_search_stop,0xA0578824
        defadr	fmdl_set_power,0xA0577988
        defadr  fmdl_set_snc,0xA0577EB0
        defadr  fmdl_get_current_level,0xA05792FC
        defadr  fmdl_set_forced_mono,0xA05780B8
        defadr  fmdl_get_forced_mono,0xA0577FB4
        defadr  fmdl_get_stereo_status,0xA0579120
        defadr  fmdl_get_snc,0xA0577DBC
//------------------------------ OBS_EXT ---------------------------------------
        defadr  Obs_MM_SetInput_ProxyURI,0xA0D18744
        defadr  Obs_SoundNotify_SendEventAUMA,0xA0CC5C8C
        defadr  Obs_Sound_SetVolumeEx,0xA0CC371C
        defadr  Obs_Sound_Set3dAudio,0xA0CC4638
        defadr  Obs_Sound_SetEqualizer,0xA0CC45C8
        defadr  Obs_Sound_SetLogSourceEx,0xA0CC3964
        defadr  Obs_Snd_SetPurpose,0xA0CC407C
        defadr  Obs_Sound_SetHandsFree,0xA0CC3FB8
        defadr  Obs_Sound_SetPWM,0xA0CC3BE8
        defadr  Obs_SetOutput_Uid,0xA0CCFD44
        defadr  Obs_Sound_SetPlayModePartial,0xA0CC4740
        defadr  Obs_Sound_SetMute,0xA0CC3EB0
        defadr  Obs_Sound_SetVisualisationsParams,0xA0CC4E10
        defadr  Obs_Sound_SetShockProtection,0xA0CC3CD8
        defadr  Obs_Sound_SetFading,0xA0CC39E0
        
//------------------------------ Запись (OBS) ----------------------------------
        defadr  Obs_Sound_SetBitrate,0xA0CC4EA4
        defadr  Obs_Sound_SetAMRFrmPrBuf,0xA0CC4BEC
        defadr  Obs_Sound_SetAMRFormat,0xA0CC4A70
        defadr  Obs_Sound_SetAMRDTX,0xA0CC4994
        defadr  Obs_Sound_SetNofChannels,0xA0CC48AC
        defadr  Obs_Sound_SetAMRMode,0xA0CC47D4
        defadr  Obs_Sound_SetFIsRecording,0xA0CC450C
        defadr  Obs_Sound_SetMaxFileSize,0xA0CC4378
        defadr  Obs_Sound_SetRecordingMode,0xA0CC3B18
        defadr  Obs_SetOutput_File,0xA0CCD008
        defadr  Obs_Sound_SetSampleRate,0xA0CC4FD4
        defadr  Obs_SetOutput_Push,0xA0CCD0A8

	END