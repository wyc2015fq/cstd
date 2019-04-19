# linux/shell 文本文件删除/删掉空行 - sxf_123456的博客 - CSDN博客
2017年04月30日 11:02:09[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：372
分别用sed awk perl grep 实现：
```
1 sed '/^$/d' input.txt > output.txt    #output file: output.txt
2 sed -i '/^$/d' input.txt      #output file: input.txt
3 awk 'NF > 0' input.txt > output.txt     #output file: output.txt
4 perl -i.backup -n -e "print if /\S/" input.txt   #output file: input.txt.backup
5 grep -v '^$' input.txt > output.txt    #output file: output.txt
```
