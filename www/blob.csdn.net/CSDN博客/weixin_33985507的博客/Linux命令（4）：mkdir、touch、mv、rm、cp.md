# Linux命令（4）：mkdir、touch、mv、rm、cp - weixin_33985507的博客 - CSDN博客
2018年01月29日 15:07:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
**mkdir 创建文件夹touch 创建文件mv 移动文件echo 追加字符**
**实例：**
mkdir test
touch test.log
mv test.log test  //移动文件到test文件夹下
mv test.log test11.log    //修改文件名称
rm test11.log //删除文件
rm -r test    //删除文件夹
echo 'zhejiang hangzhou xihuqu' >> b.txt
**cp命令**
用来将一个或多个源文件或者目录复制到指定的目的文件或目录。它可以将单个源文件复制成一个指定文件名的具体的文件或一个已经存在的目录下。cp命令还支持同时复制多个文件，当一次复制多个文件时，目标文件参数必须是一个已经存在的目录，否则将出现错误。
**实例：**
cp jing.log  ../jane.log       //复制文件到上层目录下并命名为jane.log
cp -r log  ./log1                //复制文件夹到当前目录并命名为log1
cp -r -a aaa/*   ./bbb      //复制aaa下面的所有文件到bbb文件夹下面
