# 利用自定义消息处理函数的WPARAM或LPARAM参数传递指针 - xqhrs232的专栏 - CSDN博客
2014年02月09日 16:47:34[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2895
原文地址::[http://blog.csdn.net/dongfangyu/article/details/4372602](http://blog.csdn.net/dongfangyu/article/details/4372602)
相关网帖
1、VC++的SendMessage函数如何利用wParam lParam传递参数----[http://zhidao.baidu.com/link?url=A_MKzsHhEm56PSkq5TJlvXQpwClL0RoL9_Zej5j7paT2xT_6TH0vkY_oQdbgY3Vg4Jh3Tkdnjjd1hXkgrpJMoa](http://zhidao.baidu.com/link?url=A_MKzsHhEm56PSkq5TJlvXQpwClL0RoL9_Zej5j7paT2xT_6TH0vkY_oQdbgY3Vg4Jh3Tkdnjjd1hXkgrpJMoa)
2、WPARAM和LPARAM参数详解----[http://wenku.baidu.com/link?url=HcJb_CRU6l2QA0NKYGDgqRx7GDVh0iBiaHTY2DdF06jPJ0CZQ6OCrP2HZq5zpJ9jWPIqN3MInoBWhNEnGp5NS4O89OjnuI4fgw5r0FJJjO_](http://wenku.baidu.com/link?url=HcJb_CRU6l2QA0NKYGDgqRx7GDVh0iBiaHTY2DdF06jPJ0CZQ6OCrP2HZq5zpJ9jWPIqN3MInoBWhNEnGp5NS4O89OjnuI4fgw5r0FJJjO_)
3、进程间的消息传递----[http://bbs.csdn.net/topics/320166467](http://bbs.csdn.net/topics/320166467)
4、VC跨进程传递数据----[http://wenku.baidu.com/link?url=2cXT9SoOHUfvnJrHBYL8NVwMkgzGkJTghVBiMpj5n805YL5mSVdxrKz_gQOip11SJCZngeV9cpQAMtFzzeW1-Y0_-9AeEjGxP6Vda16KYLq](http://wenku.baidu.com/link?url=2cXT9SoOHUfvnJrHBYL8NVwMkgzGkJTghVBiMpj5n805YL5mSVdxrKz_gQOip11SJCZngeV9cpQAMtFzzeW1-Y0_-9AeEjGxP6Vda16KYLq)
# 进程间的消息传递(CString值传递
有自定义消息：
#define WM_TEST WM_USER+121
 消息处理函数：
afx_msg void OnTest(WPARAM wParam,LPARAM lParam); 该消息是一个主线程向辅助线程发送数据的消息。
主线程中发送消息的代码：
       m_param.pDoc=pDoc;
    m_param.pSpecAnlyz=m_pSpecAnlyz;
//    CWinThread* pThread=AfxBeginThread(ThreadTest,static_cast<LPVOID>(&m_param));
    m_pTestThread=(CTestThread*)AfxBeginThread(RUNTIME_CLASS(CTestThread));
    Sleep(200);
    // 必须先把&m_param转化为void*指针，然后才可以进一步强制转化为WPARAM
    m_pTestThread->PostThreadMessageW(WM_TEST,(WPARAM)((void*)&m_param),1);// 发送测试消息  m_param为自定义类型，其成员为指针变量，因此 m_param无法强制转化为WPARAM。但是m_param的地址指针和WPARAM一样，都是4个字节，因此可以将m_param的地址指针起那个只转换为WPARAM。在VC.NET2005中，直接转换编译报错。考虑到void * 之后，先将&m_param转化为void*指针，然后才可以进一步强制转化为WPARAM。
同样的，在OnTest中，必须先将wParam强制转换为void * ，然后进一步转换为其他类型。
PS：网上有高手指出，局部指针变量最好不要作为wParam或 lParam传递，因为当消息响应时改变量可能已经不在了。
//==================================================================================================================
备注：
1》其实也就是传个32位指针值而已，其它的具体数据值就是通过这个指针去访问去做文章了，不过要求传全局的指针，在堆/栈上的指针很可以被系统
收回而成为非法指针！
如下形式...
unsigned char  g_Buf[1024];
SendMessage(hWnd, xxx_Msg, xxx_wParam,(LPARAM) g_Buf);
2》其实传指针只能限于同一个进程内传递的，不同进程间传递很容易造成对指针所指向的内容的非法访问
3》不同进程间传指针最好使用WM_COPYDATA 消息来传，它内建有共享内存机制；但是WM_COPYDATA 消息必须使用SendMessage来发，不能使用PostMessage！！！
