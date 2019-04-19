# DEBUG和RELEASE 版本差异及调试相关问题(VS - VC++) - xqhrs232的专栏 - CSDN博客
2012年11月03日 22:51:51[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：779
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://blog.csdn.net/houffee/article/details/2703645](http://blog.csdn.net/houffee/article/details/2703645)
相关网帖
1.[Debug与Release版本的区别](http://blog.csdn.net/tangyanming/article/details/7624112)----[http://blog.csdn.net/tangyanming/article/details/7624112](http://blog.csdn.net/tangyanming/article/details/7624112)
2.[vc6 debug release 结果不一致问题](http://blog.csdn.net/sandlu/article/details/1944126)----[http://blog.csdn.net/sandlu/article/details/1944126](http://blog.csdn.net/sandlu/article/details/1944126)
3.[如何对待Release与Debug运行结果不一致](http://jiangzhanyong.com/2007/01/release-and-debug-version-172.html)----[http://jiangzhanyong.com/2007/01/release-and-debug-version-172.html](http://jiangzhanyong.com/2007/01/release-and-debug-version-172.html)

I.内存分配问题
                1.变量未初始化。下面的程序在debug中运行的很好。
                    thing * search(thing * something)
                      BOOL found;
                      for(int i = 0; i < whatever.GetSize(); i++)
                        {
                        if(whatever[i]->field == something->field)
                           { /* found it */
                            found = TRUE;
                            break;
                           } /* found it */
                         }
                  if(found)
                           return whatever[i];
                  else
                           return NULL;
      而在release中却不行，因为debug中会自动给变量初始化found=FALSE,而在release版中则不会。所以尽可能的给变量、类或结构初始化。
                2.数据溢出的问题 
                      如：char buffer[10];
                           int counter;
                     lstrcpy(buffer, "abcdefghik");
      在debug版中buffer的NULL覆盖了counter的高位，但是除非counter>16M,什么问题也没有。但是在release版中，counter可能被放在寄存器中，这样NULL就覆盖了buffer下面的空间，可能就是函数的返回地址，这将导致ACCESS
      ERROR。
                3.DEBUG版和RELEASE版的内存分配方式是不同的。如果你在DEBUG版中申请   ele 为 
      6*sizeof(DWORD)=24bytes,实际上分配给你的是32bytes（debug版以32bytes为单位分配），而在release 
      版，分配给你的就是24bytes（release版以8bytes为单位），所以在debug版中如果你写ele[6],可能不会有什么问题，而在 
      release版中，就有ACCESS VIOLATE。
      II.ASSERT和VERIFY
                     ASSERT在Release版本中是不会被编译的。
                ASSERT宏是这样定义的
                      #ifdef _DEBUG
                         #define ASSERT(x) if( (x) == 0) report_assert_failure()
                      #else
                         #define ASSERT(x)
                      #endif
                      实际上复杂一些，但无关紧要。假如你在这些语句中加了程序中必须要有的代码
                比如
                ASSERT(pNewObj = new CMyClass);
                pNewObj->MyFunction();
                这种时候Release版本中的pNewObj不会分配到空间
                所以执行到下一个语句的时候程序会报该程序执行了非法操作的错误。这时可以用VERIFY ：
                      #ifdef _DEBUG
                         #define VERIFY(x) if( (x) == 0) report_assert_failure() 
                      #else
                         #define VERIFY(x) (x)
                      #endif
                这样的话，代码在release版中就可以执行了。
      III.参数问题：
                自定义消息的处理函数，必须定义如下：
                afx_msg LRESULT OnMyMessage(WPARAM, LPARAM);
                返回值必须是HRESULT型，否则Debug会过，而Release出错
      IV.  内存分配
      保证数据创建和清除的统一性：如果一个DLL提供一个能够创建数据的函数，那么这个DLL同时应该提供一个函数销毁这些数据。数据的创建和清除应该在同一个层次上。
      V.DLL的灾难
                      人们将不同版本DLL混合造成的不一致性形象的称为 “动态连接库的地狱“(DLL Hell) 
      ，甚至微软自己也这么说([http://msdn.microsoft.com/library/techart/dlldanger1.htm](http://msdn.microsoft.com/library/techart/dlldanger1.htm))。
               如果你的程序使用你自己的DLL时请注意：
                1.不能将debug和release版的DLL混合在一起使用。debug都是debug版，release版都是release版。
                解决办法是将debug和release的程序分别放在主程序的debug和release目录下
                2.千万不要以为静态连接库会解决问题，那只会使情况更糟糕。
      VI.RELEASE板中的调试：
                1.将ASSERT() 改为 VERIFY() 。找出定义在"#ifdef 
      _DEBUG"中的代码，如果在RELEASE版本中需要这些代码请将他们移到定义外。查找TRACE(...)中代码，因为这些代码在RELEASE中也不被编译。请认真检查那些在RELEASE中需要的代码是否并没有被便宜。
                2.变量的初始化所带来的不同，在不同的系统，或是在DEBUG/RELEASE版本间都存在这样的差异，所以请对变量进行初始化。
                3.是否在编译时已经有了警告?请将警告级别设置为3或4,然后保证在编译时没有警告出现.
      VII.将Project Settings" 中 "C++/C " 
      项目下优化选项改为Disbale（Debug）。编译器的优化可能导致许多意想不到的错误.
                此外对RELEASE版本的软件也可以进行调试，请做如下改动：
                在"Project Settings" 中 "C++/C " 项目下设置 "category" 为 "General" 
      并且将"Debug Info"设置为 "Program Database"。
                在 "Link"项目下选中"Generate Debug Info"检查框。
                "Rebuild All"
                如此做法会产生的一些限制：
                无法获得在MFC DLL中的变量的值。
                必须对该软件所使用的所有DLL工程都进行改动。
                另：
                MS BUG：MS的一份技术文档中表明，在VC5中对于DLL的"Maximize 
      Speed"优化选项并未被完全支持，因此这将会引起内存错误并导致程序崩溃。
      posted @ 4:41 AM | Feedback (0) 
      没有找到MFC80D.DLL的解决方法
      调试MFC程序的时候,有时候会出现这个错误"没有找到MFC80D.DLL,重新安装.................", 
      使用网上搜索到的好几种方法,都无效,自己瞎改,遂,好了,操作如下:
      修改项目-属性-配置属性--debug-General--output  directory 　和  intermediate directory
         他们的缺省值为./debug ,给他们改成了 $(SolutionDir)$(ConfigurationName)  和       $(ConfigurationName)
      重新编译,问题解决.
      可能网上其他解决办法是针对其它原因导致这个报错的方式,需要找专家答疑了,呵呵,简单列一下其他方法(在我这个问题上测试无效,可能对别的环境有效  :)
       )
      1  安装的是后没有把debug的一些unicode的库装进去,从安装盘里面拷贝一份.
      2  需要更改默认项目的工程属性。工程属性->配置属性->清单工具->输入和输出->嵌入清单==>改为否
          方法2的另一描述: 
               Visual Studio  2005用向导生成的项目，在运行时可能会遇到找不到MFC80UD.dll的问题。这个问题可能是Manifest
      引起的，因此我们可以通过修改项目属性(properties)->清单工具(property 
      pages)->链接(linker)-->manifest file -->Allow 
      Isolation，把“嵌入清单”选“否”，.然后编译、链接、运行即可
        方法2的再一描述:
      在头文件中加入如下声明即可
      #pragma comment(linker, "/"/manifestdependency:type='Win32' 
      name='Microsoft.VC80.CRT' version='8.0.50608.0' 
      processorArchitecture='X86' publicKeyToken='1fc8b3b9a1e18e3b'       language='*'/"")
      3    查找下MFC80UD.DLL,   拷贝到你项目的生成目标文件夹下试试.   
      4   工程属性->配置属性->常规->字符集"选"使用多字节字符集"
      5   project   properties－manifest   tools－general   －   use   FAT32      work-around->true    然后clean－rebuild
           方法5,我压根没找到怎么改,我太笨了!
      以上方法我均试用无效,估计别的环境有效吧,留此供大家参考!

