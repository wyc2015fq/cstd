# shell 实现当前目录下多个文件合并为一个文件 - sxf_123456的博客 - CSDN博客
2018年01月31日 15:08:26[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：3810
                
当前目录下多个文件合并为一个文件
1、将多个文件合并为一个文件没有添加换行符
find ./ -name "iptv_authenticate_201801*" | xargs cat > iptv_authenticate.txt
2、设置换行符^J
find ./ -name "iptv_authenticate_201801*" | xargs sed 'a\^J' > iptv_authenticate.txt
3、默认换行符
find ./ -name "iptv_authenticate_201801*" | xargs sed 'a\' > iptv_authenticate.txt
find ./ -name "iptv_liveswitch_201801*" | xargs sed 'a\' > iptv_liveswitch.txt
find ./ -name "iptv_qualified_201801*" | xargs sed 'a\' > iptv_qualified.txt
find ./ -name "iptv_vodload_201801*" | xargs sed 'a\' > iptv_vodload.txt
当前目录下所有后缀为txt文件中追加一行数据作为文件内容的第一行内容
1、方法一
for fullpath in `find . -type f -name "*.txt"`
do
    sed -i '1i\Num\tPhone\tDate\tMessage\tId\tGudge' ${fullpath}
done
备注:
-type  f 是指后边的查找文件类型为文件
2、方法二
find . -type f -name "*.txt" | xargs -I {} sed -i '1i\Num\tPhone\tDate\tMessage\tId\tGudge' {}
            
