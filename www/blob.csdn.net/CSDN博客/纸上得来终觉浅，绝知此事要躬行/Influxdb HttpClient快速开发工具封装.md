# Influxdb HttpClient快速开发工具封装 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月05日 13:32:58[boonya](https://me.csdn.net/boonya)阅读数：204








Influxdb HttpClient快速开发工具封装Influxdb HttpClient快速开发工具封装，包括：HttpClient GET POST 的请求支持，以及常用数据库功能的客户端工具封装。

**目录**

[HttpHelper](#HttpHelper)

[HttpInfluxDBClient](#HttpInfluxDBClient)

## HttpHelper

HttpClient的GET POST 访问工具类：

```
using SQ.Base;
using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Text;

namespace InfluxBD
{
    public class HttpHelper
    {

        /// <summary>
        /// GET
        /// </summary>
        /// <param name="url"></param>
        /// <returns></returns>
        public static async System.Threading.Tasks.Task<string> GetAsync(string url,string username,string password)
        {
            try
            {
                using (var client = new HttpClient())
                {
                    HttpHelper.SetRequestHeaders(client, username, password);
                    string responseString = await client.GetStringAsync(url);
                    return responseString;
                }
            }
            catch (Exception ex)
            {
                string msg = "GetStringAsync 异常:"+ex;
                Log.WriteLog4Ex(msg,ex);
                return msg;
            }
        }

        /// <summary>
        /// POST
        /// </summary>
        /// <param name="url"></param>
        /// <param name="username"></param>
        /// <param name="password"></param>
        /// <param name="sql"></param>
        /// <returns></returns>
        public static async System.Threading.Tasks.Task<string> PostAsync(string url, string username, string password, string sql)
        {
            try
            {
                using (var client = new HttpClient())
                {
                    HttpHelper.SetRequestHeaders(client, username, password);

                    if (url.Contains("/write"))
                    {
                        HttpContent httpContent = new StringContent(sql);

                        var response = await client.PostAsync(url, httpContent);

                        var responseString = await response.Content.ReadAsStringAsync();

                        return responseString;
                    }
                    else
                    {
                        var values = new List<System.Collections.Generic.KeyValuePair<string, string>>();
                        values.Add(new KeyValuePair<string, string>("q", sql));

                        var content = new FormUrlEncodedContent(values);

                        var response = await client.PostAsync(url, content);

                        var responseString = await response.Content.ReadAsStringAsync();

                        return responseString;
                    }
                }
            }
            catch (Exception ex)
            {
                string msg = "PostAsync 异常:" + ex;
                Log.WriteLog4Ex(msg, ex);
                return msg;
            }
            
        }

        private static void SetRequestHeaders(HttpClient client, string username, string password)
        {
            client.DefaultRequestHeaders.Add("User-Agent", @"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/69.0.3497.100 Safari/537.36");
            client.DefaultRequestHeaders.Add("Accept", @"text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8");
            //client.DefaultRequestHeaders.Add("Accept-Encoding", "gzip");
            string authorization= string.Format("{0}:{1}", username, password);
            client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Basic", authorization);
        }
    }
}
```

## HttpInfluxDBClient

HttpInfluxDBClient是Influxdb常用的数据库功能封装：

```
using InfluxBD.Sdk;
using InfluxDB.Net.Models;
using System;
using System.Collections.Generic;

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

        public async System.Threading.Tasks.Task<string> CreateDatabaseAsync(string database)
        {
            string sql = "CREATE DATABASE " + database;
            string url = _baseAddress + string.Format("/query", "");
            string result = await HttpHelper.PostAsync(url, _username, _password, sql);
            return result;
        }


        public async System.Threading.Tasks.Task<string> DeleteDatabaseAsync(string database)
        {
            string sql = "DROP DATABASE " + database;
            string url = _baseAddress + string.Format("/query", "");
            string result = await HttpHelper.PostAsync(url, _username, _password, sql);
            return result;
        }


        public async System.Threading.Tasks.Task<string> GetDatabasesAsync()
        {
            string sql = "SHOW DATABASES";
            string url = _baseAddress + string.Format("/query?q={0}", sql);
            string result =await HttpHelper.GetAsync(url, _username, _password);
            return result;
        }

        /// <summary>
        /// 读取数据
        /// </summary>
        /// <param name="database"></param>
        /// <param name="sql"></param>
        /// <returns></returns>
        public async System.Threading.Tasks.Task<string> QueryAsync(string database, string sql)
        {
            string url = _baseAddress + string.Format("/query?db={0}&q={1}", database, sql);
            string result = await HttpHelper.GetAsync(url, _username, _password);
            return result;
        }

        /// <summary>
        /// 写入数据
        /// </summary>
        /// <param name="database"></param>
        /// <param name="sql">示例:test,tag=logs Field0=10,Field1=10,Field2=20</param>
        /// <returns></returns>
        public async System.Threading.Tasks.Task<string> WriteAsync(string database, string sql)
        {
            string rp = "rp_policy";//1.7.1之后不支持包含数字命名
            string url = _baseAddress + string.Format("/write?u={0}&p={1}&db={2}&rp={3}&precision=ms", _username, _password, database, rp);
            string result = await HttpHelper.PostAsync(url, _username, _password, sql);
            return result;
        }

       

        /// <summary>
        /// Ping服务是否能够打开
        /// </summary>
        /// <returns></returns>
        public async System.Threading.Tasks.Task<bool> PingAsync()
        {
            string url = _baseAddress + string.Format("/ping", "");
            string result = await HttpHelper.GetAsync(url, _username, _password);
            return string.IsNullOrEmpty(result);
        }

    }
}
```





