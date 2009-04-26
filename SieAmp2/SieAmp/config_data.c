#include "..\inc\cfg_items.h"
//#define EN
//Конфигурация
//**************************** Русский конфиг ******************************
#ifndef EN
__root const CFG_HDR cfghdr0={CFG_CBOX,"Сообщать о реконфигурации",0,2};
__root const int MSG_RCONFIG = 1;
__root const CFG_CBOX_ITEM cfgcbox1[2]={"Нет","Да"};

//---------------------------------------------------------------------------

__root const CFG_HDR cfghdr_m11={CFG_LEVEL,"Настройки клавиш",1,0};


__root const CFG_HDR cfghdr2={CFG_CBOX,"Регулировка громкости",0,3};
__root const int VOL_ENABLE = 2;
__root const CFG_CBOX_ITEM cfgcbox2[3]={"Только в эльфе","Клав.разблок.","Всегда"};

__root const CFG_HDR cfghdr_m10={CFG_LEVEL,"",0,0};

//---------------------------------------------------------------------------

//**************************** Ангельский конфиг ******************************
#else
__root const CFG_HDR cfghdr0={CFG_CBOX,"Enable reconfigure msg?",0,2};
__root const int MSG_RCONFIG = 1;
__root const CFG_CBOX_ITEM cfgcbox1[2]={"No","Yes"};

//---------------------------------------------------------------------------

__root const CFG_HDR cfghdr_m11={CFG_LEVEL,"Buttons config",1,0};


__root const CFG_HDR cfghdr2={CFG_CBOX,"Volume change",0,3};
__root const int VOL_ENABLE = 2;
__root const CFG_CBOX_ITEM cfgcbox2[3]={"in ELF only","Unlock","Everywhere"};

__root const CFG_HDR cfghdr_m10={CFG_LEVEL,"",0,0};

//---------------------------------------------------------------------------
#endif
