
// ��������� ��������� �����
// �������� ���� �� ���������� mp3 ������ (c) ILYA_ZX

int randseed;

#define random randint
#define randomize Randomize

unsigned int randint( int max )
{
  return (rand(&randseed, max));
}

void Randomize(){ // �������� ��� ������ ��� ��������� ������������������
  //TDate date;
  //TTime time;
  //GetDateTime(&date,&time);
  //rndindex = (unsigned char)time.param;
  //randseed=(time.sec | (time.millisec));
  randseed=0;
}
