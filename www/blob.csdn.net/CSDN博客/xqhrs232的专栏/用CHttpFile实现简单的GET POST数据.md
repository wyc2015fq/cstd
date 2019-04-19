# 用CHttpFile实现简单的GET/POST数据 - xqhrs232的专栏 - CSDN博客
2014年12月24日 14:18:03[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：757
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://www.cppblog.com/edog/archive/2005/12/06/1554.html](http://www.cppblog.com/edog/archive/2005/12/06/1554.html)
相关文章
1、[关于使用CHttpFile](http://blog.csdn.net/sh_chenjian/article/details/4758738)----[http://blog.csdn.net/sh_chenjian/article/details/4758738](http://blog.csdn.net/sh_chenjian/article/details/4758738)
# 2、
## VC Http请求
 ----[http://blog.sina.com.cn/s/blog_827ed2e001016le5.html](http://blog.sina.com.cn/s/blog_827ed2e001016le5.html)
# 3、
VC里打开网页几种方法 
 ----[http://yubo1006.blog.163.com/blog/static/166059309201142371346524/](http://yubo1006.blog.163.com/blog/static/166059309201142371346524/)
4、CHttpFile_百度百科----[http://baike.baidu.com/link?url=GP4QCoNwZgqkxqOU5nxHYZgS82W_qIibk4FK-DLQwJExrzhFU8EcbG9siFuy1_jO4xcU88E59GDBqYj_0_ri3q](http://baike.baidu.com/link?url=GP4QCoNwZgqkxqOU5nxHYZgS82W_qIibk4FK-DLQwJExrzhFU8EcbG9siFuy1_jO4xcU88E59GDBqYj_0_ri3q)
5、Google web ApI的使用注意----[http://blog.sina.com.cn/s/blog_827ed2e001016lfb.html](http://blog.sina.com.cn/s/blog_827ed2e001016lfb.html)
一、GET 数据，下载网页，文件等，用于可下载的文件，不能用于服务端运行的程序，比如.aspx文件等，否则会返回500错误。
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CString strSentence, strWriteName="1.htm";
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)    CString strFileName="http://localhost/InDesign/" + strWriteName;
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)    CInternetSession sess;
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)    CHttpFile* fileGet;
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)try
{
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        fileGet=(CHttpFile*)sess.OpenURL(strFileName);
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)    }
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)catch(CException* e)
{
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        fileGet = 0;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)throw;
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)    }
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)if(fileGet)
{
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        DWORD dwStatus;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        DWORD dwBuffLen = sizeof(dwStatus);
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        BOOL bSuccess = fileGet->QueryInfo(HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER, &dwStatus, &dwBuffLen);
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)if( bSuccess && dwStatus>= 200&& dwStatus<300 ) 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){ 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)            CStdioFile fileWrite; 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)if(fileWrite.Open(strWriteName, CFile::modeWrite|CFile::modeCreate))
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){ 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)while(fileGet->ReadString(strSentence))
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)                    fileWrite.WriteString(strSentence+"\n");
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)                }
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)                fileWrite.Close();
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)                AfxMessageBox("下载完毕");
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)else
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)                AfxMessageBox("本地文件"+strWriteName+"打开出错."); 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)else
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)            strSentence.Format("打开网页文件出错，错误码：%d", dwStatus);
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)            AfxMessageBox(strSentence);
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        fileGet->Close();
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        delete fileGet;
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)    }
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)else
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)        AfxMessageBox("不能找到网页文件！");
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)    sess.Close();
二、POST 数据，比如用于提交注册信息等
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CString strHttpName="http://localhost/TestReg/RegForm.aspx"; // 需要提交数据的页面
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)    CString strFormData = "username=abc&password=123";    // 需要提交的数据
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)    CInternetSession sess;
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)    CHttpFile* fileGet;
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)    CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded"); // 请求头
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)try
{
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        fileGet=(CHttpFile*)sess.OpenURL(strHttpName);//打开文件
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)    }
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)catch(CException* e)
{
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        fileGet = 0;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)throw;
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)    }
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)    CString strSentence, strGetSentence = "";
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)if(fileGet)
{
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        DWORD dwStatus;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        DWORD dwBuffLen = sizeof(dwStatus);
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        BOOL bSuccess = fileGet->QueryInfo(HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER, &dwStatus, &dwBuffLen);
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)if( bSuccess && dwStatus>= 200 &&dwStatus<300 )
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){ 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)            BOOL result = fileGet->SendRequest(strHeaders, (LPVOID)(LPCTSTR)strFormData, strFormData.GetLength());
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)while(fileGet->ReadString(strSentence))    // 读取提交数据后的返回结果
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)                strGetSentence = strGetSentence + strSentence + char(13) + char(10);
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)            AfxMessageBox(strGetSentence); // 显示返回网页内容
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)else
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)            strSentence.Format("POST出错，错误码：%d", dwStatus);
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)            AfxMessageBox(strSentence);
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        fileGet->Close();
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        delete fileGet;
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)    }
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)else
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)        AfxMessageBox("不能找到网页文件！");
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)    sess.Close();
