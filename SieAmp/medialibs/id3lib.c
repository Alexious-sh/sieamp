#include "../inc/swilib.h"
#include "genres.c"
#include "id3lib.h"
//#include "debug.c"

extern char *percent_t;
extern char *percent_s;
extern char *percent_d;
extern char *percent_ts;
extern char *percent_td;

unsigned int err;
int f;
long SongSize;

//ID3v2
char *tagdata2;
unsigned int v2len;

//------------------------------------------------------------------------------
void debugWrite(char* stringg, char* file, int len)
{
 int ff; 
 char s[2];
 ff = fopen(file,A_ReadWrite+A_BIN+A_Create+A_Truncate, P_READ + P_WRITE, & err);
 lseek(f,0,S_END,&err,&err);
 fwrite(ff, stringg, len, &err);
 s[0] = 0x0D; s[1] = 0x0A;
 fwrite(ff, s, 2, &err);
 fclose(ff, &err);
}
//------------------------------------------------------------------------------

int ReadID3v1(char* filename, ID3TAGDATA *tag) // Чтение ID3 v1.1
{
  f=fopen(filename, A_ReadOnly+A_BIN, P_READ, &err);
  SongSize=lseek(f,0,S_END,&err,&err);
  lseek(f,(SongSize-128),0,&err,&err);
  fread( f,tag, 128, & err );
  fclose(f,&err);
  if((tag->istagg[0]=='T')&&(tag->istagg[1]=='A')&&(tag->istagg[2]=='G')) return 1; // Проверяем на наличие тегов
  return 0;
}

//ID3v2 :)

//---------------------------- Кодировки ---------------------------------------

int utf16_calcSymb(char *utf16, int len)
{
        unsigned int bom = *utf16 << 8 | *(utf16 + 1);
	if (bom == 0xfffe || bom == 0xfeff) return((len-2)/2); // if BOM - skipping
	else return(len/2);
}

size_t utf16_strlen(char *string)
{
	size_t len = 0;

	while (*(string + len) != 0 || *(string + len + 1) != 0)
		len += 2;

	return len;
}

static char* to_ascii(char *utf16, int le, int len)
{
	char *ascii;
	unsigned int i/*, len*/, c;

	//len = utf16_strlen(utf16) / 2 + 1;
	
	ascii = malloc(len + 1);

	for (i = 0, c = 0; i < len; i++)
	{
		unsigned int uc;
		int o = i << 1;

		if (le)
			uc = *(utf16 + o) | *(utf16 + o + 1) << 8;
		else
			uc = *(utf16 + o) << 8 | *(utf16 + o + 1);

		/* Skip BOM and surrogate pairs */
		if (uc == 0xfeff || (uc >= 0xd800 && uc <= 0xdfff))
			continue;
		
		if (uc < 0x80)
			ascii[c] = uc;
		else 
			ascii[c] = uc + 0xB0;
		c++;
	}
	
	ascii[c] = 0;
	return ascii;
}

char *convert_from_utf16(char *utf16, int len)
{
	int le = 0;
	unsigned int bom = *utf16 << 8 | *(utf16 + 1);

	if (bom == 0xfffe)
		le = 1;
	else if (bom != 0xfeff)
		return NULL;

	return to_ascii(utf16, le, len);
}

int id3_utf8_length(char *utf8)
{
  int length = 0;

  while (*utf8) {
    if ((utf8[0] & 0x80) == 0x00)
      ++length;
    else if ((utf8[0] & 0xe0) == 0xc0 &&
	     (utf8[1] & 0xc0) == 0x80) {
      if (((utf8[0] & 0x1fL) << 6) >= 0x00000080L) {
	++length;
	utf8 += 1;
      }
    }
    else if ((utf8[0] & 0xf0) == 0xe0 &&
	     (utf8[1] & 0xc0) == 0x80 &&
	     (utf8[2] & 0xc0) == 0x80) {
      if ((((utf8[0] & 0x0fL) << 12) |
	   ((utf8[1] & 0x3fL) <<  6)) >= 0x00000800L) {
	++length;
	utf8 += 2;
      }
    }
    else if ((utf8[0] & 0xf8) == 0xf0 &&
	     (utf8[1] & 0xc0) == 0x80 &&
	     (utf8[2] & 0xc0) == 0x80 &&
	     (utf8[3] & 0xc0) == 0x80) {
      if ((((utf8[0] & 0x07L) << 18) |
	   ((utf8[1] & 0x3fL) << 12)) >= 0x00010000L) {
	++length;
	utf8 += 3;
      }
    }
    else if ((utf8[0] & 0xfc) == 0xf8 &&
	     (utf8[1] & 0xc0) == 0x80 &&
	     (utf8[2] & 0xc0) == 0x80 &&
	     (utf8[3] & 0xc0) == 0x80 &&
	     (utf8[4] & 0xc0) == 0x80) {
      if ((((utf8[0] & 0x03L) << 24) |
	   ((utf8[0] & 0x3fL) << 18)) >= 0x00200000L) {
	++length;
	utf8 += 4;
      }
    }
    else if ((utf8[0] & 0xfe) == 0xfc &&
	     (utf8[1] & 0xc0) == 0x80 &&
	     (utf8[2] & 0xc0) == 0x80 &&
	     (utf8[3] & 0xc0) == 0x80 &&
	     (utf8[4] & 0xc0) == 0x80 &&
	     (utf8[5] & 0xc0) == 0x80) {
      if ((((utf8[0] & 0x01L) << 30) |
	   ((utf8[0] & 0x3fL) << 24)) >= 0x04000000L) {
	++length;
	utf8 += 5;
      }
    }

    ++utf8;
  }

  return length;
}

void DecodeWs2Str(WSHDR* wsc, char * string, int slen)
{
  extern int char16to8(int c);
  unsigned short *wsbody=wsc->wsbody;
  int c, scur=0, wcur=0, wslen=wsbody[0]; 
  do
  {
    if (wcur<wslen)
    {
     c=wsbody[wcur+1];
     if (c==10) c=13;
    }
    if (wcur<wslen) string[scur] = char16to8(c);
    else string[scur] = NULL;
    scur++;
    wcur++;
  }
  while(scur<slen);
}

// From SieJC
// Строковый вариант
char* convUTF8_to_ANSI_STR(char *UTF8_str)
{
  // Рассматривая строку UTF8 как обычную, определяем её длину
  if(!UTF8_str)return NULL;
  int st_len = strlen(UTF8_str);

  // Выделяем память - на всякий случай дохера
  int lastchar = 0;
  int dummy;
  char* tmp_out = malloc(st_len+1);
  zeromem(tmp_out,st_len+1);
  char chr, chr2, chr3;
  for(int i=0;i<st_len;i++)
  {
  chr = (*(UTF8_str+i));

	if (chr<0x80)
        {
          *(tmp_out+lastchar)=chr;
          lastchar++;
          goto L_END_CYCLE;
        }
	if (chr<0xc0)
        {
          ShowMSG(1,(int)"Bad UTF-8 Encoding encountered (chr<0xC0)");
          mfree(tmp_out);
          return NULL;
        }
	
        chr2 = *(UTF8_str+i+1);

        if (chr2<0x80)
        {
          ShowMSG(1,(int)"Bad UTF-8 Encoding encountered (chr2<0x80)");
          mfree(tmp_out);
          return NULL;
        }
	
	if (chr<0xe0) {
	    // cx, dx
                  if ((chr == 0xD0)&&(chr2 == 0x81)){*(tmp_out+lastchar) = 0xA8;}//Ё
             else if ((chr == 0xD0)&&(chr2 == 0x86)){*(tmp_out+lastchar) = 0xB2;}//І
             else if ((chr == 0xD0)&&(chr2 == 0x87)){*(tmp_out+lastchar) = 0xAF;}//Ї
             else if ((chr == 0xD0)&&(chr2 == 0x84)){*(tmp_out+lastchar) = 0xAA;}//Є
             else if ((chr == 0xD1)&&(chr2 == 0x91)){*(tmp_out+lastchar) = 0xB8;}//ё
             else if ((chr == 0xD1)&&(chr2 == 0x96)){*(tmp_out+lastchar) = 0xB3;}//і
             else if ((chr == 0xD1)&&(chr2 == 0x97)){*(tmp_out+lastchar) = 0xBF;}//ї
             else if ((chr == 0xD1)&&(chr2 == 0x94)){*(tmp_out+lastchar) = 0xBA;}//є
             else if ((chr == 0xD2)&&(chr2 == 0x91)){*(tmp_out+lastchar) = 0xE3;}//ґ->г
             else if ((chr == 0xD2)&&(chr2 == 0x90)){*(tmp_out+lastchar) = 0xC3;}//Ґ->Г
             else
          {
	    char test1 = (chr & 0x1f)<<6;
            char test2 = chr2 & 0x3f;
            *(tmp_out+lastchar)= test1 | test2 + 127 + 0x31;
          }
            i++;
            lastchar++;
            goto L_END_CYCLE;
	}
	if (chr<0xf0) {
	    // cx, dx
	    chr3= *(UTF8_str+i+2);

	    if (chr3<0x80)
            {
              ShowMSG(1,(int)"Bad UTF-8 Encoding encountered");
              mfree(tmp_out);
              return NULL;
            }
	    else
            {
              *(tmp_out+lastchar) =  ((chr & 0x0f)<<12) | ((chr2 &0x3f) <<6) | (chr3 &0x3f);
              i=i+2;
            }
	}

  L_END_CYCLE:
    dummy++;
  }
  st_len = strlen(tmp_out);
  tmp_out = realloc(tmp_out,st_len+1);
  return tmp_out;
}


void decodeFrame(char* ctagg, int leng, char* out)
{
  WSHDR* ws_info = AllocWS(256);
  CutWSTR(ws_info, 0);
  switch (ctagg[0])
  {
   case 0x01 :
    {
     if (leng > 1)
     {
        memcpy(out, convert_from_utf16(ctagg+1, leng-1), utf16_calcSymb(ctagg+1, leng-1));
#ifdef LOG
        ShowMSG(1,(int)"UTF16LE");
#endif
     }
     break;
    }
   case 0x02 :
    {
     if (leng > 1)
     {
        memcpy(out, convert_from_utf16(ctagg+1, leng-1), utf16_calcSymb(ctagg+1, leng-1));
#ifdef LOG
        ShowMSG(1,(int)"UTF16BE");
#endif
     }
     break;
    }
    case 0x03 :
    {
     memcpy(out, convUTF8_to_ANSI_STR(ctagg+1), id3_utf8_length(ctagg+1));
#ifdef LOG
     ShowMSG(1,(int)"UTF8");
#endif
     break;
    }
    default :
    {
     memcpy(out,ctagg+1,(leng-1));
#ifdef LOG
     ShowMSG(1,(int)"ASCII");
#endif
     break;
    }
  }
}

//------------------------------------------------------------------------------

//Проверка наличия ID3v2
int isID3v2Present(char*filename)
{
  char id3[3];
  f=fopen(filename, A_ReadOnly+A_BIN, P_READ, &err);
  lseek(f,0,S_SET,&err,&err);
  fread( f,id3, 3, & err );
  fclose(f,&err);
  if((id3[0]=='I')&&(id3[1]=='D')&&(id3[2]=='3'))
  {
    return(1);
  }
  else
  {
    return(0);
  }
}

int lenID3v2(char* filename) // длинна всего ID3v2 тега
{
  char size[10];
  int lenTags;
  f=fopen(filename, A_ReadOnly+A_BIN, P_READ, &err);
  fread( f,size, 10, & err );
  lenTags = size[6] * 0x200000 + size[7] * 0x4000 + size[8] * 0x80 + size[9] + 0xA;
  fclose(f,&err);
  return lenTags;
}

//Считывание всего ID3v2 тега
void LoadID3v2(char* filename)
{
   int fi;
   v2len = 0;
   v2len = lenID3v2(filename); //Получаем размер тега
   
   fi = fopen(filename,A_ReadOnly+A_BIN,P_READ,&err); //Открываем файл
   tagdata2 = malloc(v2len); //Выделяем память под тег
   fread( fi,tagdata2, v2len, & err ); //Читаем тег в tagdata2
   fclose(fi, &err); //Закрываем файл
   //debugWrite(tagdata2, "4:\\ZBin\\var\\tagdata.dbg", v2len);
}

//Ищем нужный фрейм по его id
//Результатом является индекс в массиве tagdata2 где расположен первый байт id фрейма
int FindTag(char const*tagid)
{
  int pos = 0;
  for(pos = 10; pos < (v2len-10); pos++)
  {
    if ((tagdata2[pos]==tagid[0])&&(tagdata2[pos+1]==tagid[1])&&(tagdata2[pos+2]==tagid[2])&&(tagdata2[pos+3]==tagid[3]))
    {
      return pos; //Если нашли значит текущая позиция = результат
    }
  }
  return 0;
}

//Считываем и вычисляем размер фрейма в байтах
//Функции нужно скормить уже высчитанную стартовую позицию размера фрейма
int GetFrameLen(int tagpos)
{
  int taglen = tagdata2[tagpos] * 0x200000 + tagdata2[tagpos+1] * 0x4000 + tagdata2[tagpos+2] * 0x80 + tagdata2[tagpos+3] + 0xA;
  return taglen;
}

//получаем байт кодировки фрейма + его содержимое
//Функции нужно скормить индекс нужного фрейма из V2_TAG_IDS
int getTag(int id, char* ctag)
{
  int tagpos = 0, taglen = 0;
   tagpos = FindTag(v2_id[id]); //Ищем нужный фрейм
   if(tagpos) //Если нашли
   {
    taglen = GetFrameLen(tagpos+4); //Получаем размер фрейма
    if (taglen <= 11) return(0); //Если размер фрейма <= длинне его заголовка+байт кодировки, то текста там нет(млин, и такое бывает! %) )
    //Выдираем из tagdata2 байт кодировки+содержимое фрейма
    memcpy(ctag,&tagdata2[tagpos + 10],taglen - 10);
   }
   else //Значит нет такого фрейма
   {
    return 0; //Возвращаем 0
   }
   return (taglen - 10); //Значит все пучком, отправляем размер строки ;)
}

int getComent(char* ctag)
{
   int tagpos = 0, taglen = 0, tseek = 0;
   tagpos = FindTag(v2_id[4]); //Ищем нужный фрейм
   if(tagpos) //Если нашли
   {
     char *tmp = malloc(256);
     taglen = GetFrameLen(tagpos+4); //Получаем размер фрейма
 /*Если размер фрейма <= длинне его заголовка+байт кодировки+id языка+еще какойто ипанутый байт,
   то текста там нет(млин, и такое бывает! %) )*/
     if (taglen <= 15) return(0); 
     //Выдираем из tagdata2 байт кодировки+содержимое фрейма
     memcpy(tmp,&tagdata2[tagpos + 10],taglen - 10);
     ctag[0] = tmp[0];
     switch(ctag[0])
     {
       case 0:
       case 3:
         tseek = 5;
         break;
       case 1:
       case 2:
         tseek = 8;
         break;
       default:
         return 0;
      }
     int q = 0, cc = 0;
     for(q = tseek, cc = 1; q <= taglen; q++)
     {
       ctag[cc] = tmp[q];
       cc++;
     }
     //debugWrite(ctag, "4:\\ZBin\\var\\ctag.log", taglen);
   }
   else //Значит нет такого фрейма
   {
    return 0; //Возвращаем 0
   }
   return (taglen - (9+tseek)); //Значит все пучком, отправляем размер строки ;)
}

void ReadID3v2(char*filename, ID3V2TAGDATA *tag2)
{
  if(isID3v2Present(filename))
  {
    int taglen = 0;
    LoadID3v2(filename);
    char *ctag = malloc(256);
    // Получаем заголовок
    taglen = getTag(0, ctag);
    if (taglen)
    {
      decodeFrame(ctag, taglen, tag2->title);
    }
    else CutWSTR(tag2->title,0);
    // Получаем артиста
    taglen = getTag(1, ctag);
    if (taglen)
    {
      decodeFrame(ctag, taglen, tag2->artist);
    }
    else CutWSTR(tag2->artist,0);
    // Получаем альбом
    taglen = getTag(2, ctag);
    if (taglen)
    {
      decodeFrame(ctag, taglen, tag2->album);
    }
    else CutWSTR(tag2->album,0);
    //Ищем коментарий
    taglen = getComent(ctag);
    if (taglen)
    {
      decodeFrame(ctag, taglen, tag2->comment);
    }
    else CutWSTR(tag2->comment,0);
    //Ишем год
    taglen = getTag(3, ctag);
    if (taglen)
    {
      decodeFrame(ctag, taglen, tag2->year);
    }
    else CutWSTR(tag2->year,0);
    // Ищем номер трека
    taglen = getTag(6, ctag);
    if (taglen)
    {
      decodeFrame(ctag, taglen, tag2->track);
    }
    else CutWSTR(tag2->track,0);
    // Ищем жанр
    taglen = getTag(5, ctag);
    if (taglen)
    {
      char gen[3];
      char* tmpch = malloc(256);
      decodeFrame(ctag, taglen, tmpch);
      int stop = 0, ps = 1;
      if(tmpch[0] == 0x28)
      {
        while(ps <= 3 && !stop)
        {
          if (tmpch[ps] != 0x29)
          {
            gen[ps-1] = tmpch[ps];
          }
          else stop = 1;
          ps++;
        }
        sscanf(gen, percent_d, &tag2->genre);
        if(tag2->genre>TOTAL_GENRES) tag2->genre = -1;
      }
      else tag2->genre = -1;
    }
    else tag2->genre = -1;
  }
}

char* getNameFromPath(char* filename)
{
  char *path=strrchr(filename,'\\');
  path++;
  int len = strlen(path)-4;
  char* name = malloc(len+1);
  memcpy(name, path, len);
  return(name);
}

int getMP3Label(char* filename, WSHDR* label)
{
  char* artist;
  char* title;
  int is_artist = 0, is_title = 0;
  if(isID3v2Present(filename))
  {
    int taglen = 0;
    LoadID3v2(filename);
    char *ctag = malloc(256);
    zeromem(ctag, 256);
    // Получаем название
    taglen = getTag(0, ctag);
    if (taglen > 1 && (ctag[1] != 0 || ctag[2] != 0))
    {
      title = malloc(taglen+1);
      zeromem(title, taglen+1);
      decodeFrame(ctag, taglen, title);
      is_title = 1;
    }
    zeromem(ctag, 256);
    // Получаем артиста
    taglen = getTag(1, ctag);
    //debugWrite(ctag, "4:\\ZBin\\var\\tagg.dbg", taglen);
    if (taglen > 1 && (ctag[1] != 0 || ctag[2] != 0))
    {
      artist = malloc(taglen+1);
      zeromem(artist, taglen+1);
      decodeFrame(ctag, taglen, artist);
      is_artist = 1;
    }
    mfree(ctag);
  }
  if(!is_artist || !is_title)
  {
    ID3TAGDATA* tag = malloc(sizeof(ID3TAGDATA));
    zeromem(tag, sizeof(ID3TAGDATA));
    if(ReadID3v1(filename, tag))
    {
      if(!is_artist && tag->artist[0] != 0)
      {
        artist = malloc(31);
        memcpy(artist, tag->artist, 30);
        is_artist = 1;
      }
      if(!is_title && tag->title[0] != 0)
      {
        title = malloc(31);
        memcpy(title, tag->title, 30);
        is_title = 1;
      }
    }
    mfree(tag);
  }
  if(is_artist && is_title)
  {
    wsprintf(label, "%t - %t", artist, title);
  }
  else if(is_artist || is_title) wsprintf(label, percent_t, (is_artist ? artist : title));
       else return 0;
       
  if(is_artist) mfree(artist);
  if(is_title) mfree(title);
  return 1;
}
