#include "../inc/swilib.h"
#include "file_works.h"
#include "Skin.h"
extern char ELF_PATH[256];

char * getSymbolicPath(const char * cFileName)
{
  char* path = malloc(strlen(ELF_PATH)+strlen(cFileName)+2);
  strcpy(path,ELF_PATH);
  strcat(path,cFileName);
  make_dirs(path);
  return path;
}

int sieamp_fexists(const char * cFileName)
{
  char * path = getSymbolicPath(cFileName);
  FSTATS fs;
  unsigned int ul;
  int res = (GetFileStats(path, &fs, &ul)!=-1);
  mfree(path);
  return res;
}

int make_dirs(const char * path)
{
  int c, i = 0;
  unsigned int io_error;
  char tmp[256], * s;
  strcpy(tmp, path);
  s = tmp;
  while((s = strchr(s, '\\')))
  {
    s++;
    c = *s;
    *s = 0;
    i += mkdir(tmp, &io_error);
    *s = c;
  }
  return (i);
}

char *find_eol(char *s)
{
  int c;
  s--;
  do
  {
    s++;
    c=*s;
  }
  while((c)&&(c!=10)&&(c!=13));
  return s;
}

char ascII2char(char symbol)
{
  char out;
  if(symbol < 0x40) out = symbol - 0x30;
  else if(symbol < 0x47) out = symbol - 0x37;
  else out = symbol - 0x57;
  return out;
}

void getColor(char* src, char* dst)
{
  dst[0] = (ascII2char(src[0])<<4) + ascII2char(src[1]);
  dst[1] = (ascII2char(src[2])<<4) + ascII2char(src[3]);
  dst[2] = (ascII2char(src[4])<<4) + ascII2char(src[5]);
  dst[3] = 0x64;
}

int char16to8(int c)
{
  if (c<0x400) return (c);
  c-=0x400;
  if (c<16)
  {
    if (c==1) c=0;
    else if (c==4) c=2;
    else if (c==6) c=10;
    else return (c);
  }
  else if (c>79)
  {
    if (c==0x51) c=16;
    else if (c==0x54) c=18;
    else if (c==0x56) c=11;
    else if (c==0x57) c=23;
    else return (c);
  }
  else c+=8;
  c+=168;
  return (c);
}

void ws16_2str8(WSHDR * ws, char * str, int len)
{
  for(int i = NULL; i < len; i++)
  {
    int c = ws->wsbody[i + 1];
    if (c == 10) c = 13;
    if (i < ws->wsbody[0])
      str[i] = char16to8(c);
    else
      str[i] = NULL;
  }
}

int parsePLcfg(char* cfgfile)
{
  int f, c;
  unsigned int ul;
  if ((f=fopen(cfgfile,A_ReadOnly,P_READ,&ul))!=-1)
  {
    int size_cfg=lseek(f,0,S_END,&ul,&ul);
    lseek(f,0,S_SET,&ul,&ul);
    char* s=malloc(size_cfg+1);
    if (s)
    {
      s[fread(f,s,size_cfg,&ul)]=0;
      //Теперь файл загружен
      while((c=*s)) //Пока не конец файла
      {
	if ((c==10)||(c==13)||(c==32))
	{
	  s++;
	  continue; //Конец строки, опять с начала строки
	}
        if ((c==10)||(c==13))
	{
	  s++;
	  continue; //Конец строки, опять с начала строки
	}
        if ((c==';')||c=='[')
        {
	  //Комментарий
          s=find_eol(s);
          continue;
        }
	if (!strncmp(s,"Normal=#",8))
	{
	  s+=8;
          getColor(s, NormalCOL);
          s=find_eol(s); if (*s) {*s++=0; continue;} else break;
	}
	if (!strncmp(s,"Current=#",9))
	{
	  s+=9;
	  getColor(s, CurrentCOL);
          s=find_eol(s); if (*s) {*s++=0; continue;} else break;
	}
	if (!strncmp(s,"NormalBG=#",10))
	{
	  s+=10;
	  getColor(s, BGCOL);
          s=find_eol(s); if (*s) {*s++=0; continue;} else break;
	}
	if (!strncmp(s,"SelectedBG=#",12))
	{
	  s+=12;
	  getColor(s, SelectBG);
          s=find_eol(s); if (*s) {*s++=0; continue;} else break;
	}
	break;
      }
    }
  }
  else goto LERROR;
  fclose(f, &ul);
  return(1);
  LERROR:
	ShowMSG(1,(int)"PLEDIT.txt: Parse error!");
        return(0);
}
