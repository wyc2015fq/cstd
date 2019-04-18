# [转载]MongoDB学习笔记(五) MongoDB文件存取操作 - weixin_33985507的博客 - CSDN博客
2012年10月26日 15:02:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6

## [MongoDB学习笔记(五) MongoDB文件存取操作](http://www.cnblogs.com/lipan/archive/2011/03/21/1989409.html)
Posted on 2011-03-21 07:33[lipan](http://www.cnblogs.com/lipan/) 阅读(8397) 评论(7) [编辑](http://www.cnblogs.com/lipan/admin/EditPosts.aspx?postid=1989409)[收藏](http://www.cnblogs.com/lipan/archive/2011/03/21/1989409.html#)![](https://www.cnblogs.com/lipan/aggbug/1989409.html?type=1&webview=1)
　　由于MongoDB的文档结构为BJSON格式（BJSON全称：Binary JSON），而BJSON格式本身就支持保存二进制格式的数据，因此可以把文件的二进制格式的数据直接保存到MongoDB的文档结构中。但是由于一个BJSON的最大长度不能超过4M，所以限制了单个文档中能存入的最大文件不能超过4M。为了提供对大容量文件存取的支持，samus驱动提供了“GridFS”方式来支持，“GridFS”方式文件操作需要引入新的程序集“MongoDB.GridFS.dll”。下面我们分别用两种方式来实现。
## 一、在文档对象中存取文件
　　当文件大小较小的时候，直接存入文档对象实现起来更简洁。比如大量图片文件的存取等，一般图片文件都不会超过4M。我们先实现一个上传图片存入数据库，再取出来写回页面的例子：
 1. 把图片存到BJSON中
[?](http://www.cnblogs.com/lipan/archive/2011/03/21/1989409.html#)
|12345678910|`/// <summary>``/// 把图片存到BJSON中``/// </summary>``public``void``SaveImgBJSON(``byte``[] byteImg)``{``    ``Document doc = ``new``Document();``    ``doc[``"ID"``] = 1;``    ``doc[``"Img"``] = byteImg;``    ``mongoCollection.Save(doc);``}`|
 2. 获取BJSON方式存储的图片字节数据
[?](http://www.cnblogs.com/lipan/archive/2011/03/21/1989409.html#)
|12345678|`/// <summary>``/// 获取BJSON方式存储的图片字节数据``/// </summary>``public``byte``[] GetImgBJSON()``{``  ``Document doc=  mongoCollection.FindOne(``new``Document { { ``"ID"``, 1 } });``  ``return``doc[``"Img"``] ``as``Binary;``}`|
　　上面两段代码是在对MongoDB相关操作进行BLL封装类中添加的两个方法，封装方式查看上节内容。下面看看在webform中如何调用：
　　在界面拖出一个FileUpload控件和一个Button控件，页面cs类加如下方法：
[?](http://www.cnblogs.com/lipan/archive/2011/03/21/1989409.html#)
|1234567|`protected``void``Button1_Click(``object``sender, EventArgs e)``{``    ``ImgBLL imgBll = ``new``ImgBLL();``    ``imgBll.DeleteAll();``    ``imgBll.SaveImgBJSON(FileUpload1.FileBytes);``    ``Response.BinaryWrite(imgBll.GetImgBJSON());``}`|
二、用GridFS方式存取文件
　　在实现GridFS方式前我先讲讲它的原理，为什么可以存大文件。驱动首先会在当前数据库创建两个集合："fs.files"和"fs.chunks"集合，前者记录了文件名，文件创建时间，文件类型等基本信息；后者分块存储了文件的二进制数据（并支持加密这些二进制数据）。分块的意思是把文件按照指定大小分割，然后存入多个文档中。"fs.files"怎么知道它对应的文件二进制数据在哪些块呢？那是因为在"fs.chunks"中有个"files_id"键，它对应"fs.files"的"_id"。"fs.chunks"还有一个键(int型)"n"，它表明这些块的先后顺序。这两个集合名中的"fs"也是可以通过参数自定义的。
　　如果你只是想知道怎么用，可以忽略上面这段话，下面将用法：
 1. GridFS方式的文件新建，读取，删除
[?](http://www.cnblogs.com/lipan/archive/2011/03/21/1989409.html#)
|123456789101112131415161718192021222324252627|`private``string``GridFsSave(``byte``[] byteFile)``{``    ``string``filename = Guid.NewGuid().ToString();``    ``//这里GridFile构造函数有个重载，bucket参数就是用来替换那个创建集合名中默认的"fs"的。``    ``GridFile gridFile = ``new``GridFile(mongoDatabase);``    ``using``(GridFileStream gridFileStream = gridFile.Create(filename))``    ``{``        ``gridFileStream.Write(byteFile, 0, byteFile.Length);``    ``}``    ``return``filename;``}``private``byte``[] GridFsRead(``string``filename)``{``    ``GridFile gridFile = ``new``GridFile(mongoDatabase);``    ``GridFileStream gridFileStream = gridFile.OpenRead(filename);``    ``byte``[] bytes = ``new``byte``[gridFileStream.Length];``    ``gridFileStream.Read(bytes, 0, bytes.Length);``    ``return``bytes;``}``private``void``GridFsDelete(``string``filename)``{``    ``GridFile gridFile = ``new``GridFile(mongoDatabase);``    ``gridFile.Delete(``new``Document(``"filename"``, filename));``}`|
 2. 再次封装GridFS操作，新文档只存储文件名称，相当于只是一个键，新文档还可以有除“文件名”之外其他的键。
[?](http://www.cnblogs.com/lipan/archive/2011/03/21/1989409.html#)
|12345678910111213141516171819202122|`/// <summary>``/// 把图片存到GridFS中``/// </summary>``public``void``SaveImgGridFS(``byte``[] byteImg)``{``    ``string``filename = GridFsSave(byteImg);``    ``Document doc = ``new``Document();``    ``doc[``"ID"``] = 1;``    ``doc[``"filename"``] = filename;``    ``mongoCollection.Save(doc);``}``/// <summary>``/// 获取GridFS方式存储的图片``/// </summary>``public``byte``[] GetImgGridFS()``{``    ``Document doc = mongoCollection.FindOne(``new``Document { { ``"ID"``, 1 } });``    ``string``filename = doc[``"filename"``].ToString();``    ``return``GridFsRead(filename);``}`|
三、小结
　　文件存取应该不是很难，值得注意的地方是：用第一种方式从文档中读出二进制数据时，一定要将类型转换为“Binary”类型；还有系统自带的键“_id”，它也不是string类型，是“Oid”类型的。
作者：李盼（Lipan）
出处：[[Lipan]](http://www.cnblogs.com/lipan/) （[http://www.cnblogs.com/lipan/](http://www.cnblogs.com/lipan/)）
版权声明：本文的版权归作者与博客园共有。转载时须注明本文的详细链接，否则作者将保留追究其法律责任。
