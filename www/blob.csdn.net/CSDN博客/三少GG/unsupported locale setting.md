# unsupported locale setting - 三少GG - CSDN博客
2015年12月08日 14:03:42[三少GG](https://me.csdn.net/scut1135)阅读数：1953
unsupported locale setting
Traceback (most recent call last):
  File "/usr/lib/python3/dist-packages/CommandNotFound/util.py", line 24, in crash_guard
    callback()
  File "/usr/lib/command-not-found", line 69, in main
    enable_i18n()
  File "/usr/lib/command-not-found", line 40, in enable_i18n
    locale.setlocale(locale.LC_ALL, '')
  File "/usr/lib/python3.3/locale.py", line 541, in setlocale
    return _setlocale(category, locale)
**locale.Error: unsupported locale setting**
dpkg-reconfigure locales
perl: warning: Setting locale failed.
perl: warning: Please check that your locale settings:
LANGUAGE = "en_US:",
LC_ALL = (unset),
LC_CTYPE = "zh_CN.UTF-8",
LANG = "en_US.UTF-8"
    are supported and installed on your system.
perl: warning: Falling back to the standard locale ("C").
locale: Cannot set LC_CTYPE to default locale: No such file or directory
locale: Cannot set LC_ALL to default locale: No such file or directory
******************************
Type the following in terminal to get rid of that error,
```
sudo
 apt-get install language-pack-en-base
```
```
sudo
 dpkg-reconfigure locales
```
########################################
 locale -a
locale: Cannot set LC_CTYPE to default locale: No such file or directory

解决核心：  服务器实体终端就恢复成了英文的语言环境。
**修改Ubuntu的配置文件/etc/default/locale将原来的配置内容修改为LANG=”en_US.UTF-8″LANGUAGE=”en_US:en”再在终端下运行：$sudo locale-gen -en_US:en**
注销或重启后，Ubuntu Server真正服务器实体终端就恢复成了英文的语言环境。
所以，此方法不是真正意义上的中文化，而是恢复英文的默认编码
**还是没解决，放弃。。。很可能腾讯云机器初始化问题造成**
参考： 把语言环境变量改为英文 http://wiki.ubuntu.org.cn/%E4%BF%AE%E6%94%B9locale
