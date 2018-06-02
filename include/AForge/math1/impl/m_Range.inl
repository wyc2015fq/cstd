
int i, j;

for (j = 0; j < asz.len[1]; j++)
{
  double max = a[j];
  double min = a[j];
  
  for (i = 0; i < asz.len[0]; i++)
  {
    if (a[i*al+j] > max) {
      max = a[i*al+j];
    } else if (a[i*al+j] < min) {
      min = a[i*al+j];
    }
  }
  
  ranges[j] = dRANGE(min, max);
}
