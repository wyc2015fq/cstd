# 剑指offer系列-二维数组中的查找 - 右上角同学的专栏 - CSDN博客
2016年06月28日 21:01:41[右上角徐](https://me.csdn.net/u011032983)阅读数：456
本人对java语言更熟悉，所以剑指offer代码都是通过Java实现，且涉及的核心代码全部通过牛客网的测试用例检查，感谢牛客网为我检验程序提供了极大帮助！main函数是为了在自己运行程序时，运行结果更直观化。
```java
<pre name="code" class="java">public class offerT3 {
	/**
	 * 二维数组中的查找
	 * 题目描述：
		在一个二维数组中，每一行都按照从左到右递增的顺序排序，
		每一列都按照从上到下递增的顺序排序。请完成一个函数，输
		入这样的一个二维数组和一个整数，判断数组中是否含有该整数。
		输入描述:
		sample： 待查找的二维数组
		fnum：查找的数字
		输出描述:
		查找到返回true，查找不到返回false 
	 */
	public boolean findnum(int[][] sample,int fnum){
		boolean s=false;
		int i=0;
		int j=sample[0].length-1;
		while(i<sample.length&&j>=0){
			if(sample[i][j]>fnum)
				j--;
			else
				if(sample[i][j]<fnum)
					i++;
				else{
					s=true;
					break;
				}
		}
		return s;
			
		}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		int[][] sample={{1,2,8,9},{2,4,9,12},{4,7,10,13},{6,8,11,15}};
		offerT3 t3=new offerT3();
		boolean s=t3.findnum(sample, 7);
		System.out.print(s);
	}
}
```
