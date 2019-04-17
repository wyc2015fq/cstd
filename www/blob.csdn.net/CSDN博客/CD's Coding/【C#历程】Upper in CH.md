# 【C#历程】Upper in CH - CD's Coding - CSDN博客





2014年04月19日 13:41:26[糖果天王](https://me.csdn.net/okcd00)阅读数：718








烦了博主N久的一个题目，写了半天各种问题……

记录下好了：

首字母大写：

JAVA：(3SP)



```java
//Metal-02.03 JAVA  
public class Program {  
    public static String firstLetterToUpper(String str){  
        if(str == "")return "";  
        String temp = "";  
        temp += str.substring(0,1).toUpperCase() + str.substring(1);  
        return temp;  
    }  
  public static String Puzzle(String s) {  
        
    String res = "";  
    String[] arr = s.split(" ");  
    res += firstLetterToUpper(arr[0]);  
    for(int i=1; i<arr.length;i++){  
        res += " " + firstLetterToUpper(arr[i]);  
    }  
    return res;  
  }  
}
```


末字母大写：



C#：(1SP)



```
using System;

public class Program {
    public static string Puzzle(string s) {
	string ans="";
	for(int i=0;i<s.Length;i++)
	{
		if(s[i]==' '&&i!=0) 
		{
			ans=ans.Substring(0,ans.Length-1)
				+ans.Substring(ans.Length-1).ToUpper()
				+s[i];
			continue;
		}
		if(i==(s.Length-1))
		{
			ans+=s.Substring(s.Length-1).ToUpper();
			continue;
		}
		ans+=s[i];
	}
    return ans;
	}
}
```







