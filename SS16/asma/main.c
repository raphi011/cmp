#include <stdio.h>

long asma(long a, long b, long c);

int main(int argc, char **argv) {
  long a, b, c, r;


  // Test 1
  a = -10000000000L;
  b = -10000000000L;
  c = -1048576L;
  r = asma(a, b, c);
  printf("asma(%ld, %ld, %ld) = %ld\n", a, b, c, r);


  // Test 2
  a = 10000000000L;
  b = 10000000000L;
  c = 95367431640625L;
  r = asma(a, b, c);
  printf("asma(%ld, %ld, %ld) = %ld\n", a, b, c, r);


  // Test 3
  a = 2973064094867485803L;
  b = 4862021554448358207L;
  c = 5901078158330543530L;
  r = asma(a, b, c);
  printf("asma(%ld, %ld, %ld) = %ld\n", a, b, c, r);
  return 0;
}