#include <stdio.h>


int main(int argc,char *argv[])
{
  FILE *ifp = fopen(argv[1],"rb");
  if (ifp) {
    unsigned char buf[10];
    fread(buf,3,1,ifp); // header
    while (fread(buf,2,1,ifp) == 1) {
      int i=0;
      unsigned char c=0x80;
      for(i=0;i < 8;i++) {
	printf("%c",(c & buf[1]) ? 'X' : 'O');
	c >>= 1;
      }
      c = 0x80;
      for(i=0;i < 8;i++) {
	printf("%c",(c & buf[0]) ? 'X' : 'O');
	c >>= 1;
      }
      printf(" %02X%02X\n",buf[0],buf[1]);
    }
  }
}
