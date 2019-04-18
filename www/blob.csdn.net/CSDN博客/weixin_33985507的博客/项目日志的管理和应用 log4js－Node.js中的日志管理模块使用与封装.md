# 项目日志的管理和应用 log4js－Node.js中的日志管理模块使用与封装 - weixin_33985507的博客 - CSDN博客
2017年05月25日 11:56:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：13
开发过程中，日志记录是必不可少的事情，尤其是生产系统中经常无法调试，因此日志就成了重要的调试信息来源。
[Node.js](http://lib.csdn.net/base/nodejs)，已经有现成的开源日志模块，就是log4js，源码地址：[点击打开链接](https://github.com/nomiddlename/log4js-node)
项目引用方法： npm install log4js
1、配置说明（仅以常用的dateFile日志类型举例，更多说明参考[log4js-wiki](https://github.com/nomiddlename/log4js-node/wiki/Appenders)）：
**[javascript]**[view plain](http://blog.csdn.net/youbl/article/details/32708609#)[copy](http://blog.csdn.net/youbl/article/details/32708609#)
- {   
- "appenders": [   
- // 下面一行应该是用于跟express配合输出web请求url日志的  
-   {"type": "console", "category": "console"},   
- // 定义一个日志记录器  
-   {        
- "type": "dateFile",                 // 日志文件类型，可以使用日期作为文件名的占位符  
- "filename": "e:/weblogs/logs/",     // 日志文件名，可以设置相对路径或绝对路径  
- "pattern": "debug/yyyyMMddhh.txt",  // 占位符，紧跟在filename后面  
- "absolute": true,                   // filename是否绝对路径  
- "alwaysIncludePattern": true,       // 文件名是否始终包含占位符  
- "category": "logInfo"               // 记录器名  
-   } ],  
- "levels":{ "logInfo": "DEBUG"}        // 设置记录器的默认显示级别，低于这个级别的日志，不会输出  
- }  
pattern可以使用的占位符说明，不属于下列占位符格式，均会原样输出为文件名（注：不支持单个M、d、h、m）：
yy 两位年份
yyyy 四位年份
MM 两位月份
dd  两位日期
hh  两位的小时数，按24小时制
mm 两位的分数数
ss  两位的秒数
SSS 三位的毫秒数
O    时区，大写字母O，占位符输出结果为+0800
假设当前是2014年6月20月15点，那么上面的配置最终将会记录到 e:\weblogs\logs\debug\2014062015.txt这个文件中。
2、输出日志代码，首先请把上面的配置代码保存为log4js.json文件，用于代码从文件加载配置（配置独立也便于修改和发布）：
**注意：json文件不支持任何形式的注释// 或 /**/，因此上面的配置代码里的注释要全部移除，不然会编译出错的**
**[javascript]**[view plain](http://blog.csdn.net/youbl/article/details/32708609#)[copy](http://blog.csdn.net/youbl/article/details/32708609#)
- var log4js = require('log4js');  
- 
- // 注：配置里的日志目录要先创建，才能加载配置，不然会出异常  
- log4js.configure("./log4js.json");  
- var logInfo = log4js.getLogger('logInfo');  
- logInfo.info("测试日志信息");  
上述配置和代码，比较简单，我这边参考之前用的C#版本的日志记录类，重新封装了一下log4js，定义了一个logHelper.js
1、log4js.json配置文件内容（定义了4个日志记录器，分别写入到不同的日志目录；并自定义了2个属性，把一些公共属性提取到自定义属性中）：
**[javascript]**[view plain](http://blog.csdn.net/youbl/article/details/32708609#)[copy](http://blog.csdn.net/youbl/article/details/32708609#)
- {  
- "customBaseDir" :"e:/weblogs/logs/",  
- "customDefaultAtt" :{  
- "type": "dateFile",  
- "absolute": true,  
- "alwaysIncludePattern": true  
-     },  
- "appenders": [  
-             {"type": "console", "category": "console"},  
-             {"pattern": "debug/yyyyMMddhh.txt", "category": "logDebug"},  
-             {"pattern": "info/yyyyMMddhh.txt", "category": "logInfo"},  
-             {"pattern": "warn/yyyyMMddhh.txt", "category": "logWarn"},  
-             {"pattern": "err/yyyyMMddhh.txt", "category": "logErr"}  
-         ],  
- "replaceConsole": true,  
- "levels":{ "logDebug": "DEBUG", "logInfo": "DEBUG", "logWarn": "DEBUG", "logErr": "DEBUG"}  
- }  
2、logHelper.js封装的文件内容：
**[javascript]**[view plain](http://blog.csdn.net/youbl/article/details/32708609#)[copy](http://blog.csdn.net/youbl/article/details/32708609#)
- var helper = {};  
- exports.helper = helper;  
- 
- var log4js = require('log4js');  
- var fs = require("fs");  
- var path = require("path");  
- 
- // 加载配置文件  
- var objConfig = JSON.parse(fs.readFileSync("log4js.json", "utf8"));  
- 
- // 检查配置文件所需的目录是否存在，不存在时创建  
- if(objConfig.appenders){  
- var baseDir = objConfig["customBaseDir"];  
- var defaultAtt = objConfig["customDefaultAtt"];  
- 
- for(var i= 0, j=objConfig.appenders.length; i<j; i++){  
- var item = objConfig.appenders[i];  
- if(item["type"] == "console")  
- continue;  
- 
- if(defaultAtt != null){  
- for(var att in defaultAtt){  
- if(item[att] == null)  
-                     item[att] = defaultAtt[att];  
-             }  
-         }  
- if(baseDir != null){  
- if(item["filename"] == null)  
-                 item["filename"] = baseDir;  
- else  
-                 item["filename"] = baseDir + item["filename"];  
-         }  
- var fileName = item["filename"];  
- if(fileName == null)  
- continue;  
- var pattern = item["pattern"];  
- if(pattern != null){  
-             fileName += pattern;  
-         }  
- var category = item["category"];  
- if(!isAbsoluteDir(fileName))//path.isAbsolute(fileName))  
- throw new Error("配置节" + category + "的路径不是绝对路径:" + fileName);  
- var dir = path.dirname(fileName);  
-         checkAndCreateDir(dir);  
-     }  
- }  
- 
- // 目录创建完毕，才加载配置，不然会出异常  
- log4js.configure(objConfig);  
- 
- var logDebug = log4js.getLogger('logDebug');  
- var logInfo = log4js.getLogger('logInfo');  
- var logWarn = log4js.getLogger('logWarn');  
- var logErr = log4js.getLogger('logErr');  
- 
- helper.writeDebug = function(msg){  
- if(msg == null)  
-         msg = "";  
-     logDebug.debug(msg);  
- };  
- 
- helper.writeInfo = function(msg){  
- if(msg == null)  
-         msg = "";  
-     logInfo.info(msg);  
- };  
- 
- helper.writeWarn = function(msg){  
- if(msg == null)  
-         msg = "";  
-     logWarn.warn(msg);  
- };  
- 
- helper.writeErr = function(msg, exp){  
- if(msg == null)  
-         msg = "";  
- if(exp != null)  
-         msg += "\r\n" + exp;  
-     logErr.error(msg);  
- };  
- 
- // 配合express用的方法  
- exports.use = function(app) {  
- //页面请求日志, level用auto时,默认级别是WARN  
-     app.use(log4js.connectLogger(logInfo, {level:'debug', format:':method :url'}));  
- }  
- 
- // 判断日志目录是否存在，不存在时创建日志目录  
- function checkAndCreateDir(dir){  
- if(!fs.existsSync(dir)){  
-         fs.mkdirSync(dir);  
-     }  
- }  
- 
- // 指定的字符串是否绝对路径  
- function isAbsoluteDir(path){  
- if(path == null)  
- return false;  
- var len = path.length;  
- 
- var isWindows = process.platform === 'win32';  
- if(isWindows){  
- if(len <= 1)  
- return false;  
- return path[1] == ":";  
-     }else{  
- if(len <= 0)  
- return false;  
- return path[0] == "/";  
-     }  
- }  
3、代码调用：
**[javascript]**[view plain](http://blog.csdn.net/youbl/article/details/32708609#)[copy](http://blog.csdn.net/youbl/article/details/32708609#)
- var logger = require("./logHelper").helper;  
- logger.writeInfo("哈哈1开始记录日志");  
- logger.writeErr("出错了，你怎么搞的");  
4、跟express集成，可以输出客户端GET 或 POST的url（如果没用，或者不需要记录url，这段代码忽略）：
**[javascript]**[view plain](http://blog.csdn.net/youbl/article/details/32708609#)[copy](http://blog.csdn.net/youbl/article/details/32708609#)
- - //在app.js中添加以下代码, 以便在日志中输出url请求，由于加载顺序的原因，放在其他app.use前面  
- var app = express();  
- var log = require('./logHelper');  
- log.use(app);  
