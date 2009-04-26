
IMGHDR* CreateImgHdrByAnyFile (WSHDR * path,short width, short height, int rot, RECT clip);
void DrwImg(IMGHDR *img, int x, int y);
void FreeIMG(IMGHDR* img);
void insertIMG(IMGHDR* img, IMGHDR* src, short x, short y);
