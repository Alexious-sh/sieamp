#include "../inc/swilib.h"
#include "../inc/zlib.h"

int fread32(int fh, char *buf, int len, unsigned int *err) // Читать из файла поблочно (c)Rst7
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

int fwrite32(int fh, char *buf, int len, unsigned int *err) // Писать в файл поблочно (c)Rst7
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

const char * const z_errmsg[10] = {
"need dictionary",     /* Z_NEED_DICT       2  */
"stream end",          /* Z_STREAM_END      1  */
"",                    /* Z_OK              0  */
"file error",          /* Z_ERRNO         (-1) */
"stream error",        /* Z_STREAM_ERROR  (-2) */
"data error",          /* Z_DATA_ERROR    (-3) */
"insufficient memory", /* Z_MEM_ERROR     (-4) */
"buffer error",        /* Z_BUF_ERROR     (-5) */
"incompatible version",/* Z_VERSION_ERROR (-6) */
""};

void Compression_Report_Error(char *when, int code)
{
  char q[255];

  LockSched();
  sprintf(q, "%s error: %s\n", when, z_errmsg[(-code)+2]);
  MsgBoxError(1, (int)q);
  UnlockSched();
}

// Функции-заглушки для ZLib
void* zcalloc(voidpf unk,size_t nelem, size_t elsize)
{
  return (malloc(nelem*elsize+1));
}

void zcfree(voidpf unk, void* ptr)
{
  mfree(ptr);
}

// Основная функция распаковки. Параметры:
// compr - буфер с запакованными zip данными
// comprLen - его длина
// uncompr - место под буфер куда распаковывать данные
// uncomprLen - его длина
// Длины и смещения запак. данных берутся из заголовка zip файла (см. ниже)
int unzip(Byte *compr, uLong comprLen, Byte *uncompr, uLong  uncomprLen){
    int err;
    z_stream d_stream;
    d_stream.zalloc = (alloc_func)zcalloc;
    d_stream.zfree = (free_func)zcfree;
    d_stream.opaque = (voidpf)0;
    d_stream.next_in  = compr;
    d_stream.avail_in = (uInt)comprLen;
    err = inflateInit2(&d_stream,-MAX_WBITS);
    d_stream.next_out = uncompr;
    d_stream.avail_out = (uInt)uncomprLen;
    err = inflate(&d_stream, 2);
    switch (err) {
      case Z_NEED_DICT:
	 //ret = Z_DATA_ERROR;     /* and fall through */
      case Z_DATA_ERROR:
      case Z_MEM_ERROR:
	Compression_Report_Error("Inflating", err);
	return 0;
      }
    err = inflateEnd(&d_stream);
    if(err<0) return 0;
    return 1;
}

int getint(unsigned char *ptr){  // Целое число из 4х байт return(*(long*)ptr низя! пик офф)
  return ptr[0]|(ptr[1]<<8)|((ptr[2])<<16)|((ptr[3])<<24);
}

void prepname(char *file){	// Здесь отбрасывается полный путь файла: из dir/file -> file
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
  char *buf, *file = malloc(256), *outbuf;
  if((in=fopen(fname,A_ReadOnly+A_BIN,P_READ,&err))!=-1){	// Открыть для чтения
    while(1){
      if(fread(in,file,30,&err)!=30) break;
      if(getint((unsigned char*)file)!=0x4034b50) break;
      packsize=getint((unsigned char*)file+18);
      unpacksize=getint((unsigned char*)file+22);
      filenamesize=getint((unsigned char*)file+26);
      fread(in,file,filenamesize,&err);
      file[filenamesize]=0;
      if(file[filenamesize-1]!='/'){
        if((buf=(char*)malloc(packsize+1))!=NULL){ // Выделить память
          outbuf = malloc(unpacksize+1);
          fread32(in,buf,packsize,&err);	// Читать
          prepname(file);
          sprintf(filename, "%s%s", path, file);
          if ((out=fopen(filename,A_ReadWrite+A_BIN+A_Create,P_READ+P_WRITE,&err))!=-1){ // Открыть для чтения и записи
            if(packsize==unpacksize){
              fwrite32(out,buf,packsize,&err);	// Писать
            }else{
              unzip((Byte*)buf,packsize,(Byte*)outbuf,unpacksize);	// Это распаковка!
              fwrite32(out,outbuf,unpacksize,&err);
            }
            fclose(out,&err);  // Закрыть
          }
          mfree(buf); // Освободить память (обязательно!)
          mfree(outbuf);
        }
      }
    }
  fclose(in,&err);
 }
 mfree(filename);
 mfree(file);
 return (1);
}
