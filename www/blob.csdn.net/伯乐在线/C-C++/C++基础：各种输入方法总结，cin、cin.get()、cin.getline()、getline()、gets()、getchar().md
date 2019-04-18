# C++基础：各种输入方法总结，cin、cin.get()、cin.getline()、getline()、gets()、getchar() - 文章 - 伯乐在线
原文出处： [evil_1_live](http://blog.csdn.net/livecoldsun/article/details/25489429)
在C++中，各种输入方法还是不少的，而且各有所异，本文做一点简要总结，主要涉及如下内容：cin、cin.get()、cin.getline()、getline()、gets()、getchar()。
**输入原理简述：**
程序的输入都建有一个缓冲区，即输入缓冲区。每次输入过程是这样的，当一次键盘输入结束时会将输入的数据存入输入缓冲区，而cin函数直接从输入缓冲区中取数据。正因为cin函数是直接从缓冲区取数据的，所以**有时候当缓冲区中有残留数据时，cin函数会直接取得这些残留数据而不会请求键盘输入。**
比如下面的例子：

```
void test_input()  
{  
    string str;  
    cout<<"cin的测试："<<endl;  
    cin>>str;  
    cout<<str<<endl;  
    cin>>str;  
    cout<<str<<endl;  
}
```
![](http://img.blog.csdn.net/20140510163512078)
由于cin在遇到空格时，就会停止输入，所以如果我在第一次输入时，利用空格隔开两个字符串，那么cin在第一次取的时候，只会读取前一个字符串，到空格结束，此时缓冲区还保留着前面输入的第二个字符串，那么第二次cin就会直接从缓冲区取残留数据，而不会请求输入。
当然对于以上的情况，也有解决的方案，那就是在第二次调用cin>>str，之前通过cin.sync()来清空输入缓冲区，看一下下面的例子，此处不赘述：


```
void test_input()  
{  
    string str;  
    cout<<"cin的测试："<<endl;  
    cin>>str;  
    cin.sync();  
    cout<<str<<endl;  
    cin>>str;  
    cout<<str<<endl;  
}
```
![](http://img.blog.csdn.net/20140510164624171)
**各种输入方法简介：**
**1、cin>>**
根据cin>>sth 中sth的变量类型读取数据，这里变量类型可以为int，float,char,char*,string等诸多类型。这一输入操作，在遇到结束符（Space、Tab、Enter）就结束，且对于结束符，并不保存到sth中。


```
void test_input()  
{  
    char ch1[10],ch2[10];  
    cout<<"输入两个字符串："<<endl;  
    cin>>ch1;  
    cin>>ch2;  
    cout<<"两个字符串分别为："<<endl;  
    cout<<ch1<<endl;  
    cout<<ch2<<endl;  
}
```
![](http://img.blog.csdn.net/20140510165644250)
**2、cin.get(字符数组名，接收长度，结束符)**
其中结束符意味着遇到该符号结束字符串读取,默认为ENTER，读取的字符个数最多为（长度-1），因为最后一个为”n”。要注意的是，cin.get()操作遇到结束符停止读取，但并不会将结束符从缓冲区丢弃。cin.get()有如下几种用法：
（1）接收一个字符ch=cin.get()或cin.get(char ch),二者等价，看两个例子


```
void test_input()  
{  
    char ch1,ch2;  
    cout<<"请输入两个字符："<<endl;  
    cin.get(ch1);//或ch1 = cin.get();  
    cin.get(ch2);  
    cout<<ch1<<" "<<ch2<<endl;  
    cout<<(int)ch1<<" "<<(int)ch2<<endl;  
}
```
来看几组测试：
- 
连续输入ab[enter]，结果正常，ch1,ch2分别读取了a、b，将其输出，然后在输出其ASCII值。要注意的是，以上输入并读取后，缓冲区中依然存在[Enter]没有被删除。
![](http://jbcdn2.b0.upaiyun.com/2016/11/9b68940cedaea28bc27c0d8c49b8097e.jpeg)
- 输入a[Space]b[Enter]，结果在输出时，只看到了a，输出ASCII值时候分别为97 32（空格的ASCII值），这就说明cin.get()并不会舍弃Space，依然会将其读取进去，并加以显示等操作。
![](http://jbcdn2.b0.upaiyun.com/2016/11/4670e0abdd1cc1384ab14d59d94f1ac8.jpeg)
- 输入a[Enter],输出见下图。在输出a之后，第二次的输出产生了换行的效果，而输出的第二个ASCII值为10（Enter的ASCII值），这就进一步响应了前面说到的cin.get()遇到结束符并不会将之删除。
![](http://jbcdn2.b0.upaiyun.com/2016/11/7e37397ded60b9b8a20de504a6351aab.jpeg)
（2）接收一定长度的字符串cin.get(字符数组名，接收长度，结束符)，结束符为可选参数，默认为Enter，可以接受Space 、Tab，对于结束符的处理要注意，结束符并不会丢掉，同样看几个例子。
- 对于如下代码，所做的操作时，在不遇到enter时最多读入（6-1）=5个字符到ch2中，然后读入下一个字符到ch1中，显示ch2，ch1以及其ASCII码值。附上几组测试：


```
void test_input()  
{  
    char ch1,ch2[10];  
    cout<<"请输入字符串："<<endl;  
    cin.get(ch2,6);//在不遇到结束符的情况下，最多可接收6-1=5个字符到ch2中，注意结束符为默认Enter  
    cin.get(ch1);//或ch1 = cin.get();  
    cout<<ch2<<endl;  
    cout<<ch1<<"\n"<<(int)ch1<<endl;  
}
```
- 输入：zifuchuan[Enter]，由于输入长度大于（6-1）=5，所以会首先读入“zifuc”到ch1，此时“huan”仍在缓冲区，当执行cin.get(ch1)会直接从缓冲区读入h，而不需要申请从键盘输入，看一下结果，符合分析。
![](http://jbcdn2.b0.upaiyun.com/2016/11/b44ba9ed8c517338edfe3266e8d4787e.jpeg)
- 输入：zifu[Enter]，此时输入长度小于5就遇到了默认结束符Enter，则ch2中只读入“zifu”，要注意的是，输入缓冲区里面的Enter还在，所以接下来要读入的ch1的内容将是Enter，而输出时将看到换行，ASCII码值为10，见下图
![](http://jbcdn2.b0.upaiyun.com/2016/11/ac31cbb99edbda754c58c5abf6199ea8.jpeg)
- 输入：zi fuchuan[Enter]，注意中间的空格，cin.get()对空格并不敏感，依然会读入，故而ch2读入的是“zi fu”，ch1读入的是c
![](http://jbcdn2.b0.upaiyun.com/2016/11/47ad1a7cb193b3cfa030cbf03eed84fb.jpeg)
（3）cin.get( )，注意此时没有参数可用于舍弃输入流中的不需要的字符,或者舍弃回车,弥补cin.get(字符数组名,字符数目,结束符)的不足。对（2）中的代码加入一句话cin.get()如下：


```
void test_input()  
{  
    char ch1,ch2[10];  
    cout<<"请输入字符串："<<endl;  
    cin.get(ch2,6);//在不遇到结束符的情况下，最多可接收6-1=5个字符到ch2中  
    cin.get();//注意：前面两句可以写到一块：cin.get(ch2,6).get();  
    cin.get(ch1);//或ch1 = cin.get();  
    cout<<ch2<<endl;  
    cout<<ch1<<"\n"<<(int)ch1<<endl;  
}
```
- 前面遇到的一个状况是，输入字符后，其结束符（如默认的Enter）会保留在缓冲区中，当下次读入时，又会再读入，此时就可以用到cin.get()独钓输入缓冲区不需要的字符，如：输入：zi[Enter]，由于遇到结束符，所以ch2内容为zi，此时输入缓冲区还存在着[Enter]，但cin.get()将其舍弃掉之后，cin.get(ch1)就会申请从键盘输入内容，如下所示：
![](http://jbcdn2.b0.upaiyun.com/2016/11/2baff3794253712a8adb550266c02cc3.jpeg)
**3、cin.getline(字符数组名，接收长度，结束符)**
其用法与cin.get(字符数组名，接收长度，结束符)极为类似。cin.get()当输入的字符串超长时，不会引起cin函数的错误，后面若有cin操作，会继续执行，只是直接从缓冲区中取数据。但是cin.getline()当输入超长时，会引起cin函数的错误，后面的cin操作将不再执行。如下代码：


```
void test_input()  
{  
    char ch1,ch2[10];  
    cout<<"请输入字符串："<<endl;  
    cin.getline(ch2,6);//在不遇到结束符的情况下，最多可接收6-1=5个字符到ch2中  
    cin>>ch1;  
    cout<<ch2<<endl;  
    cout<<ch1<<"\n"<<(int)ch1<<endl;  
}
```
- 测试：如下图，输入zifuchuan[Enter]，长度大于最大长度5，就会导致cin函数错误，其后既没有像cin.get()一样直接从输入缓冲区直接读数据，也没有从键盘输入。所以此处可以注意，考虑在用cin.getline()时，适度设置其最大接受长度大一点。
![](http://jbcdn2.b0.upaiyun.com/2016/11/844341f2fd7bfbf681eb2eca8c4f71e5.jpeg)
**4、getline(istream is,string str,结束符)**
同样，此处结束符为可选参数（默认依然为Enter）。然而，getline()与前面的诸多存在的差别在于，它string库函数下，而非前面的istream流，所有调用前要在前面加入#include。与之对应这一方法读入时第二个参数为string类型，而不再是char*，要注意区别。另外，该方法也不是遇到空格就结束输入的。


```
void test_input()  
{  
    string str;  
    cout<<"请输入string内容："<<endl;  
    getline(cin,str,'a');  
    cout<<str<<endl;     
}
```
![](http://img.blog.csdn.net/20140511001924250)![](http://img.blog.csdn.net/20140511001928703)
通过以上第二个图还可以看出，这一方法只有在遇到结束符（此处为‘a’）才结束，对空格甚至回车都不敏感。
**5、gets(char *ch)**
gets()方法同样接受一个字符串，但是与getline()不同，它的参数为char*，而不是string，另外若定义char ch[n]，长度为n，则样注意输入的字符串长度不要大于n，否则会报错。同样gets()对空格也不敏感。


```
void test_input()  
{  
    char ch[10];  
    cout<<"请输入char*内容："<<endl;  
    gets(ch);  
    cout<<ch<<endl;      
}
```
![](http://img.blog.csdn.net/20140511002001531)![ee3-tmp](http://jbcdn2.b0.upaiyun.com/2016/11/a7cc07f1e425f46fa1c8cf7efa1c91f9.png)
好吧，就到这里吧，其实还有getchar()、getch()等，这里就不一一赘述了。
