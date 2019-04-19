# 管理 Exchange 2016 日常性能日志-挥墨的书童-51CTO博客
`  最近一位朋友发我一张Exchange报错的截图，所最近这个错误一直反复出现，让我帮忙看一下。`
![管理 Exchange 2016 日常性能日志](https://s1.51cto.com/images/blog/201812/29/e4bdb120c7afa59e553826d4a9ce77c1.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
通过截图中的报错信息，我们可以看到，**‘DailyPerformanceLogs’**这个文件夹已经达到了最大设置的值。而这个错误就是由Exchange Diagnostic service 认为这有可能会有数据丢失，所以才抛出错误日志。
分析到这里我们就可以想到有两种解决方法：
- 禁用Exchange Diagnostic service log的检查，让它不在去检查。（治标不治本）
设置方法：使用记事本打开C:\Program Files\Microsoft\Exchange Server\V15\bin\Microsoft.Exchange.Diagnostics.Service.exe.config
![管理 Exchange 2016 日常性能日志](https://s1.51cto.com/images/blog/201812/29/16acffca168e07f9d3186f298768e65f.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
在 ‘DailyPerformanceLogs’ 下面我们需要把“LogDataLoss” = “False” 
![管理 Exchange 2016 日常性能日志](https://s1.51cto.com/images/blog/201812/29/dfe79d0dd3fd7f9eda2f8ac40e7a54cc.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
- 为日志分配一个独立的空间较大的路径
在日常Exchange运维中我们可能遇到日志把磁盘撑满而导致系统出现问题，为了避免此种情况，我们需要对这些日志文件进行管理。一般有的会使用备份软件/工具把日志备份。而对于没有专业的备份软件情况下，我们只能手动的去清理这些文件，而这样会比较痛苦。
下面是我编写的一个脚本可以把它放在任务计划中，每天定时执行，清理三天前的日志
#----- Daily Performance Logs-----#
#----- define parameters -----#
#----- get current date ----#
$Now = Get-Date
#----- define amount of days ----#
$Days = "3"
#----- define folder where files are located ----#
$TargetFolder = "C:\Program Files\Microsoft\Exchange Server\V15\Logging\Diagnostics\DailyPerformanceLogs"
#----- define extension ----#
$Extension = "*.blg"
#----- define LastWriteTime parameter based on $Days ---#
$LastWrite = $Now.AddDays(-$Days)
$Files = Get-Childitem $TargetFolder -Include $Extension -Recurse | Where {$_.LastWriteTime -le "$LastWrite"}
Remove-Item $Files
#----- W3SVC1 Folder-----#
#----- define folder where files are located ----#
$TargetFolder1 = "C:\inetpub\logs\LogFiles\W3SVC1"
#----- define extension ----#
$Extension1 = "*.log"
#----- define LastWriteTime parameter based on $Days ---#
$LastWrite = $Now.AddDays(-$Days)
$Files1 = Get-Childitem $TargetFolder1 -Include $Extension1 -Recurse | Where {$_.LastWriteTime -le "$LastWrite"}
Remove-Item $Files1
#----- W3SVC2 Folder-----#
#----- define folder where files are located ----#
$TargetFolder2 = "C:\inetpub\logs\LogFiles\W3SVC2"
#----- define extension ----#
$Extension2 = "*.log"
#----- define LastWriteTime parameter based on $Days ---#
$LastWrite = $Now.AddDays(-$Days)
$Files2 = Get-Childitem $TargetFolder2 -Include $Extension2 -Recurse | Where {$_.LastWriteTime -le "$LastWrite"}
Remove-Item $Files2
![管理 Exchange 2016 日常性能日志](https://s1.51cto.com/images/blog/201812/29/53dcca30cd3fbbab895f028a7a329c73.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
