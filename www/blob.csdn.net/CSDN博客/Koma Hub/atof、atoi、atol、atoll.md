# atof、atoi、atol、atoll - Koma Hub - CSDN博客
2019年01月29日 22:15:37[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：58
atoi.c    stdlib    964    2018/8/1    3
```cpp
/* Convert a string to a double.  */
double
atof (const char *nptr)
{
  return strtod (nptr, (char **) NULL);
}
/* Convert a string to an int.  */
int
atoi (const char *nptr)
{
  return (int) strtol (nptr, (char **) NULL, 10);
}
libc_hidden_def (atoi)
/* Convert a string to a long int.  */
long int
atol (const char *nptr)
{
  return strtol (nptr, (char **) NULL, 10);
}
/* Convert a string to a long long int.  */
long long int
atoll (const char *nptr)
{
  return strtoll (nptr, (char **) NULL, 10);
}
```
