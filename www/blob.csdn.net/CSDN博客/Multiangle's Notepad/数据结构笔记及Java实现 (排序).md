
# 数据结构笔记及Java实现 (排序) - Multiangle's Notepad - CSDN博客


2016年04月03日 17:51:44[multiangle](https://me.csdn.net/u014595019)阅读数：959标签：[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)[数据结构																](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=java&t=blog)个人分类：[java																](https://blog.csdn.net/u014595019/article/category/2911533)[算法																](https://blog.csdn.net/u014595019/article/category/6163865)[
							](https://blog.csdn.net/u014595019/article/category/2911533)



---冒泡排序

---索引

---清华数据结构公开课 02E-1

---基本介绍

---冒泡排序可以算是最简单的排序算法。假设有N个数，那么所谓冒泡排序，就是指将数组从第0个元素开始，依次将第n个元素与第n+1个元素比较，如果data[n]>data[n+1] 表示顺序不对（正序排序），需要将data[n]与data[n+1]交换。以此类推，最后循环结束时data[N-1]中的值肯定为这N个数中的最大值。然后再遍历前N-1个数，找出次大值。。以此类推。

---很容易可以算出该算法所耗费的时间，数量级为O(n^2)。

---优化部分

---此外，根据邓俊辉老师的课程，如果在N个数中，只有前K个数是乱序的，而后N-K个数已经有序了，那么实际上只需要O(K^2)即可完成排序。所以在遍历中，我们可以记录下最后一次交换发生的位置，下一次遍历的时候，遍历到最后一次交换的位置即可保证后面的数组全都有序了。

![这里写图片描述](https://img-blog.csdn.net/20160403172802491)
---Java代码

---public

---class

---Sort

---{

---// Bubble Sort

---// the complexity of time is O(n^2)

---public

---static

---<T extends  Comparable<?

---super

---T>>

---void

---bubble_sort( T[] inputArray ){

---int

---low =

---0

---;

---int

---high = inputArray.length-

---1

---;

---while

---( low < high ){

---int

---index

---= low ;

---int

---last_change_index = low ;

---// inner loop

---while

---(

---index

---< high){

---if

---(inputArray[

---index

---].compareTo(inputArray[

---index

---+

---1

---])>

---0

---){
                    T temp = inputArray[

---index

---] ;
                    inputArray[

---index

---] = inputArray[

---index

---+

---1

---] ;
                    inputArray[

---index

---+

---1

---] = temp ;
                    last_change_index =

---index

---;
                }

---index

---++ ;
            }
            System.out.println(last_change_index);
            System.out.println(Arrays.toString(inputArray));
            high = last_change_index ;
        }
    }
}

---归并排序

---索引

---清华数据结构公开课 02F-1 ~ 02F-6

---基本介绍

---归并排序主要分成两部分，前半部分是无需向量的递归分解，后半部分是有序向量的逐层归并。

![这里写图片描述](https://img-blog.csdn.net/20160403174813687)
---大概的伪代码如下所示

---def

---merge_sort

---(data,low,high)

---:

---if

---(high-low<

---2

---):

---\#如果是单元素则返回

---return

---data
    mid = (low+high)/

---2

---\#以中点为界

---left=merge_sort(data[low:mid])

---\#对前半段排序

---right=merge_sort(data[mid:high])

---\#对后半段排序

---return

---merge(left,right)

---\#归并排序结果

---归并时，分别比较两个数组头部元素大小，取走较小的元素，然后被取走元素所在数组的第二个元素成为新的头元素，以此类推

---Java代码

---public

---class

---Sort {

---// Merge Sort

---// the level of time complexity is O(n log n)

---public

---static

---<T extends Comparable<? super T>>

---void

---merge_sort

---( T[] inputArray,

---int

---low,

---int

---high){

---// sort the sequence in [low,high)

---if

---((high - low) >

---1

---){

---int

---mid = (low + high) >>

---1

---;
            merge_sort(inputArray, low, mid);
            merge_sort(inputArray,mid,high);

---// create sub list

---T[] temp = Arrays.copyOfRange(inputArray,low,high) ;

---int

---index = low ;

---int

---left_index =

---0

---;

---int

---left_limit = mid - low ;

---int

---right_index = mid-low ;

---int

---right_limit = high - low ;

---// merge two branches

---while

---((left_index<left_limit)&&(right_index<right_limit)){

---if

---(temp[left_index].compareTo(temp[right_index])<=

---0

---){
                    inputArray[index++] = temp[left_index++] ;
                }

---else

---{
                    inputArray[index++] = temp [right_index++] ;
                }
            }

---// deal the left single branch

---if

---(left_index == left_limit){

---while

---(right_index<right_limit) inputArray[index++] = temp[right_index++] ;
            }

---else

---{

---while

---(left_index<left_limit) inputArray[index++] = temp[left_index++] ;
            }
        }
        System.

---out

---.println(Arrays.toString(inputArray));
    }

---public

---static

---<T extends Comparable<? super T>>

---void

---merge_sort

---(T[] inputArray){
        merge_sort(inputArray,

---0

---,inputArray.length);
    }
}


