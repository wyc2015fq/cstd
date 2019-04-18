# 精解CString类的GetBuffer,ReleaseBuffer 函数(VC++)  - ljx0305的专栏 - CSDN博客
2010年03月04日 16:06:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：870标签：[vc++																[测试																[存储](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=vc++&t=blog)
个人分类：[MFC																[C++](https://blog.csdn.net/ljx0305/article/category/380566)](https://blog.csdn.net/ljx0305/article/category/403913)
CString str = "abcde/0cde";
输出字符串的值为: abcde 
而字符串的长度为 s.GetLength() 的值为: 5 
这是因为CString对象在赋值时只检查到'/0',后面的忽略了, 也就是说实际对象str内容为"abcde". 
而str真正的存储空间为6(字符串以'/0'结尾). 
所以说在字符长度和实际的空间是不一样的. 好!别跑! 
请看下面有趣的程序: 
CString str = "hello"; 
LPSTR pf = (LPSTR)(LPCSTR)s; 
LPSTR pa = s.GetBuffer(0); 
        你可以测得 pf == pa; 
LPSTR pb = s.GetBuffer(10); 
        你可以测得 pf != pb; 
为什么: 
我们都知道(LPSTR)(LPCSTR)s 实际指向对象str的实际字符串的内存地址, GetBuffer() 函数中的参数(其实就是重新申请的字符串的长度)如果小于等于先前的字符串长度, 则不会重新分配内存使用原来的内存所以 pf == pa, 如果大于先前的字符串长度, 则重新追加内存(也就是要复制原来的内容), 
所以pf != pb. 
注意GetBuffer()函数中的参数为重新申请的字符串的长度, 实际内存的大小应再加1. 
CString s = "hello"; 
LPSTR pf = s.GetBuffer(0); 
strcpy(pf,"hi"); 
这时对象str 的内容为 "hi" 
但是s.GetLength()的值为5, 如果加上一条语句: 
s.ReleaseBuffer(); 
则s.GetLength()的值为2 
解释: 
CString对象在内存中用一个计数器来维持可用缓冲区的大小 
void ReleaseBuffer( int nNewLength = -1 )
     {
          if( nNewLength == -1 )
          {
               nNewLength = StringLength( m_pszData );
          }
          SetLength( nNewLength );
     } 
很明显ReleaseBuffer的作用就是更新字符串的长度。 CString内，GetLength获取字符串长度并不是动态计算的，而是在赋值操作后计算并保存在一个int变量内的，当通过GetBuffer直接修改CString时，那个int变量并不可能自动更新，于是便有了ReleaseBuffer. 
CString s = "hello"; 
LPSTR pf = s.GetBuffer(0); 
strcpy(pf,"hi"); 
LPSTR ps =  (LPSTR)(LPCSTR)s;    字符串缓冲区的首地址 
*(ps+2) = 'x'; 
  则字符串的实际内容为:    "hixlo" 
*(ps+6) = 'a';        出错, 因为对象s的实际空间为 6 
而 
CString s = "hello"; 
LPSTR pf = s.GetBuffer(10); 
strcpy(pf,"hi"); 
LPSTR ps =  (LPSTR)(LPCSTR)s;    字符串缓冲区的首地址 
*(ps+2) = 'x'; 
*(ps+5)= '/0'; 
 则字符串的实际内容还是为:    "hixlo" 
*(ps+6) = 'a';         可以因为s对象的实际空间为11 
说白了  ReleaseBuffer就是更新赋值之后的字符串的长度, 而实际空间没有根本的变化, GetBuffer才是使内存空间大小变化的罪魁祸首. 
有兴趣的可以测试一下就知道了!!! 
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/shyboy_NWPU/archive/2009/07/16/4352602.aspx](http://blog.csdn.net/shyboy_NWPU/archive/2009/07/16/4352602.aspx)
