# WINCE-MFC-CDC使用出现的4KB内存泄露解决办法，坑爹的MFC - xqhrs232的专栏 - CSDN博客
2013年02月19日 00:22:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：806
原文地址::[http://blog.csdn.net/hnhyhongmingjiang/article/details/7993613](http://blog.csdn.net/hnhyhongmingjiang/article/details/7993613)
今天调试一个程序，为了偷懒想直接用以前的MFC的程序，顺手开了自己的内存监控软件，结果发现果然MFC存在一个BUG，其实也不算是BUG，只是没有在正常的使用中注意到特殊的用法
MFC的CDC一直会有4KB的内存泄露
HDC hdc = ::GetDC(m_hWnd);
CDC* pDC=CDC::FromHandle(hdc);//把这句注释掉,现象没有再重现
   // .........//中间全部注释
   // CDC::DeleteTempMap();//这点一定要加上，不然就会有4KB内存 泄露
     ::ReleaseDC(m_hWnd,hdc);
}
注:如果增加再手工调用CDC::DeleteTempMap();现象基本就不会出现了 ,按MSDN 的说法,这个函数应该会被CWinApp空闲处理OnIdle()自动调用以清除FromHandle创建的可能存在的临时对象.，还好一直都在产品上用的是WIN32写UI，没有碰到这么恶心的BUG
