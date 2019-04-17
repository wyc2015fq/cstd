# ASP.NET 实现日志文件分页查询 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月05日 13:58:32[boonya](https://me.csdn.net/boonya)阅读数：117标签：[LogFile																[Content																[Paging](https://so.csdn.net/so/search/s.do?q=Paging&t=blog)
个人分类：[ASP.NET](https://blog.csdn.net/boonya/article/category/7665529)





ASP.NET 日志文件分页查询，通过读取日志行进行分页处理，支持从头到尾和从尾到头分页。

**目录**

[后端实现](#%E5%90%8E%E7%AB%AF%E5%AE%9E%E7%8E%B0)

[TxtFileReader](#TxtFileReader)

[Pager](#Pager)

[Controller](#Controller)

[前端实现](#%E5%89%8D%E7%AB%AF%E5%AE%9E%E7%8E%B0)

[FileLog.cshtml](#FileLog.cshtml)

[效果](#%E6%95%88%E6%9E%9C)

## 后端实现

### TxtFileReader

```
using SQ.Base;
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Threading;

namespace InfluxBD.Utils
{
    /// <summary>
    /// 读取指定行的内容
    /// </summary>
    public class TxtFileReader
    {
        DataBuffer dataBuffer;
        string filePath;

        public TxtFileReader(string filePath)
        {
            this.filePath = filePath;
        }

        public void Open()
        {
            this.dataBuffer = new DataBuffer(filePath);
            lock (this)
            {
                this.dataBuffer.Open();
                while (!this.dataBuffer.dataFile.done)
                {
                    Thread.Sleep(20);
                }
                Console.WriteLine("文件总行数:{0}", this.dataBuffer.dataFile.Lines);
            }
        }

        public List<string> ReadLine(int start,int end)
        {
            List<string> list = new List<string>();
            List<int> listId = new List<int>();
            try
            {
                int total = this.dataBuffer.dataFile.Lines;
                if (end> total)
                {
                    throw new Exception("索引超过日志文件行");
                }
                for (int i = start, j = end; i <= j; i++)
                {
                    listId.Add(i);
                    list.Add(this.dataBuffer.ReadLine(i));
                }
                return list;
            }
            catch (Exception ex)
            {
                Log.WriteLog4Ex("文件多行读取异常", ex);
                return list;
            }
        }

        public List<string> FromEndReadLine(int start, int end)
        {
            List<string> list = new List<string>();
            try
            {
                int total = this.dataBuffer.dataFile.Lines;
                if (end > total)
                {
                    throw new Exception("索引超过日志文件行");
                }
                int i = start==1?total:total - start+1, j = end==total?1:total - end+1 ;
                for (; i >= j; i--)
                {
                    list.Add(this.dataBuffer.ReadLine(i));
                }
                return list;
            }
            catch (Exception ex)
            {
                Log.WriteLog4Ex("文件多行读取异常", ex);
                return list;
            }
        }

        public DataBuffer GetDataBuffer()
        {
            return this.dataBuffer;
        }

        public void Close()
        {
            this.dataBuffer.Close();
        }
    }

    public static class FileConfig
    {
        public static int STREAM_BUFFER_SIZE = 1024000;
        public static int MAP_DISTANCE = 10;
    }

    public class DataFile
    {
        /// 
        /// 数据文件名 
        /// 
        public string fileName = "";
        /// 
        /// 初始化读取完标志 
        /// 
        public bool done = false;

        /// 
        /// 当前流位置 
        /// 
        public long Position = 0;

        /// 
        /// 文件头部信息 
        /// 
        private Hashtable head = new Hashtable();
        public Hashtable Head { get { return head; } set { head = value; } }

        /// 
        /// 文件地图 
        ///        
        private ArrayList map = new ArrayList();
        public ArrayList Map { get { return map; } set { map = value; } }

        /// 
        /// 文件数据行行数 
        ///        
        private int lines = 0;
        public int Lines { get { return lines; } set { lines = value; } }
    }

    public class DataBuffer
    {
        private FileStream fs = null;
        private BufferedStream bs = null;
        private StreamReader sr = null;
        private StreamWriter sw = null;
        /// 
        /// 文件信息数据结构 
        /// 
        public DataFile dataFile = new DataFile();

        public DataBuffer(string name)
        {
            dataFile.fileName = name;
        }

        /// 
        /// 打开文件 
        /// 
        public bool Open()
        {
            try
            {
                //初始化各流 
                fs = new FileStream(dataFile.fileName, FileMode.Open, FileAccess.ReadWrite);
                bs = new BufferedStream(fs, FileConfig.STREAM_BUFFER_SIZE);
                sr = new StreamReader(fs);
                sw = new StreamWriter(fs);
                Thread initFile = new Thread(new ThreadStart(InitDataFile));
                initFile.Start();
                return true;
            }
            catch (Exception ex)
            {
                Console.WriteLine("文件打开异常:{0}",ex);
                return false;
            }
        }

        private void InitDataFile()
        {
            //另开一个读取流 
            BufferedStream bs = new BufferedStream(fs);
            StreamReader sr = new StreamReader(bs);

            //读入数据文件头信息。共14行 
            string thisLine = NextLine(ref sr);
            dataFile.Head.Add("Subject", thisLine.Substring(11));

            thisLine = NextLine(ref sr);
            dataFile.Head.Add("Date", thisLine.Substring(8));

            thisLine = NextLine(ref sr);
            dataFile.Head.Add("Time", thisLine.Substring(8));

            thisLine = NextLine(ref sr);
            dataFile.Head.Add("Channels", thisLine.Substring(12));

            thisLine = NextLine(ref sr);
            dataFile.Head.Add("Rate", thisLine.Substring(8));

            thisLine = NextLine(ref sr);
            dataFile.Head.Add("Type", thisLine.Substring(8));

            thisLine = NextLine(ref sr);
            dataFile.Head.Add("Rows", thisLine.Substring(8));

            thisLine = NextLine(ref sr);
            thisLine = NextLine(ref sr);
            dataFile.Head.Add("Electrode Labels", thisLine);
            thisLine = NextLine(ref sr);
            thisLine = NextLine(ref sr);
            thisLine = NextLine(ref sr);
            thisLine = NextLine(ref sr);
            thisLine = NextLine(ref sr);
            //降低自己的优先级 
            Thread.CurrentThread.Priority = ThreadPriority.BelowNormal;

            //数行数，建立地图 
            int lines = 1;
            //在地图中加入首条数据的位置信息 
            dataFile.Map.Add(dataFile.Position);
            //顺序建立文件地图 
            while (!sr.EndOfStream)
            {
                thisLine = NextLine(ref sr);
                if ((++lines) % FileConfig.MAP_DISTANCE == 0)
                {
                    dataFile.Map.Add(dataFile.Position);
                }
            }
            dataFile.Lines = lines;
            dataFile.done = true;
        }

        /// 
        /// 文件关闭 
        /// 
        public bool Close()
        {
            try
            {
                //顺序关闭各流 
                sw.Close();
                sr.Close();
                bs.Close();
                fs.Close();
                return true;
            }
            catch (Exception ex)
            {
                Console.WriteLine("文件关闭异常:{0}", ex);
                return false;
            }
        }

        /// 
        /// 顺序读取下一行。效率低不建议大规模使用，只在打开文件的时候使用一次 
        /// 
        /// 
        public string NextLine(ref StreamReader sr)
        {
            string next = sr.ReadLine();
            //+2是指Windows换行回车。Linux下要改为+1 
            dataFile.Position += next.Length + 2;
            return next;
        }

        //指定的目标行内容 
        public string ReadLine(long line)
        {
            try
            {
                //如果载入完毕 
                if (dataFile.done)
                {
                    //确定数据块索引号 
                    int index = (int)line / FileConfig.MAP_DISTANCE;
                    //移动到指定位置 
                    bs.Seek(long.Parse(dataFile.Map[index].ToString()), SeekOrigin.Begin);
                    //创建流读取器 
                    if (this.isUnix())
                    {
                        sr = new StreamReader(bs, Encoding.Default);
                    }
                    else
                    {
                        sr = new StreamReader(bs, Encoding.GetEncoding("gb2312"));
                    }
                    //移动到指定行 
                    for (int i = 1; i <= (line - index * FileConfig.MAP_DISTANCE); i++)
                    {
                        sr.ReadLine();
                    }
                    //返回指定行的值 
                    return sr.ReadLine();
                }
                else
                {
                    return "";
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("文件读取异常:{0}", ex);
                return "";
            }
        }

        /// <summary>
        /// 判断当前操作系统是不是Unix系统
        /// </summary>
        /// <returns></returns>
        public  bool isUnix()
        {
            OperatingSystem os = Environment.OSVersion;
            if (os.Platform==PlatformID.Unix)
            {
                return true;
            }
            return false;
        }
    }
}
```

### Pager

List 分页对象：

```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace RTVSWeb.Utils
{
    /// <summary>
    /// List数据分页对象
    /// </summary>
    public class Pager<T>
    {
        public int CurrentPage { set; get; }

        public int PageSize { set; get; }

        public int TotalPage { set; get; }

        public int TotalData { set; get; }

        private List<T> Results = new List<T>();

        public List<T> Data
        {
            set { value = Results; }
            get { return Results; }
        }
        /// <summary>
        /// 分页原始数据
        /// </summary>
        /// <param name="page"></param>
        /// <param name="pageSize"></param>
        /// <param name="deviceInfos"></param>
        public void Paging(int page, int pageSize, List<T> data)
        {
            this.PageSize = pageSize;
            this.CurrentPage = page;
            this.TotalData = data.Count;
            if (data.Count % PageSize == 0)
            {
                TotalPage = data.Count / PageSize;
            }
            else
            {
                TotalPage = data.Count / PageSize + 1;
            }

            if (CurrentPage > TotalPage)
            {
                CurrentPage = TotalPage;
            }

            if (data.Count > 0)
            {
                int left = data.Count - (CurrentPage - 1) * PageSize;
                Results = data.GetRange((CurrentPage - 1) * PageSize, left > PageSize ? PageSize : left);
            }

        }

        /// <summary>
        /// 分页原始数据
        /// </summary>
        /// <param name="page"></param>
        /// <param name="pageSize"></param>
        /// <param name="total"></param>
        /// <param name="data"></param>
        public void Paging(int page, int pageSize,int total, List<T> data)
        {
            this.PageSize = pageSize;
            this.CurrentPage = page;
            this.TotalData = total;
            if (total % PageSize == 0)
            {
                TotalPage = total / PageSize;
            }
            else
            {
                TotalPage = total / PageSize + 1;
            }

            if (CurrentPage > TotalPage)
            {
                CurrentPage = TotalPage;
            }

            if (total > 0)
            {
                int left = total - (CurrentPage - 1) * PageSize;
                Results = data;
            }

        }

    }
}
```

### Controller

分页数据接口:

```
// GET: /<controller>/
        public Pager<string> GetFileLogs(int page, int pageSize,bool readFromStart)
        {
            string filePath = Program.task.Config.LogfilePath;
            Pager<string> pager = new Pager<string>();
            TxtFileReader reader = new TxtFileReader(filePath);
            try
            {
                if (System.IO.File.Exists(filePath))
                {
                    reader.Open();
                    int total = reader.GetDataBuffer().dataFile.Lines;
                    int left = total - (page - 1) * pageSize;
                    if (readFromStart)
                    {
                        List<string> list = reader.ReadLine(page == 1 ? 1 : (page - 1) * pageSize + 1, left > pageSize ? page * pageSize : (page - 1) * pageSize + left);
                        pager.Paging(page, pageSize, total, list);
                    }
                    else
                    {
                        List<string> list = reader.FromEndReadLine(page == 1 ? 1 : (page - 1) * pageSize + 1, left > pageSize ? page * pageSize : (page - 1) * pageSize + left);
                        pager.Paging(page, pageSize, total, list);
                    }
                }
                return pager;
            }
            catch (Exception ex)
            {
                Log.WriteLog4Ex("读取日志文件异常:", ex);
                return pager;
            }
            finally
            {
                reader.Close();
            }
        }
```

## 前端实现

### FileLog.cshtml

处理分页和查询：

```
@{
    Layout = null;
}
<table style="font-weight:bold;">
    <tr><td>读取规则:</td><td><select id="readFromStart"><option value="true">从开头分页</option><option selected value="false">从尾部分页</option></select></td></tr>
    <tr><td colspan="6" align="center"><input class='btn btn-primary' type="button" value="重载" onclick="reload()"></td></tr>
</table>
<!--这个地方展示数据-->
<table id="data-table" class="table"  style="width:100%;">
    <tr>
        <th >日志文件记录</th>
    </tr>

</table>
<!--这个地方展示分页-->
<div class="m-style M-box3"></div>

<link rel="stylesheet" type="text/css" href="/js/pagination/common/highlight.min.css" media="screen">
<link rel="stylesheet" type="text/css" href="/js/pagination/common/common.css" media="screen">
<link rel="stylesheet" type="text/css" href="/js/pagination/pagination.css" media="screen">
<script src="/js/pagination/common/highlight.min.js"></script>
<script src="/js/pagination/jquery.pagination.js"></script>

<script type="text/javascript">
    $(function () {
        query(1,20);
    });

    var url = "/Log/GetFileLogs";

    var queryStr = {};

    function getJson(url, data, func) {
        $.getJSON(url, data, func);
    }

    //进行查询
    function query(page, pageSize, readFromStart) {
        queryStr.page = page;
        queryStr.pageSize = pageSize;
        queryStr.readFromStart = readFromStart;
        getJson(url+"?random=" + Math.random(), queryStr, loadlist);
    }

    function showResult(json)
    {
        $(".loaded-data").remove();
       
        for (var i = 0; i < json.data.length; i++) {
            $("#data-table").append("<tr class=\"loaded-data\"><td>" + json.data[i] + "</td></tr > ")
        }
    }
    //返回结果处理:注意JSON字段的大小写问题
    function loadlist(json) {     
        showResult(json);
        //初始化分页控件
        $(".M-box3").pagination({
            pageCount: json.totalPage,
            totalData: json.totalData,
            current: json.currentPage,
            showData: json.pageSize,
            jump: true,
            coping: true,
            homePage: '首页',
            endPage: '末页',
            prevContent: '上页',
            nextContent: '下页',
            callback: function (api)
            {
                // 当前第几页
                queryStr.page = api.getCurrent();

                $.getJSON(url + "?random=" + Math.random(), queryStr, function (json) {
                    showResult(json);
                });
            }
        });
    }

    function reload()
    {
        query(1, 20, $("#readFromStart").val());
    }

</script>
```

### 效果

![](https://img-blog.csdnimg.cn/20181205135752645.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/2018120513582923.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)](https://so.csdn.net/so/search/s.do?q=Content&t=blog)](https://so.csdn.net/so/search/s.do?q=LogFile&t=blog)




