#ifndef _FILE_WORKS_H_
#define _FILE_WORKS_H_

char * getSymbolicPath(const char * cFileName);
int sieamp_fexists(const char * cFileName);
int make_dirs(const char * path);
int parsePLcfg(char* cfgfile);
void ws16_2str8(WSHDR * ws, char * str, int len);

#endif /* _FILE_WORKS_H_ */
