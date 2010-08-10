typedef struct
{
char istagg[3];
char title[30];
char artist[30];
char album[30];
char year[4];
char comment[30];
char genre;
}
ID3TAGDATA;

typedef struct
{
char title[256];
char artist[256];
char album[256];
char comment[256];
char year[4];
char track[4];
int genre;
}
ID3V2TAGDATA;

#define ID3_ENCODING_COUNT 4
static const char* const id3_encoding[] = {
   "ISO-8859-1",   /* 0x00 */
   "UTF-16",       /* 0x01 */
   "UTF-16BE",     /* 0x02 */
   "UTF-8"};       /* 0x03 */

#define V2_TAG_IDS 7
static const char* const v2_id[] = {
  "TIT2", //Название трека
  "TPE1", //Артист
  "TALB", //Альбом
  "TYER", //Год
  "COMM", //Коментарий
  "TCON", //Жанр
  "TRCK",}; //Номер трека

// В label возвращает сроку в формате Исполнитель - Заголовок, если находит теги
// Если нет ID3v2, то берется ID3v1, если нет и его, то имя файла без расширения
int getMP3Label(char* filename, WSHDR* label);

//////////////////////////////// ID3v1 /////////////////////////////////////////
//ID3TAGDATA *tag;

// Читаем тег, функция возвращает "1", если тег есть и "0", если нет
// Сами данные тега возвращаются в структуре ID3TAGDATA *tag
int ReadID3v1(char*filename, ID3TAGDATA *tag);

//////////////////////////////// ID3v2 /////////////////////////////////////////
//ID3TAGDATA *tag2;

// Проверка наличия ID3v2 в файле 1 - есть, 0 - нету :)
int isID3v2Present(char*filename);

// Длинна всего тега в байтах. Мало ли, пригодится...
int lenID3v2(char*filename);

// Получаем структуру с ID3v2
void ReadID3v2(char*filename, ID3V2TAGDATA *tag2);
