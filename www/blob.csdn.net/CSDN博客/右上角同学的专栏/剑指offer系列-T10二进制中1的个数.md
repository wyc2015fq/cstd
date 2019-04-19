# 剑指offer系列-T10二进制中1的个数 - 右上角同学的专栏 - CSDN博客
2016年07月01日 22:31:38[右上角徐](https://me.csdn.net/u011032983)阅读数：148
本次博客的程序让我们再次勾起与二进制的爱恨情仇，二进制数值最基本的位运算：与（&）、或（|）、非（~）,异或（^），左移（<<），算数右移（>>）,逻辑右移（>>>），都能帮我们解决很多问题。
```java
/**
 * @author xhl
 * 二进制中1的个数
 * 题目描述:
 * 输入一个整数，输出该数二进制表示中1的个数。其中负数用补码表示。
 *
 */
public class offerT10 {
	/*方法1：时间复杂度最小，比较的次数少，只操作为1的位。n=n&(n-1)每次去掉最右边的1，去一次count++一次，直到剩下0*/
	int NumberOf1(int n){
		int count=0;
		while(n!=0){
			n=n&(n-1);//每次处理掉最右边的那个1
			count++;
		}
		return count;
	}
	/*方法2：每一位都通过flag&n判断该位是0或者1*/
	int NumberOf12(int n){
		int count=0;
		int flag=1;
		while(flag!=0){
			if((flag&n)!=0)
				count++;
			flag=flag<<1;
		}
		return count;
	}
	/*方法3：书上说了不能n<<1,因为忽略了负数的情况，导致死循环，但Java里的<<<是逻辑右移*/
	int NumberOf13(int n){
		int count=0;
		int flag=1;
		while(n!=0){
			if((flag&n)!=0)
				count++;
			n=n>>>1;
		}
		return count;
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		offerT10 o = new offerT10();
		System.out.println(o.NumberOf13(-1));
	}
}
```
