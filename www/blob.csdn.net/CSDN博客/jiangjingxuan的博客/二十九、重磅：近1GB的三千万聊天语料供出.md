# 二十九、重磅：近1GB的三千万聊天语料供出 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 13:56:55[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：1353











![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_412_default_big.jpeg)



经过半个月的倾力打造，建设好的聊天语料库包含三千多万条简体中文高质量聊天语料，近1G的纯文本数据。此语料库全部基于2万部影视剧字幕，经由爬取、分类、解压、语言识别、编码识别、编码转换、过滤清洗等一系列繁琐过程。把整个建设过程分享出来供大家玩耍。

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

注意：本文提到的程序和脚本都分享在[https://github.com/warmheartli/ChatBotCourse](https://github.com/warmheartli/ChatBotCourse/tree/master/subtitle/preprocess)。如需直接获取最终语料库，请见文章末尾。

## 第一步：爬取影视剧字幕

请见我的这篇文章《[二十八-脑洞大开：基于美剧字幕的聊天语料库建设方案](http://www.shareditor.com/blogshow/?blogId=105)》



## 第二步：压缩格式分类

下载的字幕有zip格式和rar格式，因为数量比较多，需要做筛选分类，以便后面的处理，这步看似简单实则不易，因为要解决：文件多无法ls的问题、文件名带特殊字符的问题、文件名重名误覆盖问题、扩展名千奇百怪的问题，我写成了python脚本mv_zip.py如下：

```python
import glob
import os
import fnmatch
import shutil
import sys

def iterfindfiles(path, fnexp):
    for root, dirs, files in os.walk(path):
        for filename in fnmatch.filter(files, fnexp):
            yield os.path.join(root, filename)

i=0
for filename in iterfindfiles(r"./input/", "*.zip"):
    i=i+1
    newfilename = "zip/" + str(i) + "_" + os.path.basename(filename)
    print filename + " <===> " + newfilename
    shutil.move(filename, newfilename)
```

其中的扩展名根据压缩文件可能有的扩展名修改成*.rar、*.RAR、*.zip、*.ZIP等



## 第三步：解压

解压这一步需要根据所用的操作系统下载不同的解压工具，建议使用unrar和unzip，为了解决解压后文件名可能重名覆盖的问题，我总结出如下两句脚本来实现批量解压：

```bash
i=0; for file in `ls`; do mkdir output/${i}; echo "unzip $file -d output/${i}";unzip -P abc $file -d output/${i} > /dev/null; ((i++)); done
i=0; for file in `ls`; do mkdir output/${i}; echo "${i} unrar x $file output/${i}";unrar x $file output/${i} > /dev/null; ((i++)); done
```



## 第四步：srt、ass、ssa字幕文件分类整理

当你下载大量字幕并解压后你会发现字幕文件类型有很多种，包括srt、lrc、ass、ssa、sup、idx、str、vtt，但是整体量级上来看srt、ass、ssa占绝对优势，因此简单起见，我们抛弃掉其他格式，只保留这三种，具体分类整理的脚本可以参考第二部压缩格式分类的方法按扩展名整理



## 第五步：清理目录

在我边整理边分析的过程中发现，我为了避免重名把文件放到不同目录里后，如果再经过一步文件类型整理，会产生非常多的空目录，每次ls都要拉好几屏，所以写了一个自动清理空目录的脚本clear_empty_dir.py，如下：

```python
import glob
import os
import fnmatch
import shutil
import sys

def iterfindfiles(path, fnexp):
    for root, dirs, files in os.walk(path):
        if 0 == len(files) and len(dirs) == 0:
            print root
            os.rmdir(root)

iterfindfiles(r"./input/", "")
```



## 第六步：清理非字幕文件

在整个字幕文件分析过程中，总有很多其他文件干扰你的视线，比如txt、html、doc、docx，因为不是我们想要的，因此干脆直接干掉，批量删除脚本del_file.py如下：

```python
import glob
import os
import fnmatch
import shutil
import sys

def iterfindfiles(path, fnexp):
    for root, dirs, files in os.walk(path):
        for filename in fnmatch.filter(files, fnexp):
            yield os.path.join(root, filename)

for suffix in ("*.mp4", "*.txt", "*.JPG", "*.htm", "*.doc", "*.docx", "*.nfo", "*.sub", "*.idx"):
    for filename in iterfindfiles(r"./input/", suffix):
        print filename
        os.remove(filename)
```



## 第七步：多层解压缩

把抓取到的字幕压缩包解压后有的文件里面依然还有压缩包，继续解压才能看到字幕文件，因此上面这些步骤再来一次，不过要做好心理准备，没准需要再来n次！



## 第八步：舍弃剩余的少量文件

经过以上几步的处理后剩下一批无扩展名的、特殊扩展名如：“srt.简体”，7z等、少量压缩文件，总体不超过50M，想想伟大思想家马克思教导我们要抓主要矛盾，因此这部分我们直接抛弃掉



## 第九步：编码识别与转码

字幕文件就是这样的没有规范，乃至于各种编码齐聚，什么utf-8、utf-16、gbk、unicode、iso8859琳琅满目应有尽有，我们要统一到一种编码方便使用，索性我们统一到utf-8，get_charset_and_conv.py如下:

```python
import chardet
import sys
import os

if __name__ == '__main__':
    if len(sys.argv) == 2:
        for root, dirs, files in os.walk(sys.argv[1]):
            for file in files:
                file_path = root + "/" + file
                f = open(file_path,'r')
                data = f.read()
                f.close()
                encoding = chardet.detect(data)["encoding"]
                if encoding not in ("UTF-8-SIG", "UTF-16LE", "utf-8", "ascii"):
                    try:
                        gb_content = data.decode("gb18030")
                        gb_content.encode('utf-8')
                        f = open(file_path, 'w')
                        f.write(gb_content.encode('utf-8'))
                        f.close()
                    except:
                        print "except:", file_path
```



## 第十步：筛选中文

考虑到我朝广大人民的爱国热情，我只做中文，所以什么英文、韩文、日文、俄文、火星文、鸟语……全都不要，参考extract_sentence_srt.py如下：

```python
# coding:utf-8
import chardet
import os
import re

cn=ur"([\u4e00-\u9fa5]+)"
pattern_cn = re.compile(cn)
jp1=ur"([\u3040-\u309F]+)"
pattern_jp1 = re.compile(jp1)
jp2=ur"([\u30A0-\u30FF]+)"
pattern_jp2 = re.compile(jp2)

for root, dirs, files in os.walk("./srt"):
    file_count = len(files)
    if file_count > 0:
        for index, file in enumerate(files):
            f = open(root + "/" + file, "r")
            content = f.read()
            f.close()
            encoding = chardet.detect(content)["encoding"]
            try:
                for sentence in content.decode(encoding).split('\n'):
                    if len(sentence) > 0:
                        match_cn =  pattern_cn.findall(sentence)
                        match_jp1 =  pattern_jp1.findall(sentence)
                        match_jp2 =  pattern_jp2.findall(sentence)
                        sentence = sentence.strip()
                        if len(match_cn)>0 and len(match_jp1)==0 and len(match_jp2) == 0 and len(sentence)>1 and len(sentence.split(' ')) < 10:
                            print sentence.encode('utf-8')
            except:
                continue
```

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址



## 第十一步：字幕中的句子提取

不同格式的字幕有特定的格式，除了句子之外还有很多字幕的控制语句，我们一律过滤掉，只提取我们想要的重点内容，因为不同的格式都不一样，在这里不一一举例了，感兴趣可以去我的github查看，在这里单独列出ssa格式字幕的部分代码供参考：

```python
if line.find('Dialogue') == 0 and len(line) < 500:
    fields = line.split(',')
    sentence = fields[len(fields)-1]
    tag_fields = sentence.split('}')
    if len(tag_fields) > 1:
        sentence = tag_fields[len(tag_fields)-1]
```



## 第十二步：内容过滤

经过上面几步的处理，其实已经形成了完整的语料库了，只是里面还有一些不像聊天的内容我们需要进一步做优化，包括：过滤特殊的unicode字符、过滤特殊的关键词（如：字幕、时间轴、校对……）、去除字幕样式标签、去除html标签、去除连续特殊字符、去除转义字符、去除剧集信息等，具体代码如下：

```python
# coding:utf-8
import sys
import re
import chardet

if __name__ == '__main__':
    #illegal=ur"([\u2000-\u2010]+)"
    illegal=ur"([\u0000-\u2010]+)"
    pattern_illegals = [re.compile(ur"([\u2000-\u2010]+)"), re.compile(ur"([\u0090-\u0099]+)")]
    filters = ["字幕", "时间轴:", "校对:", "翻译:", "后期:", "监制:"]
    filters.append("时间轴：")
    filters.append("校对：")
    filters.append("翻译：")
    filters.append("后期：")
    filters.append("监制：")
    filters.append("禁止用作任何商业盈利行为")
    filters.append("http")
    htmltagregex = re.compile(r'<[^>]+>',re.S)
    brace_regex = re.compile(r'\{.*\}',re.S)
    slash_regex = re.compile(r'\\\w',re.S)
    repeat_regex = re.compile(r'[-=]{10}',re.S)
    f = open("./corpus/all.out", "r")
    count=0
    while True:
        line = f.readline()
        if line:
            line = line.strip()

            # 编码识别，不是utf-8就过滤
            gb_content = ''
            try:
                gb_content = line.decode("utf-8")
            except Exception as e:
                sys.stderr.write("decode error:  ", line)
                continue

            # 中文识别，不是中文就过滤
            need_continue = False
            for pattern_illegal in pattern_illegals:
                match_illegal = pattern_illegal.findall(gb_content)
                if len(match_illegal) > 0:
                    sys.stderr.write("match_illegal error: %s\n" % line)
                    need_continue = True
                    break
            if need_continue:
                continue

            # 关键词过滤
            need_continue = False
            for filter in filters:
                try:
                    line.index(filter)
                    sys.stderr.write("filter keyword of %s %s\n" % (filter, line))
                    need_continue = True
                    break
                except:
                    pass
            if need_continue:
                continue

            # 去掉剧集信息
            if re.match('.*第.*季.*', line):
                sys.stderr.write("filter copora %s\n" % line)
                continue
            if re.match('.*第.*集.*', line):
                sys.stderr.write("filter copora %s\n" % line)
                continue
            if re.match('.*第.*帧.*', line):
                sys.stderr.write("filter copora %s\n" % line)
                continue

            # 去html标签
            line = htmltagregex.sub('',line)

            # 去花括号修饰
            line = brace_regex.sub('', line)

            # 去转义
            line = slash_regex.sub('', line)

            # 去重复
            new_line = repeat_regex.sub('', line)
            if len(new_line) != len(line):
                continue

            # 去特殊字符
            line = line.replace('-', '').strip()

            if len(line) > 0:
                sys.stdout.write("%s\n" % line)
            count+=1
        else:
            break
    f.close()
    pass
```





