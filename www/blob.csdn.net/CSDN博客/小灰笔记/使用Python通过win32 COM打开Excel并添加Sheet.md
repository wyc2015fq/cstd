# 使用Python通过win32 COM打开Excel并添加Sheet - 小灰笔记 - CSDN博客





2017年05月08日 21:47:00[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：3974








            对win32 COM不是很熟悉，不知道一个程序究竟有多少属性或者方法可以操作。仅仅是一个Sheet页的添加就费了我好长时间，因为这种成功来自于试探。

      编辑代码如下：

#!/usr/bin/python



from win32com.client import Dispatch



xlApp = Dispatch('Excel.Application')

xlApp.Visible = True

xlApp.Workbooks.Add()

xlApp.Worksheets.Add()

      程序运行结果：

![](https://img-blog.csdn.net/20170508214612714?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




      我用的Excel版本默认打开的时候只有一个Sheet页，通过以上操作后打开的时候出现了两个Sheet页，可见创建Sheet页成功。

      如果要指明Sheet页的名字，那么就得在创建的时候修改一个属性。修改代码如下：

#!/usr/bin/python



from win32com.client import Dispatch



xlApp = Dispatch('Excel.Application')

xlApp.Visible = True

xlApp.Workbooks.Add()

xlApp.Worksheets.Add().Name = 'test'

xlSheet = xlApp.Worksheets('test')

xlSheet.Cells(1,1).Value = 'title'

xlSheet.Cells(2,1).Value = 123

      程序执行结果如下：

![](https://img-blog.csdn.net/20170508214623183?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




      从上面的结果可以看出，操作不仅实现了创建新的Sheet页同时还实现了给新创建的Sheet页命名的功能。在创建并命名结束后，又通过名称所引导这个Sheet页实现了信息的写入。



