# java排序之冒泡、插入、选择、快速等排序算法 - 零度的博客专栏 - CSDN博客
2016年04月15日 11:21:52[零度anngle](https://me.csdn.net/zmx729618)阅读数：392标签：[冒泡排序																[排序算法](https://so.csdn.net/so/search/s.do?q=排序算法&t=blog)](https://so.csdn.net/so/search/s.do?q=冒泡排序&t=blog)
个人分类：[算法/排序](https://blog.csdn.net/zmx729618/article/category/6185096)
1、冒泡排序
```
package sort.test;
/**
 * 相邻元素比较
 * @author zhangwenchao
 *
 */
public class BubbleSort {
	
	public static void main(String[] args) {
		boolean isChange = true;//记录每次有没有交换值的状态
		int a[]={49,38,65,97,76,13,27,49,78,34,12,64,5,4,62,99,98,54,56,17,18,23,34,15,35,25,53,51};      
        for(int i=0;i<a.length-1;i++){
        	isChange = false; //每次循环之前先修改为：不需要在交换的状态。
            for(int j=0;j<a.length-1-i;j++){    
                if(a[j]>a[j+1]){    
                	int temp=a[j];    
                    a[j]=a[j+1];    
                    a[j+1]=temp; 
                    isChange = true;  //如果有交换发生，需要为true
                }    
            }
            
            if(!isChange){
            	break; //如果一趟下来之后没有发生一次交换操作，说明数组已经有序了，直接跳出循环 
            }
            
        }    
        for(int i=0;i<a.length;i++)    
            System.out.println(a[i]);       
	}
}
```
2、插入排序
```
package sort.test;
/**
 * 从1开始把一个无需数组的依次插入到有序数组中。
 * @author zhangwenchao
 *
 */
public class InsertSort{
	public static void main(String[] agrs) {
		int a[]={49,38,65,97,76,13,27,49,78,34,12,64,5,4,62,99,98,54,56,17,18,23,34,15,35,25,53,51};    
        int temp=0;    
        for(int i=1;i<a.length;i++){    
            temp=a[i];
            int j=i-1;
            for(;j>=0 && temp<a[j];j--){ //J停留在需要插入位置  ，跳出循环。 
                a[j+1]=a[j];//将已经排好顺序的序列中，大于temp的值整体后移一个单位 ，将temp插入到自己应该的位置   
            }    
            a[j+1]=temp;    
        }    
  
        for(int i=0;i<a.length;i++)    
            System.out.println(a[i]);    
	}
	
}
```
3、选择排序
```
package sort.test;
/**
 * 选择一个最小值放到其位置
 * @author zhangwenchao
 *
 */
public class SelectSort {
	
	public static void main(String[] args) {
		
		int a[]={1,54,6,3,78,34,12,45};       
        for(int i=0;i<a.length;i++){            
            for(int j=i+1;j<a.length;j++){    
                if(a[j]<a[i]){    
                    int temp=a[j];
                    a[j]=a[i];
                    a[i]=temp; 
                }    
            }    
   
        } 
        
        for(int i=0;i<a.length;i++)    
            System.out.println(a[i]); 
		
	}
}
```
4、快速排序
```
package sort.test;
import java.util.Random;
public class QuickSort {
	
    public void quickSort(int[] list, int low, int high) {       
        if (low < high) {       
            int middle = getMiddle(list, low, high);  //将list数组进行一分为二       
            quickSort(list, low, middle - 1);         //对低字表进行递归排序       
            quickSort(list, middle + 1, high);        //对高字表进行递归排序       
        }       
    }
    
    public int getMiddle(int[] list, int low, int high) {       
        int tmp = list[low];    //数组的第一个作为中轴       
        while (low < high){       
            if(low < high && list[high] >= tmp) {       
                high--;       
            }       
            list[low] = list[high];   //比中轴小的记录移到低端
            if(low < high && list[low] <= tmp) {       
                low++;       
            }       
            list[high] = list[low];   //比中轴大的记录移到高端       
        }       
        list[low] = tmp;              //中轴记录到尾     
        System.out.println( high+"?="+low);
        return low;                   //返回中轴的位置       
  
    }
    
    /**
     * 选取数组第一个元算作为 中轴数据，对组数进行一份为二，
     * 返回分解之后的中轴数据对应的下标
     * @param args
     */
    public int getMiddleIndex(int[] list, int low, int high) {  
    	int middleDate = list[low];
        while(low <high){
        	
        	while(low <high && list[high]>=middleDate){
        		high--;
        	}
        	list[low] = list[high];
        	
        	while(low <high && list[low] <= middleDate){
        		low++;
        	}
        	list[high] = list[low];
        	
        	
        }
        list[low] = middleDate;
        System.out.println( high+"?="+low);
        return low;
    }
    
    public void quickSort2(int[] list, int low, int high) {   
    	if(low < high){
	    	int middleIndex = getMiddleIndex(list, low, high);
	    	quickSort2(list, low, middleIndex-1);
	    	quickSort2(list, middleIndex+1, high);
    	}  
    }
    
    
	
	
	public static void main(String[] args) {
		
		Random r  = new Random(10);
		int[] list = new int[20];
		for(int i=0;i<20;i++){
			list[i]=Math.abs(r.nextInt(100)) ;
			System.out.print(list[i]+" ");
	
		}
		System.out.println();
		
		
		QuickSort qs = new QuickSort();
		
		
		
       /*if (list.length > 0) {    //查看数组是否为空       
        	qs.quickSort(list, 0, list.length - 1);       
        }*/
        
        
        qs.quickSort2(list, 0, list.length - 1);
		
        for(int i=0;i<list.length;i++){    
			 System.out.print(list[i]+" ");
        }
	}
}
```
5、对象排序
```
package sort.test;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
public class ObjectSort {
	
	static class User {  
		  
	    private String name;    //姓名   
	    private String birthday;//出生日期   
	      
	    public  User(String name, String birthday) {  
	        this.name = name;  
	        this.birthday = birthday;  
	    }  
	  
	    public String getName() {  
	        return name;  
	    }  
	    public void setName(String name) {  
	        this.name = name;  
	    }  
	    public String getBirthday() {  
	        return birthday;  
	    }  
	    public void setBirthday(String birthday) {  
	        this.birthday = birthday;  
	    }  
	}
	
	public static void main(String[] args){
		
        List<User> userList = new ArrayList<User>();  
        
        userList.add(new User("王琦", "2014-03-28 10:00:24"));  
        userList.add(new User("刘明", "2014-06-20 09:10:55"));  
        userList.add(new User("张国龙", "2014-06-12 09:05:40"));  
        userList.add(new User("刘青", "2014-05-23 13:37:02"));  
        userList.add(new User("费楠", "2014-04-22 09:41:46"));  
        userList.add(new User("王国栋", "2014-05-19 15:35:31")); 
        
        Collections.sort(userList, new Comparator<User>(){  
            @Override  
            public int compare(User a, User b) {//将List依照出生日期倒序排序   
                return b.getBirthday().compareTo(a.getBirthday());  
            }  
        }); 
        
        for (User user:userList) {  
            System.err.println("姓名：" + user.getName() + "，出生日期：" + user.getBirthday());  
        } 
		
	}
}
```
