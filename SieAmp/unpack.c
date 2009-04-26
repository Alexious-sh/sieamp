#include "../inc/swilib.h"
#include "../inc/zlib.h"

int fread32(int fh, char *buf, int len, unsigned int *err) // ������ �� ����� �������� (c)Rst7
{
int clen;
int rlen; 
int total=0;
while(len)
{
if (len>16384) clen=16384; else clen=len;
total+=(rlen=fread(fh, buf, clen, err));
if (rlen!=clen) break;
buf+=rlen;
len-=clen;
}
return(total);
}

int fwrite32(int fh, char *buf, int len, unsigned int *err) // ������ � ���� �������� (c)Rst7
{
int clen;
int rlen; 
int total=0;
while(len)
{
if (len>16384) clen=16384; else clen=len;
total+=(rlen=fwrite(fh, buf, clen, err));
if (rlen!=clen) break;
buf+=rlen;
len-=clen;
}
return(total);
}

// �������� ������� ����������. ���������:
// compr - ����� � ������������� zip �������
// comprLen - ��� �����
// uncompr - ����� ��� ����� ���� ������������� ������
// uncomprLen - ��� �����
// ����� � �������� �����. ������ ������� �� ��������� zip ����� (��. ����)
int unzip(Byte *compr, uLong comprLen, Byte *uncompr, uLong  uncomprLen){
    int err;
    z_stream d_stream;
    d_stream.zalloc = (alloc_func)0;
    d_stream.zfree = (free_func)0;
    d_stream.opaque = (voidpf)0;
    d_stream.next_in  = compr;
    d_stream.avail_in = (uInt)comprLen;
    err = inflateInit2(&d_stream,-MAX_WBITS);
    if(err!=Z_OK){ 
   unerr:
      return err;
    }
    d_stream.next_out = uncompr;
    d_stream.avail_out = (uInt)uncomprLen;
    err = inflate(&d_stream, 2);
    if(err<0) goto unerr;
    err = inflateEnd(&d_stream);
    if(err<0) goto unerr;
    return 0;
}

int getint(unsigned char *ptr){  // ����� ����� �� 4� ���� return(*(long*)ptr ����! ��� ���)
  return ptr[0]|(ptr[1]<<8)|((ptr[2])<<16)|((ptr[3])<<24);
}

void prepname(char *file){	// ����� ������������� ������ ���� �����: �� dir/file -> file
  char *s=file+strlen(file);
  while(--s>file && *s!='/');
  if(s>file){
    do{ 
      *file++=*++s;
    } while(*s!=0);
  }
}

int UnPack(const char* fname, const char* path)
{
  int in,out;
  int packsize=1, unpacksize=2, filenamesize=3;
  unsigned int err;
  char* filename = malloc(256);
  char *buf, *file = malloc(256);
  if((in=fopen(fname,A_ReadOnly+A_BIN,P_READ,&err))!=-1){	// ������� ��� ������
    while(1){
      if(fread(in,file,30,&err)!=30) break;
      if(getint((unsigned char*)file)!=0x4034b50) break;
      packsize=getint((unsigned char*)file+18);
      unpacksize=getint((unsigned char*)file+22);
      filenamesize=getint((unsigned char*)file+26);
      fread(in,file,filenamesize,&err);
      file[filenamesize]=0;
      if(file[filenamesize-1]!='/'){
        if((buf=(char*)malloc((packsize==unpacksize)?packsize:(packsize+unpacksize)))!=NULL){ // �������� ������
          fread32(in,buf,packsize,&err);	// ������
          prepname(file);
          sprintf(filename, "%s%s", path, file);
          if ((out=fopen(filename,A_ReadWrite+A_BIN+A_Create,P_READ+P_WRITE,&err))!=-1){ // ������� ��� ������ � ������
            if(packsize==unpacksize){
              fwrite32(out,buf,packsize,&err);	// ������
            }else{
              unzip((Byte*)buf,packsize,(Byte*)(buf+packsize),unpacksize);	// ��� ����������!
              fwrite32(out,buf+packsize,unpacksize,&err);
            }
            fclose(out,&err);  // �������
          }
          mfree(buf); // ���������� ������ (�����������!)
        }
      }
    }
  fclose(in,&err);
 }
 mfree(filename);
 mfree(file);
 return (1);
}
