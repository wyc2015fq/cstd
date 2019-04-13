
# Gradle基础：12:Option使用说明 - 知行合一 止于至善 - CSDN博客

2018年12月10日 08:16:17[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：333


![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
在前面的文章中使用过gradle -v来检查gradle的版本，-v是gradle命令的一个Option，这篇文章介绍一下gradle常用的Option以及使用方法。
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# gradle命令使用方式
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)命令格式：
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
> gradle [option列表] [task列表]
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 获取gradle Option信息
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)使用gradle --help或者gradle -h或者gradle -?即可获得gradle常用Option使用方式的说明。
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# Option的生命周期状态
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)Option有如下四个状态
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)Internal：一般为Gradle内部使用
Incubating：孵化阶段，接收用户反馈，可能发生变化
Public：稳定阶段
Deprecated：废弃阶段，后续版本可能不再支持
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)

Internal
Incubating
Public
Deprecated
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 常见Option说明
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)|Option缩写|Option|用法说明|生命周期|
|---|---|---|---|
|-?或者-h|–help|显示常见Option的使用说明信息.|public|
|-a|–no-rebuild|对于project的依赖不进行rebuild|deprecated|
|-b|–build-file|使用指定的脚本进行构建取代build.gradle.|public|
|-|–build-cache|使用缓存进行构建，gradle会尝试使用前次的缓存进行构建|public|
|-c|–settings-file|指定设定文件的脚本取代settings.gradle|public|
|-|–configure-on-demand|只配置必须的工程，在大型项目的多工程构建时能够减少配置时间|incubating|
|-|–console|指定控制台输出信息的类型，可选值：plain/auto/rich/verbose，auto为缺省设定值|public|
|-|–continue|任务失败，后续任务仍会继续执行|public|
|-D|–system-prop|用于设定JVM的系统属性(使用示例： -Dmyprop=myvalue).|public|
|-d|–debug|debug模式，包含普通的stacktrace信息|public|
|-|–daemon|使用Gradle Daemon进行进行构建，当进程没有运行时会启动进程|public|
|-|–foreground|以foreground方式启动Gradle Daemon进程|incubating|
|-g|–gradle-user-home|指定gradle用户的home目录，缺省为${HOME}/.gradle|public|
|-I|–init-script|指定初始化脚本|public|
|-i|–info|设定log级别为info|public|
|-|–include-build|组合方式引入指定构建|incubating|
|-m|–dry-run|模拟执行任务而不实际执行，initialization和configuration阶段的动作会被执行，而execution阶段的不会被执行|public|
|-|–max-workers|设定gradle可并行的workers数量|incubating|
|-|–no-build-cache|不使用缓存|public|
|-|–no-configure-on-demand|不使用configure-on-demand|incubating|
|-|–no-daemon|不使用gradle daemon进行构建，缺省方式使用daemon构建，偶尔需要不使用的情况此Option较为有用|public|
|-|–no-parallel|不使用并行方式构建|incubating|
|-|–no-scan|不生成build scan|incubating|
|-|–offline|离线方式进行构建，不会去访问网络资源|public|
|-P|–project-prop|设定构建脚本所使用的属性 (使用示例. -Pmyprop=myvalue).|public|
|-p|–project-dir|指定gradle的构建目录，缺省为当前目录|public|
|-|–parallel|并行方式进行构建，gradle会尝试检测可以使用的线程个数进行构建|incubating|
|-|–profile|对构建时间进行优并在<build_dir>/reports/profile目录下生成相关报表|public|
|-|–project-cache-dir|指定项目缓存目录，缺省为根项目目录下的.gradle目录|public|
|-q|–quiet|只显示错误日志信息|public|
|-|–recompile-scripts|强制构建脚本编译执行|deprecated|
|-|–refresh-dependencies|更新依赖状态|public|
|-|–rerun-tasks|忽略前次任务执行的缓存结果|public|
|-S|–full-stacktrace|显示所有异常的stacktrace的全部信息|public|
|-s|–stacktrace|显示所有异常的stacktrace的信息|public|
|-|–scan|生成build scan|incubating|
|-|–status|显示Gradle Daemon状态信息|public|
|-|–stop|停止Gradle Daemon进程|public|
|-t|–continuous|开启持续构建，当任务文件输入发生变化时，gradle不退出而会重新执行任务|incubating|
|-u|–no-search-upward|不向上搜索服务目录的设定文件|public|
|-|–update-locks|执行特定的dependency lock相关更新|incubating|
|-v|–version|显示版本信息|public|
|-w|–warn|设定日志级别为warn|public|
|-|–warning-mode|设定警告模式，可设定为：all/summary/none，缺省为summary|public|
|-|–write-locks|生成或更新lock信息|incubating|
|-x|–exclude-task|指定不被执行的任务|public|
> 基本原则：

> 为了避免不必要的伤害，incubating的特性在稳定之后再使用，而deprecated的特性则尽量尽早替换为代替方案，这属于后续版本升级的潜在技术债务。

> 参考内容

> [https://docs.gradle.org/current/userguide/feature_lifecycle.html](https://docs.gradle.org/current/userguide/feature_lifecycle.html)

> [https://docs.gradle.org/current/userguide/command_line_interface.html](https://docs.gradle.org/current/userguide/command_line_interface.html)

[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
