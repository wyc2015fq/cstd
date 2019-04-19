# VC中采用不同的方法创建字体 - xqhrs232的专栏 - CSDN博客
2010年08月27日 11:04:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1179标签：[文档																[null																[工作](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=文档&t=blog)
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://blog.csdn.net/wubind/archive/2007/12/26/1968042.aspx](http://blog.csdn.net/wubind/archive/2007/12/26/1968042.aspx)
采用不同的方法创建字体
2007-12-15 10:08
[例3.8]编写一个单文档的应用程序Li3_8,采用不同的方法创建字体，并根据创建的字体输出不同的文本。
（1）       使用MFCAppWizard创建一个单文档应用程序Li3_8.
（2）       选择项目工作区的ClassView,展开Cli3_8View类，打开成员函数OnDraw().
（3）       在函数OnDraw()中添加如下代码。
void CLi3_8View::OnDraw(CDC* pDC)
{
       CLi3_8Doc* pDoc = GetDocument();
       ASSERT_VALID(pDoc);
       // TODO: add draw code for native data here
    CString outstr[5];
    outstr[1]="1.使用函数CreatPointFont()创建字体字";
    outstr[2]="2.使用函数CreatFontIndirect()创建倾斜、带下划线的黑体字";
    outstr[3]="3.使用函数CreateFont()创建带删除线的大号字";
    outstr[4]="4.使用库存字体对象创建ANSI标准的等宽字";
    CFont *OldFont,NewFont;
    LOGFONT MyFont={
        30,
        10,
        0,
        0,
        0,
        1,
        1,
        0,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH,
        "黑体"
    };
    pDC->TextOut(0,10,"创建字体的几种方法:");
    for(int i=1;i<5;i++){
        switch(i){
        case 1:
            //使用函数CreatPointFont()创建字体
            NewFont.CreatePointFont(200,"宋体",NULL);
            break;
        case 2:
            //使用函数CreateFontIndirect()创建字体
            NewFont.CreateFontIndirect(&MyFont);
            break;
        case 3:
            //使用函数CreateFont()创建字体
            NewFont.CreateFont(30,10,0,0,FW_HEAVY,false,false,
                               true,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
                               CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
                               DEFAULT_PITCH|FF_DONTCARE,"大号字");
            break;
        case 4:
            //使用库存字体对象创建字体
            pDC->SelectStockObject(ANSI_FIXED_FONT);
            break;
        }
        OldFont=pDC->SelectObject(&NewFont);
        pDC->TextOut(0,60*i,outstr[i]);
        pDC->SelectObject(OldFont);
        NewFont.DeleteObject();
    }
} 
（4）       编译、链接并运行程序，结果如图3.9所示。
![](https://p-blog.csdn.net/images/p_blog_csdn_net/wubind/untitled.JPG)
图3.9创建字体的不同方法
例程总结：
    经过例程的学习研究，我掌握了四种方法来创建四种不同的字体的方法：
　 1.使用函数CreatePointFont()创建宋体字
　 2.使用函数CreateFontIndirect()创建倾斜、带下划线的黑体字
3.使用函数CreateFont()创建带删除线的大号字
    4.使用库存字体对象创建ANSI标准的等宽字
    我已经掌握了使用CFont创建新字体对象和旧字体对象指针的方法,我已经能够使用LOGFONT来创建一个结构体变量，并将它传送给函数CreateFontIndirect()来创建一个新字体，并了解各个变量的意义，我懂得了使用文档指针来调用它的成员函数来输出新字体，使用完新字体之后，恢复旧字体，并删除旧字体。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/wubind/archive/2007/12/26/1968042.aspx](http://blog.csdn.net/wubind/archive/2007/12/26/1968042.aspx)
