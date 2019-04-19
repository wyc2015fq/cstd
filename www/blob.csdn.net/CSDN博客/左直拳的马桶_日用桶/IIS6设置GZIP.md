# IIS6设置GZIP - 左直拳的马桶_日用桶 - CSDN博客
2011年05月17日 19:26:00[左直拳](https://me.csdn.net/leftfist)阅读数：1951标签：[iis																[exe																[dll																[asp																[html](https://so.csdn.net/so/search/s.do?q=html&t=blog)](https://so.csdn.net/so/search/s.do?q=asp&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=exe&t=blog)](https://so.csdn.net/so/search/s.do?q=iis&t=blog)
个人分类：[IIS																[Web](https://blog.csdn.net/leftfist/article/category/674421)](https://blog.csdn.net/leftfist/article/category/2459577)
1、IIS里要设置
![](https://img-blog.csdn.net/20131008145919015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2、C:/WINDOWS/system32/inetsrv/MetaBase.xml要设置
该文件很重要，请修改前先备份！ 
<IIsCompressionScheme    Location ="/LM/W3SVC/Filters/Compression/deflate"
        HcCompressionDll="%windir%/system32/inetsrv/gzip.dll"
        HcCreateFlags="0"
        HcDoDynamicCompression="TRUE"
        HcDoOnDemandCompression="TRUE"
        HcDoStaticCompression="FALSE"
        HcDynamicCompressionLevel="9"
        HcFileExtensions="htm
            html
            shtml
            txt
            js
            css"
        HcOnDemandCompLevel="10"
        HcPriority="1"
        HcScriptFileExtensions="aspx
            ascx
            asmx
            ashx
            asp
            dll
            exe"
    >
</IIsCompressionScheme>
<IIsCompressionScheme    Location ="/LM/W3SVC/Filters/Compression/gzip"
        HcCompressionDll="%windir%/system32/inetsrv/gzip.dll"
        HcCreateFlags="1"
        HcDoDynamicCompression="TRUE"
        HcDoOnDemandCompression="TRUE"
        HcDoStaticCompression="TRUE"
        HcDynamicCompressionLevel="9"
        HcFileExtensions="htm
            html
            shtml
            txt
            js
            css"
        HcOnDemandCompLevel="10"
        HcPriority="1"
        HcScriptFileExtensions="aspx
            ascx
            asmx
            ashx
            asp
            dll
            exe"
    >
</IIsCompressionScheme>
注意 蓝色的这些值，一定要每行一个，排在一行，用空格隔开是不行的。
正常保存的时候，因为IIS正在使用，应该会失败。需要先将IIS停止，再保存，然后再开启IIS。
命令：iisreset /stop
iisreset /start
3、到中国站长网检测检测
http://tool.chinaz.com/Gzips/
