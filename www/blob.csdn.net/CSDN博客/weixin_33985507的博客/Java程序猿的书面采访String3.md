# Java程序猿的书面采访String3 - weixin_33985507的博客 - CSDN博客
2015年07月03日 08:49:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4

```java
public class SameString {
    //思想二：每个字符都相应着自己的ASC码，第一个思想的算法复杂度为O（nlogn）。一般能够利用空间来减少时间复杂度
	//能够开辟一个大小为256的数组空间，而且将256个数组元素都置为0，然后遍历第一个字符串把字符的ASC作为数组下标。数组元素都加1，
	//然后遍历第二个字符串将数组元素的各个值都减1，假设最后数组元素的值为0的话说明就是同样的字符串。此算法的时间复杂度为O（n）
	public static void compare(String s1,String s2){
		byte[] b1=s1.getBytes();
		byte[] b2=s2.getBytes();
		int[] bCount=new int[256];
		
		for(int i=0;i<256;i++){
			bCount[i]=0;
		}
		for(int i=0;i<b1.length;i++){
			bCount[b1[i]-'0']++;
		}
		for(int i=0;i<b2.length;i++){
			bCount[b2[i]-'0']--;
		}
		for(int i=0;i<256;i++)
			if(bCount[i]!=0){
				System.out.println("not equal!");
				return;
			}
			   System.out.println("equal!");
			
		
	}
	public static void main (String args[]){
		String s1="aaabbbccc";
		String s2="ababacbcc";
		compare(s1, s2);
		String s3="aabbccdd";
		String s4="abc";
		compare(s3, s4);
		
	}
	
}
```
```java
结果：equal
not equal
```
版权声明：本文博客原创文章。博客，未经同意，不得转载。
