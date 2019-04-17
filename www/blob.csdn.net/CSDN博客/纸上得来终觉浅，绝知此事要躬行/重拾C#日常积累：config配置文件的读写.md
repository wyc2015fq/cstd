# 重拾C#日常积累：config配置文件的读写 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年04月16日 10:36:56[boonya](https://me.csdn.net/boonya)阅读数：529








注；现在FrameWork2.0以上使用的是：ConfigurationManager或WebConfigurationManager。并且AppSettings属性是只读的，并不支持修改属性值．文章参考了网上一些内容进行整理输出。

#### 一、配置文件的说明

最常见的配置文件是App.config。应用程序配置文件是标准的 XML 文件，XML 标记和属性是区分大小写的。它是可以按需要更改的，开发人员可以使用配置文件来更改设置，而不必重编译应用程序。配置文件的根节点是configuration。我们经常访问的是appSettings，它是由.Net预定义的配置节。我们经常使用的配置文件的架构是客诉下面的形式。先大概有个印象，通过后面的实例会有一个比较清楚的认识。下面的“配置节”可以理解为进行配置一个XML的节点。

常见配置文件模式：

<configuration>                      // 顶层配置节
<configSections>                    //配置节声明区域，包含配置节和命名空间声明
<section>                         //配置节声明
<sectionGroup>                //定义配置节组
<section>                        //配置节组中的配置节声明
<appSettings>                       //预定义配置节
<Custom element for configuration section>   //配置节设置区域

下面是一个最常见的应用程序配置文件的例子，只有appSettings节：

```
<?xml version="1.0" encoding="utf-8"?>
<configuration>
    <appSettings>
        <add key="connectionstring" value="User Source=.;Password=;Initial Catalog=test;Provider=SQLOLEDB.1;" />
        <add key="TemplatePATH" value="Template" />
    </appSettings>
</configuration>
```
注意：在预定义的 appSettings 节（注意大小写），有很多的元素，这些元素名称都是“add”，有两个属性分别是“key”和“value”。




#### 二、如何使用ConfigurationManager？

##### 1、添加引用：添加System.configguration

![](https://images2015.cnblogs.com/blog/1034396/201707/1034396-20170705082226362-1776360269.png)![](https://images2015.cnblogs.com/blog/1034396/201707/1034396-20170705082255065-1290242399.png)![](https://images2015.cnblogs.com/blog/1034396/201707/1034396-20170705082350206-269869899.png)

##### 2、引用空间

![](https://images2015.cnblogs.com/blog/1034396/201707/1034396-20170705082546534-222638326.png)



##### 3、App.config配置文件配置节

常用配置节：

（1）普通配置节

```
<appSettings>  
　　<add key="COM1" value="COM1,9600,8,None,1,已启用" />
</appSettings>
```

（2）数据源配置节

```
<connectionStrings>
　　<add name="kyd" connectionString="server=.;database=UFDATA_999_2017;user=sa;pwd=123"/>
</connectionStrings>
```


（3）自定义配置节

![](https://img-blog.csdn.net/20180416101401476?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 4、获取配置的方式

![](https://img-blog.csdn.net/20180416104657347)

示例：

(1)、AppSettings

[https://msdn.microsoft.com/zh-cn/library/system.configuration.configurationmanager.appsettings(v=vs.110).aspx](https://msdn.microsoft.com/zh-cn/library/system.configuration.configurationmanager.appsettings(v=vs.110).aspx)




```
//系统自定义Key前缀
public static readonly string SysCustomKey = ConfigurationManager.AppSettings["redisKey"] ?? "";
```

（2）、ConnectionStrings

[https://msdn.microsoft.com/zh-cn/library/system.configuration.configurationmanager.connectionstrings(v=vs.110).aspx](https://msdn.microsoft.com/zh-cn/library/system.configuration.configurationmanager.connectionstrings(v=vs.110).aspx)




```
private static readonly string RedisConnectionString =
 ConfigurationManager.ConnectionStrings["RedisExchangeHosts"].ConnectionString;
```


#### 三、App.config配置文件读写

##### 1、依据连接串名字connectionName返回数据连接字符串  

```
//依据连接串名字connectionName返回数据连接字符串  
        public static string GetConnectionStringsConfig(string connectionName)
        {
            //指定config文件读取
            string file = System.Windows.Forms.Application.ExecutablePath;
            System.Configuration.Configuration config = ConfigurationManager.OpenExeConfiguration(file);
            string connectionString =
                config.ConnectionStrings.ConnectionStrings[connectionName].ConnectionString.ToString();
            return connectionString;
        }
```

##### 2、更新连接字符串

```
///<summary> 
        ///更新连接字符串  
        ///</summary> 
        ///<param name="newName">连接字符串名称</param> 
        ///<param name="newConString">连接字符串内容</param> 
        ///<param name="newProviderName">数据提供程序名称</param> 
        public static void UpdateConnectionStringsConfig(string newName, string newConString, string newProviderName)
        {
            //指定config文件读取
            string file = System.Windows.Forms.Application.ExecutablePath;
            Configuration config = ConfigurationManager.OpenExeConfiguration(file);

            bool exist = false; //记录该连接串是否已经存在  
            //如果要更改的连接串已经存在  
            if (config.ConnectionStrings.ConnectionStrings[newName] != null)
            {
                exist = true;
            }
            // 如果连接串已存在，首先删除它  
            if (exist)
            {
                config.ConnectionStrings.ConnectionStrings.Remove(newName);
            }
            //新建一个连接字符串实例  
            ConnectionStringSettings mySettings =
                new ConnectionStringSettings(newName, newConString, newProviderName);
            // 将新的连接串添加到配置文件中.  
            config.ConnectionStrings.ConnectionStrings.Add(mySettings);
            // 保存对配置文件所作的更改  
            config.Save(ConfigurationSaveMode.Modified);
            // 强制重新载入配置文件的ConnectionStrings配置节  
            ConfigurationManager.RefreshSection("connectionStrings");
        }
```

##### 3、返回*.exe.config文件中appSettings配置节的value项  

```
///<summary> 
        ///返回*.exe.config文件中appSettings配置节的value项  
        ///</summary> 
        ///<param name="strKey"></param> 
        ///<returns></returns> 
        public static string GetAppConfig(string strKey)
        {
            string file = System.Windows.Forms.Application.ExecutablePath;
            Configuration config = ConfigurationManager.OpenExeConfiguration(file);
            foreach (string key in config.AppSettings.Settings.AllKeys)
            {
                if (key == strKey)
                {
                    return config.AppSettings.Settings[strKey].Value.ToString();
                }
            }
            return null;
        }
```

##### 4、在*.exe.config文件中appSettings配置节增加一对键值对  

```
///<summary>  
        ///在*.exe.config文件中appSettings配置节增加一对键值对  
        ///</summary>  
        ///<param name="newKey"></param>  
        ///<param name="newValue"></param>  
        public static void UpdateAppConfig(string newKey, string newValue)
        {
            string file = System.Windows.Forms.Application.ExecutablePath;
            Configuration config = ConfigurationManager.OpenExeConfiguration(file);
            bool exist = false;
            foreach (string key in config.AppSettings.Settings.AllKeys)
            {
                if (key == newKey)
                {
                    exist = true;
                }
            }
            if (exist)
            {
                config.AppSettings.Settings.Remove(newKey);
            }
            config.AppSettings.Settings.Add(newKey, newValue);
            config.Save(ConfigurationSaveMode.Modified);
            ConfigurationManager.RefreshSection("appSettings");
        }
```

##### 5、config读取

```
using System.Configuration;
//省略其他代码
public SalesOrderData()
        {
            string str = "";
            str = ConfigurationManager.ConnectionStrings["kyd"].ToString();
            conn = new SqlConnection(str);
            cmd = conn.CreateCommand();
        }
```

##### 6、重置修改值

![](https://img-blog.csdn.net/20180416102450915?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


##### 7、保存修改值

![](https://img-blog.csdn.net/20180416102413717?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


#### 四、快速实现读写配置文件

这里以key、value进行操作为例：

```
using System;  
    using System.Collections.Generic;  
    using System.Text;  
    using System.Configuration;  
      
    namespace BoonyaTests  
    {  
        /// <summary>  
        /// 对exe.Config文件中的appSettings段进行读写配置操作  
        /// 注意：调试时，写操作将写在vhost.exe.config文件中  
        /// </summary>  
        public class ConfigAppSettings  
        {  
            /// <summary>  
            /// 写入值  
            /// </summary>  
            /// <param name="key"></param>  
            /// <param name="value"></param>  
            public static void SetValue(string key, string value)  
            {  
                //增加的内容写在appSettings段下 <add key="RegCode" value="0"/>  
                System.Configuration.Configuration config = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);  
                if (config.AppSettings.Settings[key] == null)  
                {  
                    config.AppSettings.Settings.Add(key, value);  
                }  
                else  
                {  
                    config.AppSettings.Settings[key].Value = value;  
                }  
                config.Save(ConfigurationSaveMode.Modified);  
                ConfigurationManager.RefreshSection("appSettings");//重新加载新的配置文件   
            }  
      
            /// <summary>  
            /// 读取指定key的值  
            /// </summary>  
            /// <param name="key"></param>  
            /// <returns></returns>  
            public static string GetValue(string key)  
            {   
                System.Configuration.Configuration config = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);  
                if (config.AppSettings.Settings[key] == null)  
                    return "";  
                else  
                    return config.AppSettings.Settings[key].Value;  
            }  
      
        }  
    }
```


参考文章：

[https://www.cnblogs.com/qq450867541/p/7119433.html](https://www.cnblogs.com/qq450867541/p/7119433.html)

[https://www.cnblogs.com/feiyuhuo/p/5243967.html](https://www.cnblogs.com/feiyuhuo/p/5243967.html)

[https://www.cnblogs.com/lgx5/p/7353690.html](https://www.cnblogs.com/lgx5/p/7353690.html)




