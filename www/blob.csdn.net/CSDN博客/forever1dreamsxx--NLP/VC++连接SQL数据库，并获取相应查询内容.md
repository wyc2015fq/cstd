
# VC++连接SQL数据库，并获取相应查询内容 - forever1dreamsxx--NLP - CSDN博客


2013年10月25日 16:13:26[forever1dreamsxx](https://me.csdn.net/forever1dreamsxx)阅读数：3764


前几天用VC++，获取SQL中的内容并保存，由于不是很熟悉，走了很多弯路，现在总结哈，希望能给需要的人一些帮助。
直接贴代码。
```python
#include "string"
#include "map"
//连接SQL引用的
#import "C:\Program Files\Common Files\System\ado\msado15.dll"no_namespace rename("EOF","ADOEOF")
using namespace std;
//你自己定义的函数
void XXXDlg::YourFun() 
{
	// TODO: Add your control notification handler code here
	::CoInitialize(NULL);
	_ConnectionPtr m_pConnection("ADODB.Recordset");
	_RecordsetPtr m_pRecordset("ADODB.Recordset");
	_CommandPtr m_pCommand;
	//连接数据库信息，写在ini文件中的，filepath是ini文件的路径
	//ini文件内容：
	//[Info]
	//Database = driver={SQL Server};Server=数据库电脑ip地址（比如本机127.0.0.1）,1433;Database=数据库名称;
	//name = 数据库名字
	//pwd = 你的数据库密码
	//filepath1 = F:/
	//filepath2 = .txt
	CString Database;
	GetPrivateProfileString("Info", "Database", "", Database.GetBuffer(MAX_PATH), MAX_PATH, filepath);
	
	//将获取的数据库链接字段，转换成指定格式
	_bstr_t strConnect = Database;
	
	//数据库连接
	try
	{
		m_pConnection.CreateInstance(__uuidof(Connection));
		CString name;
		CString pwd;
		GetPrivateProfileString("Info", "name", "", name.GetBuffer(MAX_PATH), MAX_PATH, filepath);
		GetPrivateProfileString("Info", "pwd", "", pwd.GetBuffer(MAX_PATH), MAX_PATH, filepath);
		_bstr_t name1 = name;
		_bstr_t pwd1 = pwd;
		HRESULT ConnectResult = m_pConnection->Open(strConnect,name1,pwd1,adModeUnknown);
		if(SUCCEEDED(ConnectResult))
		{
			MessageBox("连接数据库成功！");
		}
		
	}
	catch(_com_error e)
	{
		MessageBox("连接数据库失败！");
		return;
	}
	
	//查询相关内容信息
	try
	{
		_bstr_t CommandText = "select a.字段1,b.字段2,b.字段3,b.字段4 from 表名1 a,表名2 b where 相关条件";
		//m_pConnection->Execute(CommandText, NULL, adCmdText);
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		m_pRecordset->Open(CommandText, m_pConnection.GetInterfacePtr(), 
			adOpenDynamic, adLockOptimistic, adCmdText);
		_variant_t 字段1Result;
		_variant_t 字段2Result;
		_variant_t 字段3Result;
		_variant_t 字段4Result;
		//我是将读出来的信息，保存到txt文件中
		//下面是从ini文件中读取保存文件的地址
		CString filepath1;
		CString filepath2;
		GetPrivateProfileString("Info", "filepath1", "", filepath1.GetBuffer(MAX_PATH), MAX_PATH, filepath);
		GetPrivateProfileString("Info", "filepath2", "", filepath2.GetBuffer(MAX_PATH), MAX_PATH, filepath);
		string m1 = filepath1.GetBuffer(0);
		string m2 = filepath2.GetBuffer(0);
		CString m3 = m1.c_str();
		CString m4 = m2.c_str();		
	
		//定义一个map容器以免，重复操作文件的打开关闭操作
		map<string, auto_ptr<CStdioFile> > filename;
		//遍历查询结果，并保存
		while(!m_pRecordset->ADOEOF)
		{		
			字段1Result = m_pRecordset->GetCollect("字段1");
			CString 字段1Results;
			字段Results=(LPCSTR)_bstr_t(字段1Result);
			CString fileEnd = m3 + 字段1Results + m4;
		
			//我是以 字段1 为文件名保存其他三个字段的内容
			CStdioFile* pfile = NULL;	
			if(filename.find((LPCSTR)fileEnd)!=filename.end())
			{
				pfile = filename[(LPCSTR)fileEnd].get();
			}
			else
			{
				pfile = new CStdioFile(fileEnd,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);
				filename[(LPCSTR)fileEnd] = auto_ptr<CStdioFile>(pfile);
			}
			字段2Result = m_pRecordset->GetCollect("字段2");
			CString 字段2Results;
			字段2Results=(LPCSTR)_bstr_t(字段2Result);	
			pfile->SeekToEnd();//先定位到文件尾部
			pfile->WriteString(字段2Results);//写入文件	
			pfile->WriteString("\t");//每个字段内容用talbe键分开
				
			字段3Result = m_pRecordset->GetCollect("字段3");
			CString 字段3Results;
			字段3Results=(LPCSTR)_bstr_t(字段3Result);
			pfile->SeekToEnd();//先定位到文件尾部
			pfile->WriteString(字段3Results);	
			pfile->WriteString("\t");
				
			字段4Result = m_pRecordset->GetCollect("字段4");
			CString 字段4Results;
			字段4Results=(LPCSTR)_bstr_t(字段4Result);
			pfile->SeekToEnd();//先定位到文件尾部
			pfile->WriteString(字段4Results);	
			pfile->WriteString("\r\n");//每条记录一行，记录完换行
				
			m_pRecordset->MoveNext();	//记录完一条记录，指向下一条记录	
		}
		//循环关闭打开的文件指针
		map<string,auto_ptr<CStdioFile> >::const_iterator map_it = filename.begin();
		while(map_it!=filename.end())
		{
			(map_it->second)->Close();
			map_it++;		}
		filename.clear();
		//MessageBox(s);
	}
	catch(_com_error e)
	{
		return;
	}
}
```
才疏学浅，如有不对，望海涵，哈哈。


