void factorial(int number, unsigned int * output)
{
  unsigned int i;
  for(i=1; i<=number; ++i)
  {
      *output = *output*i;
  }
}
