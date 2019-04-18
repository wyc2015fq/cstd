# C++多个文本读取问题 - wishchinYang的专栏 - CSDN博客
2015年09月11日 14:21:47[wishchin](https://me.csdn.net/wishchin)阅读数：538
个人分类：[C++编程																[C+/代码迁移](https://blog.csdn.net/wishchin/article/category/1552163)](https://blog.csdn.net/wishchin/article/category/1508333)
                
        同时使用两个 ifstream和 freopen 第二个就会失去效用，不知道错在了哪里！
1. 使用freopen打开：
```cpp
bool CPicToolsDlg::readTxt2Seq(
	std::string TxtName, std::vector<float >  &Seq )
{
	std::string TestTxt(TxtName);  //  
	freopen(TestTxt.c_str(),"r",stdin);// 打开文件！
	float  ViewValue;
	Seq.resize(0);
	char ch;
	while(std::cin  )
	{
		std::cin >> ViewValue;  
		Seq.push_back(ViewValue);
		
	}
	///fin.close();
	fclose(stdin);
	return true;
}
```
2. 使用C格式的fopen打开
```cpp
bool CPicToolsDlg::readTxt2Seq1(
	std::string TxtName, std::vector<float >  &Seq )
{
	std::string TestTxt(TxtName);  //  
	FILE *fp  = NULL;
	if ((fp = fopen(TestTxt.c_str(), "r")) == NULL){
		printf("Error in open file %s\n", TestTxt.c_str());
		return false;
	}
	else{
		printf(" file  opened%s\n", TestTxt.c_str());
	}
	//三元组，使用三元组逐个写入
	float  ViewValue;
	Seq.resize(0);
	//char Temp[100];
	//if (fin.is_open() )
	//{
	//	fin.getline(Temp,100);
	//	int i    = 0;
	//	while(i< 100 && Temp[i] !="\0" )//寻址级别不同，到底问题出在哪里了！
	//	{
	//		float  ViewValue(Temp[i] );
	//		Seq.push_back(ViewValue);
	//		++ i;
	//	}
	//}
	//流出现问题
	//if (fin.is_open() )	{
	//	while( fin.get(ch) )//寻址级别不同，到底问题出在哪里了！
	//	{
	//		if (ch !=(int)(" ") ){
	//			float  ViewValue(ch );
	//			//fin >> ViewValue;     //利用 重定向输入 浮点值  
	//			//std::cin >>ViewValue2; 
	//			//std::cin >>ViewValue3; 
	//			//P.x =  ViewValue;
	//			//P.y = ViewValue2;
	//			//P.z = ViewValue3;
	//			Seq.push_back(ViewValue);
	//		}
	//	}
	//}
	
	while (!feof(fp))//首先读取点云数据到点表points同时指定点对应的id:
	{
		int ret = fscanf(fp,"%f", &ViewValue);
		Seq.push_back(ViewValue);
	}//printf("%d\n", n);
	fclose(fp);
	//fin.close();
	//fclose(stdin);
	return true;
}
```
          使用C语言是注意事项，int ret = fscanf(fp,"%lf", &ViewValue);   lf 和f 有何不同之处？
3.修改后的代码，第一个使用freopen，第二个使用fopen可以正常运行了
```cpp
void CPicToolsDlg::OnBnClickedButtonKmptest()
{
	//字符串匹配
	//打开目录文件
	CFileDialog FileDlg(
		TRUE, _T("*.pcd"), NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("*.txt|*.txt| *.pcd|*.pcd| All Files (*.*) |*.*||"), NULL);//选取约定！	
	FileDlg.m_ofn.lpstrTitle = _T("打开待显示序列文本！！！");	// 打开文件对话框的标题名
	if (FileDlg.DoModal() != IDOK)	{
		MessageBox("打开文件失败！！！请重新检测", "提示", 0);
		return ;
	}
	CString FilePath = FileDlg.GetPathName();
	std::string MyFileName = FilePath;
	std::vector<float >   Seq(0);
	this->readTxt2Seq(MyFileName,Seq);
	CFileDialog FileDlg2(
		TRUE, _T("*.pcd"), NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("*.txt|*.txt| *.pcd|*.pcd| All Files (*.*) |*.*||"), NULL);//选取约定！	
	FileDlg2.m_ofn.lpstrTitle = _T("打开待显示模式文本！！！");	// 打开文件对话框的标题名
	if (FileDlg2.DoModal() != IDOK)	{
		MessageBox("打开文件失败！！！请重新检测", "提示", 0);
		return ;
	}
	CString FilePath2 = FileDlg2.GetPathName();
	std::string MyFileName2       = FilePath2;
	std::vector<float >   P(0);
	this->readTxt2Seq1(MyFileName2,P);
	const std::vector<float>  Pattern(P);
	int Pos =
		this->Kmptest(Seq ,Pattern );
}
```

