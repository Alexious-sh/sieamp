extern int MSG_Report;

int isPlaying(); // 0 - ������ �� ����, 1 - ����, 2 - �����.
void Pause();
void Resume();
void Stop();
void SetVolume(int volume);
void SetPosition(int pos_ms);
int GetPosition();
void playSoundFile(char* filename, int vol);
int getDuration();
void SetEq(int type);
void getHobj();

void ParseMsg(GBS_MSG *msg);
