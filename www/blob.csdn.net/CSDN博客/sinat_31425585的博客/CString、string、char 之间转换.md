# CString、string、char*之间转换 - sinat_31425585的博客 - CSDN博客
2016年08月25日 11:46:44[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：269
                
原文链接为：`http://www.cnblogs.com/bluestorm/p/3168720.html`这三种类型各有各的优点，比如CString比较灵活，是基于MFC常用的类型，安全性也最高，但可移植性最差。string是使用STL时必不可少的类型，所以是做工程时必须熟练掌握的；char*是从学习C语言开始就已经和我们形影不离的了，有许多API都是以char*作为参数输入的。所以熟练掌握三者之间的转换十分必要。
以下我用简单的图示指出三者之间的关系，并以标号对应转换的方法。
![](https://img-blog.csdn.net/20160825114021553?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
```cpp
//1 string to CString   
  CString.format("%s",string.c_str()); 
//2 CString to string
string str(CString.GetBuffer(str.GetLength()));
//3 string to char *
char *p=string.c_str();
//4 char * to string
string str(char*);
//5 CString to char *
strcpy(char,CString,sizeof(char));
//6 char * to CString
CString.format("%s",char*);
```
 CString的format方法是非常好用的。string的c_str()也是非常常用的，但要注意和char *转换时，要把char定义成为const char*，这样是最安全的。
以上函数UNICODE编码也没问题：unicode下照用，加个_T()宏就行了,像这样子_T("%s")
补充：
CString 可能是 CStringW/CStringA，在与 string 转换时，如果是 CStringW，还涉及编码转换问题。下面以 CStringA 来说明。
1 string to CString  
CString.format("%s",string.c_str());
CStringA = string.c_str() 就可以了
2 CString to string
string str(CString.GetBuffer(str.GetLength()));
GetBuffer 有参数的话，可能导致内部的分配空间动作，要进行后续 ReleaseBuffer 操作。
string = CStringA
string = CStringA.GetBuffer();
3 string to char *
char *p=string.c_str();
4 char * to string
string str(char*);
5 CString to char *
strcpy(char *,CString,sizeof(char));
按照 3 风格，这里应该 char *  = CStringA; 或者 char *p = CStringA.GetBuffer();
6 char * to CString
CStringA = char * 就可以了
