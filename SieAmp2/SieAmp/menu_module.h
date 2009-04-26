

char *percent_t="%t";
char *percent_s="%s";
char *percent_d="%d";
char *percent_ts="%t%s";
char *percent_td="%t%d";
//==============================================================================
#pragma inline
void patch_rect(RECT*rc,int x,int y, int x2, int y2)
{
  rc->x=x;
  rc->y=y;
  rc->x2=x2;
  rc->y2=y2;
}

#pragma inline
void patch_header(const HEADER_DESC* head)
{
  ((HEADER_DESC*)head)->rc.x=0;
  ((HEADER_DESC*)head)->rc.y=YDISP;
  ((HEADER_DESC*)head)->rc.x2=ScreenW()-1;
  ((HEADER_DESC*)head)->rc.y2=HeaderH()+YDISP-1;
}

#pragma inline
void patch_header_small(HEADER_DESC* head)
{
  head->rc.x=3;
  head->rc.y=0x18;
  head->rc.x2=ScreenW()-6;
  head->rc.y2=0x18+0x13;
}

#pragma inline
void patch_input(const INPUTDIA_DESC* inp)
{
  ((INPUTDIA_DESC*)inp)->rc.x=0;
  ((INPUTDIA_DESC*)inp)->rc.y=HeaderH()+1+YDISP;
  ((INPUTDIA_DESC*)inp)->rc.x2=ScreenW()-1;
  ((INPUTDIA_DESC*)inp)->rc.y2=ScreenH()-SoftkeyH()-1;
}
//==============================================================================
void edchat_locret(void){}

static const int MenuSoftkeys[]={0,1,2};

SOFTKEY_DESC Menu_sk[]=
{
  {0x0018,0x0000,(int)"Выбор"},
  {0x0001,0x0000,(int)"Выход"},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

SOFTKEYSTAB Menu_skt=
{
  Menu_sk,0
};
