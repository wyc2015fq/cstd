# Java程序排序之插入排序 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2011年09月01日 14:10:36[boonya](https://me.csdn.net/boonya)阅读数：1335








1、插入排序:

有一个已经有序的数据序列，要求在这个已经排好的数据序列中插入一个数，但要求插入后此数据序列仍然有序，这个时候就要用到一种新的排序方法——[插入排序法](http://baike.baidu.com/view/1443814.htm),插入排序的基本操作就是将一个数据插入到已经排好序的有序数据中,从而得到一个新的、个数加一的有序数据，算法适用于少量数据的排序，时间复杂度为O(n^2)。是稳定的排序方法。

2、插入排序算法步骤

　　1.从有序数列和无序数列{a2,a3,…,an}开始进行排序； 

　  2.处理第i个元素时(i=2,3,…,n) , 数列{a1,a2,…,ai-1}是已有序的,而数列{ai,ai+1,…,an}是无序的。用ai与ai-1，a i-2,…,a1进行比较,找出合适的位置将ai插入；

　　3.重复第二步，共进行n-i次插入处理，数列全部有序。 

　　void insertSort(Type* arr,long len)/*InsertSort algorithm*/

　　{ 

　　long i=0,j=0;/*iterator value*/ 

　　Type tmpData; 

　　assertF(arr!=NULL,"In InsertSort sort,arr is NULL\n");

　　for(i=1;i<len;i++) 

　　{ 

　　j=i; 

　　tmpData=*(arr + i); 

　　while(j > 0 && tmpData < arr[j-1])

　　{ 

　　arr[j]=arr[j-1]; 

　　j--; 

　　} 

　　arr[j]=tmpData; 

　　} 

} 

插入排序算法思路

假定这个[数组](http://baike.baidu.com/view/209670.htm)的序是排好的,然后从头往后,如果有数比当前外层元素的值大,则将这个数的位置往后挪,直到当前外层元素的值大于或等于它前面的位置为止.这具算法在排完前k个数之后,可以保证a[1…k]是局部有序的,保证了插入过程的正确性.

3、算法描述

　一般来说，插入排序都采用in-place在数组上实现。具体算法描述如下：

　　1. 从第一个元素开始，该元素可以认为已经被排序 

　　2. 取出下一个元素，在已经排序的元素序列中从后向前扫描 

　　3. 如果该元素（已排序）大于新元素，将该元素移到下一位置 

　　4. 重复步骤3，直到找到已排序的元素小于或者等于新元素的位置 

　　5. 将新元素插入到下一位置中 

　　6. 重复步骤2 

如果比较操作的代价比交换操作大的话，可以采用[二分查找](http://baike.baidu.com/view/610605.htm)法来减少比较操作的数目。该算法可以认为是插入排序的一个变种，称为二分查找排序

4、C语言示例代码

　　示例代码为C语言，输入参数中，需要排序的数组为array[],起始索引为first，终止索引为last。示例代码的函数采用in-place排序，调用完成后，array[]中从first到last处于升序排列。

　　void insertion_sort(int array[], unsigned int first, unsigned int last)

　　{ 

　　int i,j; 

　　int temp; 

　　for (i = first+1; i<=last;i++) 

　　{ 

　　temp = array[i]; 

　　j=i-1; 

　　//与已排序的数逐一比较, 大于temp时, 该数移后 

　　while((j>=first) && (array[j] > temp))

　　{ 

　　array[j+1] = array[j]; 

　　j--; 

　　} 

　　array[j+1] = temp; 

　　} 

　　} 

　　这个更好: 

　　void InsertSort(char array[],unsigned int n)

　　{ 

　　int i,j; 

　　int temp; 

　　for(i=1;i<n;i++) 

　　{ 

　　temp = array[i]*;//store the original sorted array in temp*

*　　for(j=i ; j>0 && temp < array[j-1] ; j--)//compare the new array with temp*

{ 

　　array[j]=array[j-1];//all larger elements are moved one pot to the right

　　} 

　　array[j]=temp; 

　　} 

　　} 

　　这个是c++语言版本的插入排序。为了支持list使用了std::advance()。

　　#include <iterator> 

　　template<typename biIter> 

　　void inser*tion_sort(biIter begin, biIter end)*

*　　{*

　　typedef typename std::iterator_traits<biIter>::value_type value_type;

　　biIter bond = begin; 

　　std::advance(bond, 1); 

　　for(; bond!=end; std::advance(bond, 1)) {

　　value_type key = *bond; 

　　biIter ins = bond; 

　　biIter pre = ins; 

　　std::advance(pre, -1); 

　　while(ins!=begin && *pre>key) { 

　　*ins = *pre; 

　　std::advance(ins, -1); 

　　std::advance(pre, -1); 

　　} 

　　*ins = key; 

　　} 

　　}

5、Java语言示例代码

　　public class InsertSort { 

　　public static void sort(Comparable[] data) {

　　// 数组长度 

　　int len = data.length; 

　　// 从下标为1开始依次插入 

　　for (int i = 1; i < len; i++) { 

　　// 当前待插入的数据 

　　Comparable currentData = data[i];

　　int temp = i; 

　　while (temp > 0 && data[temp - 1].compareTo(currentData) > 0) {

　　// 向右移动 

　　data[temp] = data[temp - 1]; 

　　temp--; 

　　}// end while 

　　data[temp] = currentData; 

　　}// end for 

　　}// end sort 

　　public static void main(String[] args) {

　　// 在JDK1.5版本以上,基本数据类型可以自动装箱 

　　// int,double等基本类型的包装类已实现了Comparable接口

　　Comparable[] c = { 4, 9, 23, 1, 45, 27, 5, 2 };

　　sort(c); 

　　} 

} 

6、算法的时间复杂度

　　如果目标是把n个元素的序列升序排列，那么采用插入排序存在最好情况和最坏情况。最好情况就是，序列已经是升序排列了，在这种情况下，需要进行的比较操作需(n-1)次即可。最坏情况就是，序列是降序排列，那么此时需要进行的比较共有n(n-1)/2次。插入排序的赋值操作是比较操作的次数加上 (n-1)次。平均来说插入排序算法的时间复杂度为O(n^2)。因而，插入排序不适合对于数据量比较大的排序应用。但是，如果需要排序的数据量很小，例如，量级小于千，那么插入排序还是一个不错的选择。



