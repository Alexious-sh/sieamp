#ifndef _M3UPARSE_H_
#define _M3UPARSE_H_

typedef struct
{
  TTime time;
  WSHDR *track;
  char path[128];
//  char track[64];
//  char path[128];
  int index;
  int played;
  void *next;
  void *prev;
}M3U;

int get_file_size(char* fname);
void FreeM3UList();
M3U *GetM3UByItem(int curitem);
int TotalM3UItem();
int ParseM3UFile(char *fname);

#endif
