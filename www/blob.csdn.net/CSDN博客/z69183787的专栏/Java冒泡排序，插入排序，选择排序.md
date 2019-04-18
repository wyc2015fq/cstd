# Java冒泡排序，插入排序，选择排序 - z69183787的专栏 - CSDN博客
2012年12月17日 10:15:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：761
冒泡排序：
```java
package com.zyg.test.sort;
//博客：http://blog.csdn.net/m_changgong
public class BubbleSort {
	public static void main(String[] args) {
		int[] array = {10,5,7,12};
		System.out.print("排序前；");
		printArray(array);
		bubbleAscSort(array);
		bubbleDescSort(array);
	}
	
	/**
	 * 冒泡升序排序
	 * @param array
	 */
	public static void bubbleAscSort(int[] array){
		int length = array.length;
		int temp=0;
		for(int i=0;i<length-1;i++){
			for(int j=0;j<length-i-1;j++){
				if(array[j+1]<array[j]){
					temp = array[j];
					array[j] = array[j+1];
					array[j+1] = temp;
				}
			}
		}
		
		System.out.println();
		System.out.print("升序排序后；");
		printArray(array);
	}
	
	/**
	 * 冒泡降序排序
	 * @param array
	 */
	public static void bubbleDescSort(int[] array){
		int length = array.length;
		int temp=0;
		for(int i=0;i<length-1;i++){
			for(int j=0;j<length-i-1;j++){
				if(array[j+1]>array[j]){
					temp = array[j];
					array[j] = array[j+1];
					array[j+1] = temp;
				}
			}
		}
		
		System.out.println();
		System.out.print("降序排序后；");
		printArray(array);
	}
	
	public static void printArray(int[] array){
		int length = array.length;
		for(int i=0;i<length;i++){
			System.out.print(array[i]);
			if(i!=length-1){
				System.out.print(",");
			}
		}
	}
}
```
插入排序（Insertion Sort）的算法描述是一种简单直观的[排序算法](http://zh.wikipedia.org/wiki/%E6%8E%92%E5%BA%8F%E7%AE%97%E6%B3%95)。它的工作原理是通过构建有序序列，对于未排序数据，在已排序序列中从后向前扫描，找到相应位置并插入。插入排序在实现上，通常采用in-place排序（即只需用到O(1)的额外空间的排序），因而在从后向前扫描过程中，需要反复把已排序元素逐步向后挪位，为最新元素提供插入空间
```java
package com.zyg.test.sort;
public class InsertSort {
	public static void main(String[] args) {
		int[] array = {2,3,1,8,6};
		System.out.print("排序前；");
		printArray(array);
		insertAscSort(array);
		insertDescSort(array);
	}
	
	/**
	 * 插入升序排序
	 * @param array
	 */
	public static void insertAscSort(int[] array){
		int length = array.length;
		for(int out = 1;out<length;out++){
			int temp = array[out];
			int in = out;
			while(in>0 && array[in-1]>temp){
				array[in] = array[in-1];
				--in;
			}
			array[in]=temp;
		}
		
		System.out.println();
		System.out.print("升序排序后；");
		printArray(array);
	}
	
	/**
	 * 插入降序排序
	 * @param array
	 */
	public static void insertDescSort(int[] array){
		int length = array.length;
		for(int out = 1;out<length;out++){
			int temp = array[out];
			int in = out;
			while(in>0 && array[in-1]<temp){
				array[in] = array[in-1];
				--in;
			}
			array[in]=temp;
		}
		
		System.out.println();
		System.out.print("升序排序后；");
		printArray(array);
	}
	
	public static void printArray(int[] array){
		int length = array.length;
		for(int i=0;i<length;i++){
			System.out.print(array[i]);
			if(i!=length-1){
				System.out.print(",");
			}
		}
	}
}
```
选择排序(Selection sort)是一种简单直观的[排序算法](http://zh.wikipedia.org/wiki/%E6%8E%92%E5%BA%8F%E7%AE%97%E6%B3%95)。它的工作原理如下。首先在未排序序列中找到最小（大）元素，存放到排序序列的起始位置，然后，再从剩余未排序元素中继续寻找最小（大）元素，然后放到已排序序列的末尾。以此类推，直到所有元素均排序完毕。
```java
package com.zyg.test.sort;
public class SelectSort {
	public static void main(String[] args) {
		int[] array = {23,10,7,12,5,1};
		System.out.print("排序前；");
		printArray(array);
		selectAscSort(array);
		selectDescSort(array);
	}
	
	/**
	 * 选择升序排序
	 * @param array
	 */
	public static void selectAscSort(int[] array){
		int length = array.length;
		int min = array[0];
		int minIdx = 0;
		for(int i=0;i<length-1;i++){
			for(int j=i+1;j<length;j++){
				if(array[j]<min){//将比较符号改为>即是降序排序算法
					min = array[j];
					minIdx = j;
				}
			}
			array[minIdx]= array[i];
			array[i] = min;
			//重置min、minIdx值
			min = array[i+1];
			minIdx = i+1;
		}
		
		System.out.println();
		System.out.print("升序排序后；");
		printArray(array);
	}
	
	/**
	 * 选择降序排序
	 * @param array
	 */
	public static void selectDescSort(int[] array){
		int length = array.length;
		int max = array[0];
		int maxIdx = 0;
		for(int i=0;i<length-1;i++){
			for(int j=i;j<length;j++){
				if(array[j]>max){//将比较符号改为<即是升序排序算法
					max = array[j];
					maxIdx = j;
				}
			}
			array[maxIdx]= array[i];
			array[i] = max;
			//重置max、maxIdx值
			max = array[i+1];
			maxIdx = i+1;
		}
		
		System.out.println();
		System.out.print("降序排序后；");
		printArray(array);
	}
	
	public static void printArray(int[] array){
		int length = array.length;
		for(int i=0;i<length;i++){
			System.out.print(array[i]);
			if(i!=length-1){
				System.out.print(",");
			}
		}
	}
}
```
