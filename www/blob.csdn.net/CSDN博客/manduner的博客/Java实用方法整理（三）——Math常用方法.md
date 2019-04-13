
# Java实用方法整理（三）——Math常用方法 - manduner的博客 - CSDN博客


2018年03月26日 23:24:15[Manduner_TJU](https://me.csdn.net/manduner)阅读数：129


```python
/* Math：用于数学运算的类
 * 成员变量：
 * 		public static final double PI;
 * 		public static final double E;
 * 常用成员方法：
 * 	public static int abs(int a)     绝对值   
	public static double ceil(double a)        向上取整 
	public static double floor(double a)       向下取整
	public static int max(int a,int b)         最大值
	public static int min(int a,int b)         最小值 
	public static double pow(double a,double b)     a的b次幂 
	public static double randow()       随机数[0.0,1.0] 
	public static int round(float a)       四舍五入 
	public static double sqrt(double a)       正平方根 
 * 
 * 以上成员方法有的返回值还有其他类型，但是使用方法是一样的		
 */
public class MathDemo {
	
	//需求：产生1到100的随机数
	public static void main(String[] args) {
		int result = 0;
		result = (int) (Math.random()*100) + 1;
		System.out.println("随机数为：" + result);
	}
}
```
生成给定范围内的随机数，要求返回值为int类型
```python
import java.util.Scanner;
/*需求：设计一个方法，可以实现获取任意范围内的随机数
 * 
 * 分析：
 * （1）键盘录入两个数据 a,b 
 * （2）获取(a,b)间的随机数
 * （3）输出随机数
 * 
 */
public class MathDemo2 {
	public static void main(String[] args) {
		Scanner sc = new Scanner(System.in);
		int start = 0;
		int end = 0;
		int result = 0 ;
		System.out.println("请输入开始数据：");
		start = sc.nextInt();
		System.out.println("请输入结束数据：");
		end = sc.nextInt();
		
		for(int i = 0; i < 100;i++) {
			result = getRandom(start,end);
			System.out.println(result);
		}
		
	}
	public static int getRandom(int start, int end) {
		int rd = 0;
		
		//start不能大于end
		/*if(start >= end) {
			System.out.println("开始数据不能大于结束数据");
			return -1;
		}*/
		
		//类似于生成[1,100]之间的随机数
		//生成[start,end]之间的随机数
		rd = (int)(Math.random()*(end - start + 1)) + start; 
		
		
		return rd;
		
	}
}
```


