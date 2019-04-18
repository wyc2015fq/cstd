# showModalDialog打开的页面需要跳转或刷新 - z69183787的专栏 - CSDN博客
2013年11月16日 19:12:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1631
**最终解决：ModalDialog只能使用form提交本页面后，跳转至需要的页面。这样子页面任意跳转后，父页面也还能取得子页面的数据。**
问题描述：使用showModalDialog打开模式窗口(假设为page1.html)，如果在page1.html中执行location.href='page2.html'执行页面跳转，则浏览器会在新窗口中打开page2.html，而不会将page1.html跳转到page2.html；
上述情况，在很多场合下都常遇到，具体解决方法如下：
1、在showModalDialog打开的窗口中放入iframe或frameset，然后在此框架中显示page1.html，此时再在page1.html中执行location.href=‘page2.html’则会正常跳转至page2.html，而不会打开一个新窗口；
2、如果不用iframe显示page1.html，则可以将location.href='page2.html'替换为window.name = "dialogPage"; window.open("page2.html","dialogPage");此时就可以实现正常的页面跳转；[查看示例](http://www24.brinkster.com/bryan2u/showModalDialogOpener.htm)
使用此方法如果是刷新本页面，即window.open('page1.html','dialogPage')，可能会由于页面缓存的原因，而使得页面无法更新；此时可以设置页面不允许缓存(Response.Cache.SetNoStore())解决该问题；
附：
如果在showModalDialog打开的页面page1.html中往父页面传值，可以使用dialogArguments.document.getElementById("目标控件ID").innerHTML进行赋值；
