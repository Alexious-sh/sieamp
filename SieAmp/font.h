int GetWidth(char c);
int GetHeigth(char c);
int isCharValid(char c);
void DrawChar(int x, int y, char c,const char* clid);
int GetStrWidth(char str[40]);
int DrawStr(char str[40] ,int x,int y,const char* clid);
void DrawStrUnderline(char str[40] ,int x,int y, char* clid, int lineattr);
void DrawStrShadowed(char str[40] ,int x,int y,const char* clid, const char* shadow);
