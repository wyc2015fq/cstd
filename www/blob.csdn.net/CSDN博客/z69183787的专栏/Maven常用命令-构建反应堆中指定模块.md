# Maven常用命令 - 构建反应堆中指定模块 - z69183787的专栏 - CSDN博客
2016年05月16日 10:51:15[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1561
mvn -h 可以看到很多命令及其用途；
-am --also-make 同时构建所列模块的依赖模块；
-amd -also-make-dependents 同时构建依赖于所列模块的模块；
-pl --projects <arg> 构建制定的模块，模块间用逗号分隔；
-rf -resume-from <arg> 从指定的模块恢复反应堆。
假设有如下聚合模块：account-aggregator ，其依次包含如下几个模块：account-parent, account-email, account-persist；
其中，~email和~persist模块都继承~parent模块。
**[html]**[view
 plain](http://blog.csdn.net/jason5186/article/details/39530087#)[copy](http://blog.csdn.net/jason5186/article/details/39530087#)
[print](http://blog.csdn.net/jason5186/article/details/39530087#)[?](http://blog.csdn.net/jason5186/article/details/39530087#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/474683/fork)
- <modules>
- <module>account-email</module>
- <module>account-persist</module>
- <module>account-parent</module>
- </modules>
例一：
当执行mvn clean install后会得到如下几个模块：~aggregator, ~parent, ~email, ~persist。
例二：
-pl 构建制定模块
当执行mvn clean install -pl accouont-email,account-persist后会得到如下几个模块：~email和~persist。
例三：
-am 选项可以同时构建所列模块的依赖模块。
当执行mvn clean install -pl account-email -am 后会得到如下几个模块：~parent和~email。
例四：
-amd 选项可以同时构建依赖于所列模块的模块。
mvn clean install -pl account-parent -amd 后会得到如下模块：~parent、~email和~persist。
例五：
-rf 选项可以在完整的反应堆构建顺序基础上，指定从哪个模块开始构建。
mvn clean install -rf account-email ，由于~email位于第三，它之后有~persist，因此会得到如下输出模块：~email和~persist。
例六：
在-pl -am或者-pl -amd的基础上，还能应用-rf参数，以对裁剪后的反应堆再次裁剪。
mvn clean install -pl account-parent -amd -rf account-email
其中，-pl 和 -amd参数会得到：~parent, ~email和~persist模块，在此基础上 -rf参数制定从~email开始构建，于是会得到：~email和~persist。
