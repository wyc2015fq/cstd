# C++发送HTTP请求获取网页HTML代码 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月10日 00:06:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：75
个人分类：[网络](https://blog.csdn.net/leixiaohua1020/article/category/8433201)










可以使用CInternetSession类发送HTTP请求获取网页HTML代码。

示例代码如下（strHtml用于存储HTML代码）：



```cpp
CInternetSession session;
CHttpFile *file = NULL;
CString strURL = " http://www.baidu.com";
CString strHtml = "”;   //存放网页数据

try{
       file = (CHttpFile*)session.OpenURL(strURL);

}catch(CInternetException * m_pException){
       file = NULL;
       m_pException->m_dwError;
       m_pException->Delete();
       session.Close();
       MessageBox("CInternetException");

}
CString strLine;
if(file != NULL){

       while(file->ReadString(strLine) != NULL){
       strHtml += strLine;
       }

 

}else{
       MessageBox("fail");
}

session.Close();
file->Close();
delete file;
file = NULL;
```










