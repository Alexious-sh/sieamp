
IMGHDR* CreateImgHdrByAnyFile (WSHDR * path,short width, short height, int rot, RECT clip);
void DrwImg(IMGHDR *img, int x, int y);
void DrwCropImg(IMGHDR *img, int x, int y, int deltx, int delty, int w, int h);
void DrawDigit(IMGHDR *img, int digit, int x, int y, int w);
void FreeIMG(IMGHDR* img);
void insertIMG(IMGHDR* img, IMGHDR* src, short x, short y);
