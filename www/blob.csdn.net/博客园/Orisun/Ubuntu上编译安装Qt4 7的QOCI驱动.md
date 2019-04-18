# Ubuntu上编译安装Qt4.7的QOCI驱动 - Orisun - 博客园







# [Ubuntu上编译安装Qt4.7的QOCI驱动](https://www.cnblogs.com/zhangchaoyang/articles/1984242.html)





刚刚在Ubuntu上安装上Oracle10g和Qt4.7，现在要安装Qt连接Oracle的驱动QOCI，方法步骤其实是参照Nokia给我文档，在/opt/qtsdk-2010.05/qt/doc/html/sql-driver.html中，我在此翻译给大家。

1.把/home/orisun/.bashrc中新添加的内容追加到/home/root/.bashrc中

2.

root@zcypc:/opt/qtsdk-2010.05/qt/src/plugins/sqldrivers/oci# qmake -o Makefile "INCLUDEPATH+=$ORACLE_HOME/rdbms/public $ORACLE_HOME/rdbms/demo" "LIBS+=-L$ORACLE_HOME/lib -Wl,-rpath,$ORACLE_HOME/lib -lclntsh -lnnz10" oci.pro
root@zcypc:/opt/qtsdk-2010.05/qt/src/plugins/sqldrivers/oci# make

3.把编译好的libqsqloci.so放到/opt/qtsdk-2010.05/qt/plugins/sqldrivers目录下，注意不是/opt/qtsdk-2010.05/qt/src/plugins/sqldrivers。这样Qt就能load QOCI了。

4.












