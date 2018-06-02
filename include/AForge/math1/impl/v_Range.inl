
int i, l=0, r=0;

for (i = 1; i < n; i++)
{
  if (a[i] < a[l]) {
    l = i;
  } else if (a[i] > a[r]) {
    r = i;
  }
}