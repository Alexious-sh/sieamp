#include "../inc/swilib.h"

char ELF_PATH[256];

extern int MSG_Report;
extern void kill_data(void *p, void (*func_p)(void *));
extern int strcmp_nocase(const char *s, const char *d);
extern const int MSG_RCONFIG;
extern void ParseMsg(GBS_MSG *msg);

extern int ShowGUI();
int ScrW, ScrH;
int MAINGUI_ID;
int maincsm_id;
int start = 0;

typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;

#pragma segment="ELFBEGIN"
void ElfKiller(void) // ����� ��� ���� �� �������� ;)
{
  kill_data(__segment_begin("ELFBEGIN"),(void (*)(void *))mfree_adr());
}

void maincsm_oncreate(CSM_RAM *data) // �������� �������
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
  csm->gui_id=ShowGUI(); // ��� ������ ��������� ���� ������ ������
  MAINGUI_ID = csm->gui_id;
}

void maincsm_onclose(CSM_RAM *csm) // ������� �������
{
  SUBPROC((void *)ElfKiller);
}

int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg) // ���������� ��������� ������ �������
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
  if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id)) csm->csm.state=-3; // �� ��� ���, �� ��� ���� �����
  return(1);
}

static unsigned short maincsm_name_body[140];
static const int minus11=-11; // �������� ��������� :)
static const struct // ��������� ������ CSM
{
  CSM_DESC maincsm;
  WSHDR maincsm_name;
}MAINCSM =
{
  {
  maincsm_onmessage,
  maincsm_oncreate,
#ifdef NEWSGOLD
  0, 0, 0, 0,
#endif
  maincsm_onclose,
  sizeof(MAIN_CSM),
  1,
  &minus11
  },
  {
    maincsm_name_body,
    NAMECSM_MAGIC1,
    NAMECSM_MAGIC2,
    0x0,
    139
  }
};

static void UpdateCSMname(void) // �������, ���������� ��� ����� XTask'�
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name), "Radio");
}

int main(const char *exename, const char *filename) // �������� ���� ����� �������� ;)
{
  ScrW=ScreenW();
  ScrH=ScreenH();
  char *path=strrchr(exename,'\\');
  int l;
  if (!path) return 0; //����� �����-��
  path++;
  l=path-exename;
  memcpy(ELF_PATH,exename,l);
  MAIN_CSM main_csm;
  UpdateCSMname();
  LockSched();
  maincsm_id=CreateCSM(&MAINCSM.maincsm,&main_csm,0);
  UnlockSched();
  return 0;
}
