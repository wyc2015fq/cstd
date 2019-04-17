# Java程序排序之选择排序 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2011年09月01日 15:11:43[boonya](https://me.csdn.net/boonya)阅读数：4009








1、选择排序的基本思想

每一趟在n-i+1（i=1，2，…n-1）个记录中选取关键字最小的记录作为有序序列中第i个记录。我们主要介绍简单选择排序、树型选择排序和堆排序。　

**简单选择排序**的基本思想：第i趟简单选择排序是指通过n-i次关键字的比较，从n-i+1个记录中选出关键字最小的记录，并和第i个记录进行交换。共需进行i-1趟比较，直到所有记录排序完成为止。例如：进行第i趟选择时，从当前候选记录中选出关键字最小的k号记录，并和第i个记录进行交换。图9.5给出了一个简单选择排序示例，说明了前三趟选择后的结果。图中大括号内为当前候选记录，大括号外为当前已经排好序的记录。　

**　　{** 48　62 35 77 55 14 35 98　**}**　↑　↑　i k

　　14　**{** 62 35 77 55 48 35 98　**}**　↑　↑　

　　i k 14　35 **{** 62 77 55 48 35 98　**}**　↑　↑ i k 14　35 35**{**77 55 48 62 98　**}**　↑　↑ i k 选择排序示例　简单选择排序的算法具体描述如下：　

　　void SelectSort(RecordType r[], int length)　/*

　　对记录[数组](http://baike.baidu.com/view/209670.htm)r做简单选择排序，length为数组的长度

　　*/　{　n=length;　for ( i=1 ; i<= n－1; ++i)

　　{　k=i；　for ( j=i+1 ; j<= n ; ++j) if (r[j].key < r[k].key ) k=j；　if ( k!=i) { x= r[i]； r[i]= r[k]； r[k]=x; }　}　} /* SelectSort */

2、       简单选择排序算法分析

在简单选择排序过程中，所需移动记录的次数比较少。最好情况下，即待排序记录初始状态就已经是正序排列了，则不需要移动记录。最坏情况下，即待排序记录初始状态是按逆序排列的，则需要移动记录的次数最多为3（n-1）。简单选择排序过程中需要进行的比较次数与初始状态下待排序的记录序列的排列情况无关。当i=1时，需进行n-1次比较；当i=2时，需进行n-2次比较；依次类推，共需要进行的比较次数是∑ =(n-1)+(n-2)+…+2+1=n(n-1)/2，即进行比较操作的时间复杂度为O(n2)。

选择排序法 是对 定位比较交换法（也就是冒泡排序法） 的一种改进。在讲选择排序法之前我们先来了解一下定位比较交换法。为了便于理解，设有10个数分别存在数组元素a[0]～a[9]中。定位比较交换法是由大到小依次定位a[0]～a[9]中恰当的值（和武林大会中的比武差不多），a[9]中放的自然是最小的数。如定位a[0],先假定a[0]中当前值是最大数，a[0]与后面的元素一一比较，如果a[4]更大，则将a[0]、a[4]交换，a[0]已更新再与后面的a[5]～a[9]比较，如果a[8]还要大，则将a[0]、a[8]交换，a[0]又是新数，再与a[9]比较。一轮比完以后，a[0]就是最大的数了，本次比武的[武状元](http://baike.baidu.com/view/56198.htm)诞生了，接下来从a[1]开始，因为状元要休息了，再来一轮a[1]就是次大的数，也就是榜眼，然后从a[2]开始，比出探花，真成比武大会了，当比到a[8]以后，排序就完成了。

3、java选择排序法代码

　　public static void main(String[] args) {

　　Random random=new Random();

　　int[] pData=new int[10];

　　for(int i=0;i<pData.length;i++){ //随机生成10个排序数

　　Integer a =random.nextInt(100);

　　pData[i]= a; 

　　System.out.print(pData[i]+" ");

　　} 

　　System.out.println();

　　pData=Choose(pData);

　　for(int i=0;i<pData.length;i++){

　　System.out.print(pData[i]+" ");

　　} 

　　System.out.println();

　　} 

　　public static int[] Choose(int[] pData){

　　System.out.println();

　　for (int i = 0; i < pData.length; i++) {

　　for (int j = i; j < pData.length; j++) {

　　if(pData[j]<pData[i]){

　　int a=pData[j]; 

　　pData[j]=pData[i]; 

　　pData[i]=a; 

　　} 

　　} 

　　} 

　　return pData; 

　　} 







