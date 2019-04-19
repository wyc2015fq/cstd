# 常用PHP文件操作函数 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年06月12日 09:48:21[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：115
注：文件操作函数的行为受到 php.ini 中设置的影响。
当在 Unix 平台上规定路径时，正斜杠 (/) 用作目录分隔符。而在 Windows 平台上，正斜杠 (/) 和反斜杠 (\) 均可使用。
- basename()返回路径中的文件名。分为带扩展名和不带扩展名的。
- copy()	复制文件。
- dirname()	返回路径中的目录部分。
- disk_free_space()	返回目录的可用空间。
- disk_total_space()	返回一个目录的磁盘总容量。
- file_exists()	检查文件或目录是否存在。
- file_get_contents()	将文件读入字符串。
- file_put_contents()	将字符串写入文件。
- filesize()	返回文件大小。
- filetype()	返回文件类型。
- glob()	返回一个包含匹配指定模式的文件名/目录的数组。
- is_dir()	判断指定的文件名是否是一个目录。
- is_writable()	判断文件是否可写。
- mkdir()	创建目录
- move_uploaded_file()	将上传的文件移动到新位置。
- parse_ini_file()	解析一个配置文件。
- realpath()	返回绝对路径名。
- rename()	重名名文件或目录。
- tempnam()	创建唯一的临时文件。
- tmpfile()	建立临时文件。
- unlink()	删除文件。
- chmod()	改变文件模式。
- chown()	改变文件所有者。
- chgrp()	改变文件组。
$config = parse_ini_file($config_include_dir . "/aec/platform.ini");
$agent_name = $config["agent_name"];
ini文件
[SDK]
agent_name=star
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%B8%B8%E7%94%A8PHP%E6%96%87%E4%BB%B6%E6%93%8D%E4%BD%9C%E5%87%BD%E6%95%B0&action=edit&section=1)]艺搜参考
[http://www.w3school.com.cn/php/php_ref_filesystem.asp](http://www.w3school.com.cn/php/php_ref_filesystem.asp)
