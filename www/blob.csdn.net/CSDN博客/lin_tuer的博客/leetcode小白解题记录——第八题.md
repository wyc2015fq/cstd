
# leetcode小白解题记录——第八题 - lin_tuer的博客 - CSDN博客


2016年09月19日 21:32:06[lin_tuer](https://me.csdn.net/lin_tuer)阅读数：718


8. String to Integer (atoi)

Implementatoito convert a string to an integer.
Hint:Carefully consider all possible input cases. If you want a challenge, please do not see below and ask yourself what are the possible input cases.
Notes:It is intended for this problem to be specified vaguely (ie, no given input specs). You are responsible to gather all the input requirements up front.
Update (2015-02-10):
The signature of the`C++`function had been updated. If you still see your function signature
 accepts a`const char *`argument, please click the reload buttonto
 reset your code definition.
[spoilers alert... click to show requirements for atoi.](https://leetcode.com/problems/string-to-integer-atoi/#)
[Subscribe](https://leetcode.com/subscribe/)to see which companies asked this question


这一题好好看这些提示，细心处理，就不会有什么问题
```java
public class Solution {
    public int myAtoi(String str) {
        //null or empty string
        String s=str;
		if(s == null || s.length()==0) return 0;
		
		//去空格
		s=s.trim();
		
		//判断正负号
		boolean sign=true;
		int i=0;
		if(s.charAt(0)=='+'){
			i++;
		}else if(s.charAt(0)=='-'){
			sign=false;
			i++;
		}
				
	   //计算真实值
		double tmp=0;
		for(;i<s.length();i++){
			int digit=s.charAt(i)-'0';//将字符转换为数字
			if(digit<0 || digit>9) break;
			if(sign){
				tmp=tmp*10+digit;
				if(tmp > Integer.MAX_VALUE ) return Integer.MAX_VALUE;
			}else{
				tmp=tmp*10-digit;
				if(tmp<Integer.MIN_VALUE) return Integer.MIN_VALUE;
			}
		}
		
		return (int)tmp;
	}
}
```


