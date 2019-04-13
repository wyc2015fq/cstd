
# C\#使用BeginInvoke和EndInvoke异步下载和获取返回结果 - 致林 - 博客园






# [C\#使用BeginInvoke和EndInvoke异步下载和获取返回结果](https://www.cnblogs.com/bincoding/p/7803221.html)
场景：为了防止UI卡死，使用异步下载文件
问题：采用多线程下载，关闭窗口后下载线程不能停止，线程操作麻烦。
参考：C\#客户端的异步操作：[http://www.cnblogs.com/fish-li/archive/2011/10/23/2222013.html](http://www.cnblogs.com/fish-li/archive/2011/10/23/2222013.html)
方案：采用BeginInvoke的方式调用下载方法，委托会自动启动新线程，停止时也不需要手动控制。使用EndInvoke获取返回结果。
try{
    IAsyncResult ir= process.BeginInvoke(newHttpDownloadDelegate(HttpDownload),url,path);boolresult =process.EndInvoke(ir);
}catch(Exception ex)
{
    MessageBox.Show(ex.Message);
}//方法声明publicdelegateboolHttpDownloadDelegate(stringurl,stringpath)publicboolHttpDownload(stringurl,stringpath)
{//下载方法...}
HTTP下载：
///<summary>///http下载文件///</summary>///<param name="url">下载文件地址</param>///<param name="path">文件存放地址，包含文件名</param>///<returns></returns>publicboolHttpDownload(stringurl,stringpath)
        {stringtempPath = System.IO.Path.GetDirectoryName(path) +@"\temp";
            System.IO.Directory.CreateDirectory(tempPath);//创建临时文件目录stringtempFile = tempPath +@"\"+ System.IO.Path.GetFileName(path) +".temp";//临时文件if(System.IO.File.Exists(tempFile))
            {
                System.IO.File.Delete(tempFile);//存在则删除}try{
                FileStream fs=newFileStream(tempFile, FileMode.Append, FileAccess.Write, FileShare.ReadWrite);//设置参数HttpWebRequest request = WebRequest.Create(url)asHttpWebRequest;//发送请求并获取相应回应数据HttpWebResponse response = request.GetResponse()asHttpWebResponse;//直到request.GetResponse()程序才开始向目标网页发送Post请求Stream responseStream =response.GetResponseStream();//创建本地文件写入流//Stream stream = new FileStream(tempFile, FileMode.Create);byte[] bArr =newbyte[1024];intsize = responseStream.Read(bArr,0, (int)bArr.Length);while(size >0)
                {//stream.Write(bArr, 0, size);fs.Write(bArr,0, size);
                    size= responseStream.Read(bArr,0, (int)bArr.Length);
                }//stream.Close();fs.Close();
                responseStream.Close();
                System.IO.File.Move(tempFile, path);returntrue;
            }catch(Exception ex)
            {returnfalse;
            }
        }





