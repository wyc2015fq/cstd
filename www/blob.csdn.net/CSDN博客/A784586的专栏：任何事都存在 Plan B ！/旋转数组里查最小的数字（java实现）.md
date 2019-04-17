# 旋转数组里查最小的数字（java实现） - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年01月11日 16:36:30[QuJack](https://me.csdn.net/A784586)阅读数：229标签：[二分查找](https://so.csdn.net/so/search/s.do?q=二分查找&t=blog)
个人分类：[算法](https://blog.csdn.net/A784586/article/category/6556570)












把一个数组最开始的若干个元素搬到数组的末尾，我们称之为数组的旋转。
输入一个非递减排序的数组的一个旋转，输出旋转数组的最小元素。
例如数组{3,4,5,1,2}为{1,2,3,4,5}的一个旋转，该数组的最小值为1。

NOTE：给出的所有元素都大于0，若数组大小为0，请返回0。

输入{1，0,1,1,1}或者{1,1,1,0,1}也能找出最小的0；




思路分析：输入为{3,4,5,1,2}这种类型，前一个递增序列的第一个值肯定大于第二个递增序列的值；利用二分查找就可以锁定第二个序列的第一个值就是所要找的最小的值；利用两个游标（指针）p,q.分别指向自己所在的递增序列；当输入的为{1，0,1,1,1}或者{1,1,1,0,1}类型，array[p]==array[q]
 && array[p]==array[mid]，那么就要结合顺序查找到0.具体过程最容易理解的思路是用测试用例，跟着代码过程走一遍执行过程。代码完整，可执行，可以复制到编译器，断点调试，了解执行过程，或者画图理解执行过程就能很好理解代码思路。

java二分查找+顺序查找实现：

package com.mytest.mymain;

import java.util.ArrayList;

public class OrderFindMin {

    public static int findmin(int array[],int p,int q){

        int result=array[p];

        for(int i=p+1;i<q;i++){

            if(array[i]<result){

                result=array[i];

            }

        }

        return result;

    }


    public static int minNumberInRotateArray(int [] array) {

        if(array.length==0){

            return 0;

        }

        int p=0;

        int q=array.length-1;

        int mid=p;

        while(array[p]>=array[q]){

            mid=(p+q)/2;


            if(p==q-1){

              return array[q];

            }

            if(array[p]==array[q] && array[p]==array[mid]){

                return findmin(array,p,q);       

            }


            if(array[mid]>=array[p]){

                p=mid;

            }else if(array[mid]<=array[p]){

                q=mid;

            }


        }

        return array[mid];


    }

    public static void main(String[] args){
int array01[]={1,0,1,1,1};
int array[]={3,4,5,6,7,8,1,2};
System.out.println(minNumberInRotateArray(array01));

    }



}




