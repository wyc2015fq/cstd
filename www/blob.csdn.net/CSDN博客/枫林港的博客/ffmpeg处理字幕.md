# ffmpeg处理字幕 - 枫林港的博客 - CSDN博客





2018年07月26日 23:15:58[枫林港](https://me.csdn.net/u013601132)阅读数：1881








# ffmpeg处理字幕

## ffmpeg加载font

以ass为例，第一次用ffmpeg将ass字幕加到视频里面的时候报下面的错误：

> 
 Fontconfig error: Cannot load default config file

[Parsed_ass_0 @ 00000000042c33c0] No usable fontconfig configuration file found, using fallback.

Fontconfig error: Cannot load default config file

[Parsed_ass_0 @ 00000000042c33c0] Added subtitle file: ‘subs.ass’ (3 styles, 68 events)


解决办法参考：

[https://superuser.com/questions/794014/ffmpeg-ass-filter-not-working](https://superuser.com/questions/794014/ffmpeg-ass-filter-not-working)

[http://www.yaosansi.com/post/ffmpeg-burn-subtitles-into-video/](http://www.yaosansi.com/post/ffmpeg-burn-subtitles-into-video/)

也就是要配置一下fonts.conf。

### windows下

增加环境变量：

```bash
export FC_CONFIG_DIR=$(pwd)/conf

export FONTCONFIG_FILE=$(pwd)/conf/fonts.conf

export FONTCONFIG_PATH=$(pwd)/conf

export
```

VS2013调试设置环境变量：

![这里写图片描述](https://img-blog.csdn.net/20180726230938246?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

ffmpeg.exe所在的目录ffmpeg-3.3\msvc\MSVC_ffmpeg-3.0\Debug，下面新增conf目录，config目录下面放fonts.conf，fonts.conf内容如下：

```xml
<?xml version="1.0"?>
<fontconfig>

<dir>C:\WINDOWS\Fonts</dir>

<match target="pattern">
<test qual="any" name="family"><string>mono</string></test>
<edit name="family" mode="assign"><string>monospace</string></edit>
</match>

<match target="pattern">
<test qual="all" name="family" compare="not_eq"><string>sans-serif</string></test>
<test qual="all" name="family" compare="not_eq"><string>serif</string></test>
<test qual="all" name="family" compare="not_eq"><string>monospace</string></test>
<edit name="family" mode="append_last"><string>sans-serif</string></edit>
</match>

<alias>
<family>Times</family>
<prefer><family>Times New Roman</family></prefer>
<default><family>serif</family></default>
</alias>
<alias>
<family>Helvetica</family>
<prefer><family>Arial</family></prefer>
<default><family>sans</family></default>
</alias>
<alias>
<family>Courier</family>
<prefer><family>Courier New</family></prefer>
<default><family>monospace</family></default>
</alias>
<alias>
<family>serif</family>
<prefer><family>Times New Roman</family></prefer>
</alias>
<alias>
<family>sans</family>
<prefer><family>Arial</family></prefer>
</alias>
<alias>
<family>monospace</family>
<prefer><family>Andale Mono</family></prefer>
</alias>
<match target="pattern">
<test name="family" compare="eq">
<string>Courier New</string>
</test>
<edit name="family" mode="prepend">
<string>monospace</string>
</edit>
</match>
<match target="pattern">
<test name="family" compare="eq">
<string>Courier</string>
</test>
<edit name="family" mode="prepend">
<string>monospace</string>
</edit>
</match>

</fontconfig>
```

## 字幕嵌入视频

下面的命令可以将字幕合成到视频里面，成为硬字幕，不是放到mkv容器里面，所以需要重新编码：

```bash
ffmpeg -ss 00:01:15 -i "input.mkv" -vf ass=" input.ass" -t 00:02:00 -map 0:0 -acodec copy   " output.mkv"
```

## 字幕放入mkv容器

片源input.mkv，字幕input.ass。原始片源有mkv内嵌字幕，ffmpeg没有找到参数来去掉mkv内嵌的字幕，可以用MKVToolNix GUI来去掉（[https://mkvtoolnix.download/index.html](https://mkvtoolnix.download/index.html)）。MKVToolNix GUI版本12.0.0，其64位版本在win7下有问题，报mkvmerge启动不了，32位的可以在win7下运行。 
![这里写图片描述](https://img-blog.csdn.net/20180726231205476?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
选择混流，打开input.mkv后，不要勾选SubRip/SRT，然后单击“开始混流”按钮，处理完后的码流就没有内嵌字幕了。

libass好像只支持UTF-8编码的字幕，libass/ass.c里面的process_text函数，只处理了0xEFBBBF这个BOM前缀：

```
while (1) {

            if ((p == '\r') || (p == '\n'))

                ++p;

            else if (p[0] == '\xef' && p[1] == '\xbb' && p[2] == '\xbf')

                p += 3;         // U+FFFE (BOM)

            else

                break;

        }
```

BOM前缀编码：

> 
Byte-order mark Description 

EF BB BF UTF-8 

FF FE UTF-16 aka UCS-2, little endian 

FE FF UTF-16 aka UCS-2, big endian 

00 00 FF FE UTF-32 aka UCS-4, little endian. 

00 00 FE FF UTF-32 aka UCS-4, big-endian.


所以非UTF-8编码的字幕，需要用aegisub32.exe转换一下，打开字幕后，菜单->文件->导出字幕，在下面的对话框中的“文字编码”选择UTF-8，点导出就可以保存成UTF-8编码的字幕： 
![这里写图片描述](https://img-blog.csdn.net/20180726231258387?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
ffmpeg命令合成mkv：

```bash
ffmpeg -i input.mkv -i input.ass -c copy output.mkv
```

不能用-map参数，-map参数会重新编码并将字幕嵌入到视频上面，成为硬字幕。

将上面的命令中输出文件改成mp4后缀，不能将字幕放入mp4容器，有可能mp4只支持苹果的MOV text。





