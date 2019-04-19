# linux常用解压命令 - sxf_123456的博客 - CSDN博客
2018年03月30日 21:04:51[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：681
                1、zip -r myfile.zip  /myfile #将myfile目录下所有文件(包括子目录)压缩为myfile.zip
2、unzip -d /home   myfile.zip #将myfile.zip 文件解压到/home目录
3、zip -d myfile.zip  smart.txt #删除myfile.zip中的smart.txt文件
4、zip -m myfile.zip  smart.txt #将smart.txt文件添加到myfile.zip中
5、zip -r myfile.zip  file1 file2 file3 /home #将file1,file2,file3和home目录压缩到myfile.zip文件中
6、tar -cvf  jpg.tar  *.jpg  #将目录下所有的jpg文件压缩到jpg.tar中
7、tar -czf  jpg.tar.gz   *.jpg  #将目录下所有的jpg文件压缩到jpg.tar.gz
8、tar -cjf  jpg.tar.bz2  *jpg  #将目录下所有的jpg文件压缩到jpg.tar.bz2
9、zip jpg.zip  *.jpg  #将所有的jpg文件，压缩为jpg.zip
10、tar -xvf file.tar  #解压tar包
11、tar -xzvf file.tar.gz  #解压tar.gz
12、tar -xjvf file.tar.bz2  #解压tar.bz2
13、tar -xZvf file.tar.Z    #解压tar.Z
14、unzip file.zip  #解压zip
