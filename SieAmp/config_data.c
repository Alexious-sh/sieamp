#include "..\inc\cfg_items.h"
//#define EN
//������������
//**************************** ������� ������ ******************************
#ifndef EN
__root const CFG_HDR cfghdr0={CFG_CBOX,"�������� � ��������������",0,2};
__root const int MSG_RCONFIG = 1;
__root const CFG_CBOX_ITEM cfgcbox1[2]={"���","��"};

//---------------------------------------------------------------------------

__root const CFG_HDR cfghdr_m11={CFG_LEVEL,"��������� ������",1,0};


__root const CFG_HDR cfghdr2={CFG_CBOX,"����������� ���������",0,3};
__root const int VOL_ENABLE = 2;
__root const CFG_CBOX_ITEM cfgcbox2[3]={"������ � �����","����.�������.","������"};

__root const CFG_HDR cfghdr_m10={CFG_LEVEL,"",0,0};

//---------------------------------------------------------------------------

//**************************** ���������� ������ ******************************
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
