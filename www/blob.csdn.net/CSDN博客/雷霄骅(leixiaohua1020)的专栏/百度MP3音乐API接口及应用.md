# 百度MP3音乐API接口及应用 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月22日 00:56:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：27
个人分类：[纯编程](https://blog.csdn.net/leixiaohua1020/article/category/1647545)










当你在百度去搜索一首歌时,你会发现有种更简单的方法,嘿嘿,告诉你个秘密,百度有个不公开的API

http://box.zhangmen.baidu.com/x?op=12&count=1&title=大约在冬季$$齐秦$$$$

用上面的地址,红色部分改成歌名与作者名,然后百度就会给你一个XML:



```
<?xml version="1.0" encoding="gb2312" ?> 
<result>
   <count>1</count> 
<data>
<encode>http://song.feifa-radio.com/Q/20050701/jingxuan/YjI$.Wma</encode> 
<decode>1.Wma</decode> 
<type>2</type> 
<lrcid>49684</lrcid> 
    </data>
</result>
```


其中的count值为1是说返回的是一个,这个没什么用,接下来的东西就有用了,encode里的值是歌曲加密后的地址,加密只是对文件名加密的,我们需要的只是前面的路径,也就是 http://song.feifa-radio.com/Q/20050701/jingxuan/ 这部分,然后复制decode 的值: 1.Wma 与前面的相拼就是正确的下载地址:
http://song.feifa-radio.com/Q/20050701/jingxuan/1.Wma
后面的type的值为2表示此歌曲文件类型是wma的,其它的:1表示rm,0表示MP3,通常我们下的类型都是MP3或WMA的,所以只要有2或0的
lrcid这个的值是百度服务器上这首歌的歌词文件的文件名,这个文件的路径是:http://box.zhangmen.baidu.com/bdlrc/496/49684.lrc


这个地址解析下:
http://box.zhangmen.baidu.com/bdlrc/ 这个是百度lrc歌词存放地址,后面的496是一个的不定的,民就是说歌曲不同那个目录名也不同,它的算法是拿歌词文件名(也就是上面说的 49684) 除以一百,然后取小于等于其结果的最大整数,如上面的:49684/100 =496.84 取小于等于496.84 的最大整数就是496,于是这首歌完整的歌词地址就出来了:http://box.zhangmen.baidu.com/bdlrc/496/49684.lrc

似乎说得复杂了了,呵呵,希望对大家有用







```
<appSettings>  
  <!--搜搜音乐地址 http://cgi.music.soso.com/fcgi-bin/m.q?w=[歌名/歌手名]&p=[页数]&t=[格式]-->  
  <!--格式: -1-全部 0-MP3 1-RM ，2-WMA-->  
  <add key="SosoUrl" value="http://cgi.music.soso.com/fcgi-bin/m.q?w={0}&p=1&t={1}"/>  
  <!--百度音乐地址 http://mp3.baidu.com/m?f=ms&rf=idx&tn=baidump3&ct=134217728&lf=&rn=&word=[歌名/歌手名]&lm=[格式]&pg=[页数]-->  
  <!---格式：-1-全部 0-MP3 1-RM ，2-WMA-->  
  <add key="BaiduUrl" value="http://mp3.baidu.com/m?f=ms&rf=idx&tn=baidump3&ct=134217728&lf=&rn=&word={0}&lm={1}"/>  
  <!--百度音乐快搜地址 http://box.zhangmen.baidu.com/x?op=12&count=1&title=[歌名]$$[歌手名]$$$$-->  
  <add key="BaiduQuickUrl" value="http://box.zhangmen.baidu.com/x?op=12&count=1&title={0}$${1}$$$$"/>  
  <!--千千静听歌词信息地址 http://ttlrcct.qianqian.com/dll/lyricsvr.dll?sh?Artist=[歌手名]&Title=[歌名]-->  
  <add key="TTLrcListUrl" value="http://ttlrcct.qianqian.com/dll/lyricsvr.dll?sh?Artist={0}&Title={1}"/>  
  <!--千千静听歌词地址 http://ttlrccnc.qianqian.com/dll/lyricsvr.dll?dl?Id=[歌曲ID]&Code=[title和artist]&uid=03&mac=002421585787&hds=WD-WMAV22344505"-->  
  <add key="TTLrcUrl" value="http://ttlrccnc.qianqian.com/dll/lyricsvr.dll?dl?Id={0}&Code={1}&uid=03&mac=002421585787&hds=WD-WMAV22344505"/>  
</appSettings>
```


 原文地址：[http://cloud21.iteye.com/blog/611914](http://cloud21.iteye.com/blog/611914)






