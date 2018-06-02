{
  arrtype* arr, *arr1, *arr2, *arr3, t0, t1;
  arr = (arrtype*)dst, arr1 = arr;
  step /= sizeof(arr[0]);
  
  while (--len) {
    arr += step, arr1++;
    arr2 = arr, arr3 = arr1;
    
    do {
      t0 = arr2[0], t1 = arr3[0];
      arr2[0] = t1, arr3[0] = t0;
      arr2++, arr3 += step;
    }
    while (arr2 != arr3);
  }
}