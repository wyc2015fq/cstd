# Linux - - 杂记 - 默槑 - 博客园







# [Linux - - 杂记](https://www.cnblogs.com/modaidai/p/7414116.html)





1、linux怎么查看系统是32位还是64位

　　先su -root

　　方法一：getconf LONG_BIT

　　方法二：uname -a                                 #还有方法三 不过不用那么多了  敲了这两个命令 32 还是 64 一看便知



2、top -u root             #相当于win的任务管理器 首页是查看当前运行的程序 按c是查看详情 按k可以输入任务单号 结束任务

3、ls -lh                      #以M为单位查看文件大小

4、du -sh ./*.xls | sort  #检索出当前文件夹中 .xls结尾的所有文件 并排序

5、ctrl + r                    #查找history命令

6、sed (还有各种参数)  #以行为单位操作 文件 进行显示、删除、追加












