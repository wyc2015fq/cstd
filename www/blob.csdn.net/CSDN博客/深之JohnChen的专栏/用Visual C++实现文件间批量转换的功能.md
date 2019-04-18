# 用Visual C++实现文件间批量转换的功能 - 深之JohnChen的专栏 - CSDN博客

2006年03月29日 16:01:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1714


用Visual C++实现文件间批量转换的功能

一、前言

　　本人在开发惠普色谱工作站增强软件的过程中，实现了把指定目录下的色谱数据文件全部转换成纯文本文件的功能。下面，通过用一个具体的例子来说明此功能的实现方法。

　　全部代码用Visual C++6.0在Windows95/98/2000下编译通过。

　　二、实例

　　首先用MFC AppWizard生成一个SDI风格的应用程序test，生成过程中全部使用缺省设置。

　　其次，利用资源编辑器，在主菜单“文件”下增加一个菜单项“转换”，属性为：

　　　ID:ID_CONVERT

　　　Caption: 转换

　　　Prompt: 在不同格式文件之间进行转换/n转换文件

　　然后用“CTRL-W”热键激活MFC ClassWizard，为CmainFrame类增加响应ID_CONVERT消息的命令函数OnConvert()。加入转换功能的代码如下所示：

　　　void CMainFrame::OnConvert() 
　　　　{
　　　　　LPMALLOC pMalloc;//利用shell扩展功能
　　　　　BROWSEINFO bi;
　　　　　if (SUCCEEDED(SHGetMalloc(&pMalloc)))//为生成目录选择对话框分配自由内存
　　　　　　{
　　　　　　　ZeroMemory(&bi,sizeof(bi));//清零分配的空间
　　　　　　　char pszDirName[MAX_PATH];//存放选择的目录名
　　　　　　　LPITEMIDLIST pidl;
　　　　　　　bi.hwndOwner = GetSafeHwnd();
　　　　　　　bi.pidlRoot = NULL;
　　　　　　　bi.pszDisplayName = pszDirName;
　　　　　　　bi.lpszTitle = _T("选择要批量转换文件所在的目录");
　　　　　　　bi.ulFlags = BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;
　　　　　　　bi.lpfn = NULL;
　　　　　　　bi.lParam = 0;
　　　　　　　if ((pidl = ::SHBrowseForFolder(&bi)) != NULL)//调用选择目录对话框
　　　　　　　　{
　　　　　　　　　if (::SHGetPathFromIDList(pidl, pszDirName))//获得所选择的目录
　　　　　　　　　　{ 
　　　　　　　　　　　file://设置选择的目录为当前目录，以便查找
　　　　　　　　　　　　SetCurrentDirectory(pszDirName);
　　　　　　　　　　　　file://定义一个查找
　　　　　　　　　　　　CFileFind findch1;
　　　　　　　　　　　　CString strconv;
　　　　　　　　　　　　CString strsour;
　　　　　　　　　　if(findch1.FindFile("*.CH1"))//在当前目录进行查找
　　　　　　　　　　　{
　　　　　　　　　　　　CFile SourceFile;
　　　　　　　　　　　　CStdioFile TargetFile;
　　　　　　　　　　　　BOOL bfindresult;
　　　　　　　　　　　　do
　　　　　　　　　　　　{
　　　　　　　　　　　　　file://查找下一个符合条件的文件
　　　　　　　　　　　　　　bfindresult= findch1.FindNextFile();
　　　　　　　　　　　　　　file://获得查找到的文件名
　　　　　　　　　　　　　　strsour=findch1.GetFilePath();
　　　　　　　　　　　　　　strconv=strsour;
　　　　　　　　　　　　　　file://把文件名转换为小写
　　　　　　　　　　　　　　strconv.MakeLower();
　　　　　　　　　　　　　　file://把*.ch1类型的文件转换为*.txt
　　　　　　　　　　　　　　strconv.Replace(".ch1",".txt");
　　　　　　　　　　　　　　file://打开*.ch1类型的文件作为源文件
　　　　　　　　　　　　　　SourceFile.Open(strsour,CFile::modeRead);
　　　　　　　　　　　　　　file://打开*.txt类型的文件作为目标文件
　　　　　　　　　　　　　　TargetFile.Open(strconv,CFile::modeCreate|CFile::modeWrite);

　　　　　　　　　　　　　　file://此处调用*.ch1类型的文件的解码函数
　　　　　　　　　　　　　　file://此处调用转换成文本文件的函数 
　　　　　　　　　　　　　　file://文件使用完毕，要关闭
　　　　　　　　　　　　　　SourceFile.Close();
　　　　　　　　　　　　　　TargetFile.Close();
　　　　　　　　　　　　　}while(bfindresult);
　　　　　　　　　　　　　MessageBox("转换完毕！","转换完毕！",MB_OK);
　　　　　　　　　　　　}
　　　　　　　　　　else
　　　　　　　　　　　{
　　　　　　　　　　　　MessageBox("没找到CH1文件","没找到",MB_OK);
　　　　　　　　　　　}
　　　　　　　　　　findch1.Close();//关闭这个搜索
　　　　　　　　　}
　　　　　　　　pMalloc->Free(pidl);//释放使用完的资源
　　　　　　　　}
　　　　　　　pMalloc->Release();//释放使用完的资源
　　　　　　}
　　　　　}

　　编译并运行程序，选择“文件”菜单下的“转换”命令， 选择一个目录就完成了对此目录下所有具有.ch1扩展名的文件的转换工作。

　　三、结论

　　本程序利用了Windows95/98/2000下的shell扩展功能，实现了对一个用户指定目录的拾取，再利用MFC的CfileFind类，来检索出所有想要转换的源文件，最后通过编码转换把源文件内容输出到指定类型的目标文件。这个方法也适合于要大量在不同格式的文件之间进行转换的工作，如图形文件、声音文件等的转换。希望有兴趣的朋友和我交流讨论。


