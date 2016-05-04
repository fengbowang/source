
typedef long (*f)(long i1, long i2, long i3, long i4, long i5);

long t(long i,  char *c)
{
    printf ("v = %d, %d\r\n", i, c);
    //printf ("v = %l, %s\r\n", i, c);
}

long t2(long i,  char *c)
{
    //printf ("v = %d, %d\r\n", i, c);
    printf ("v = %d, %s\r\n", i, c);
}


long t3(long i1,  long i2, char *c)
{
    printf ("sizeof(long) =  %d\r\n", sizeof(long));
    printf ("v1 = %d, v = %d, %s\r\n", i1,  i2, c);
}

int main(int c, char *v[])
{
    long i1 = 1;
   long i2 = 2;
   long i3 = 3;
   long i4 = 5;
   long i5 = 5 ;
   f   f1 = (f )t;
   f   f2 = (f )t2;
   f   f3 = (f )t3;

   if (c > 1)
    i1 = atoi(v[1]);
   if (c > 2)
    i2 = atoi(v[2]);
   if (c > 3)
    i3 = atoi(v[3]);
   if (c > 4)
    i4 = atoi(v[4]);
   if (c > 5)
    i5 = atoi(v[5]);

 f1(i1, i2, i3, i4, i5);

 f2(i1, "hello, world\r\n", i3, i4, i5);
  f3(i1, i2,  "tamade, world\r\n", i4, i5);
   
}
