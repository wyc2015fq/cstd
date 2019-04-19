# GetDocument() - 三少GG - CSDN博客
2012年02月24日 20:54:19[三少GG](https://me.csdn.net/scut1135)阅读数：1718标签：[文档																[mfc](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=文档&t=blog)
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
视图对象是用来显示文档对象的内容，函数GetDocument()用于获取当前文档对象的指针m_pDocument.
 而函数OnDraw()是一个虚函数，负责文档对象的数据在用户视图区的显示输出。在向导生成的成员函数OnDraw()中调用了函数GetDocument().通过获取的文档类指针可 以在视图中显示文档内容。
void CMysdiView::OnDraw(CDC* pDC)
{
CMysdiDoc* pDoc=GetDocument();
ASSERT_VALID(pDoc);
pDC->TextOut(100,100,pDoc->m_szText);
//m_szText是文档成员变量
}
【[http://hi.baidu.com/luckyhare/blog/item/26e985a7e8a71a98d043580b.html](http://hi.baidu.com/luckyhare/blog/item/26e985a7e8a71a98d043580b.html)】
>>[获取MFC窗口其它类的指针](http://hi.baidu.com/core22/blog/item/1216164cf03d68ffd62afc4c.html)
>>[单文档与多文档](http://wenku.baidu.com/view/fc3336d3240c844769eaee53.html)
