# 嵌入式应用中CGI编程中POST、GET及环境变量详解 - DoubleLi - 博客园






原载地址：[http://3633188.blog.51cto.com/3623188/828095](http://blog.csdn.net/3623188/828095)


###     1.POST和GET

     一个CGI程序在于服务器之间的信息传输和数据传输一般通过两种方法，即POST和GET。具体是哪一种方法这需要通过CGI的一个环境变量REQUEST_METHOD判断（具体怎么判断我会在下面详细讲解），在这之前先讲一下URL编码。

     1.1 URL编码

      虽然在设置表单信息的传输方式时有POST和GET两种方法，但是不管采取哪种方法，浏览器采取的编码方式却是完全相同的。编码规则如下：

      □ 变量之间使用“&”分开

      □ 变量与其对应值之间使用“=”链接

      □ 空格符使用“+”代替

      □ 保留的控制字符则使用百分号接相应的十六进制ASCII代替

      □ 空格是非法字符

      □ 任意不可打印的ASCII 控制字符都为非法字符

      □ 某些具有特殊意义的字符也用百分号接相应的十六进制ASCII代替 



**[html]**[view plain](http://blog.csdn.net/gl1987807/article/details/7958937#)[copy](http://blog.csdn.net/gl1987807/article/details/7958937#)

[print](http://blog.csdn.net/gl1987807/article/details/7958937#)[?](http://blog.csdn.net/gl1987807/article/details/7958937#)

- <body>   
- <form name="form1" action="/cgi-bin/pass.cgi" method="get">   
- <table align="center">   
- <tr><td align="center" colspan="2"></td></tr>   
- <tr>   
- <td align="right">用户名</td>   
- <td><input type="text" name="Username"></td>   
- </tr>   
- <tr>   
- <td align="right">密  码</td>   
- <td><input type="password" name="Password"></td>   
- </tr>   
- <tr>   
- <td><input type="submit" value="登    录"></td>   
- <td><input type="reset" value="取    消"></td>   
- </tr>   
- </table>   
- </form>   
- </body>  


 如果我们在用户名后面填写Tom，密码后填写1234，则在点击提交后传给服务器的变量格式如下：

Username=Tom&Password=1234

     下面讲解POST和GET具体的具体工作方式

###     2.POST和GET工作方式 

     2.1 POST

     如果在form表单中method使用POST方法，那么服务器会将会把从表单中填入的数据接收，并传给相应的CGI程序（就是action中指定的CGI程序），同时把REQUEST_METHOD环境变量设置为POST，而相应的CGI程序检查该环境变量，以确定其工作在POST接收数据方式，然后读取这个数据。注意使用POST这种方法传输数据时，Http在数据发送完后，并不会发送相应的数据传输完毕提示信息，所以Http服务器提供了另一个环境变量CONTENET_LENGTH,该环境变量记录了传输过来了多少个字节长度的数据（单位为字节），所以在编写CGI程序时，如果method为POST，就需要通过该变量来限定读取的数据的长度（如何实现，下面讲解）。

另外还有个环境变量CONTENET_TYPE,记录从浏览器端发送来的数据类型，现在一般发送的MIME类型为Content-type: text/html\n\n，具体怎么使用在CGI中下面介绍。在确认两个环境变量的内容都符合后，就开始按下列规则解析表单传输过来的数据，就是URL编码的逆过程（不再赘述）。

    2.2 GET

    基本上GET方法和POST方法相同，不同的是，使用GET方法时，数据被存储到一个叫做QUERY_STRING的环境变量中了，具体如何得到该变量里的内容，会在下面的例子中详细讲述。

    说了这么多，通过实例看一下，具体实现时如何编写CGI程序。

    表单仍然和上面的HTML代码相同。下面通过一个返回所填内容的CGI程序讲解。代码如下：




**[cpp]**[view plain](http://blog.csdn.net/gl1987807/article/details/7958937#)[copy](http://blog.csdn.net/gl1987807/article/details/7958937#)

[print](http://blog.csdn.net/gl1987807/article/details/7958937#)[?](http://blog.csdn.net/gl1987807/article/details/7958937#)

- #include <stdio.h>   
-  #include <stdlib.h>   
-  #include <string.h>   
- 
- char* getcgidata(FILE* fp, char* requestmethod);   
- int main()   
- {   
- char *input;   
- char *req_method;   
- char name[64];   
- char pass[64];   
- int i = 0;   
- int j = 0;   
- 
- //         printf("Content-type: text/plain; charset=iso-8859-1\n\n");   
-                 printf("Content-type: text/html\n\n");   
-                 printf("The following is query reuslt:<br><br>");   
- 
-                 req_method = getenv("REQUEST_METHOD");   
-                 input = getcgidata(stdin, req_method);   
- 
- // 我们获取的input字符串可能像如下的形式   
- // Username="admin"&Password="aaaaa"   
- // 其中"Username="和"&Password="都是固定的   
- // 而"admin"和"aaaaa"都是变化的，也是我们要获取的   
- 
- // 前面9个字符是UserName=   
- // 在"UserName="和"&"之间的是我们要取出来的用户名   
- for ( i = 9; i < (int)strlen(input); i++ )   
-                 {   
- if ( input[i] == '&' )   
-                              {   
-                                             name[j] = '\0';   
- break;   
-                              }                                       
-                              name[j++] = input[i];   
-                 }   
- 
- // 前面9个字符 + "&Password="10个字符 + Username的字符数   
- // 是我们不要的，故省略掉，不拷贝   
- for ( i = 19 + strlen(name), j = 0; i < (int)strlen(input); i++ )   
-                 {   
-                              pass[j++] = input[i];   
-                 }   
-                 pass[j] = '\0';   
- 
-                 printf("Your Username is %s<br>Your Password is %s<br> \n", name, pass);   
- 
- return 0;   
- }   
- 
- char* getcgidata(FILE* fp, char* requestmethod)   
- {   
- char* input;   
- int len;   
- int size = 1024;   
- int i = 0;   
- 
- if (!strcmp(requestmethod, "GET"))   
-                 {   
-                              input = getenv("QUERY_STRING");   
- return input;   
-                 }   
- else if (!strcmp(requestmethod, "POST"))   
-                 {   
-                              len = atoi(getenv("CONTENT_LENGTH"));   
-                              input = (char*)malloc(sizeof(char)*(size + 1));   
- 
- if (len == 0)   
-                              {   
-                                             input[0] = '\0';   
- return input;   
-                              }   
- 
- while(1)   
-                              {   
-                                             input[i] = (char)fgetc(fp);   
- if (i == size)   
-                                             {   
-                                                          input[i+1] = '\0';   
- return input;   
-                                             }   
- 
-                                             --len;   
- if (feof(fp) || (!(len)))   
-                                             {   
-                                                          i++;   
-                                                          input[i] = '\0';   
- return input;   
-                                             }   
-                                             i++;   
- 
-                              }   
-                 }   
- return NULL;  


 下面开讲：首先注意这行代码 printf("Content-type: text/html\n\n");

通过它告诉服务器要输出的内容是文本内容或者HTML，在编写CGI程序时容易遗留这一行，则会提示服务器内部出错，无法完成你的请求，需要注意的是后面两个“\n\n”，这是必须的，具体为什么，我也不清楚，这样写是正确。在这个地方，有的网友做的时候汉字输出后是乱码，这样的话，可以在“\n\n”，之前输出编码信息，在window下一般为gb2312.

   往下走，就是这一行了： req_method = getenv("REQUEST_METHOD");这是通过getenv()函数得到环境变量的值，在调用函数里判断采用的那种方法，然后做出相应的操作。

    if (!strcmp(requestmethod, "GET")) 
                { 
                             input = getenv("QUERY_STRING"); 
                             return input; 
                } 
                else if (!strcmp(requestmethod, "POST")) 
                {            //if (getenv(″CONTENT-LENGTH″)) 
                             len = atoi(getenv("CONTENT_LENGTH")); 
                             input = (char*)malloc(sizeof(char)*(size + 1));

   此处通过strcmp()函数，判断具体的方法，如果是GET方法，则通过getenv()函数直接获取QUERY_STRING中的内容，返回给主函数。继续往下走，就是当method为POST时，如何通过环境变量CONTENET_LENGTH来限制接收数据的数量，这一句 if (getenv(″CONTENT-LENGTH″))判断CONTENET_LENGTH是否存在，但是在编程时可以直接使用atoi()函数，所以代码中我注释掉了这一行（编程时自己注意差别）
　　len=atoi (getenv(″CONTENT-LENGTH″));
　　此行首先检查环境变量CONTENT-LENGTH是否存在的同时，将此环境变量的值转换成整数,并赋给变量len。请注意Web服务器并不以文件结束符来终止它的输出,所以如果不检查环境变量CONTENT-LENGTH,CGI程序就无法知道什么时候输入结束了。

下面这句 input = (char*)malloc(sizeof(char)*(size + 1));就是申请一段内存空间，用于数据存储。

    再往下，就是C语言基础了，这里不再赘述。



    一般理解了这个例子就可以掌握POST和GET方法数据的获取方式了。












