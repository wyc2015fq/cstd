# 用shell脚本生成data pipeline的日志 - happyhorizon的算法天空 - CSDN博客
2018年04月10日 16:20:27[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：231
# 用shell脚本生成data pipeline的日志
在数据处理的data pipeline通常主要的数据处理工作都是用shell脚本批量完成. 特别有必要生成各被试数据处理日志文件以便pipeline的调试, 优化, 数据处理的追踪. Python提供了一个日志记录工具包: loggings, 其实shell脚本也可以方便地生成日志. 
## 用echo命令
用echo将需要记录的内容输出, 并通过管道重定位到日志文件中
```bash
for g in `cat test` ; do  
  echo "line in test: "$g >> .logmore   
done
```
## 用cat命令配合EOF
对于内容很多的情况, 可以用EOF结合cat命令。 
EOF是END Of File的缩写,表示自定义终止符.既然自定义,那么EOF就不是固定的,可以随意设置别名,在linux控制台按ctrl-d就代表EOF, 终止正在运行的命令. 
EOF一般会配合cat能够多行文本输出. 
通过cat配合重定向(<, >, <<, >>)能够生成文件并追加操作, 其用法如下:
```
<<EOF        //开始
....
EOF            //结束
```
这种方法可以在生成pipeline中某些批量处理的shell脚本, 也可以生成日志记录.
```
cat <<stage_tpl3 > data_proc.log  // statge_tpl3是自定义终止符, 将所有的日志都写入data_proc.log文件
#!/bin/sh
if [ -f ../template_list ] ; then
    template_list=\`cat ../template_list\`
    ….
else
    ...
fi
...
stage_tpl3
```
## 用反引号“生成日志必要的信息
反引号` ` 
反引号` `是命令替换，Shell可以先执行` `中的命令，将输出结果暂时保存，在适当的地方输出。语法: `command`
```bash
#!/usr/bin/env bash
# encoding: utf-8
echo [`date`] [`hostname`] [`uname -a`] [`pwd`] [$0 $@] >> .log
```
得到的.log文件内容如下:  
[Tue Apr 10 16:03:07 CST 2018] [DL] [Linux DL 4.4.0-116-generic #140-Ubuntu SMP Mon Feb 12 21:23:04 UTC 2018 x86_64 x86_64 x86_64 GNU/Linux] [/home/dl/] [./test ]
