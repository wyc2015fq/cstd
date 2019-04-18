# linux文本分析工具awk解读 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月28日 10:33:49[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：1761








        awk是一个强大的文本分析工具，相对于grep的查找、sed的编辑，awk在其对数据分析并生成报告时，显得尤为强大。awk把文件逐行的读入，以空格为默认分隔符将每行切片，切开的部分再进行各种分析处理。显然awk适用于文件中的每行都被特定的分隔符隔开，文件是一个行列矩阵。



### 语法
awk [选项参数]'script'var=value file(s)或
awk [选项参数]-f scriptfile var=value file(s)



选项参数说明：

```
awk --help
用法: awk [POSIX 或 GNU 风格选项] -f 脚本文件 [--] 文件 ...
用法: awk [POSIX 或 GNU 风格选项] [--] '程序' 文件 ...
POSIX 选项:                     GNU 长选项:
        -f 脚本文件             --file=脚本文件
        -F fs                   --field-separator=fs
        -v var=val              --assign=var=val
        -m[fr] val
        -O                      --optimize
        -W compat               --compat
        -W copyleft             --copyleft
        -W copyright            --copyright
        -W dump-variables[=file]        --dump-variables[=file]
        -W exec=file            --exec=file
        -W gen-po               --gen-po
        -W help                 --help
        -W lint[=fatal]         --lint[=fatal]
        -W lint-old             --lint-old
        -W non-decimal-data     --non-decimal-data
        -W profile[=file]       --profile[=file]
        -W posix                --posix
        -W re-interval          --re-interval
        -W source=program-text  --source=program-text
        -W traditional          --traditional
        -W usage                --usage
        -W use-lc-numeric       --use-lc-numeric
        -W version              --version
```

## 运算符
|运算符|描述|
|----|----|
|= += -= *= /= %= ^= **=|赋值|
|?:|C条件表达式|
||||逻辑或|
|&&|逻辑与|
|~ ~!|匹配正则表达式和不匹配正则表达式|
|< <= > >= != ==|关系运算符|
|空格|连接|
|+ -|加，减|
|* / %|乘，除与求余|
|+ - !|一元加，减和逻辑非|
|^ ***|求幂|
|++ --|增加或减少，作为前缀或后缀|
|$|字段引用|
|in|数组成员|




官网参考：https://www.gnu.org/software/gawk/manual/gawk.html

参考：http://www.cnblogs.com/wangqiguo/p/5863266.html

           http://www.runoob.com/linux/linux-comm-awk.html

基本用法练习：

1）输出第一列和第四列：
`awk '{print $1,$4}' train_features_p.txt`



2）过滤第一列大于2且第二列等于3：


`awk '$1>2 && $2=="3" {print $1,$2,$3}' train_features_p.txt`

更多强大的内置函数和变量，有需要可以通过参考加以应用，可以作为大数据平台辅助的数据处理。






如果要实现从文档中提取满足条件的行数，可以如下：


`cat all.txt|awk '{if(($1>0.5)) {print $1}}' > 1.txt`这个命令就把all.txt中第一列大于0.5的记录输出1.txt中

再通过wc -l 1.txt就可以得到有多少行。













