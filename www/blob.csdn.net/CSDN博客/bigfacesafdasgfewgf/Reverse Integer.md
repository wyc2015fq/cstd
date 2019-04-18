# Reverse Integer - bigfacesafdasgfewgf - CSDN博客





2015年01月20日 10:14:04[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：522
个人分类：[Algorithm Probelm																[LeetCode](https://blog.csdn.net/puqutogether/article/category/2832605)](https://blog.csdn.net/puqutogether/article/category/2812137)








**Reverse Integer **




Reverse digits of an integer.

Example1: x = 123, return 321
Example2: x = -123, return -321

[click to show spoilers.](https://oj.leetcode.com/problems/reverse-integer/#)

Have you thought about this?
Here are some good questions to ask before coding. Bonus points for you if you have already thought through this!

If the integer's last digit is 0, what should the output be? ie, cases such as 10, 100.

Did you notice that the reversed integer might overflow? Assume the input is a 32-bit integer, then the reverse of 1000000003 overflows. How should you handle such cases?

For the purpose of this problem, assume that your function returns 0 when the reversed integer overflows.

Update (2014-11-10):

Test cases had been added to test the overflow behavior.














```cpp
class Solution {
public:
    int reverse(int x) {
		//cout<<"x: "<<x<<endl; 
        bool flag = true; 
        if(x < 0){
            flag = false; 
            x = -1 * x; 
        }
        string s = ""; 
        int i=0, len=0; 
		if(x == 0){
			s = "0"; 
			len = 1; 
		}
        else{
			while(x > 0){
				len++; 
				s += x%10 + '0';  // obtain the string
				x = x / 10; 
			}
		}
		//cout<<s<<endl;  

        char max1[100] = "2147483647"; 
        char max2[100] = "2147483648"; 
        if(flag && s.length() >= 10 && s > max1){
            return 0; 
        }
        if(!flag && s.length() >= 10 && s > max2){
            return 0; 
        }

		//cout<<"len: "<<len<<endl;  //
        for(i = 0; i < len && s[i]== '0'; i++); 
		//cout<<i<<endl; 
        if(i == len){
            return 0; 
        }
        int result = 0; 
        for(int j = i; j <= len-1; j++){
            result = result * 10 + s[j] - '0'; 
			//cout<<result<<endl; 
        }
        if(!flag){
            result = -1 * result; 
        }
        return result; 
    }
};
```









![](https://img-blog.csdn.net/20150120101345566?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





