# Java常见的几种排序算法-插入、选择、冒泡、快排、堆排等 - 零度的博客专栏 - CSDN博客
2017年04月01日 11:44:05[零度anngle](https://me.csdn.net/zmx729618)阅读数：781标签：[算法](https://so.csdn.net/so/search/s.do?q=算法&t=blog)
个人分类：[算法/排序](https://blog.csdn.net/zmx729618/article/category/6185096)
                
本文就是介绍一些常见的排序算法。排序是一个非常常见的应用场景，很多时候，我们需要根据自己需要排序的数据类型，来自定义排序算法，但是，在这里，我们只介绍这些基础排序算法，包括：插入排序、选择排序、冒泡排序、快速排序（重点）、堆排序、归并排序等等。看下图：
![](https://img-my.csdn.net/uploads/201304/23/1366700675_1111.JPG)
给定数组：int data[] = {9,2,7,19,100,97,63,208,55,78}
一、直接插入排序(内部排序、O(n2)、稳定)
原理：从待排序的数中选出一个来，插入到前面的合适位置。
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/64123945#)[copy](http://blog.csdn.net/z69183787/article/details/64123945#)[print](http://blog.csdn.net/z69183787/article/details/64123945#)[?](http://blog.csdn.net/z69183787/article/details/64123945#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/2278162/fork)
- package com.xtfggef.algo.sort;    
- publicclass InsertSort {    
- staticint data[] = { 9, 2, 7, 19, 100, 97, 63, 208, 55, 78 };    
- publicstaticvoid insertSort() {    
- int tmp, j = 0;    
- for (int k = 0; k < data.length; k++) {//-----1-----  
-             tmp = data[k];    
-             j = k - 1;    
- while (j >= 0 && tmp < data[j]) {//-----2-----  
-                 data[j + 1] = data[j];    
-                 j--;    
-             }    
-             data[j + 1] = tmp;//------3-------  
-         }    
-     }    
- publicstaticvoid main(String[] args) {    
-         print();    
-         System.out.println();    
-         insertSort();    
-         System.out.println();    
-         print();    
-     }    
- 
- staticvoid print() {    
- for (int i = 0; i < data.length; i++) {    
-             System.out.print(data[i] + " ");    
-         }    
-     }    
- }    
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.xtfggef.algo.sort;  
public class InsertSort {  
    static int data[] = { 9, 2, 7, 19, 100, 97, 63, 208, 55, 78 };  
    public static void insertSort() {  
        int tmp, j = 0;  
        for (int k = 0; k < data.length; k++) {//-----1-----  
            tmp = data[k];  
            j = k - 1;  
            while (j >= 0 && tmp < data[j]) {//-----2-----  
                data[j + 1] = data[j];  
                j--;  
            }  
            data[j + 1] = tmp;//------3-------  
        }  
    }  
    public static void main(String[] args) {  
        print();  
        System.out.println();  
        insertSort();  
        System.out.println();  
        print();  
    }  
  
    static void print() {  
        for (int i = 0; i < data.length; i++) {  
            System.out.print(data[i] + " ");  
        }  
    }  
}
```
我简单的讲解一下过程：思路上从待排序的数据中选出一个，插入到前面合适的位置，耗时点在插入方面，合适的位置意味着我们需要进行比较找出哪是合适的位置，举个例子：对于9,2,7,19,100,97,63,208,55,78这组数，第一个数9前面没有，不做操作，当第一个数完后，剩下的数就是待排序的数，我们将要从除去9开始的书中选出一个插入到前面合适的位置，拿到2后，放在tmp上，进行注释中的2处的代码，2处的代码就是通过循环找出这个合适的位置，发现比tmp大的数，立即将该数向后移动一位（这样做的目的是：前面需要空出一位来进行插入），最后通过注释3处的代码将数插入。
本排序适合：基本有序的数据
二、选择排序（O(n2)、不稳定）
与直接插入排序正好相反，选择排序是从待排序的数中选出最小的放在已经排好的后面，这个算法选数耗时。
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/64123945#)[copy](http://blog.csdn.net/z69183787/article/details/64123945#)[print](http://blog.csdn.net/z69183787/article/details/64123945#)[?](http://blog.csdn.net/z69183787/article/details/64123945#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/2278162/fork)
- package com.xtfggef.algo.sort;    
- publicclass SelectSort {    
- staticint data[] = { 9, 2, 7, 19, 100, 97, 63, 208, 55, 78 };    
- publicstaticvoid selectSort() {    
- int i, j, k, tmp = 0;    
- for (i = 0; i < data.length - 1; i++) {    
-             k = i;    
- for (j = i + 1; j < data.length; j++)    
- if (data[j] < data[k])    
-                     k = j;    
- if (k != i) {    
-                 tmp = data[i];    
-                 data[i] = data[k];    
-                 data[k] = tmp;    
-             }    
-         }    
-     }    
- publicstaticvoid main(String[] args) {    
-         print();    
-         System.out.println();    
-         selectSort();    
-         System.out.println();    
-         print();    
-     }    
- 
- staticvoid print() {    
- for (int i = 0; i < data.length; i++) {    
-             System.out.print(data[i] + " ");    
-         }    
-     }    
- }    
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.xtfggef.algo.sort;  
public class SelectSort {  
    static int data[] = { 9, 2, 7, 19, 100, 97, 63, 208, 55, 78 };  
    public static void selectSort() {  
        int i, j, k, tmp = 0;  
        for (i = 0; i < data.length - 1; i++) {  
            k = i;  
            for (j = i + 1; j < data.length; j++)  
                if (data[j] < data[k])  
                    k = j;  
            if (k != i) {  
                tmp = data[i];  
                data[i] = data[k];  
                data[k] = tmp;  
            }  
        }  
    }  
    public static void main(String[] args) {  
        print();  
        System.out.println();  
        selectSort();  
        System.out.println();  
        print();  
    }  
  
    static void print() {  
        for (int i = 0; i < data.length; i++) {  
            System.out.print(data[i] + " ");  
        }  
    }  
}
```
通过循环，找出最小的数的下标，赋值于k，即k永远保持待排序数据中最小的数的下标，最后和当前位置i互换数据即可。
三、快速排序（O(nlogn)、不稳定）
快速排序简称快排，是一种比较快的排序，适合基本无序的数据，为什么这么说呢？下面我说下快排的思路：
设置两个指针：i和j，分别指向第一个和最后一个，i像后移动，j向前移动，选第一个数为标准（一般这样做，当然快排的关键就是这个“标准”的选取），从后面开始，找到第一个比标准小的数，互换位置，然后再从前面，找到第一个比标准大的数，互换位置，第一趟的结果就是标准左边的都小于标准，右边的都大于标准（但不一定有序），分成两拨后，继续递归的使用上述方法，最终有序！代码如下：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/64123945#)[copy](http://blog.csdn.net/z69183787/article/details/64123945#)[print](http://blog.csdn.net/z69183787/article/details/64123945#)[?](http://blog.csdn.net/z69183787/article/details/64123945#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/2278162/fork)
- package com.xtfggef.algo.sort;    
- publicclass QuickSortTest {    
- staticclass QuickSort {    
- publicint data[];    
- privateint partition(int array[], int low, int high) {    
- int key = array[low];    
- while (low < high) {    
- while (low < high && array[high] >= key)    
-                     high--;    
-                 array[low] = array[high];    
- while (low < high && array[low] <= key)    
-                     low++;    
-                 array[high] = array[low];    
-             }    
-             array[low] = key;    
- return low;    
-         }    
- 
- publicint[] sort(int low, int high) {    
- if (low < high) {    
- int result = partition(data, low, high);    
-                 sort(low, result - 1);    
-                 sort(result + 1, high);    
-             }    
- return data;    
-         }    
-     }    
- 
- staticvoid print(int data[]) {    
- for (int i = 0; i < data.length; i++) {    
-             System.out.print(data[i] + " ");    
-         }    
-     }    
- 
- publicstaticvoid main(String[] args) {    
- int data[] = { 20, 3, 10, 9, 186, 99, 200, 96, 3000 };    
-         print(data);    
-         System.out.println();    
-         QuickSort qs = new QuickSort();    
-         qs.data = data;    
-         qs.sort(0, data.length - 1);    
-         print(data);    
-     }    
- }    
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.xtfggef.algo.sort;  
public class QuickSortTest {  
    static class QuickSort {  
        public int data[];  
        private int partition(int array[], int low, int high) {  
            int key = array[low];  
            while (low < high) {  
                while (low < high && array[high] >= key)  
                    high--;  
                array[low] = array[high];  
                while (low < high && array[low] <= key)  
                    low++;  
                array[high] = array[low];  
            }  
            array[low] = key;  
            return low;  
        }  
  
        public int[] sort(int low, int high) {  
            if (low < high) {  
                int result = partition(data, low, high);  
                sort(low, result - 1);  
                sort(result + 1, high);  
            }  
            return data;  
        }  
    }  
  
    static void print(int data[]) {  
        for (int i = 0; i < data.length; i++) {  
            System.out.print(data[i] + " ");  
        }  
    }  
  
    public static void main(String[] args) {  
        int data[] = { 20, 3, 10, 9, 186, 99, 200, 96, 3000 };  
        print(data);  
        System.out.println();  
        QuickSort qs = new QuickSort();  
        qs.data = data;  
        qs.sort(0, data.length - 1);  
        print(data);  
    }  
}
```
![](https://img-my.csdn.net/uploads/201304/23/1366700691_2623.jpg)
看看上面的图，基本就明白了。
四、冒泡排序（稳定、基本有序可达O(n)，最坏情况为O(n2)）
冒泡排序是一种很简单，不论是理解还是时间起来都比较容易的一种排序算法，思路简单：小的数一点一点向前起泡，最终有序。
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/64123945#)[copy](http://blog.csdn.net/z69183787/article/details/64123945#)[print](http://blog.csdn.net/z69183787/article/details/64123945#)[?](http://blog.csdn.net/z69183787/article/details/64123945#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/2278162/fork)
- package com.xtfggef.algo.sort;    
- publicclass BubbleSort {    
- staticint data[] = { 9, 2, 7, 19, 100, 97, 63, 208, 55, 78 };    
- publicstaticvoid bubbleSort() {    
- int i, j, tmp = 0;    
- for (i = 0; i < data.length - 1; i++) {    
- for (j = data.length - 1; j > i; j--) {    
- if (data[j] < data[j - 1]) {    
-                     tmp = data[j];    
-                     data[j] = data[j - 1];    
-                     data[j - 1] = tmp;    
-                 }    
-             }    
-         }    
-     }    
- 
- publicstaticvoid main(String[] args) {    
-         print();    
-         System.out.println();    
-         bubbleSort();    
-         System.out.println();    
-         print();    
-     }    
- 
- staticvoid print() {    
- for (int i = 0; i < data.length; i++) {    
-             System.out.print(data[i] + " ");    
-         }    
-     }    
- }    
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.xtfggef.algo.sort;  
public class BubbleSort {  
    static int data[] = { 9, 2, 7, 19, 100, 97, 63, 208, 55, 78 };  
    public static void bubbleSort() {  
        int i, j, tmp = 0;  
        for (i = 0; i < data.length - 1; i++) {  
            for (j = data.length - 1; j > i; j--) {  
                if (data[j] < data[j - 1]) {  
                    tmp = data[j];  
                    data[j] = data[j - 1];  
                    data[j - 1] = tmp;  
                }  
            }  
        }  
    }  
  
    public static void main(String[] args) {  
        print();  
        System.out.println();  
        bubbleSort();  
        System.out.println();  
        print();  
    }  
  
    static void print() {  
        for (int i = 0; i < data.length; i++) {  
            System.out.print(data[i] + " ");  
        }  
    }  
}
```
五、堆排序
我们这里不详细介绍概念，堆的话，大家只要记得堆是一个完全二叉树（什么是完全二叉树，请不懂的读者去查资料），堆排序分为两种堆，大顶堆和小顶堆，大顶堆的意思就是堆顶元素是整个堆中最大的，小顶堆的意思就是堆顶元素是整个堆中最小的，满足：任何一非叶节点的关键字不大于或者不小于其左右孩子节点的关键字。堆排序是一个相对难理解的过程，下面我会较为清楚、详细的讲解一下堆排序。堆排序分为三个过程：
建堆：从一个数组顺序读取元素，建立一个堆（完全二叉树）
初始化：将堆进行调整，使得堆顶为最大（最大堆）或者最小（最小堆）的元素
维护：将堆顶元素出堆后，需要将堆的最后一个节点补充到堆顶，因为这样破坏了堆的秩序，所以需要进行维护。下面我们图示一下：
一般情况，建堆和初始化同步进行，
![](https://img-my.csdn.net/uploads/201304/23/1366700732_8215.png)
![](https://img-my.csdn.net/uploads/201304/23/1366700755_5814.png)
最后为如下所示，即为建堆、初始化成功。
![](https://img-my.csdn.net/uploads/201304/23/1366700769_3567.png)
我们可以观察下这个最大堆，看出堆顶是整个堆中最大的元素，而且除叶子节点外每个节点都大于其子节点。下面的过程就是当我们输出堆顶元素后，对堆进行维护。
![](https://img-my.csdn.net/uploads/201304/23/1366700781_2166.png)
过程是这样：将堆顶元素出堆后，用最后一个元素补充堆顶元素，这样破坏了之前的秩序，需要重新维护堆，在堆顶元素的左右节点中选出较小的和堆顶互换，然后一直递归下去，所以每次出一个元素，需要一次维护，堆排序适合解决topK问题，能将复杂度降到nlogK。下面是代码：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/64123945#)[copy](http://blog.csdn.net/z69183787/article/details/64123945#)[print](http://blog.csdn.net/z69183787/article/details/64123945#)[?](http://blog.csdn.net/z69183787/article/details/64123945#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/2278162/fork)
- package com.xtfggef.algo.sort;    
- publicclass HeapSort {    
- privatestaticint[] sort = newint[] { 1, 0, 10, 20, 3, 5, 6, 4, 9, 8, 12,    
- 17, 34, 11 };    
- publicstaticvoid main(String[] args) {    
-         buildMaxHeapify(sort);    
-         heapSort(sort);    
-         print(sort);    
-     }    
- 
- privatestaticvoid buildMaxHeapify(int[] data) {    
- // 没有子节点的才需要创建最大堆，从最后一个的父节点开始  
- int startIndex = getParentIndex(data.length - 1);    
- // 从尾端开始创建最大堆，每次都是正确的堆  
- for (int i = startIndex; i >= 0; i--) {    
-             maxHeapify(data, data.length, i);    
-         }    
-     }    
- 
- /** 
-      * 创建最大堆 
-      *  
-      * @param data 
-      * @param heapSize 
-      *            需要创建最大堆的大小，一般在sort的时候用到，因为最多值放在末尾，末尾就不再归入最大堆了 
-      * @param index 
-      *            当前需要创建最大堆的位置 
-      */
- privatestaticvoid maxHeapify(int[] data, int heapSize, int index) {    
- // 当前点与左右子节点比较  
- int left = getChildLeftIndex(index);    
- int right = getChildRightIndex(index);    
- 
- int largest = index;    
- if (left < heapSize && data[index] < data[left]) {    
-             largest = left;    
-         }    
- if (right < heapSize && data[largest] < data[right]) {    
-             largest = right;    
-         }    
- // 得到最大值后可能需要交换，如果交换了，其子节点可能就不是最大堆了，需要重新调整  
- if (largest != index) {    
- int temp = data[index];    
-             data[index] = data[largest];    
-             data[largest] = temp;    
-             maxHeapify(data, heapSize, largest);    
-         }    
-     }    
- 
- /** 
-      * 排序，最大值放在末尾，data虽然是最大堆，在排序后就成了递增的 
-      *  
-      * @param data 
-      */
- privatestaticvoid heapSort(int[] data) {    
- // 末尾与头交换，交换后调整最大堆  
- for (int i = data.length - 1; i > 0; i--) {    
- int temp = data[0];    
-             data[0] = data[i];    
-             data[i] = temp;    
-             maxHeapify(data, i, 0);    
-         }    
-     }    
- 
- /** 
-      * 父节点位置 
-      *  
-      * @param current 
-      * @return 
-      */
- privatestaticint getParentIndex(int current) {    
- return (current - 1) >> 1;    
-     }    
- 
- /** 
-      * 左子节点position 注意括号，加法优先级更高 
-      *  
-      * @param current 
-      * @return 
-      */
- privatestaticint getChildLeftIndex(int current) {    
- return (current << 1) + 1;    
-     }    
- 
- /** 
-      * 右子节点position 
-      *  
-      * @param current 
-      * @return 
-      */
- privatestaticint getChildRightIndex(int current) {    
- return (current << 1) + 2;    
-     }    
- 
- privatestaticvoid print(int[] data) {    
- int pre = -2;    
- for (int i = 0; i < data.length; i++) {    
- if (pre < (int) getLog(i + 1)) {    
-                 pre = (int) getLog(i + 1);    
-                 System.out.println();    
-             }    
-             System.out.print(data[i] + " |");    
-         }    
-     }    
- 
- /** 
-      * 以2为底的对数 
-      *  
-      * @param param 
-      * @return 
-      */
- privatestaticdouble getLog(double param) {    
- return Math.log(param) / Math.log(2);    
-     }    
- }    
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.xtfggef.algo.sort;  
public class HeapSort {  
    private static int[] sort = new int[] { 1, 0, 10, 20, 3, 5, 6, 4, 9, 8, 12,  
            17, 34, 11 };  
    public static void main(String[] args) {  
        buildMaxHeapify(sort);  
        heapSort(sort);  
        print(sort);  
    }  
  
    private static void buildMaxHeapify(int[] data) {  
        // 没有子节点的才需要创建最大堆，从最后一个的父节点开始  
        int startIndex = getParentIndex(data.length - 1);  
        // 从尾端开始创建最大堆，每次都是正确的堆  
        for (int i = startIndex; i >= 0; i--) {  
            maxHeapify(data, data.length, i);  
        }  
    }  
  
    /** 
     * 创建最大堆 
     *  
     * @param data 
     * @param heapSize 
     *            需要创建最大堆的大小，一般在sort的时候用到，因为最多值放在末尾，末尾就不再归入最大堆了 
     * @param index 
     *            当前需要创建最大堆的位置 
     */  
    private static void maxHeapify(int[] data, int heapSize, int index) {  
        // 当前点与左右子节点比较  
        int left = getChildLeftIndex(index);  
        int right = getChildRightIndex(index);  
  
        int largest = index;  
        if (left < heapSize && data[index] < data[left]) {  
            largest = left;  
        }  
        if (right < heapSize && data[largest] < data[right]) {  
            largest = right;  
        }  
        // 得到最大值后可能需要交换，如果交换了，其子节点可能就不是最大堆了，需要重新调整  
        if (largest != index) {  
            int temp = data[index];  
            data[index] = data[largest];  
            data[largest] = temp;  
            maxHeapify(data, heapSize, largest);  
        }  
    }  
  
    /** 
     * 排序，最大值放在末尾，data虽然是最大堆，在排序后就成了递增的 
     *  
     * @param data 
     */  
    private static void heapSort(int[] data) {  
        // 末尾与头交换，交换后调整最大堆  
        for (int i = data.length - 1; i > 0; i--) {  
            int temp = data[0];  
            data[0] = data[i];  
            data[i] = temp;  
            maxHeapify(data, i, 0);  
        }  
    }  
  
    /** 
     * 父节点位置 
     *  
     * @param current 
     * @return 
     */  
    private static int getParentIndex(int current) {  
        return (current - 1) >> 1;  
    }  
  
    /** 
     * 左子节点position 注意括号，加法优先级更高 
     *  
     * @param current 
     * @return 
     */  
    private static int getChildLeftIndex(int current) {  
        return (current << 1) + 1;  
    }  
  
    /** 
     * 右子节点position 
     *  
     * @param current 
     * @return 
     */  
    private static int getChildRightIndex(int current) {  
        return (current << 1) + 2;  
    }  
  
    private static void print(int[] data) {  
        int pre = -2;  
        for (int i = 0; i < data.length; i++) {  
            if (pre < (int) getLog(i + 1)) {  
                pre = (int) getLog(i + 1);  
                System.out.println();  
            }  
            System.out.print(data[i] + " |");  
        }  
    }  
  
    /** 
     * 以2为底的对数 
     *  
     * @param param 
     * @return 
     */  
    private static double getLog(double param) {  
        return Math.log(param) / Math.log(2);  
    }  
}
```
慢慢理解一下，还是容易明白的！
六、归并排序
归并排序是建立在归并操作上的一种有效的排序算法。该算法是采用分治法（Divide and Conquer）的一个非常典型的应用。
首先考虑下如何将将二个有序数列合并。这个非常简单，只要从比较二个数列的第一个数，谁小就先取谁，取了后就在对应数列中删除这个数。然后再进行比较，如果有数列为空，那直接将另一个数列的数据依次取出即可。
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/64123945#)[copy](http://blog.csdn.net/z69183787/article/details/64123945#)[print](http://blog.csdn.net/z69183787/article/details/64123945#)[?](http://blog.csdn.net/z69183787/article/details/64123945#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/2278162/fork)
- package com.xtfggef.algo.sort;    
- publicclass SortTest {    
- // 将有序数组a[]和b[]合并到c[]中  
- staticvoid MemeryArray(int a[], int n, int b[], int m, int c[]) {    
- int i, j, k;    
-         i = j = k = 0;    
- while (i < n && j < m) {    
- if (a[i] < b[j])    
-                 c[k++] = a[i++];    
- else
-                 c[k++] = b[j++];    
-         }    
- 
- while (i < n)    
-             c[k++] = a[i++];    
- 
- while (j < m)    
-             c[k++] = b[j++];    
-     }    
- 
- publicstaticvoid main(String[] args) {    
- int a[] = { 2, 7, 8, 10, 299 };    
- int b[] = { 5, 9, 14, 20, 66, 88, 92 };    
- int c[] = newint[a.length + b.length];    
-         MemeryArray(a, 5, b, 7, c);    
-         print(c);    
-     }    
- 
- privatestaticvoid print(int[] c) {    
- for (int i = 0; i < c.length; i++) {    
-             System.out.print(c[i] + " ");    
-         }    
-     }    
- }    
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.xtfggef.algo.sort;  
public class SortTest {  
    // 将有序数组a[]和b[]合并到c[]中  
    static void MemeryArray(int a[], int n, int b[], int m, int c[]) {  
        int i, j, k;  
        i = j = k = 0;  
        while (i < n && j < m) {  
            if (a[i] < b[j])  
                c[k++] = a[i++];  
            else  
                c[k++] = b[j++];  
        }  
  
        while (i < n)  
            c[k++] = a[i++];  
  
        while (j < m)  
            c[k++] = b[j++];  
    }  
      
    public static void main(String[] args) {  
        int a[] = { 2, 7, 8, 10, 299 };  
        int b[] = { 5, 9, 14, 20, 66, 88, 92 };  
        int c[] = new int[a.length + b.length];  
        MemeryArray(a, 5, b, 7, c);  
        print(c);  
    }  
  
    private static void print(int[] c) {  
        for (int i = 0; i < c.length; i++) {  
            System.out.print(c[i] + " ");  
        }  
    }  
}
```
可以看出合并有序数列的效率是比较高的，可以达到O(n)。解决了上面的合并有序数列问题，再来看归并排序，其的基本思路就是将数组分成二组A，B，如果这二组组内的数据都是有序的，那么就可以很方便的将这二组数据进行排序。如何让这二组组内数据有序了？可以将A，B组各自再分成二组。依次类推，当分出来的小组只有一个数据时，可以认为这个小组组内已经达到了有序，然后再合并相邻的二个小组就可以了。这样通过先递归的分解数列，再合并数列就完成了归并排序。下面是归并排序代码：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/64123945#)[copy](http://blog.csdn.net/z69183787/article/details/64123945#)[print](http://blog.csdn.net/z69183787/article/details/64123945#)[?](http://blog.csdn.net/z69183787/article/details/64123945#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/2278162/fork)
- package com.xtfggef.algo.sort;    
- 
- publicclass MergeSort {    
- 
- privatestaticvoid mergeSort(int[] data, int start, int end) {    
- if (end > start) {    
- int pos = (start + end) / 2;    
-             mergeSort(data, start, pos);    
-             mergeSort(data, pos + 1, end);    
-             merge(data, start, pos, end);    
-         }    
-     }    
- 
- privatestaticvoid merge(int[] data, int start, int pos, int end) {    
- int len1 = pos - start + 1;    
- int len2 = end - pos;    
- int A[] = newint[len1 + 1];    
- int B[] = newint[len2 + 1];    
- for (int i = 0; i < len1; i++) {    
-             A[i] = data[i + start - 1];    
-         }    
-         A[len1] = Integer.MAX_VALUE;    
- for (int i = 0; i < len2; i++) {    
-             B[i] = data[i + pos];    
-         }    
-         B[len2] = Integer.MAX_VALUE;    
- int m = 0, n = 0;    
- for (int i = start - 1; i < end; i++) {    
- if (A[m] > B[n]) {    
-                 data[i] = B[n];    
-                 n++;    
-             } else {    
-                 data[i] = A[m];    
-                 m++;    
-             }    
-         }    
-     }    
- 
- privatestaticvoid print(int[] data) {    
- for (int i = 0; i < data.length; i++) {    
-             System.out.print(data[i] + " ");    
-         }    
-     }    
- 
- publicstaticvoid main(String args[]) {    
- int data[] = { 8, 16, 99, 732, 10, 1, 29, 66 };    
-         print(data);    
-         System.out.println();    
-         mergeSort(data, 1, data.length);    
-         print(data);    
-     }    
- }    
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.xtfggef.algo.sort;  
  
public class MergeSort {  
  
    private static void mergeSort(int[] data, int start, int end) {  
        if (end > start) {  
            int pos = (start + end) / 2;  
            mergeSort(data, start, pos);  
            mergeSort(data, pos + 1, end);  
            merge(data, start, pos, end);  
        }  
    }  
  
    private static void merge(int[] data, int start, int pos, int end) {  
        int len1 = pos - start + 1;  
        int len2 = end - pos;  
        int A[] = new int[len1 + 1];  
        int B[] = new int[len2 + 1];  
        for (int i = 0; i < len1; i++) {  
            A[i] = data[i + start - 1];  
        }  
        A[len1] = Integer.MAX_VALUE;  
        for (int i = 0; i < len2; i++) {  
            B[i] = data[i + pos];  
        }  
        B[len2] = Integer.MAX_VALUE;  
        int m = 0, n = 0;  
        for (int i = start - 1; i < end; i++) {  
            if (A[m] > B[n]) {  
                data[i] = B[n];  
                n++;  
            } else {  
                data[i] = A[m];  
                m++;  
            }  
        }  
    }  
  
    private static void print(int[] data) {  
        for (int i = 0; i < data.length; i++) {  
            System.out.print(data[i] + " ");  
        }  
    }  
  
    public static void main(String args[]) {  
        int data[] = { 8, 16, 99, 732, 10, 1, 29, 66 };  
        print(data);  
        System.out.println();  
        mergeSort(data, 1, data.length);  
        print(data);  
    }  
}
```
七、希尔排序（不稳定、O（nlogn））
d1 = n/2，d2 = d1/2 ...
举例一下：{9,8,7,6,5,4,3,2,1,0} 10个数，现分为5组(9,4),(8,3),(7,2),(6,1),(5,0)，然后分别对每组进行直接插入排序得到：
(4,9),(3,8),(2,7),(1,6),(0,5)，再将这5组分为2组（4,3,2,1,0）,（9,8,7,6,5）分别对这两组进行直插排序，得：(0,1,2,3,4),(5,6,7,8,9)最终有序。
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/64123945#)[copy](http://blog.csdn.net/z69183787/article/details/64123945#)[print](http://blog.csdn.net/z69183787/article/details/64123945#)[?](http://blog.csdn.net/z69183787/article/details/64123945#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/2278162/fork)
- package com.xtfggef.algo.sort;    
- publicclass ShellSort {    
- staticvoid shellsort(int[] a, int n) {    
- int i, j, temp;    
- int gap = 0;    
- while (gap <= n) {    
-             gap = gap * 3 + 1;    
-         }    
- while (gap > 0) {    
- for (i = gap; i < n; i++) {    
-                 j = i - gap;    
-                 temp = a[i];    
- while ((j >= 0) && (a[j] > temp)) {    
-                     a[j + gap] = a[j];    
-                     j = j - gap;    
-                 }    
-                 a[j + gap] = temp;    
-             }    
-             gap = (gap - 1) / 3;    
-         }    
-     }    
- 
- staticvoid print(int data[]) {    
- for (int i = 0; i < data.length; i++) {    
-             System.out.print(data[i] + " ");    
-         }    
-     }    
- 
- publicstaticvoid main(String[] args) {    
- int data[] = { 2, 68, 7, 19, 1, 28, 66, 200 };    
-         print(data);    
-         System.out.println();    
-         shellsort(data, data.length);    
-         print(data);    
-     }    
- }    
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.xtfggef.algo.sort;  
public class ShellSort {  
    static void shellsort(int[] a, int n) {  
        int i, j, temp;  
        int gap = 0;  
        while (gap <= n) {  
            gap = gap * 3 + 1;  
        }  
        while (gap > 0) {  
            for (i = gap; i < n; i++) {  
                j = i - gap;  
                temp = a[i];  
                while ((j >= 0) && (a[j] > temp)) {  
                    a[j + gap] = a[j];  
                    j = j - gap;  
                }  
                a[j + gap] = temp;  
            }  
            gap = (gap - 1) / 3;  
        }  
    }  
  
    static void print(int data[]) {  
        for (int i = 0; i < data.length; i++) {  
            System.out.print(data[i] + " ");  
        }  
    }  
  
    public static void main(String[] args) {  
        int data[] = { 2, 68, 7, 19, 1, 28, 66, 200 };  
        print(data);  
        System.out.println();  
        shellsort(data, data.length);  
        print(data);  
    }  
}
```
八、多路快排
JDK1.8中Arrays.sort()采用的排序算法，具有较快的时间复杂度和稳定性，基本思路为：
1. 选取两个中轴P1, P2。
2. 假设P1<P2，否则交换。
3. 过程中原数组会分为四个部分：小于中轴1，大于中轴2，介于两个中轴之间，未排序部分（刚开始除了两个中轴，其它元素都属于这部分）。
4. 开始后，从未排序部分选取一个数，和两个中轴作比较，然后放到合适的位置，一直到未排序部分无数据，结束一趟排序。
5. 递归地处理子数组，稳定排序，时间复杂度稳定为O(nlogn)。
详情可以参见我的另一篇博文《[Java之美[从菜鸟到高手演练]之Arrays类及其方法分析](http://blog.csdn.net/zhangerqing/article/details/42750027)》
九、其他排序
下面的一段转载自博友@[清蒸水皮](http://blog.csdn.net/quietwave/article/details/8008572) --- 补充于2015年1月14日
==============================================
计数排序
当输入的元素是 n 个 0 到 k 之间的整数时，它的运行时间是 O(n + k)。计数排序不是比较排序，排序的速度快于任何比较排序算法。
由于用来计数的数组C的长度取决于待排序数组中数据的范围（等于待排序数组的最大值与最小值的差加上1），这使得计数排序对于数据范围很大的数组，需要大量时间和内存。例如：计数排序是用来排序0到100之间的数字的最好的算法，但是它不适合按字母顺序排序人名。但是，计数排序可以用在基数排序中的算法来排序数据范围很大的数组。
算法的步骤如下：
- 找出待排序的数组中最大和最小的元素
- 统计数组中每个值为i的元素出现的次数，存入数组C的第i项
- 对所有的计数累加（从C中的第一个元素开始，每一项和前一项相加）
- 反向填充目标数组：将每个元素i放在新数组的第C(i)项，每放一个元素就将C(i)减去1
贴上代码：
**[cpp]**[view plain](http://blog.csdn.net/z69183787/article/details/64123945#)[copy](http://blog.csdn.net/z69183787/article/details/64123945#)[print](http://blog.csdn.net/z69183787/article/details/64123945#)[?](http://blog.csdn.net/z69183787/article/details/64123945#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/2278162/fork)
- #include <stdio.h>  
- #include <stdlib.h>  
- #include <time.h>  
- 
- //对于排序的关键字范围，一定是0-99  
- #define NUM_RANGE (100)  
- 
- void print_arr(int *arr, int n)    
- {    
- int i;    
- for(i=0; i<n; i++){    
- if(!i){    
-                        printf("%d", arr[i]);    
-                }else{    
-                        printf(" %d", arr[i]);    
-                }    
-        }    
-        printf("\n");    
- }    
- 
- /* 
- 算法的步骤如下： 
-     1.找出待排序的数组中最大和最小的元素 
-     2.统计数组中每个值为i的元素出现的次数，存入数组C的第i项 
-     3.对所有的计数累加（从C中的第一个元素开始，每一项和前一项相加） 
-     4.反向填充目标数组：将每个元素i放在新数组的第C(i)项，每放一个元素就将C(i)减去1 
- */
- 
- void counting_sort(int *ini_arr, int *sorted_arr, int n)    
- {    
- int *count_arr = (int *)malloc(sizeof(int) * NUM_RANGE);    
- int i, j, k;    
- 
- //统计数组中，每个元素出现的次数  
- for(k=0; k<NUM_RANGE; k++){    
-                count_arr[k] = 0;    
-        }    
- 
- for(i=0; i<n; i++){    
-                count_arr[ini_arr[i]]++;    
-        }    
- 
- 
- for(k=1; k<NUM_RANGE; k++){    
-                count_arr[k] += count_arr[k-1];    
-        }    
- 
- for(j=n-1 ; j>=0; j--){    
- int elem = ini_arr[j];    
- int index = count_arr[elem]-1;    
-            sorted_arr[index] = elem;    
-            count_arr[elem]--;    
-        }    
-        free(count_arr);    
- }    
- 
- 
- int main(int argc, char* argv[])    
- {    
- int n;    
- if(argc < 2){    
-                n = 10;    
-        }else{    
-                n = atoi(argv[1]);    
-        }    
- int i;    
- int *arr = (int *)malloc(sizeof(int) * n);    
- int *sorted_arr = (int *)malloc(sizeof(int) *n);    
-        srand(time(0));    
- 
- 
- for(i=0; i<n; i++){    
-                arr[i] = rand() % NUM_RANGE;    
-        }    
- 
-        printf("ini_array: ");    
-        print_arr(arr, n);    
-        counting_sort(arr, sorted_arr, n);    
-        printf("sorted_array: ");    
-        print_arr(sorted_arr, n);    
-        free(arr);    
-        free(sorted_arr);    
- return 0;    
- }   
![](http://static.blog.csdn.net/images/save_snippets.png)
```cpp
#include <stdio.h>  
#include <stdlib.h>  
#include <time.h>  
  
//对于排序的关键字范围，一定是0-99  
#define NUM_RANGE (100)  
  
void print_arr(int *arr, int n)  
{  
       int i;  
       for(i=0; i<n; i++){  
               if(!i){  
                       printf("%d", arr[i]);  
               }else{  
                       printf(" %d", arr[i]);  
               }  
       }  
       printf("\n");  
}  
  
/* 
算法的步骤如下： 
    1.找出待排序的数组中最大和最小的元素 
    2.统计数组中每个值为i的元素出现的次数，存入数组C的第i项 
    3.对所有的计数累加（从C中的第一个元素开始，每一项和前一项相加） 
    4.反向填充目标数组：将每个元素i放在新数组的第C(i)项，每放一个元素就将C(i)减去1 
*/  
  
void counting_sort(int *ini_arr, int *sorted_arr, int n)  
{  
       int *count_arr = (int *)malloc(sizeof(int) * NUM_RANGE);  
       int i, j, k;  
  
       //统计数组中，每个元素出现的次数  
       for(k=0; k<NUM_RANGE; k++){  
               count_arr[k] = 0;  
       }  
         
       for(i=0; i<n; i++){  
               count_arr[ini_arr[i]]++;  
       }  
  
  
       for(k=1; k<NUM_RANGE; k++){  
               count_arr[k] += count_arr[k-1];  
       }  
  
       for(j=n-1 ; j>=0; j--){  
           int elem = ini_arr[j];  
           int index = count_arr[elem]-1;  
           sorted_arr[index] = elem;  
           count_arr[elem]--;  
       }  
       free(count_arr);  
}  
  
  
int main(int argc, char* argv[])  
{  
       int n;  
       if(argc < 2){  
               n = 10;  
       }else{  
               n = atoi(argv[1]);  
       }  
       int i;  
       int *arr = (int *)malloc(sizeof(int) * n);  
       int *sorted_arr = (int *)malloc(sizeof(int) *n);  
       srand(time(0));  
  
         
       for(i=0; i<n; i++){  
               arr[i] = rand() % NUM_RANGE;  
       }  
  
       printf("ini_array: ");  
       print_arr(arr, n);  
       counting_sort(arr, sorted_arr, n);  
       printf("sorted_array: ");  
       print_arr(sorted_arr, n);  
       free(arr);  
       free(sorted_arr);  
       return 0;  
}
```
桶排序
桶排序的基本思想
假设有一组长度为N的待排关键字序列K[1....n]。首先将这个序列划分成M个的子区间(桶) 。然后基于某种映射函数 ，将待排序列的关键字k映射到第i个桶中(即桶数组B的下标 i) ，那么该关键字k就作为B[i]中的元素(每个桶B[i]都是一组大小为N/M的序列)。接着对每个桶B[i]中的所有元素进行比较排序(可以使用快排)。然后依次枚举输出B[0]....B[M]中的全部内容即是一个有序序列。假如待排序列K= {49、 38 、 35、 97 、 76、 73 、 27、 49 }。这些数据全部在1—100之间。因此我们定制10个桶，然后确定映射函数f(k)=k/10。则第一个关键字49将定位到第4个桶中(49/10=4)。依次将所有关键字全部堆入桶中，并在每个非空的桶中进行快速排序。
桶排序代价分析
桶排序利用函数的映射关系，减少了几乎所有的比较工作。实际上，桶排序的f(k)值的计算，其作用就相当于快排中划分，已经把大量数据分割成了基本有序的数据块(桶)。然后只需要对桶中的少量数据做先进的比较排序即可。
对N个关键字进行桶排序的时间复杂度分为两个部分：
(1) 循环计算每个关键字的桶映射函数，这个时间复杂度是O(N)。
(2) 利用先进的比较排序算法对每个桶内的所有数据进行排序，其时间复杂度为 ∑ O(Ni*logNi) 。其中Ni 为第i个桶的数据量。
很显然，第(2)部分是桶排序性能好坏的决定因素。尽量减少桶内数据的数量是提高效率的唯一办法(因为基于比较排序的最好平均时间复杂度只能达到O(N*logN)了)。因此，我们需要尽量做到下面两点：
(1) 映射函数f(k)能够将N个数据平均的分配到M个桶中，这样每个桶就有[N/M]个数据量。
(2) 尽量的增大桶的数量。极限情况下每个桶只能得到一个数据，这样就完全避开了桶内数据的“比较”排序操作。 当然，做到这一点很不容易，数据量巨大的情况下，f(k)函数会使得桶集合的数量巨大，空间浪费严重。这就是一个时间代价和空间代价的权衡问题了。
对于N个待排数据，M个桶，平均每个桶[N/M]个数据的桶排序平均时间复杂度为：
O(N)+O(M*(N/M)*log(N/M))=O(N+N*(logN-logM))=O(N+N*logN-N*logM)
当N=M时，即极限情况下每个桶只有一个数据时。桶排序的最好效率能够达到O(N)。
总结： 桶排序的平均时间复杂度为线性的O(N+C)，其中C=N*(logN-logM)。如果相对于同样的N，桶数量M越大，其效率越高，最好的时间复杂度达到O(N)。 当然桶排序的空间复杂度 为O(N+M)，如果输入数据非常庞大，而桶的数量也非常多，则空间代价无疑是昂贵的。此外，桶排序是稳定的。
我个人还有一个感受：在查找算法中，基于比较的查找算法最好的时间复杂度也是O(logN)。比如折半查找、平衡二叉树、红黑树等。但是Hash表却有O(C)线性级别的查找效率(不冲突情况下查找效率达到O(1))。大家好好体会一下：Hash表的思想和桶排序是不是有一曲同工之妙呢?
基数排序
上面的问题是多关键字的排序，但单关键字也仍然可以使用这种方式。
比如字符串“abcd” “aesc” "dwsc" "rews"就可以把每个字符看成一个关键字。另外还有整数 425、321、235、432也可以每个位上的数字为一个关键字。
基数排序的思想就是将待排数据中的每组关键字依次进行桶分配。比如下面的待排序列：
278、109、063、930、589、184、505、269、008、083
我们将每个数值的个位，十位，百位分成三个关键字： 278 -> k1(个位)=8 ，k2(十位)=7 ，k3=(百位)=2。
然后从最低位个位开始(从最次关键字开始)，对所有数据的k1关键字进行桶分配(因为，每个数字都是 0-9的，因此桶大小为10)，再依次输出桶中的数据得到下面的序列。
930、063、083、184、505、278、008、109、589、269
再对上面的序列接着进行针对k2的桶分配，输出序列为：
505、008、109、930、063、269、278、083、184、589
最后针对k3的桶分配，输出序列为：
008、063、083、109、184、269、278、505、589、930
性能分析
很明显，基数排序的性能比桶排序要略差。每一次关键字的桶分配都需要O(N)的时间复杂度，而且分配之后得到新的关键字序列又需要O(N)的时间复杂度。假如待排数据可以分为d个关键字，则基数排序的时间复杂度将是O(d*2N) ，当然d要远远小于N，因此基本上还是线性级别的。基数排序的空间复杂度为O(N+M)，其中M为桶的数量。一般来说N>>M，因此额外空间需要大概N个左右。
但是，对比桶排序，基数排序每次需要的桶的数量并不多。而且基数排序几乎不需要任何“比较”操作，而桶排序在桶相对较少的情况下，桶内多个数据必须进行基于比较操作的排序。因此，在实际应用中，基数排序的应用范围更加广泛。
=============================================
![](https://img-my.csdn.net/uploads/201304/23/1366700799_2653.png)
文章最后，给大家推荐一个数据结构学习的网站，有flash演示的：http://www.tyut.edu.cn/kecheng1/site01/index.asp
