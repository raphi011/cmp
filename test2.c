void asma(unsigned long x[], unsigned long y[], unsigned long r[])  
{  
  unsigned long borrow, r0;  
  r0 = x[0]-y[0];  
  borrow = r0>x[0];  
  r[0] = r0;  
  r[1] = x[1]-y[1]-borrow;  
}
