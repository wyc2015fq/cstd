# mongoDB学习笔记——存取图片（C#) - 左直拳的马桶_日用桶 - CSDN博客
2014年02月10日 23:10:28[左直拳](https://me.csdn.net/leftfist)阅读数：12102
作为一个NoSql数据库的代表，存取多媒体数据，应该是强项吧？那么，图片在mongoDB里是如何存取的呢？（其实，关系型数据库存取图片也一点问题没有，所以我看NoSql的强项不在于是否存储多媒体，而在于采用键值对的方式来存储数据。）
mongoDB存取图片有两种方式：
*"由于MongoDB的文档结构为BJSON格式（BJSON全称：Binary JSON），而BJSON格式本身就支持保存二进制格式的数据，因此可以把文件的二进制格式的数据直接保存到MongoDB的文档结构中。但是由于一个BJSON的最大长度不能超过4M，所以限制了单个文档中能存入的最大文件不能超过4M。为了提供对大容量文件存取的支持，samus驱动提供了“GridFS”方式来支持，“GridFS”方式文件操作需要引入新的程序集“MongoDB.GridFS.dll”。"*
**一、在文档对象中存取文件**
　　当文件大小较小的时候，直接存入文档对象实现起来更简洁。比如大量图片文件的存取等，一般图片文件都不会超过4M。我们先实现一个上传图片存入数据库，再取出来写回页面的例子：
![](https://img-blog.csdn.net/20140210225156609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using MongoDB.Bson;
using MongoDB.Driver;
namespace mongotest
{
    public partial class Form3 : Form
    {
        public Form3()
        {
            InitializeComponent();
            Init();
        }
        //数据库连接字符串
        const string strconn = "mongodb://127.0.0.1:27017";
        //数据库名称
        const string dbName = "test";
        MongoServer server;
        MongoDatabase db;
        void Init()
        {
            //创建数据库链接
            server = MongoDB.Driver.MongoServer.Create(strconn);
            //获得数据库
            db = server.GetDatabase(dbName);
        }
        private void SaveImgBJSON(byte[] byteImg)
        {
            BsonDocument doc = new BsonDocument();
            doc["ID"] = 1;
            doc["Img"] = byteImg;
            MongoCollection col = db.GetCollection("thins");
            col.Save(doc);
        }
        private void btnSaveImg_Click(object sender, EventArgs e)
        {
            byte[] byteImg = File.ReadAllBytes(@"c:\temp\yl.jpg");
            SaveImgBJSON(byteImg);
        }
        private void btnShowImg_Click(object sender, EventArgs e)
        {
            MongoCollection col = db.GetCollection("thins");
            var query = new QueryDocument { { "ID", 1} };
            var result = col.FindAs<BsonDocument>(query);
            byte[] buff = (byte[])((BsonDocument)result.ToList()[0]).GetValue("Img");
            MemoryStream MS = new MemoryStream(buff);
            pictureBox1.Image = Image.FromStream(MS);
        }
    }
}
```
**二、用GridFS方式存取文件**
*在实现GridFS方式前我先讲讲它的原理，为什么可以存大文件。驱动首先会在当前数据库创建两个集合："fs.files"和"fs.chunks"集合，前者记录了文件名，文件创建时间，文件类型等基本信息；后者分块存储了文件的二进制数据（并支持加密这些二进制数据）。分块的意思是把文件按照指定大小分割，然后存入多个文档中。"fs.files"怎么知道它对应的文件二进制数据在哪些块呢？那是因为在"fs.chunks"中有个"files_id"键，它对应"fs.files"的"_id"。"fs.chunks"还有一个键(int型)"n"，它表明这些块的先后顺序。这两个集合名中的"fs"也是可以通过参数自定义的。*
（我想起了Sql Server的FileStream）
　　如果你只是想知道怎么用，可以忽略上面这段话，下面将用法：
这里引用了两个第三方dll，可以到https://github.com/samus/mongodb-csharp下载，编译后得到。
```
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using MongoDB;
using MongoDB.GridFS;
namespace mongotest
{
    public partial class Form2 : Form
    {
        public Form2()
        {
            InitializeComponent();
            Init();
        }
        private void btnGridFSSave_Click(object sender, EventArgs e)
        {
            byte[] byteImg = File.ReadAllBytes(@"c:\temp\yl.jpg");
            filename = GridFsSave(byteImg);
            MessageBox.Show(filename);
        }
        private void btnGridFSShow_Click(object sender, EventArgs e)
        {
            byte[] buff = GridFsRead(filename);
            MemoryStream MS = new MemoryStream(buff);
            pictureBox1.Image = Image.FromStream(MS);
        }
        private Mongo mongo;
        private IMongoDatabase test;
        string filename;
        void Init()
        {
            var connstr = @"Server=localhost:27017";
            mongo = new Mongo(connstr);
            mongo.Connect();
            test = mongo["test"];
        }
        private string GridFsSave(byte[] byteFile)
        {
            string filename = Guid.NewGuid().ToString();
            //这里GridFile构造函数有个重载，bucket参数就是用来替换那个创建集合名中默认的"fs"的。
            GridFile gridFile = new GridFile(test);
            using (GridFileStream gridFileStream = gridFile.Create(filename))
            {
                gridFileStream.Write(byteFile, 0, byteFile.Length);
            }
            return filename;
        }
        private byte[] GridFsRead(string filename)
        {
            GridFile gridFile = new GridFile(test);
            GridFileStream gridFileStream = gridFile.OpenRead(filename);
            byte[] bytes = new byte[gridFileStream.Length];
            gridFileStream.Read(bytes, 0, bytes.Length);
            return bytes;
        }
    }
}
```
存储在数据库中如图：
![](https://img-blog.csdn.net/20140210231220234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
参考文章：
http://www.cnblogs.com/lipan/archive/2011/03/21/1989409.html
