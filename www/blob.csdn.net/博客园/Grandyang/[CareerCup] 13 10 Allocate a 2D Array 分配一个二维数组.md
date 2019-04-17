# [CareerCup] 13.10 Allocate a 2D Array 分配一个二维数组 - Grandyang - 博客园







# [[CareerCup] 13.10 Allocate a 2D Array 分配一个二维数组](https://www.cnblogs.com/grandyang/p/4934913.html)







13.10 Write a function in C called my2DAlloc which allocates a two-dimensional array. Minimize the number of calls to malloc and make sure that the memory is accessible by the notation arr[i][j].



这道题让我们写个C语言函数my2DAlloc用来给一个二维数组分配内存，并且让我们尽可能的少调用malloc函数。一个二维数组实际是数组的数组，我们用指针来表示数组，用双指针来表示二维数组。我们首先建立一个一维数组，对于每个位置，再建立一个一维数组，这样我们就得到了一个二维数组，参见如下代码：

```
int** my2DAlloc(int rows, int cols) {
    int **rowptr = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; ++i) {
        rowptr[i] = (int*)malloc(cols * sizeof(int));
    }
    return rowptr;
}
```



关于释放内存，我们不能仅仅释放rowptr，我们要确保每个cell中的内存也被释放了，参见如下代码：

```
void my2DDealloc(int **rowptr, int rows) {
    for (int i = 0; i < rows; ++i) {
        free(rowptr[i]);
    }
    free(rowptr);
}
```



其实我们还可以在连续的内存块上来分配内存，例如对于一个5行6列的二维数组，我们可以在开头的五个内存块里存上每一行的起始地址，后面的五行数据是连续排列的，一行接着一行，参见代码如下：



```
class Solution {
public:
    int** my2DAlloc(int rows, int cols) {
        int header = rows * sizeof(int*);
        int data = rows * cols * sizeof(int*);
        int **rowptr = (int**)malloc(header + data);
        if (rowptr == NULL) return NULL;
        int *buf = (int*)(rowptr + rows);
        for (int i = 0; i < rows; ++i) {
            rowptr[i] = buf + i * cols;
        }
        return rowptr;
    }
};
```



这样申请连续的一段内存空间的好处是只需要调用一次malloc就行，而且在释放的时候也不需要特别的写函数来free，好处还是蛮多的。














