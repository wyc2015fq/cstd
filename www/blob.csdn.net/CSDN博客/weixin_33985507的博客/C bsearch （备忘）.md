# C bsearch  （备忘） - weixin_33985507的博客 - CSDN博客
2010年12月20日 10:25:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
**bsearch**
void*bsearch(const void *key, const void *base, size_t n_elements,
size_t el_size, int (*compare)(const void *, const void *)); 
Example:
```
/*
 * bsearch.c
 *
 * 使用 bsearch 在一个元素类型为结构的数组中查找。
 */
#include <stdlib.h>
#include <string.h>
typedef struct
{
    char    key[10];
    int     other_data;
} Record;
int r_compare(const void *a, const void * b)
{
    return strcmp( ((Record *)a)->key, ((Record *)b)->key );
}
int main(int argc, char *argv[])
{
    Record    array[50];
    Record    key;
    Record    *ans;
    /* ...
     * 用50个元素填充数组并进行排序的代码
     */
    
    strcpy(key.key, "value");
    ans = bsearch(&key, array, 50, sizeof(Record), r_compare);
    /*
     * ans 现在指向关键字字段与值匹配的数据元素，如果无匹配，ans 为 NULL
     */
    
    return 0;
}
```
[http://files.cnblogs.com/bruceleeliya/bsearch.rar](http://files.cnblogs.com/bruceleeliya/bsearch.rar)
