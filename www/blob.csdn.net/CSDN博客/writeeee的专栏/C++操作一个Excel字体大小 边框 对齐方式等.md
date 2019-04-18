# C++操作一个Excel字体大小 边框 对齐方式等 - writeeee的专栏 - CSDN博客
2015年10月16日 14:17:13[writeeee](https://me.csdn.net/writeeee)阅读数：2866
```cpp
</pre><pre name="code" class="cpp">
```
```cpp
void CMy12Dlg::OnOK() 
{
	// TODO: Add extra validation here
//用m_ExlApp对象创建Excel2003进程 
       if(!m_ExlApp.CreateDispatch("Excel.Application",NULL))
       { 
              AfxMessageBox("创建Excel服务失败!");
              return;
       } 
       //设置为可见 
       m_ExlApp.SetVisible(TRUE); 
       ///////////////////下面得到应用程序所在的路径/////////////////// 
       CString theAppPath,theAppName; 
       char Path[MAX_PATH]; 
       GetModuleFileName(NULL,Path,MAX_PATH);//得到应用程序的全路径 
       theAppPath=(CString)Path; 
       theAppName=AfxGetApp()->m_pszAppName; 
       theAppName+=".exe"; 
       //把最后的文件名去掉 
       int length1,length2; 
       length1=theAppPath.GetLength(); 
       length2=theAppName.GetLength(); 
       theAppPath.Delete(length1-length2,length2); 
    //////////////////////////////////////////////////////////////// 
       CString TempPath=""; 
       TempPath=theAppPath+"Template.xls";//EXCEL模板的路径 
       m_ExlBooks.AttachDispatch(m_ExlApp.GetWorkbooks(),TRUE); 
       m_ExlBook.AttachDispatch(m_ExlBooks.Add((_variant_t)TempPath),TRUE);//加载EXCEL模板 
       m_ExlSheets.AttachDispatch(m_ExlBook.GetSheets(),TRUE);//加载Sheet页面 
       //添加新的Sheet页面 
       m_ExlSheets.Add(vtMissing,vtMissing,_variant_t((long)1),vtMissing); 
       //删除第二个Sheet页面 
       m_ExlSheet.AttachDispatch(m_ExlSheets.GetItem(_variant_t((long)2)),TRUE); 
       m_ExlSheet.Delete(); 
       //把第一个Sheet页面的名字改变为TestSheet 
       m_ExlSheet.AttachDispatch(m_ExlSheets.GetItem(_variant_t((long)1)),TRUE); 
       m_ExlSheet.SetName("TestSheet"); 
       ///////合并第一行单元格A1至D1////// 
       //加载要合并的单元格 
       m_ExlRge.AttachDispatch(m_ExlSheet.GetRange(_variant_t("A1"),_variant_t("D1")),TRUE); 
       m_ExlRge.Merge(_variant_t((long)0)); 
       ////////设置表格内容//////// 
       m_ExlRge.AttachDispatch(m_ExlSheet.GetCells(),TRUE);//加载所有单元格 
       m_ExlRge.SetItem(_variant_t((long)1),_variant_t((long)1),_variant_t("数学系研究生课程统计")); 
       m_ExlRge.SetItem(_variant_t((long)2),_variant_t((long)1),_variant_t("课程名")); 
       m_ExlRge.SetItem(_variant_t((long)2),_variant_t((long)2),_variant_t("课时")); 
       m_ExlRge.SetItem(_variant_t((long)2),_variant_t((long)3),_variant_t("难度")); 
       m_ExlRge.SetItem(_variant_t((long)2),_variant_t((long)4),_variant_t("教学方式")); 
       m_ExlRge.SetItem(_variant_t((long)3),_variant_t((long)1),_variant_t("泛函分析")); 
       m_ExlRge.SetItem(_variant_t((long)3),_variant_t((long)2),_variant_t("60")); 
       m_ExlRge.SetItem(_variant_t((long)3),_variant_t((long)3),_variant_t("普通")); 
       m_ExlRge.SetItem(_variant_t((long)3),_variant_t((long)4),_variant_t("老师讲课")); 
       m_ExlRge.SetItem(_variant_t((long)4),_variant_t((long)1),_variant_t("微分流形")); 
       m_ExlRge.SetItem(_variant_t((long)4),_variant_t((long)2),_variant_t("40")); 
       m_ExlRge.SetItem(_variant_t((long)4),_variant_t((long)3),_variant_t("变态难")); 
       m_ExlRge.SetItem(_variant_t((long)4),_variant_t((long)4),_variant_t("自学")); 
       m_ExlRge.SetItem(_variant_t((long)5),_variant_t((long)1),_variant_t("二阶椭圆型方程与方程组")); 
       m_ExlRge.SetItem(_variant_t((long)5),_variant_t((long)2),_variant_t("60")); 
       m_ExlRge.SetItem(_variant_t((long)5),_variant_t((long)3),_variant_t("很难")); 
       m_ExlRge.SetItem(_variant_t((long)5),_variant_t((long)4),_variant_t("讨论")); 
       m_ExlRge.AttachDispatch(m_ExlSheet.GetUsedRange());//加载已使用的单元格 
       m_ExlRge.SetWrapText(_variant_t((long)1));//设置单元格内的文本为自动换行 
       //设置齐方式为水平垂直居中 
       //水平对齐：默认＝1,居中＝-4108,左＝-4131,右＝-4152 
       //垂直对齐：默认＝2,居中＝-4108,左＝-4160,右＝-4107 
       m_ExlRge.SetHorizontalAlignment(_variant_t((long)-4108)); 
       m_ExlRge.SetVerticalAlignment(_variant_t((long)-4108)); 
       ///////设置整体的字体、字号及颜色////// 
       Font ft; 
       ft.AttachDispatch(m_ExlRge.GetFont()); 
       ft.SetName(_variant_t("宋体"));//字体 
       ft.SetColorIndex(_variant_t((long)11));//字的颜色 
       ft.SetSize(_variant_t((long)12));//字号 
       ///////////设置标题字体及颜色////////// 
       m_ExlRge.AttachDispatch(m_ExlSheet.GetRange(_variant_t("A1"),_variant_t("D1"))); 
       ft.AttachDispatch(m_ExlRge.GetFont()); 
       ft.SetBold(_variant_t((long)1));//粗体 
       ft.SetSize(_variant_t((long)13)); 
       ft.SetColorIndex(_variant_t((long)2)); 
       CellFormat cf; 
       cf.AttachDispatch(m_ExlRge.GetCells()); 
       //////////////设置底色///////////////// 
       Interior it; 
       it.AttachDispatch(m_ExlRge.GetInterior()); 
       it.SetColorIndex(_variant_t((long)11));//标题底色 
       ////表格内容的底色//// 
       m_ExlRge.AttachDispatch(m_ExlSheet.GetRange(_variant_t("A2"),_variant_t("D5"))); 
       it.AttachDispatch(m_ExlRge.GetInterior()); 
       it.SetColorIndex(_variant_t((long)15)); 
       //////////////为表格设置边框///////////// 
       Range UnitRge; 
       CString CellName; 
       for(int i=1;i<=4;i++) 
       { 
              for(int j=1;j<=4;j++) 
              {     
                     CellName.Format("%c%d",j+64,i);//单元格的名称 
                     UnitRge.AttachDispatch(m_ExlRge.GetRange(_variant_t(CellName),_variant_t(CellName)));//加载单元格 
                     //LineStyle=线型 Weight=线宽 ColorIndex=线的颜色(-4105为自动) 
                     UnitRge.BorderAround(_variant_t((long)1),_variant_t((long)2),_variant_t((long)-4105),vtMissing);//设置边框 
              } 
       } 
       //释放对象（相当重要！） 
       m_ExlRge.ReleaseDispatch(); 
       m_ExlSheet.ReleaseDispatch(); 
       m_ExlSheets.ReleaseDispatch(); 
       m_ExlBook.ReleaseDispatch(); 
       m_ExlBooks.ReleaseDispatch(); 
       //m_ExlApp一定要释放，否则程序结束后还会有一个Excel进程驻留在内存中，而且程序重复运行的时候会出错 
       m_ExlApp.ReleaseDispatch(); 
       //退出程序 m_ExlApp.Quit(); 
	
//	CDialog::OnOK();
}
```
以上代码，需要先导出Excel的com接口调用类。
1.在VC中新建一控制台程序，选支持MFC（当然，你也可以不选择支持MFC的，不过会很麻烦） 
2.按CTRL+W调出MFC ClassWizard，Add Class->From a type library,   选择Excel.exe  选择 你需要的，"_"开头的应该是必须的。
