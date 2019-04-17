# 【Linq】C#的学习旅程 _2015编程之美挑战赛初赛 CodeHunt赛区 - CD's Coding - CSDN博客





2015年04月26日 16:42:07[糖果天王](https://me.csdn.net/okcd00)阅读数：1504








![](https://img-blog.csdn.net/20150426160243177?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 序言

2015 编程之美挑战赛·初赛结束了，小伙伴们似乎都在Hihocoder上愉快的玩耍……

只有我一个人默默地打着Code Hunt…… 

啊对了，默默的全题3SP的AK了哟……

来逐题写下题解和解题源码吧…… （P.S 有什么不好的或者说得不对的地方请告诉我哦，我会立马改正的~(●'◡'●)）

《Update Apr.27th Result》

![](https://img-blog.csdn.net/20150427152824593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


完整代码及博文中使用无水印图片可以在这里下载（0积分）：[http://download.csdn.net/detail/okcd00/8634751](http://download.csdn.net/detail/okcd00/8634751)





![](https://img-blog.csdn.net/20150426161011356?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


## Section 00 BAIYUN

### 00.01 Tutorial 教程

点进去会有一个小动画，亲切的告诉你玩法，

新手们可以看看这个哦~~

### 00.02 求最值

Math里有这样一个函数，返回a与b之间的最值：Math.Max(a,b)



```
//00.02
using System;
public class Program {
  public static int Puzzle(int a, int b) {
    return Math.Max(a,b);
  }
}
```



### 00.03 找规律



给了两个数对，仔细观察得知（好吧其实我做过所以有经验了）答案是<a1b2+a2b1, a2b2>


```
//00.03
using System;
public class Program {
    // Each array contains two elements
    public static int[] Puzzle(int[] a, int[] b) {
      int[] t=new int[2];
      t[0]=a[0]*b[1]+a[1]*b[0];
      t[1]=a[1]*b[1];
      return t;
    }
}
```



### 00.04 进制转换（十进制转二进制）

进制转换的话，至少十进制为源数的情况下是很方便的有相应的函数的：

以前的文章里也有提到过：[Here](http://blog.csdn.net/okcd00/article/details/45138293)

以及还写过一个C++的模板：[Here](http://blog.csdn.net/okcd00/article/details/45271067)





```
//00.04
using System;
public class Program {
  public static string Puzzle(int n) {
    string j = Convert.ToString(n, 2);
    return j;
  }
}
```




### 00.05 元素前移



这题是把字符串用空格分隔开，每一个部分作为一个元素，循环左移n位（当然等效于n%元素个数 位），得到的新字符串。

其中，分隔通过Split函数实现，循环左移可以通过 newArr[i]=oldArr[(i+n)/len]来实现

最终把string数组连起来的时候中间还需要加空格，这时需要用到的就是Join(a,b)函数，将数组b中的数连起来，两两之间用a分隔。



```
//00.05
using System;
public class Program {
    public static string Puzzle(string s, int n) 
    {
      string[] sArr=new string[100];
      sArr=s.Split(' '); 
      int len=sArr.Length;
      string[] ss=new string[len];
      for(int i=0;i<len;i++) ss[i]=sArr[(n+i)%len];
      return String.Join(" ",ss);
    }
}
```





![](https://img-blog.csdn.net/20150426164518670?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


## Section 01 BAOBAO

### 01.01 判断元素是否完全相同

为了判断元素是否完全一致，我用了Linq里的集合操作，简单的说，找 （a集合的非交集部分）与（b集合的非交集部分）的并集，如果这个集合没有东西，那就是完全相等咯~



```
//01.01
using System;
using System.Linq;
public class Program {
  public static bool Puzzle(int[] a, int[] b) 
  {
    int[] result=a.Except(b).Concat(b.Except(a)).ToArray();
    return result.Length==0;
  }
}
```




### 01.02 ISBN校验

没错，看到茫茫多的这东西……我第一反应跟大家一样：这是神马玩意!

不过还是非常良心的每个字符串前面都有个”978-“用来提醒大家，也就是说，给的是一个完整的ISBN。

我们需要判断的True/False，其实是：这个是不是一个合格的ISBN码，判断标准是，从前向后看，前12位，奇数位乘以1，偶数位乘以3，结果对10取模，如果校验码第13位数和这个模加起来刚好是10或者0，这个ISBN校验就成立。（不太理解的话下面有例子）

为了检验题目中给出的是不是这个ISBN，还专门写了个C++程序来做判断：


```cpp
#include <iostream>
#include <cstring>
#include <string>
using namespace std;

int n[13]={0};

int main()
{
	string s;
	while(cin>>s && s.length())
	{
		int pos=0,ans=0;
		for(int i=0;i<s.length();i++)
		{
			if(s[i]!='-')
			n[pos++]=s[i]-'0'; 
		} 
		for(int i=0;i<12;i++)
		{
			ans+=n[i]*(i%2?3:1);
		}
		ans%=10;
		ans=10-ans;
		cout<<ans<<endl;
	}
	return 0;
}
```



关于ISBN，详情请见维基百科：[Here](http://zh.wikipedia.org/wiki/%E5%9B%BD%E9%99%85%E6%A0%87%E5%87%86%E4%B9%A6%E5%8F%B7#.E6.A0.A1.E9.A9.97.E7.A2.BC.E7.9A.84.E8.A8.88.E7.AE.97.E6.96.B9.E6.B3.95)

考虑到架梯子困难的同学，还是在这里贴张截图

![](https://img-blog.csdn.net/20150426163240041?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






```
//01.02
using System;
public class Program 
{
    public static bool Puzzle(string s) 
    {
      int ans=0;
      int pl=0;
      char a='0';
      s=s.Replace("-","");
      for(int i=0;i<s.Length-1;i++)
      {
        if(i%2==0) pl=1;
        else pl=3;
        ans+=((int)s[i]-(int)a)*pl;
      }
      ans%=10; if(ans!=0) ans=10-ans;
      return ans==(int)s[s.Length-1]-(int)a;
    }
}
```




### 01.03 进制转换（十进制转四进制）



当发现所有的数字都在0-3之间徘徊的时候大约就能感觉到是四进制数了，进制转换的实现并不困难：



```
//01.03
using System;
public class Program {
    public static string Puzzle(int n) 
    {
      string s="";
      while(true)
      {
        int r=n/4,m=n%4;
        s=m.ToString()+s;
        n=r;
        if(n==0)break;
      }
      return s;
    }
}
```



### 01.04 集合求交集

这里也用到了Linq中的函数，求集合交集，这里需要注意的地方在于直接求交集并不能返回int[]类型，我们需要再加上ToArray()

```
//01.04
using System;
using System.Linq;
public class Program {
    public static int[] Puzzle(int[] a, int[] b) 
    {
        return a.Intersect(b).ToArray();
    }
}
```





至此，解题报告及完整代码说明完毕，若有不明白的或者我有哪里说得不对的，亦或是你有什么更好的算法，请务必在下面评论告诉我o(^▽^)o ~~

我们大家一起努力，一起进步吧~

By okcd00（糖果天王）




## 题目截图

大家可能想看看题目的数据或者检查下是不是真的3SP之类的，每道题的3SP截图如下所示：



### Section 00
![](https://img-blog.csdn.net/20150426163856884?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20150426164014142?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20150426163908865?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20150426163917804?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)








### Section 01

![](https://img-blog.csdn.net/20150426163956976?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20150426164002077?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20150426164120520?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20150426164127681?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





### 可怕的01.02测试数据

![](https://img-blog.csdn.net/20150426164235463?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




