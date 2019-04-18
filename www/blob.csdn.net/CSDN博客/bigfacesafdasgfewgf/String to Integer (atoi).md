# String to Integer (atoi) - bigfacesafdasgfewgf - CSDN博客





2015年01月20日 22:55:50[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：352标签：[C++																[string																[atoi](https://so.csdn.net/so/search/s.do?q=atoi&t=blog)
个人分类：[C/C++																[Algorithm Probelm																[LeetCode](https://blog.csdn.net/puqutogether/article/category/2832605)




**String to Integer (atoi) **




Implement atoi to convert a string to an integer.

Hint: Carefully consider all possible input cases. If you want a challenge, please do not see below and ask yourself what are the possible input cases.

Notes: It is intended for this problem to be specified vaguely (ie, no given input specs). You are responsible to gather all the input requirements up front.

[spoilers alert... click to show requirements for atoi.](https://oj.leetcode.com/problems/string-to-integer-atoi/#)

Requirements for atoi:
The function first discards as many whitespace characters as necessary until the first non-whitespace character is found. Then, starting from this character, takes an optional initial plus or minus sign followed
 by as many numerical digits as possible, and interprets them as a numerical value.

The string can contain additional characters after those that form the integral number, which are ignored and have no effect on the behavior of this function.

If the first sequence of non-whitespace characters in str is not a valid integral number, or if no such sequence exists because either str is empty or it contains only whitespace characters, no conversion is performed.

If no valid conversion could be performed, a zero value is returned. If the correct value is out of the range of representable values, INT_MAX (2147483647) or INT_MIN (-2147483648) is returned.











```cpp
class Solution {
public:
    int atoi(const char *str) {
        int len = strlen(str); 
		//cout<<len<<endl; 
        int i=0; 
        if(*str == '\0'){  // empty
            return 0; 
        }

		while(i<len && str[i] == ' ')  // !((str[i]=='-' && str[i+1]>='0' && str[i]<='9') || (str[i]>='0' && str[i]<='9'))
		{i++;}
		//cout<<"i: "<<i<<endl; 
        if(i==len){  // all are blank
            return 0; 
        }
        else{
            bool flag=true; 
            if(str[i] == '-' && (str[i+1]>='0' && str[i+1]<='9')){  //negative
                flag=false; 
                i++; 
            }
			else if(str[i] == '+' && (str[i+1]>='0' && str[i+1]<='9'))
			{
				flag=true; 
				i++; 
			}
			else if ((str[i]>='0' && str[i]<='9'))
			{
			}
			else 
			{
				return 0; 
			}
            //positive
            string s=""; 
            while(i < len)
			{
                if(str[i]>='0' && str[i]<='9'){
                    s += str[i];  //contecate
                }
				else
				{
					break; 
				}
				i++; 
            }
			//cout<<s<<endl; 
            string max="2147483647"; 
            string min="2147483648"; 

            if(flag && (s.length() > 10 || (s.length() == 10 && s>max)))
			{
                return INT_MAX; 
            }
            if(!flag && (s.length() > 10 || (s.length() == 10 && s>min))){
                return INT_MIN; 
            }
            int result=0; 
            for(i = 0; i < s.length(); i++){
                result = result*10 + s[i]-'0'; 
            }
			if(!flag)
			{
				result = -1*result; 
			}
            return result; 
            
        }
    }
};
```






**题目不难，但是测试案例特别多，情况复杂，我给出几个：**



```cpp
char str[100] = "      -11919730356x";  // "", "  -0012a42"=-12, "+-2"=0, "+1"=1, "      -11919730356x"
```](https://blog.csdn.net/puqutogether/article/category/2812137)](https://blog.csdn.net/puqutogether/article/category/2595123)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=C++&t=blog)




