### 让StarDict直接用上mp3真人发音库，不用转wav



今天要用到StarDict的真人发音库，发现有WyabdcRealPeopleTTS 和OtdRealPeopleTTS
两个库。WyabdcRealPeopleTTS的音质太差，不用。那就只有用OtdRealPeopleTTS了，
但OtdRealPeopleTTS是mp3格式的，StarDict只能放wav格式的，还要把mp3转为wav，
本来转换一下不是问题，但转换后150M 的文件变成3G的文件，就算磁盘空间充足也不
是这样浪费的吧！何况这都可以存放很多部片子了～

由于我已经装了mplayer以及mp3解码器，以为在音效设置里面把play改为mplayer,
语音库路径改为/usr/share/OtdRealPeopleTTS就可以了。但修改后发现还是没发声。

决定下开源码来看一下，打开/stardict-3.0.1/src/readword.cpp一看，发现了问题所在

## 一、

```cpp
bool ReadWord::RealTts_canRead(const gchar *word)
{ 
bool return_val = false;
if (!ttspath.empty() && word && g_ascii_isalpha(word[0])) {
std::string lowerword;
const gchar *p = word;
while (*p) {
if (*p!=' ')
lowerword+=g_ascii_tolower(*p);
p++;
}
std::string filename;
std::list<std::string>::const_iterator it;
for (it=ttspath.begin(); it!=ttspath.end(); ++it) {
filename = *it + G_DIR_SEPARATOR_S + lowerword[0] + G_DIR_SEPARATOR_S + lowerword + ".wav"; #1
return_val = g_file_test(filename.c_str(), G_FILE_TEST_EXISTS); #2
if (return_val)
break;
}
}
return return_val;
}
```

红色标记第一句是找到wav文件的全路径，第二句是判断文件是否存在。由于是硬编定为wav，
所以前面修改音效设置是没有用的。

改为：

```cpp
bool ReadWord::RealTts_canRead(const gchar *word)
{ 
bool return_val = false;
if (!ttspath.empty() && word && g_ascii_isalpha(word[0])) {
std::string lowerword;
const gchar *p = word;
while (*p) {
if (*p!=' ')
lowerword+=g_ascii_tolower(*p);
p++;
}
std::string filename;
std::list<std::string>::const_iterator it;
for (it=ttspath.begin(); it!=ttspath.end(); ++it) {
std::string suffix, aword;
aword = *it + G_DIR_SEPARATOR_S + "a" + G_DIR_SEPARATOR_S + "a.mp3";
if (g_file_test(aword.c_str(), G_FILE_TEST_EXISTS))
suffix = ".mp3";
else
suffix = ".wav";
filename = *it + G_DIR_SEPARATOR_S + lowerword[0] + G_DIR_SEPARATOR_S + lowerword + suffix;
return_val = g_file_test(filename.c_str(), G_FILE_TEST_EXISTS);
if (return_val)
break;
}
}
return return_val;
}
```

在/stardict-3.0.1/src/readword.cpp中有两个这样的地方，另一个在
void ReadWord::Command_read(const gchar *word)这个函数中，都这样改。



## 二、

在/stardict-3.0.1/src/conf.cpp中
```cpp
#if defined(CONFIG_GTK) || defined (CONFIG_GPE)
add_entry("/apps/stardict/preferences/dictionary/play_command", std::string("play"));
#endif
```
把play改为mplayer。这个其实可以在安装运行以后再手动设置。

先装好mplayer及mp3解码器，archlinux用户不用急着装，在PKGBUILD设置了依赖。
接着编译源码，进入/stardict-3.0.1
```shell
./configure --prefix=/usr --sysconfdir=/etc --mandir=/usr/share/man --disable-gnome-support --disable-schemas-install --disable-espeak --disable-gucharmap --disable-festival --disable-advertisement --disable-updateinfo
```
注意：要加上红色标记的那些参数

```shell
make
```

好了，运行，没声?～

对了，把发音库拷到/usr/share/OtdRealPeopleTTS下面，
运行，还是没有声音～
从上面的代码我们可以看到，StarDict只能读到/usr/share/OtdRealPeopleTTS下面路径和文件名都是小写的
声音文件。
这么多文件要一个个手工修改？当然不是了～
我这里有个shell脚本，修改这么多文件就靠它了～
运行它后，再运行StarDict应该就可以了


OtdRealPeopleTTS真人发音库下载地址：[http://ubuntu:ubuntuftp@ftp.ubuntu.org. ... TTS.tar.gz](http://ubuntu:ubuntuftp@ftp.ubuntu.org.cn/home/dbzhang800/wiki/OtdRealPeopleTTS.tar.gz)
修改好的源码：[http://myfilestorage.googlecode.com/fil ... .1.tar.bz2](http://myfilestorage.googlecode.com/files/stardict-3.0.1.tar.bz2)
convert脚本(打开可以看到使用方法)：<http://myfilestorage.googlecode.com/files/convert>

由于这些天在学习使用archlinux，所以特意写了个PKGBUILD
<http://myfilestorage.googlecode.com/files/PKGBUILD>





白鳍豚 写道:

arch
2009-10-21 19:56:13 (167 KB/s) - 已保存 “stardict-3.0.1.tar.bz2.part” [1964195/1964195])

==> 错误： stardict_gcc43.patch 没有在创建目录中找到，也不是一个 URL。



不好意思，在extra仓库里面的stardict文件夹里面是自带stardict_gcc43.patch和sigc++.patch这两个文件的，
我忘了把它们上传到网上并加个连接。
现在好了http://myfilestorage.googlecode.com/files/PKGBUILD

 

 

《142000个单词语音库》mp3[ISO]
<http://www.verycd.com/topics/89350/> 

引用
这是根据“《142000个单词语音库》[MP3]”原贴地址转来的，原文件是wav文件，....现在转成mp3格式的，做成iso的镜像，只有370MB。

 

 

《142000个单词语音库》wav file
<http://www.verycd.com/topics/133276/> 
这 个语音库是由两部分合起来的，从Merriam-Webster Collegiate 11th Dictionary中提取的，大约有8万个左右，另外一部分是从沪江的罗德安提供的10万个语音库里提取的，去掉重复的就是现在的这个14万，在此非常 感谢罗先生的努力。

两个语音库重复的部分我保留了Webster的，如果大家喜欢罗先生的语音库可以去 <http://blog.hjenglish.com/luodean/articles/283067.html>下载，只要解压覆盖我的就可以，我们两个的文件命名规则都是一样的，以单词拼写为文件名。

##  