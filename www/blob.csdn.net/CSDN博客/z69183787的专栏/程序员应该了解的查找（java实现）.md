# 程序员应该了解的查找（java实现） - z69183787的专栏 - CSDN博客
2016年11月23日 17:09:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：498
上一星期写了关于排序的几篇博文，得到很多道友的支持，在这里非常感谢。
相比较排序来说，今天所说的查找就简单多了，今天我们先来说：
**1，顺序查找**
**2，折半查找**
**一、顺序查找的基本思想：**
从表的一端开始，顺序扫描表，依次将扫描到的结点关键字和给定值（假定为a）相比较，若当前结点关键字与a相等，则查找成功；若扫描结束后，仍未找到关键字等于a的结点，则查找失败。
说白了就是，从头到尾，一个一个地比，找着相同的就成功，找不到就失败。很明显的缺点就是查找效率低。
适用于线性表的顺序存储结构和链式存储结构。
![](https://img-my.csdn.net/uploads/201205/20/1337477221_6028.png)
计算平均查找长度。
例如上表，查找1，需要1次，查找2需要2次，依次往下推，可知查找16需要16次，
可以看出，我们只要将这些查找次数求和（我们初中学的，上底加下底乘以高除以2），然后除以结点数，即为平均查找长度。
设n=节点数
平均查找长度=（n+1）/2
**用[Java](http://lib.csdn.net/base/javaee)实现：**
**[plain]**[view
 plain](http://blog.csdn.net/pzhtpf/article/details/7584252#)[copy](http://blog.csdn.net/pzhtpf/article/details/7584252#)
- import java.util.Scanner;  
- 
- public class SequentialSearch {  
-     int a[]={49,38,65,97,76,13,27,49,78,34,12,64,5,4,62,99,98,54,56,17,18,23,34,15,35,25,53,51};  
-     public SequentialSearch(){  
-         System.out.println("请输入要查询的数字：");  
-         Scanner input=new Scanner(System.in);  
-         int input1=input.nextInt();  
-         for(int i=0;i<a.length;i++){  
-             if(a[i]==input1){  
-                 System.out.println(input1+"的位置为："+i);  
-                 break;  
-             }  
-             if(i==a.length-1)  
-                 System.out.println("No Result!");  
-         }  
-     }   
- }  
**二、二分法查找（折半查找）的基本思想：**
前提：
（1）确定该区间的中点位置：mid=（low+high）/2    
min代表区间中间的结点的位置，low代表区间最左结点位置，high代表区间最右结点位置
（2）将待查a值与结点mid的关键字（下面用R[mid].key）比较，若相等，则查找成功，否则确定新的查找区间：
如果R[mid].key>a，则由表的有序性可知，R[mid].key右侧的值都大于a，所以等于a的关键字如果存在，必然在R[mid].key左边的表中。这时high=mid-1
如果R[mid].key<a,则等于a的关键字如果存在，必然在R[mid].key右边的表中。这时low=mid+1
如果R[mid].key=a，则查找成功。
（3）下一次查找针对新的查找区间，重复步骤（1）和（2）
（4）在查找过程中，low逐步增加，high逐步减少，如果low>high，则查找失败。
![](https://img-my.csdn.net/uploads/201205/20/1337477373_3991.png)
平均查找长度=Log2(n+1)-1
**注：虽然二分法查找的效率高，但是要将表按关键字排序。而排序本身是一种很费时的运算，所以二分法比较适用于顺序存储结构。为保持表的有序性，在顺序结构中插入和删除都必须移动大量的结点。因此，二分查找特别适用于那种一经建立就很少改动而又经常需要查找的线性表。**
**所以说用折半查找时序列必须是有序的！**
**用java实现：**
**[plain]**[view
 plain](http://blog.csdn.net/pzhtpf/article/details/7584252#)[copy](http://blog.csdn.net/pzhtpf/article/details/7584252#)
- <pre class="plain" name="code">import java.util.ArrayList;  
- import java.util.List;  
- 
- public class binarySearch {  
- public binarySearch(){  
-     List<Integer> list=new ArrayList<Integer>();  
-     for(int i=0;i<10000;i+=2){             //往list加入逐渐增大1-10000的所有偶数，作为实验数组，很明显，他是有序的！  
-         list.add(i);                       //这里当然也可用数组  
-     }  
-     int low=0;  
-     int high=list.size();  
-     int key=3334;  
-     while(low<=high){  
-         int mid=(low+high)/2;  
-         if(key==list.get(mid)){  
-             System.out.println("此数值在list中的位置为："+mid);  
-             break;  
-         }  
-     if(key>list.get(mid)){  
-         low=mid+1;          //当小于时，是low指针向后移动，high指针不变  
-     }  
-     if(key<list.get(mid)){  
-         high=mid-1;        //当大于时，是high指针向前移动，low指针不变  
-     }  
- 
-     }  
-     if(low>high){  
-     System.out.println("没有查到结果！");  
-     }  
- }  
- }  
