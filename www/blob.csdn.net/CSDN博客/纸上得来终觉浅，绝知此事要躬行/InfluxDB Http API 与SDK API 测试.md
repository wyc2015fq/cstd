# InfluxDB Http API 与SDK API 测试 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年11月13日 10:00:55[boonya](https://me.csdn.net/boonya)阅读数：456








Http 类似Rest的接口使用起来很方便，Sdk的接口是原生的适合改造和理解内部实现的原理。具体怎么用视情况而定，web端直接通过浏览器就可以访问，可以快速查看数据并且可以跨语言访问，SDK基于nuget包的方式更适合扩展性的项目。

**目录**

[Http API](#Http%20API)

[Sdk API](#Sdk%20API)

[测试](#%E6%B5%8B%E8%AF%95)

[条件查询](#%E6%9D%A1%E4%BB%B6%E6%9F%A5%E8%AF%A2)

[GIt下载](#GIt%E4%B8%8B%E8%BD%BD)

### Http API

```
using System;

namespace InfluxBD
{
    /// <summary>
    /// InfluxDB时序数据库操作HTTP客户端
    /// 参考:https://www.cnblogs.com/dehai/p/4887309.html
    /// </summary>
    public class HttpInfluxDBClient
    {
        string _baseAddress;
        string _username;
        string _password;

        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="baseAddress"></param>
        /// <param name="username"></param>
        /// <param name="password"></param>
        public HttpInfluxDBClient(string baseAddress, string username, string password)
        {
            this._baseAddress = baseAddress;
            this._username = username;
            this._password = password;
        }

        public string CreateDatabase(string database)
        {
            string sql = "CREATE DATABASE " + database;
            string url = _baseAddress + string.Format("/query?q={0}", sql);
            string result = HttpHelper.Post(url, sql, _username, _password);
            return result;
        }


        public string DeleteDatabase(string database)
        {
            string sql = "DROP DATABASE " + database;
            string url = _baseAddress + string.Format("/query?q={0}", sql);
            string result = HttpHelper.Post(url, sql, _username, _password);
            return result;
        }


        public string GetDatabases()
        {
            string sql = "SHOW DATABASES";
            string url = _baseAddress + string.Format("/query?q={0}", sql);
            string result = HttpHelper.Get(url, _username, _password);
            return result;
        }

        /// <summary>
        /// 读取数据
        /// </summary>
        /// <param name="database"></param>
        /// <param name="sql"></param>
        /// <returns></returns>
        public string Query(string database, string sql)
        {
            string url = _baseAddress + string.Format("/query?db={0}&q={1}", database, sql);
            string result = HttpHelper.Get(url, _username, _password);
            return result;
        }

        /// <summary>
        /// 写入数据
        /// </summary>
        /// <param name="database"></param>
        /// <param name="sql">示例:test,tag=logs Field0=10,Field1=10,Field2=20</param>
        /// <returns></returns>
        public string Write(string database, string sql)
        {
            string url = _baseAddress + string.Format("/write?db={0}", database);
            string result = HttpHelper.Post(url, sql, _username, _password);
            return result;
        }
    }
}
```

对于其他语言都是通用的，很方便。

### Sdk API

```
using InfluxDB.Net;
using InfluxDB.Net.Enums;
using InfluxDB.Net.Infrastructure.Configuration;
using InfluxDB.Net.Infrastructure.Influx;
using InfluxDB.Net.Models;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace InfluxBD.Sdk
{
    public class InfluxDBSdkHelper
    {
        private InfluxDb _client;

        /// <summary>
        /// 操作状态回调
        /// </summary>
        /// <param name="status"></param>
        public delegate void StatusCallBack(bool status);

        /// <summary>
        /// 操作结果回调
        /// </summary>
        /// <param name="json"></param>
        public delegate void ResultCallBack(string json);

        public InfluxDBSdkHelper(string baseUrl, string username, string password)
        {
            _client = new InfluxDb(baseUrl, username, password);
        }

        public async void CreateDatabase(string database, StatusCallBack statusCallBack)
        {
            InfluxDbApiResponse response = await _client.CreateDatabaseAsync(database);
            statusCallBack?.Invoke(response.Success);
        }


        public async void DeleteDatabase(string database, StatusCallBack statusCallBack)
        {   
            InfluxDbApiResponse deleteResponse = await _client.DropDatabaseAsync(database);
            statusCallBack?.Invoke(deleteResponse.Success);
        }


        public async void GetDatabases(ResultCallBack resultCallBack)
        { 
            List<Database> databases = await _client.ShowDatabasesAsync();
            string result = Newtonsoft.Json.JsonConvert.SerializeObject(databases);
            resultCallBack?.Invoke(result);
        }

        public async void Write(string database, InfluxDB.Net.Models.Point point, StatusCallBack statusCallBack)
        {
            //基于InfluxData.Net.InfluxDb.Models.Point实体准备数据
            //var point = new Point()
            //{
            //    Measurement = "logs",//表名
            //    Tags = new Dictionary<string, object>()
            //    {
            //       { "Id", 158}
            //    },
            //    Fields = new Dictionary<string, object>()
            //    {
            //       { "Val", "webInfo" }
            //    },
            //    Timestamp = DateTime.UtcNow
            //};
            InfluxDbApiResponse writeResponse = await _client.WriteAsync(database, point);
            statusCallBack?.Invoke(writeResponse.Success);
        }

        public async void Query(string database,string sql, ResultCallBack resultCallBack)
        {
            List<Serie> series = await _client.QueryAsync(database, sql);
            string result = Newtonsoft.Json.JsonConvert.SerializeObject(series); 
            resultCallBack?.Invoke(result);
        }

    }
}
```

### 测试

以Http API为例，数据库连接和账号信息：

![](https://img-blog.csdnimg.cn/20181113095213171.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

保存数据：表（test） 标签（type,可以多个，任意类型） 字段（Field0..，数值类型)

![](https://img-blog.csdnimg.cn/20181113095147923.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

查询数据：查询和普通sql差不多，条件查询可以去看官方文档

![](https://img-blog.csdnimg.cn/20181113095510904.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

浏览器可以这样访问：
[http://172.16.9.229:8086/query?u=admin&p=admin&db=rtvsweb&q=show%20databases](http://172.16.9.229:8086/query?u=admin&p=admin&db=rtvsweb&q=show%20databases)
[http://172.16.9.229:8086/query?u=admin&p=admin&db=rtvsweb&q=select%20*%20from%20logs](http://172.16.9.229:8086/query?u=admin&p=admin&db=rtvsweb&q=select%20*%20from%20logs)

注:因为本地是window10上安装的Docker每次开机后需要重启Docker，我的influxdb是安装在docker里面的，懒得启动了，所以上面的测试没有展示数据敬请见谅。 

### 条件查询

![](https://img-blog.csdnimg.cn/20181113113249573.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

### GIt下载

项目地址:[https://github.com/MSAspDotNetCore/InfluxDBUtilAndTest](https://github.com/MSAspDotNetCore/InfluxDBUtilAndTest)



