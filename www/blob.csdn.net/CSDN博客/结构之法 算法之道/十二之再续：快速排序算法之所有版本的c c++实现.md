# 十二之再续：快速排序算法之所有版本的c/c++实现 - 结构之法 算法之道 - CSDN博客





2011年03月20日 15:52:00[v_JULY_v](https://me.csdn.net/v_JULY_v)阅读数：82452
所属专栏：[经典算法研究](https://blog.csdn.net/column/details/Dijkstra.html)









**十二之再续、快速排序算法所有版本的c/c++实现**


作者：July、二零一一年三月二十日。
出处：[**http://blog.csdn.net/v_JULY_v**](http://blog.csdn.net/v_JULY_v)。
--------------------------------------------------



**前言：**

    相信，经过本人之前写的前俩篇关于快速排序算法的文章：第一篇、[一、快速排序算法](http://blog.csdn.net/v_JULY_v/archive/2011/01/04/6116297.aspx)，及第二篇、[**一之续、快速排序算法的深入分析**](http://blog.csdn.net/v_JULY_v/archive/2011/02/27/6211155.aspx)，各位，已经对快速排序算法有了足够的了解与认识。但仅仅停留在对一个算法的认识层次上，显然是不够的，即便你认识的有多透彻与深入。最好是，编程实现它。
    而网上，快速排序的各种写法层次不清，缺乏统一、整体的阐述与实现，即，没有个一锤定音，如此，我便打算自己去实现它了。

    于是，今花了一个上午，把快速排序算法的各种版本全部都写程序一一实现了一下。包括网上有的，没的，算法导论上的，国内教材上通用的，随机化的，三数取中分割法的，递归的，非递归的，所有版本都用c/c++全部写了个遍。
    鉴于时间仓促下，一个人考虑问题总有不周之处，以及水平有限等等，不正之处，还望各位不吝赐教。不过，以下，所有全部c/c++源码，都经本人一一调试，若有任何问题，恳请指正。

    ok，本文主要分为以下几部分内容：

第一部分、递归版
一、算法导论上的单向扫描版本
二、国内教材双向扫描版
      2.1、Hoare版本
      2.2、Hoare的几个变形版本
三、随机化版本
四、三数取中分割法
第二部分、非递归版

    好的，请一一细看。


**第一部分、快速排序的递归版本一、算法导论上的版本**在我写的第二篇文章中，我们已经知道：
“再到后来，N.Lomuto又提出了一种新的版本，此版本....，即优化了PARTITION程序,它现在写在了 算法导论 一书上”：

快速排序算法的关键是PARTITION过程，它对A[p..r]进行就地重排：

PARTITION(A, p, r)
1  x ← A[r]         //以最后一个元素，A[r]为主元
2  i ← p - 1
3  for j ← p to r - 1    //注，j从p指向的是r-1，不是r。
4       do if A[j] ≤ x
5             then i ← i + 1
6                  exchange A[i] <-> A[j]
7  exchange A[i + 1] <-> A[r]    //最后，交换主元
8  return i + 1

然后，对整个数组进行递归排序：

QUICKSORT(A, p, r)
1 if p < r
2    then q ← PARTITION(A, p, r)   //关键
3         QUICKSORT(A, p, q - 1)
4         QUICKSORT(A, q + 1, r)

    根据上述伪代码，我们不难写出以下的c/c++程序：
首先是，PARTITION过程：

int partition(int data[],int lo,int hi) 
{
 int key=data[hi];  //以最后一个元素，data[hi]为主元
 int i=lo-1;
 for(int j=lo;j<hi;j++)   ///注，j从p指向的是r-1，不是r。
 {
  if(data[j]<=key)
  {
   i=i+1;
   swap(&data[i],&data[j]);
  }
 }
 swap(&data[i+1],&data[hi]);   //不能改为swap(&data[i+1],&key)
 return i+1;
}

补充说明：举个例子，如下为第一趟排序（更多详尽的分析请参考第二篇文章）：
第一趟(4步)：
   a：3   8   7   1   2   5   6   **4**   //以最后一个元素，data[hi]为主元

   b：3   1   7   8   2   5   6   4

   c：3   1   2   8   7   5   6   4

   d：3   1   2   **4**   7   5   6   8    //最后，swap(&data[i+1],&data[hi])

  而其中swap函数的编写，是足够简单的：  

void swap(int *a,int *b)
{
 int temp=*a;
 *a=*b;
 *b=temp;
}

    然后是，调用partition，对整个数组进行递归排序：

void QuickSort(int data[], int lo, int hi)
{
    if (lo<hi)
    {
        int k = partition(data, lo, hi);
        QuickSort(data, lo, k-1);
        QuickSort(data, k+1, hi);
    }
} 

**现在**，我有一个问题要问各位了，保持其它的不变，稍微修改一下上述的partition过程，如下：

int partition(int data[],int lo,int hi)   **//请读者思考**{
 int key=data[hi];  //以最后一个元素，data[hi]为主元
 int i=lo-1;
 for(int j=lo;j<**=**hi;j++)   //现在，我让j从lo指向了hi，不是hi-1。
 {
  if(data[j]<=key)
  {
   i=i+1;
   swap(&data[i],&data[j]);
  }
 }
 //swap(&data[i+1],&data[hi]);   //去掉这行
 return i;                       //返回i，非i+1.
}

    如上，其它的不变，请问，让j扫描到了最后一个元素，后与data[i+1]交换，去掉最后的swap(&data[i+1],&data[hi])，然后，再返回i。请问，如此，是否可行？
其实这个问题，就是我第二篇文章中，所提到的：
    “上述的PARTITION(A, p, r)版本，可不可以改成这样咧?以下这样列”：

PARTITION(A, p, r)   **//请读者思考版本。**1  x ← A[r]
2  i ← p - 1
3  for j ← p to r      //让j 从p指向了最后一个元素r
4       do if A[j] ≤ x
5             then i ← i + 1
6                  exchange A[i] <-> A[j]
//7  exchange A[i + 1] <-> A[r]   去掉此最后的步骤
8  return i      //返回 i，不再返回 i+1.

    望读者思考，后把结果在评论里告知我。

我这里简单论述下：上述请读者思考版本，只是代码做了以下三处修改而已：1、j从 p->r；2、去掉最后的交换步骤；3、返回 i。首先，无论是我的版本，还是算法导论上的原装版本，都是准确无误的，且我都已经编写程序测试通过了。但，其实这俩种写法，思路是完全一致的。

    为什么这么说列?请具体看以下的请读者思考版本，

int partition(int data[],int lo,int hi)   //请读者思考
{
 int key=data[hi];  //以最后一个元素，data[hi]为主元
 int i=lo-1;
 for(int j=lo;j<=hi;j++)   //....
 {
  if(data[j]<=key)           //如果让j从lo指向hi，那么当j指到hi时，是一定会有A[j]<=x的
  {
   i=i+1;
   swap(&data[i],&data[j]);
  }
 }
 //swap(&data[i+1],&data[hi]);   //事实是，应该加上这句，直接交换，即可。
 return i;                       //
}

 我们知道当j最后指到了r之后，是一定会有A[j]<=x的（即=），所以这个if判断就有点多余，没有意义。所以应该如算法导论上的版本那般，最后直接交换swap(&data[i+1],&data[hi]);   即可，返回i+1。**所以，总体说来，算法导论上的版本那样写，比**请读者思考版本**更规范，更合乎情理。**ok，请接着往下阅读。



    当然，上述partition过程中，也可以去掉swap函数的调用，直接写在分割函数里：

int partition(int data[],int lo,int hi)
{
 int i,j,t;
 int key = data[hi];   //还是以最后一个元素作为哨兵，即主元元素
 i = lo-1;
 for (j =lo;j<=hi;j++)
  if(data[j]<key)
  {
   i++;
   t = data[j];
   data[j] = data[i];
   data[i] = t;
  }
  data[hi] = data[i+1];  //先,data[i+1]赋给data[hi]
  data[i+1] = key;       //后，把事先保存的key值，即data[hi]赋给data[i+1]
  //不可调换这俩条语句的顺序。
  return i+1;
}

**提醒：**1、程序中尽量不要有任何多余的代码。
2、你最好绝对清楚的知道，程序的某一步，是该用if，还是该用while，等任何细节的东西。

   ok，以上程序的测试用例，可以简单编写如下：

int main()
{
 int a[8]={3,8,7,1,2,5,6,4};
 QuickSort(a,0,N-1);
 for(int i=0;i<8;i++)
  cout<<a[i]<<endl;
 return 0;
}

    当然，如果，你如果对以上的测试用例不够放心，可以采取1~10000的随机数进行极限测试，保证程序的万无一失（主函数中测试用的随机数例子，即所谓的“极限”测试，下文会给出）。
    至于上述程序是什么结果，相信，不用我再啰嗦。:D。

补充一种写法：

void quickSort(int p, int q)   
{   
 if(p < q)   
 {   
  int x = a[p];    //以第一个元素为主元
  int i = p;   
  for(int j = p+1; j < q; j++)   
  {   
   if(a[j] < x)   
   {   
    i++;   
    int temp = a[i];   
    a[i] = a[j];   
    a[j] = temp;   
   }   
  }   
  int temp = a[p];   
  a[p] = a[i];   
  a[i] = temp;   
  quickSort(p, i);   
  quickSort(i+1, q);   
 }   
}   


**二、国内教材双向扫描版**    国内教材上一般所用的通用版本，是我写的第二篇文章中所提到的霍尔排序或其变形，而非上述所述的算法导论上的版本。而且，现在网上一般的朋友，也是更倾向于采用此种思路来实现快速排序算法。ok，请看：
**2.1、Hoare版本**    那么，什么是霍尔提出的快速排序版本列?如下，即是：

HOARE-PARTITION(A, p, r)
 1  x ← A[p]
 2  i ← p - 1
 3  j ← r + 1
 4  while TRUE
 5      do repeat j ← j - 1
 6           until A[j] ≤ x
 7         repeat i ← i + 1
 8           until A[i] ≥ x
 9         if i < j
10            then exchange A[i] <-> A[j]
11            else return j

     同样，根据以上伪代码，不难写出以下的c/c++代码：

//此处原来的代码有几点错误，后听从了Joshua的建议，现修改如下：
int partition(int data[],int lo,int hi)  //。
{
 int key=data[lo];
 int l=lo-1;
 int h=hi+1;
 for(;;)
 {
  do{
   h--;
  }while(data[h]>key);

  do{
   l++;
  }while(data[l]<key);

  if(l<h)
  {
   swap(data[l],data[h]);
  }
  else
  {
   return h;   
   //各位注意了，这里的返回值是h。不是返回各位习以为常的枢纽元素，即不是l之类的。
  }
 }
} 或者原来的代码修改成这样（已经过测试，有误）：

int partition(int data[],int lo,int hi)  //。
{
 int key=data[lo];
 int l=lo;
 int h=hi;
 for(;;)
 {
  while(data[h]>key)   //不能加 “=”
   h--;
  while(data[l]<key)    //不能加 “=”
   l++;
  if(l<h)
  {
   swap(data[l],data[h]);
  }
  else
  {
   return h;   //各位注意了，这里的返回值是h。不是返回各位习以为常的枢纽元素，即不是l之类的。
  }
 }
}//这个版本，已经证明有误，因为当data[l] == data[h] == key的时候，将会进入死循环，所以淘汰。因此，使用上面的do-while 形式吧。

    读者可以试下，对这个序列进行排序，用上述淘汰版本将立马进入死循环：int data[16]={ 1000, 0, 6, 5, 4, 3, 2, 1, 7, 156, 44, 23, 123, 11, 5 };。

**或者，如朋友颜沙所说：**如果data数组有相同元素就可能陷入死循环，比如：
      2 3 4 5 6 2 
  l->|             |<-h

交换l和h单元后重新又回到：
      2 3 4 5 6 2 
  l->|             |<-h

而第一个程序不存在这种情况，因为它总是在l和h调整后比较，也就是l终究会大于等于h。

.

    相信，你已经看出来了，**上述的第一个程序中partition过程的返回值h并不是枢纽元的位置，但是仍然保证了A[p..j] <= A[j+1...q]。**    这种方法在效率上与以下将要介绍的Hoare的几个变形版本差别甚微，只不过是上述代码相对更为紧凑点而已。

**       2.2、Hoare的几个变形版本**    ok，可能，你对上述的最初的霍尔排序partition过程，理解比较费力，没关系，我再写几种变形，相信，你立马就能了解此双向扫描是怎么一回事了。

int partition(int data[],int lo,int hi)  //双向扫描。
{
 int key=data[lo];   //以第一个元素为主元
 int l=lo;
 int h=hi;
 while(l<h)
 {
  while(key<=data[h] && l<h) 
   h--;
  data[l]=data[h];
  while(data[l]<=key && l<h) 
   l++;
  data[h]=data[l];
 }
 data[l]=key;  //1.key。只有出现要赋值的情况，才事先保存好第一个元素的值。
 return l;     **//**这里和以下所有的Hoare的变形版本都是返回的是枢纽元素，即主元元素l。
}

补充说明：同样，还是举上述那个例子，如下为第一趟排序（更多详尽的分析请参考第二篇文章）：
第一趟(五步曲)：
   a：**3**   8   7   1   2   5   6   4   //以第一个元素为主元
        2   8   7   1       5   6   4
   b：2       7   1   8   5   6   4
   c：2   1   7       8   5   6   4
   d：2   1       7   8   5   6   4
   e：2   1   **3**   7   8   5   6   4   //最后补上，关键字3

然后，对整个数组进行递归排序：

void QuickSort(int data[], int lo, int hi)
{
    if (lo<hi)
    {
        int k = partition(data, lo, hi);
        QuickSort(data, lo, k-1);
        QuickSort(data, k+1, hi);
    }
}

当然，你也可以这么写，把递归过程写在同一个排序过程里：

void QuickSort(int data[],int lo,int hi)
{
   int i,j,temp;
   temp=data[lo];    //还是以第一个元素为主元。
   i=lo;
   j=hi;
   if(lo>hi)
      return;
   while(i!=j)
   {
      while(data[j]>=temp && j>i)
         j--;
      if(j>i)
         data[i++]=data[j];
      while(data[i]<=temp && j>i)
         i++;
      if(j>i)
   data[j--]=data[i];     
   }
   data[i]=temp;                       //2.temp。同上，返回的是枢纽元素，即主元元素。
   QuickSort(data,lo,i-1);  //递归左边
   QuickSort(data,i+1,hi);  //递归右边
}

  或者，如下：void quicksort (int[] a, int lo, int hi)
{
//  lo is the lower index, hi is the upper index
//  of the region of array a that is to be sorted
    int i=lo, j=hi, h;

    // comparison element x
    int x=a[(lo+hi)/2];

    //  partition
    do
    {    
        while (a[i]<x) i++; 
        while (a[j]>x) j--;
        if (i<=j)
        {
            h=a[i]; a[i]=a[j]; a[j]=h;
            i++; j--;
        }
    } while (i<=j);

    //  recursion
    if (lo<j) quicksort(a, lo, j);
    if (i<hi) quicksort(a, i, hi);
}

**另，**本人在一本国内的数据结构教材上（注，此处非指**严**那本），看到的一种写法，发现如下问题：一、冗余繁杂，二、错误之处无所不在，除了会犯一些注释上的错误，一些最基本的代码，都会弄错。详情，如下：

void QuickSort(int data[],int lo,int hi)
{
 int i,j,key;
 if(lo<hi)
 {
  i=lo;
  j=hi;
  key=data[lo]; 
  //已经测试：原教材上，原句为“data[0]=data[lo];”，有误。
  //因为只能用一个临时变量key保存着主元，data[lo]，而若为以上，则相当于覆盖原元素data[0]的值了。
        do
        {
   while(data[j]>=key&&i<j) 
    j--;        
   if(i<j)                             
   { 
    data[i]=data[j];
    //i++;  这是教材上的语句，为使代码简洁，我特意去掉。
   }           
   while(data[i]<=key&&i<j)
    i++;     
   if(i<j)                       
   { 
    data[j]=data[i];
    //j--;    这是教材上的语句，为使代码简洁，我特意去掉。
   }              
        }while(i!=j);
        data[i]=key;        //3.key。
  //已经测试：原教材上，原句为“data[i]=data[0];”，有误。
        QuickSort(data,lo,i-1);     //对标准值左半部递归调用本函数
        QuickSort(data,i+1,hi);    //对标准值右半部递归调用本函数
 }
}

    然后，你能很轻易的看到，这个写法，与上是同一写法，之所以写出来，是希望各位慎看国内的教材，多多质疑+思考，勿轻信。

    ok，再给出一种取中间元素为主元的实现：

void QuickSort(int data[],int lo,int hi)
{ 
 int pivot,l,r,temp;
 l = lo; 
 r = hi; 
 pivot=data[(lo+hi)/2]; //取中位值作为分界值 
 while(l<r)  
 {  
  while(data[l]<pivot) 
   ++l;  
  while(data[r]>pivot) 
   --r;     
  if(l>=r) 
   break;  
  temp = data[l];  
  data[l] = data[r];  
  data[r] = temp;  
  ++l;  
  --r;  
 } 
 if(l==r) 
  l++; 
 if(lo<r) 
  QuickSort(data,lo,l-1); 
 if(l<hi) 
  QuickSort(data,r+1,hi); 
}

或者，这样写：

 void quickSort(int arr[], int left, int right) 
{
 int i = left, j = right;
 int tmp;
 int pivot = arr[(left + right) / 2];   //取中间元素为主元

 /* partition */
 while (i <= j) 
 {
  while (arr[i] < pivot)
   i++;
  while (arr[j] > pivot)
   j--;
  if (i <= j)
  {
   tmp = arr[i];
   arr[i] = arr[j];
   arr[j] = tmp;
   i++;
   j--;
  }
 }
}

上述演示过程，如下图所示（取中间元素为主元，第一趟排序）：![](http://hi.csdn.net/attachment/201104/2/8394323_13017037916Buw.jpg)




**三、快速排序的随机化版本**    以下是完整测试程序，由于给的注释够详尽了，就再做多余的解释了：

//交换两个元素值，咱们换一种方式，采取引用“&” 
void swap(int& a , int& b)
{
 int temp = a;
 a = b;
 b = temp;
}

//返回属于[lo,hi)的随机整数 
int rand(int lo,int hi)
{
 int size = hi-lo+1;
 return  lo+ rand()%size; 
}

//分割，换一种方式，采取指针a指向数组中第一个元素 
int RandPartition(int* data, int lo , int hi)
{    
//普通的分割方法和随机化分割方法的区别就在于下面三行 
 swap(data[rand(lo,hi)], data[lo]);
    int key = data[lo];
 int i = lo;

    for(int j=lo+1; j<=hi; j++)
 {
  if(data[j]<=key)
  {
   i = i+1;
   swap(data[i], data[j]);
  }            
 } 
 swap(data[i],data[lo]);
 return i;
}

//逐步分割排序 
void RandQuickSortMid(int* data, int lo, int hi)
{
 if(lo<hi)
 {
  int k = RandPartition(data,lo,hi);
  RandQuickSortMid(data,lo,k-1);
  RandQuickSortMid(data,k+1,hi);
 }
}
int main()
{
 const int N = 100; //此就是上文说所的“极限”测试。为了保证程序的准确无误，你也可以让N=10000。
 int *data = new int[N];      
    for(int i =0; i<N; i++)
  data[i] = rand();   //同样，随机化的版本，采取随机输入。 
    for(i=0; i<N; i++)
  cout<<data[i]<<" ";
    RandQuickSortMid(data,0,N-1);
 cout<<endl;
 for(i=0; i<N; i++)
  cout<<data[i]<<" ";
 cout<<endl;
    return 0;
}


**四、三数取中分割法**

    我想，如果你爱思考，可能你已经在想一个问题了，那就是，像上面的程序版本，其中算法导论上采取单向扫描中，是以最后一个元素为枢纽元素，即主元，而在Hoare版本及其几个变形中，都是以第一个元素、或中间元素为主元，最后，上述给的快速排序算法的随机化版本，则是以序列中任一一个元素作为主元。
    那么，枢纽元素的选取，即主元元素的选取是否决定快速排序最终的效率列?

    答案是肯定的，当我们采取data[lo],data[mid],data[hi]三者之中的那个第二大的元素为主元时，便能尽最大限度保证快速排序算法不会出现O（N^2）的最坏情况。这就是所谓的三数取中分割方法。当然，针对的还是那个Partition过程。

    ok，直接写代码：

//三数取中分割方法 
int RandPartition(int* a, int p , int q)
{    
 //三数取中方法的关键就在于下述六行，
 int m=(p+q)/2;
 if(a[p]<a[m])
  swap(a[p],a[m]);
 if(a[q]<a[m])
  swap(a[q],a[m]);
 if(a[q]<a[p])
  swap(a[q],a[p]);
 int key = a[p];
 int i = p;

 for(int j = p+1; j <= q; j++)
 {
  if(a[j] <= key)
  {
   i = i+1;
   if(i != j) 
    swap(a[i], a[j]);                 
  }            
 } 

 swap(a[i],a[p]);  
 return i;
}

void QuickSort(int data[], int lo, int hi)
{
    if (lo<hi)
    {
        int k = RandPartition(data, lo, hi);
        QuickSort(data, lo, k-1);
        QuickSort(data, k+1, hi);
    }
}

    经过测试，这种方法可行且有效，不过到底其性能、效率有多好，还有待日后进一步的测试。


**第二部分、快速排序的非递归版**    ok，相信，您已经看到，上述所有的快速排序算法，都是递归版本的，那还有什么办法可以实现此快速排序算法列?对了，递归，与之相对的，就是非递归了。
    以下，就是快速排序算法的非递归实现：

  template <class T>
int RandPartition(T data[],int lo,int hi)
{
 T v=data[lo];
 while(lo<hi)
 {  
  while(lo<hi && data[hi]>=v) 
   hi--;
  data[lo]=data[hi];
  while(lo<hi && data[lo]<=v) 
   lo++;
  data[hi]=data[lo];
 }
 data[lo]=v;
 return lo; 
}

**//**快速排序的非递归算法
template <class T>
void QuickSort(T data[],int lo,int hi)
{
 stack<int> st;
 int key;
 do{
  while(lo<hi)
  {
   key=partition(data,lo,hi);   
//递归的本质是什么?对了，就是借助栈，进栈，出栈来实现的。
   if( (key-lo)<(key-hi) )
   {
    st.push(key+1);    
    st.push(hi);
    hi=key-1;
   }
   else
   {
    st.push(lo);
    st.push(key-1);
    lo=key+1;
   }   
  }
  if(st.empty()) 
   return;
  hi=st.top();
  st.pop();  
  lo=st.top();
  st.pop();  
 }while(1);
}

void QuickSort(int data[], int lo, int hi)
{
    if (lo<hi)
    {
        int k = RandPartition(data, lo, hi);
        QuickSort(data, lo, k-1);
        QuickSort(data, k+1, hi);
    }
}

    如果你还尚不知道快速排序算法的原理与算法思想，请参考本人写的关于快速排序算法的前俩篇文章：[**一之续、快速排序算法的深入分析**](http://blog.csdn.net/v_JULY_v/archive/2011/02/27/6211155.aspx)，及[一、快速排序算法](http://blog.csdn.net/v_JULY_v/archive/2011/01/04/6116297.aspx)。如果您看完了此篇文章后，还是不知如何从头实现快速排序算法，那么好吧，伸出手指，数数，1,2,3,4,5....数到100之后，再来看此文。

    -------------------------------------------------------------
    据本文评论里头网友ybt631的建议，表示非常感谢，并补充阐述下所谓的**并行快速排序**：

    Intel Threading Building Blocks(简称TBB)是一个C++的并行编程模板库，它能使你的程序充分利用多核CPU的性能优势，方便使用，效率很高。
    以下是，**parallel_sort.h**头文件中的关键代码：00039 template<typename RandomAccessIterator, typename Compare>
00040 class quick_sort_range: private no_assign {
00041 
00042     inline size_t median_of_three(const RandomAccessIterator &array, size_t l, size_t m, size_t r) const {
00043         return comp(array[l], array[m]) ? ( comp(array[m], array[r]) ? m : ( comp( array[l], array[r]) ? r : l ) ) 
00044                                         : ( comp(array[r], array[m]) ? m : ( comp( array[r], array[l] ) ? r : l ) );
00045     }
00046 
00047     inline size_t pseudo_median_of_nine( const RandomAccessIterator &array, const quick_sort_range &range ) const {
00048         size_t offset = range.size/8u;
00049         return median_of_three(array, 
00050                                median_of_three(array, 0, offset, offset*2),
00051                                median_of_three(array, offset*3, offset*4, offset*5),
00052                                median_of_three(array, offset*6, offset*7, range.size - 1) );
00053 
00054     }
00055 
00056 public:
00057 
00058     static const size_t grainsize = 500;
00059     const Compare ∁
00060     RandomAccessIterator begin;
00061     size_t size;
00062 
00063     quick_sort_range( RandomAccessIterator begin_, size_t size_, const Compare &comp_ ) :
00064         comp(comp_), begin(begin_), size(size_) {}
00065 
00066     bool empty() const {return size==0;}
00067     bool is_divisible() const {return size>=grainsize;}
00068 
00069     quick_sort_range( quick_sort_range& range, split ) : comp(range.comp) {
00070         RandomAccessIterator array = range.begin;
00071         RandomAccessIterator key0 = range.begin; 
00072         size_t m = pseudo_median_of_nine(array, range);
00073         if (m) std::swap ( array[0], array[m] );
00074 
00075         size_t i=0;
00076         size_t j=range.size;
00077         // Partition interval [i+1,j-1] with key *key0.
00078         for(;;) {
00079             __TBB_ASSERT( i<j, NULL );
00080             // Loop must terminate since array[l]==*key0.
00081             do {
00082                 --j;
00083                 __TBB_ASSERT( i<=j, "bad ordering relation?" );
00084             } while( comp( *key0, array[j] ));
00085             do {
00086                 __TBB_ASSERT( i<=j, NULL );
00087                 if( i==j ) goto partition;
00088                 ++i;
00089             } while( comp( array[i],*key0 ));
00090             if( i==j ) goto partition;
00091             std::swap( array[i], array[j] );
00092         }
00093 partition:
00094         // Put the partition key were it belongs
00095         std::swap( array[j], *key0 );
00096         // array[l..j) is less or equal to key.
00097         // array(j..r) is greater or equal to key.
00098         // array[j] is equal to key
00099         i=j+1;
00100         begin = array+i;
00101         size = range.size-i;
00102         range.size = j;
00103     }
00104 };
00105 
....
00218 #endif

 再贴一下插入排序、快速排序之其中的俩种版本、及插入排序与快速排序结合运用的实现代码，如下：

** /// 插入排序,最坏情况下为O(n^2)template< typename InPos, typename ValueType >void _isort( InPos posBegin_, InPos posEnd_, ValueType* ){/*****************************************************************************    伪代码如下：*        for i = [1, n)*            t = x*            for( j = i; j > 0 && x[j-1] > t; j-- )*                x[j] = x[j-1]*            x[j] = x[j-1] ****************************************************************************/ if( posBegin_ == posEnd_ ) {  return; }  /// 循环迭代，将每个元素插入到合适的位置 for( InPos pos = posBegin_; pos != posEnd_; ++pos ) {  ValueType Val = *pos;  InPos posPrev = pos;  InPos pos2 = pos;  /// 当元素比前一个元素大时，交换  for( ;pos2 != posBegin_ && *(--posPrev) > Val ; --pos2 )  {   *pos2 = *posPrev;  }  *pos2 = Val; }}/// 快速排序1，平均情况下需要O(nlogn)的时间template< typename InPos >inline void qsort1( InPos posBegin_, InPos posEnd_ ){/*****************************************************************************    伪代码如下：*        void qsort(l, n)*            if(l >= u)*                return;*            m = l*            for i = [l+1, u]*                if( x < x[l]*                    swap(++m, i)*            swap(l, m)*            qsort(l, m-1)*            qsort(m+1, u) ****************************************************************************/ if( posBegin_ == posEnd_ ) {  return; }  /// 将比第一个元素小的元素移至前半部 InPos pos = posBegin_; InPos posLess = posBegin_; for( ++pos; pos != posEnd_; ++pos ) {  if( *pos < *posBegin_ )  {   swap( *pos, *(++posLess) );  } }  /// 把第一个元素插到两快元素中央 swap( *posBegin_, *(posLess) );  /// 对前半部、后半部执行快速排序 qsort1(posBegin_, posLess); qsort1(++posLess, posEnd_);};/// 快速排序2,原理与1基本相同，通过两端同时迭代加快平均速度template<typename InPos>void qsort2( InPos posBegin_, InPos posEnd_ ){ if( distance(posBegin_, posEnd_) <= 0 ) {  return; }  InPos posL = posBegin_; InPos posR = posEnd_;  while( true ) {  /// 找到不小于第一个元素的数  do  {   ++posL;  }while( *posL < *posBegin_ && posL != posEnd_ );    /// 找到不大于第一个元素的数  do   {   --posR;  } while ( *posR > *posBegin_ );    /// 两个区域交叉时跳出循环  if( distance(posL, posR) <= 0 )  {   break;  }  /// 交换找到的元素  swap(*posL, *posR); }  /// 将第一个元素换到合适的位置 swap(*posBegin_, *posR); /// 对前半部、后半部执行快速排序2 qsort2(posBegin_, posR); qsort2(++posR, posEnd_);}/// 当元素个数小与g_iSortMax时使用插入排序，g_iSortMax是根据STL库选取的const int g_iSortMax = 32;/// 该排序算法是快速排序与插入排序的结合template<typename InPos>void qsort3( InPos posBegin_, InPos posEnd_ ){ if( distance(posBegin_, posEnd_) <= 0 ) {  return; }  /// 小与g_iSortMax时使用插入排序 if( distance(posBegin_, posEnd_) <= g_iSortMax ) {  return isort(posBegin_, posEnd_); }  /// 大与g_iSortMax时使用快速排序 InPos posL = posBegin_; InPos posR = posEnd_;  while( true ) {  do  {   ++posL;  }while( *posL < *posBegin_ && posL != posEnd_ );    do   {   --posR;  } while ( *posR > *posBegin_ );    if( distance(posL, posR) <= 0 )  {   break;  }  swap(*posL, *posR); } swap(*posBegin_, *posR); qsort3(posBegin_, posR); qsort3(++posR, posEnd_);}**





**版权所有。转载本BLOG内任何文章，请以超链接形式注明出处。否则，一经发现，必定永久谴责+追究法律责任。谢谢，各位。**





