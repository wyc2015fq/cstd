# C++查找一个目录下所有特定扩展名的文件 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月23日 21:41:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：30









记录一个小技巧，查找一个目录下所有特定扩展名的的文件名。代码所示是查找所有后缀为“.txt”的文件名，并将结果存储于名为statfileurllist的vector中

注：也可以查找子目录



```cpp
//存储文件名列表
vector<CString> statfileurllist;
//查找TXT文件
WIN32_FIND_DATAA wfd;
CString sPath ="*.txt";//查找指定目录下的所有格式的文件。
//CString sPath ="reso\\*.xls";
HANDLE hFile = FindFirstFile(sPath.GetBuffer(),&wfd);
if(INVALID_HANDLE_VALUE == hFile)
{
	TRACE("不合法!\n");
	return;
}do{
	statfileurllist.push_back(wfd.cFileName);
	TRACE("%s\n",wfd.cFileName);
}while(FindNextFile(hFile,&wfd));
```










