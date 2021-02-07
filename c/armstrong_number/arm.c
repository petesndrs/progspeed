#include <stdbool.h>
#include <stdio.h>
#include <math.h>

int numdigits(int n)
{
  int i = 10;
  int digits = 1;
  while (true) {
    if (n < i) return digits;
    i *= 10;
    digits++;
  }
}

int main()
{
  for (int n=10; n<10000000; ++n)
  {
    int total = 0;
    int digits = numdigits(n);
    int temp_n = n;

    for (int i=0; i<digits; ++i)
    {
      int a = temp_n%10;
      total = total + pow(a,digits);
      temp_n = temp_n/10;
    }

    if (total == n)
    {
      printf("Armstrong number: %d\n", n);
    }
  }
}
