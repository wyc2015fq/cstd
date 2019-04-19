# C++ string类型的字符串长度获取的三种方法 - fanyun的博客 - CSDN博客
2018年01月21日 20:43:44[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：64622
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)

(1).用string的成员方法length()获取字符串长度
               length()比较直观，表示的就是该字符串的长度。
**[cpp]**[view
 plain](http://blog.csdn.net/fanyun_01/article/details/78980275#)[copy](http://blog.csdn.net/fanyun_01/article/details/78980275#)
[print](http://blog.csdn.net/fanyun_01/article/details/78980275#)[?](http://blog.csdn.net/fanyun_01/article/details/78980275#)
- #include <string>
- #include <iostream>
- 
- usingnamespace std;  
- int main()  
- {  
-     string str = "my string";  
-     cout << str.length() << endl;  
- return 0;  
- }  
(2).用string的成员方法size()获取字符串长度
           size()表示的是string这个容器中的元素个数。如果使用过std::vector之类的容器的话，可以把string看做是一个vector<char> (这里只是举例，并不能等价)， char就是这个容器的元素类型。那么size()表示的就是这个vector(容器)中char的个数。
**[cpp]**[view
 plain](http://blog.csdn.net/fanyun_01/article/details/78980275#)[copy](http://blog.csdn.net/fanyun_01/article/details/78980275#)
[print](http://blog.csdn.net/fanyun_01/article/details/78980275#)[?](http://blog.csdn.net/fanyun_01/article/details/78980275#)
- #include <string>
- #include <iostream>
- 
- usingnamespace std;  
- int main()  
- {  
-     string str = "Test string";  
-     cout << str.size() << endl;  
- return 0;  
- }  
(3).用strlen获取字符串长度
strlen同样也可以用于C++的string。但是需要用c_str()将C++ string转换为char*类型。
**[cpp]**[view
 plain](http://blog.csdn.net/fanyun_01/article/details/78980275#)[copy](http://blog.csdn.net/fanyun_01/article/details/78980275#)
[print](http://blog.csdn.net/fanyun_01/article/details/78980275#)[?](http://blog.csdn.net/fanyun_01/article/details/78980275#)
- #include <stdio.h>
- #include <string>
- usingnamespace std;  
- void main()  
- {  
- string str;  
- str = "Test string";  
- len = str.length();  
- printf("%d",len);  
- }  
