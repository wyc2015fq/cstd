# java实现对一个字符串中的数值进行从小到大的排序 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2016年12月05日 10:34:29[QuJack](https://me.csdn.net/A784586)阅读数：3911








java实现对一个字符串中的数值进行从小到大的排序。

排序前串："20 78 9 -7 88 36 29"

排序后串：" -7 9 20 29 36 78 88"






```java
public class WrapperDemo {
public static void main(String[] args) {
String numString="20 78 9 -7 88 36 29";
System.out.println("排序前：  "+numString);
numString=sortNumString(numString);
System.out.println("排序后：  "+numString);
}

public static String sortNumString(String numString) {
//1.将字符串用空格切割为字符串数组
String str_arr[]=stringtoArray(numString);
//2.将字符串数组转换为整数型
int arr[]=stringtoInt(str_arr);

//3.排序
mysortarr(arr);
//4.整型数组转换为字符串
String newstring=inttostring(arr);
return newstring;
}

private static String inttostring(int[] arr) {
StringBuilder sb=new StringBuilder();

int i=0;
for(;i<arr.length;i++)
  sb.append(arr[i]);
return sb.toString();
}

public static void mysortarr(int []array) {
Arrays.sort(array);
}

public  static int[] stringtoInt(String[] str_arr) {
int arr[]=new int[str_arr.length];
for(int i=0;i< str_arr.length;i++)
arr[i]=Integer.parseInt(str_arr[i]);
return arr;
}



public static String[] stringtoArray(String numString) {
String []arr_str=numString.split(" ");
return arr_str;
}

}
```






