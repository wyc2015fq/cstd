
# 怎样将GIS图形复制到Windows剪贴板，粘贴到Word中 - 西西吹雪 - 博客园
# [编程　&　生活      --       西西吹雪](https://www.cnblogs.com/watsonyin/)
开心工作，快乐生活  (原创文章，转载请注明出处)

### 导航
[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/watsonyin/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/%E8%A5%BF%E8%A5%BF%E5%90%B9%E9%9B%AA)[订阅](https://www.cnblogs.com/watsonyin/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 -		88文章 -		10评论 -		1793引用 -		18
公告

[怎样将GIS图形复制到Windows剪贴板，粘贴到Word中](https://www.cnblogs.com/watsonyin/archive/2007/11/22/968651.html)
很久没写随笔了，有点想关了这个博客，不再更新，但还是没有下定决心。趁这些天比较闲，发一个以前写着玩的功能吧。
复制、粘贴功能是一件很简单的事情，但怎样将GIS图形粘贴到word中呢？最简单的思路还是将GIS图形保存为Image形式，发送到剪贴板，再粘贴到Word。但如果Image格式采用栅格的格式，如BMP或JPEG，则粘贴后在Word中拉伸时，效果会受到影响，比较合适的格式是采用WMF或EMF这种矢量的格式。将Visio格式复制到Word中时，可以看到就是这种效果，而且文字还是可以编辑的。好，下面我们就看看怎样实现这个功能。
第一步当然是将图形复制为一个MetaFile对象：
![](/Images/OutliningIndicators/None.gif)publicMetafile GetGeometryMetafile(ISymbol pSymbol, IGeometry pGeometry)
![](/Images/OutliningIndicators/ExpandedBlockStart.gif)![](/Images/OutliningIndicators/ContractedBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](/Images/OutliningIndicators/InBlock.gif)//先获取一个IDisplayTransformation对象
![](/Images/OutliningIndicators/InBlock.gif)IDisplayTransformation pDisTrans=newDisplayTransformationClass();
![](/Images/OutliningIndicators/InBlock.gif)IEnvelope pEnv=pGeometry.Envelope;
![](/Images/OutliningIndicators/InBlock.gif)Rectangle rect=newRectangle(0,0,500,500);
![](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)tagRECT r;
![](/Images/OutliningIndicators/InBlock.gif)r.left=rect.Left;
![](/Images/OutliningIndicators/InBlock.gif)r.right=rect.Right;
![](/Images/OutliningIndicators/InBlock.gif)r.bottom=rect.Bottom;
![](/Images/OutliningIndicators/InBlock.gif)r.top=rect.Top;
![](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)pDisTrans.set_DeviceFrame(refr);
![](/Images/OutliningIndicators/InBlock.gif)pDisTrans.Bounds=pEnv;
![](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)pDisTrans.Resolution=72;
![](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)pDisTrans.ReferenceScale=1.0;
![](/Images/OutliningIndicators/InBlock.gif)pDisTrans.ScaleRatio=1;
![](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)//创建WMF
![](/Images/OutliningIndicators/InBlock.gif)MemoryStream ms=newMemoryStream();
![](/Images/OutliningIndicators/InBlock.gif)Graphics g=CreateGraphics();
![](/Images/OutliningIndicators/InBlock.gif)IntPtr hdc=g.GetHdc();
![](/Images/OutliningIndicators/InBlock.gif)Metafile mf=newMetafile(ms, hdc,newRectangle(0,0,500,500),MetafileFrameUnit.Pixel, EmfType.EmfPlusDual);
![](/Images/OutliningIndicators/InBlock.gif)g.ReleaseHdc(hdc);
![](/Images/OutliningIndicators/InBlock.gif)g.Dispose();
![](/Images/OutliningIndicators/InBlock.gif)g=Graphics.FromImage(mf);
![](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)g.FillRectangle(newSolidBrush(Color.White),newRectangle(0,0,500,500));
![](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)pSymbol.SetupDC((int)g.GetHdc(), pDisTrans);
![](/Images/OutliningIndicators/InBlock.gif)pSymbol.Draw(pGeometry);
![](/Images/OutliningIndicators/InBlock.gif)pSymbol.ResetDC();
![](/Images/OutliningIndicators/InBlock.gif)g.ReleaseHdc();
![](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)g.DrawString("by watson",this.Font,newSolidBrush(Color.Blue),newPointF(20,20));
![](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)g.Save();
![](/Images/OutliningIndicators/InBlock.gif)g.Dispose();
![](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)returnmf;
![](/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
第二步：发送到剪贴板：
![](/Images/OutliningIndicators/None.gif)IDataObject data=newDataObject();
![](/Images/OutliningIndicators/None.gif)data.SetData(DataFormats.MetafilePict, mf);
![](/Images/OutliningIndicators/None.gif)Clipboard.SetDataObject(data,true);
测试 .... 什么，不成功？？
估计是.net不支持将metafile格式复制到剪贴板吧。
于是，用于搜索引擎，搜索一翻，在某国外论坛找到解决方案：
![](/Images/OutliningIndicators/None.gif)publicclassClipboardMetafileHelper
![](/Images/OutliningIndicators/ExpandedBlockStart.gif)![](/Images/OutliningIndicators/ContractedBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](/Images/OutliningIndicators/InBlock.gif)[DllImport("user32.dll")]
![](/Images/OutliningIndicators/InBlock.gif)staticexternboolOpenClipboard(IntPtr hWndNewOwner);
![](/Images/OutliningIndicators/InBlock.gif)[DllImport("user32.dll")]
![](/Images/OutliningIndicators/InBlock.gif)staticexternboolEmptyClipboard();
![](/Images/OutliningIndicators/InBlock.gif)[DllImport("user32.dll")]
![](/Images/OutliningIndicators/InBlock.gif)staticexternIntPtr SetClipboardData(uintuFormat, IntPtr hMem);
![](/Images/OutliningIndicators/InBlock.gif)[DllImport("user32.dll")]
![](/Images/OutliningIndicators/InBlock.gif)staticexternboolCloseClipboard();
![](/Images/OutliningIndicators/InBlock.gif)[DllImport("gdi32.dll")]
![](/Images/OutliningIndicators/InBlock.gif)staticexternIntPtr CopyEnhMetaFile(IntPtr hemfSrc, IntPtr hNULL);
![](/Images/OutliningIndicators/InBlock.gif)[DllImport("gdi32.dll")]
![](/Images/OutliningIndicators/InBlock.gif)staticexternboolDeleteEnhMetaFile(IntPtr hemf);
![](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)//Metafile mf is set to an invalid state inside this function
![](/Images/OutliningIndicators/InBlock.gif)staticpublicboolPutEnhMetafileOnClipboard(IntPtr hWnd, Metafile mf)
![](/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](/Images/OutliningIndicators/InBlock.gif)boolbResult=false;
![](/Images/OutliningIndicators/InBlock.gif)IntPtr hEMF, hEMF2;
![](/Images/OutliningIndicators/InBlock.gif)hEMF=mf.GetHenhmetafile();//invalidates mf
![](/Images/OutliningIndicators/InBlock.gif)if(!hEMF.Equals(newIntPtr(0)))
![](/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](/Images/OutliningIndicators/InBlock.gif)hEMF2=CopyEnhMetaFile(hEMF,newIntPtr(0));
![](/Images/OutliningIndicators/InBlock.gif)if(!hEMF2.Equals(newIntPtr(0)))
![](/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](/Images/OutliningIndicators/InBlock.gif)if(OpenClipboard(hWnd))
![](/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](/Images/OutliningIndicators/InBlock.gif)if(EmptyClipboard())
![](/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](/Images/OutliningIndicators/ContractedSubBlock.gif)IntPtr hRes=SetClipboardData(14/**//*CF_ENHMETAFILE*/, hEMF2);
![](/Images/OutliningIndicators/InBlock.gif)bResult=hRes.Equals(hEMF2);
![](/Images/OutliningIndicators/InBlock.gif)CloseClipboard();
![](/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)}
![](/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)}
![](/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)}
![](/Images/OutliningIndicators/InBlock.gif)DeleteEnhMetaFile(hEMF);
![](/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)}
![](/Images/OutliningIndicators/InBlock.gif)returnbResult;
![](/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)}
![](/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
调用PutEnhMetafileOnClipboard方法就可以了，
再测试....大功告成！
再看一看PutEnhMetafileOnClipboard中的代码，比较特别的是CopyEnhMetaFile函数，将MetaFile在内存中复制一份，看来是.net中的metafile与Windows中的metafile格式有一定出入，windows剪贴板不认识的缘故吧。





posted on2007-11-22 15:17[西西吹雪](https://www.cnblogs.com/watsonyin/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=968651)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)






Powered by:
博客园
Copyright © 西西吹雪
