#include <stdio.h>
#include <string.h>
#include <mcheck.h>
#include <stdlib.h>

extern long asmb_callchecking(long a, long b[], long c[], size_t n);

int test(long a, long b[], long c[], size_t n, long result)
{
  int i;
  long r;
  printf("calling asmb(%ld, b, c, %ld)\n", a, n);
  for (i=0; i<n; i++)
    printf(" b[%d] = %ld, c[%d] = %ld\n", i, b[i], i, c[i]);
  r = asmb_callchecking(a,b,c,n);
  if (r==result) {
    printf("= %ld ok\n", r);
    return 1;
  } else {
    printf("liefert %ld, erwartet %ld\n",r,result);
    return 0;
  }
}

int main()
{
  unsigned long b[]={10000000000L,4862021554448358207L};
  unsigned long c[]={-95367431640625L,5901078158330543530L};

  int f;

  f = test(2973064094867485803,b,c,2,-256855990144222);
  if (!f)
    fprintf(stdout,"\nTest failed.\n");
  else
    fprintf(stdout,"\nTest succeeded.\n");
  return !f;
}
