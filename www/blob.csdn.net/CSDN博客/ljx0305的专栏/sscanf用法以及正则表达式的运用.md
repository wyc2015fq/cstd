# sscanf用法以及正则表达式的运用 - ljx0305的专栏 - CSDN博客
2009年08月10日 16:52:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：595
```
%[ ] 的用法：%[ ]表示要读入一个字符集合, 如果[ 后面第一个字符是”^”，则表示反意思。
                     [ ]内的字符串可以是1或更多字符组成。空字符集（%[]）是违反规定的，可
                     导致不可预知的结果。%[^]也是违反规定的。
          
%[a-z] 读取在 a-z 之间的字符串，如果不在此之前则停止，如
              char s[]="hello, my friend” ; // 注意: ,逗号在不 a-z之间
              sscanf( s, “%[a-z]”, string ) ; // string=hello
%[^a-z] 读取不在 a-z 之间的字符串，如果碰到a-z之间的字符则停止，如
              char s[]="HELLOkitty” ; // 注意: ,逗号在不 a-z之间
              sscanf( s, “%[^a-z]”, string ) ; // string=HELLO
%*[^=] 前面带 * 号表示不保存变量。跳过符合条件的字符串。
              char s[]="notepad=1.0.0.1001" ;
       char szfilename [32] = "" ;
       int i = sscanf( s, "%*[^=]", szfilename ) ;
```
```
// szfilename=NULL,因为没保存
int i = sscanf( s, "%*[^=]=%s", szfilename ) ;
```
```
// szfilename=1.0.0.1001
%40c 读取40个字符
%[^=] 读取字符串直到碰到’=’号，’^’后面可以带更多字符,如：
              char s[]="notepad=1.0.0.1001" ;
              char szfilename [32] = "" ;
             int i = sscanf( s, "%[^=]", szfilename ) ;
```
```
// szfilename=notepad 
       如果参数格式是：%[^=:] ，那么也可以从 notepad:1.0.0.1001读取notepad
```
参考：
[http://blog.csdn.net/beingstudio/articles/1806661.aspx](http://blog.csdn.net/beingstudio/articles/1806661.aspx)
[http://hi.baidu.com/lbird/blog/item/07e09c8282dbe992f703a6b0.html](http://hi.baidu.com/lbird/blog/item/07e09c8282dbe992f703a6b0.html)
[http://hi.baidu.com/yinjianren/blog/item/28dce5ca75e30585c9176896.html](http://hi.baidu.com/yinjianren/blog/item/28dce5ca75e30585c9176896.html)
引用:http://blog.chinaunix.net/u/21684/showart_499274.html
