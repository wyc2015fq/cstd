
arrtype* arr, *arr1, *arr2, *arr3, t0, t1;
arr = (arrtype*)dst;
arr1 = arr;
step /= sizeof(arr[0]);

for (y = 1; y < len; y++)
{
  arr += step, arr1 += 3;
  arr2 = arr;
  arr3 = arr1;

  for (; arr2 != arr3; arr2 += 3, arr3 += step) {
    t0 = arr2[0];
    t1 = arr3[0];
    arr2[0] = t1;
    arr3[0] = t0;
    t0 = arr2[1];
    t1 = arr3[1];
    arr2[1] = t1;
    arr3[1] = t0;
    t0 = arr2[2];
    t1 = arr3[2];
    arr2[2] = t1;
    arr3[2] = t0;
  }
}