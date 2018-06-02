// 非比较排序，不需要比较，交换，在线性时间内完成排序。缺点：空间要求较多，不是原地排序，典型的空间换取时间。
// 计数排序
// 计数排序利用一个特点：已经排好序（例如从小到大）的数组中，第i个元素为x，则数组中一定有：小于等于x的元素有i个。计数排序需要一个空间代价：n + max_num.
// 桶排序
// 桶排序包含两个步骤：分发元素到每个桶内；顺序回收桶内元素。伪代码如下：
// BUCKET_SORT (A)
// n ← length [A]
// For i = 1 to n do
// Insert A[i] into list B[nA[i]]
// For i = 0 to n-1 do
// Sort list B with Insertion sort
// Concatenate the lists B[0], B[1], . . B[n-1] together in order.
// 代码实现
// bucket_sort是直接遍历count 判断是否大于1,回收元素，counting_sort是把count的内容转化为原数组排序后该元素前面元素的个数，同样需要遍历一次。
// 测试结果：
//
// [cpp] view plaincopyprint?
// Test number : 10000000
// count_sort : 112
// BucketSort : 70
// std::sort : 4646
//
//计数和桶排序时间上的优势不用说了，但是这些非比较排序算法对空间要求比较高，而且跟排序数据的分布也有关。上面的测试结果是基于所有随机数据都在[0-255]之间的假设前提。数据分布越散乱，空间复杂度越高。
//
// 桶排序的思路尤其适合字符串数组的排序，因为acsii字符都位于0，255之间。更加一般的桶排序可以使用链表
void BucketSort(int* array, int length)
{
  int i, k, idx;
  enum { MAX = 256 };
  int bucket[MAX];

  for (i = (0); i < MAX; i++) {
    bucket[i] = 0;
  }

  for (i = 0; i < length; i++) {
    bucket[array[i]]++;
  }

  for (i = 0, idx = 0; i < MAX; i++)
    for (k = bucket[i]; k > 0; k--) { // 回收每个桶内元素
      array[idx++] = i;
    }
}
void counting_sort(int* arr, int* dst, int length)
{
  enum { max_num = 255 };
  int i, count[max_num];

  for (i = (0); i < max_num; i++) {
    count[i] = 0;
  }

  for (i = (0); i < length; i++) {
    count[ arr[i] ]++;
  }

  for (i = (1); i < max_num; i++) {
    count[i] += count[i - 1];
  }

  for (i = length - 1; i >= 0; i--) {
    dst[ count[arr[i]] - 1 ] = arr[i]; // 必须减一
    count[ arr[i] ]--;
  }
}
int test_BucketSort()
{
  if (1) {
    int i;
    int myints[] = {10, 20, 30, 5, 15};
    int length = sizeof(myints) / sizeof(myints[0]);
    int result[100];
    counting_sort(myints, result, length);

    for (i = (0); i < length; i++) {
      printf("%d ", result[i]);
    }

    printf("\n");
  }

  if (1) {
    int idx;
    clock_t t;
    enum { test_num = 1000000 }; // 1 million
    int* testPtr = MALLOC(int, test_num);
    int* testPtr1 = MALLOC(int, test_num);
    int* testPtr2 = MALLOC(int, test_num);
    srand((int)time(0));
    printf("Test number \t:\t\t%d\n", test_num);

    for (idx = 0; idx < test_num; idx++) {
      testPtr2[idx] = testPtr1[idx] = testPtr[idx] = rand() % 255;
    }

    t = clock();
    // sort
    // keep the testPtr1, change the testPtr.
    counting_sort(testPtr1, testPtr, test_num);
    printf("count_sort \t:\t\t%d\n" , clock() - t);
    t = clock();
    BucketSort(testPtr1, test_num);
    printf("BucketSort \t:\t\t%d\n", clock() - t);
    t = clock();
    QSORT(testPtr2, testPtr2 + test_num, LESS, int);
    printf("QSORT \t:\t\t %d", clock() - t);

    // Test the result validity
    for (idx = 0; idx < test_num; idx++) {
      if (testPtr[idx] != testPtr1[idx] || testPtr1[idx] != testPtr2[idx]) {
        printf("wrong");
      }
    }

    free(testPtr);
    free(testPtr1);
    free(testPtr2);
    printf("\n");
  }

  system("PAUSE");
  return 0;
}

