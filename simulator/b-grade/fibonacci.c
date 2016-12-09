
void fibonacci(int C[])
{
  // calculates 10 terms of fibonacci series
  int first = 0;
  int second = 1;
  int next, i;

  for ( i = 0 ; i < 10 ; i++ )
  {
     if ( i <= 1 )
     {  next = i;
        C[i]=next;
     }
     else
     {
        next = first + second;
        first = second;
        second = next;
        C[i]=next;
     }
  }
}
