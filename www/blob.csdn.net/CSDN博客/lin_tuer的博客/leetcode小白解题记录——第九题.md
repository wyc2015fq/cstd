
# leetcode小白解题记录——第九题 - lin_tuer的博客 - CSDN博客


2016年09月19日 21:23:33[lin_tuer](https://me.csdn.net/lin_tuer)阅读数：518


给自己：努力就会进步
9. Palindrome Number

Determine whether an integer is a palindrome. Do this without extra space.
[click to show spoilers.](https://leetcode.com/problems/palindrome-number/#)
Some hints:Could negative integers be palindromes? (ie, -1)
If you are thinking of converting the integer to string, note the restriction of using extra space.
You could also try reversing an integer. However, if you have solved the problem "Reverse Integer", you know that the reversed integer might overflow. How would you handle such case?
There is a more generic way of solving this problem.

[Subscribe](https://leetcode.com/subscribe/)to see which companies asked this question

这是第一道我比较独立的解题经历，虽然比较简单，可是能够发现自己已经形成了一套
行之有效的思维方式，感谢诸位前辈的经验贴，还是那句话，努力就会成长。

解题思路：

用取余的方式从个位开始分离整数，我们可以发现，偶数个的时候，会分离至逐渐相等的情况；奇数时，eg121，只能用12=1*10+2的形式来判断是否是回文的，考虑到这些，问题就很好解决啦。还有一种情况是，末尾是0，这样的一定不是回文数，eg,10,100


```java
public class Solution {
    public boolean isPalindrome(int x) {
        //容易忘记考虑的情况 121 10
		//如果x越界，返回false
		if(x>Integer.MAX_VALUE || x<Integer.MIN_VALUE) return false;
		//1. 负数不是
		if(x<0) return false;
		//2. 0-9是
		if(x>=0 && x<=9) return true;
		//3. 取数判定
		int temp=0;
		int flag=1;
		while(x>0){
			int yushu=x%10;
			if(flag==1&&yushu==0) return false; //如果最后一位为0的话，直接返回false
			temp=10*temp+yushu;
			x=x/10;
			if(temp==x || (x*10+yushu)==temp) return true;
			else if(temp>x) return false;
			flag++;
		}
        return false;
    }
}
```
Happy，虽然只是一小步~

