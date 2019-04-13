
# Rundeck基础：6：基础概念：操作Job - 知行合一 止于至善 - CSDN博客

2019年04月11日 05:52:16[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：43


![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
这篇文章继续介绍如何在添加的Node上进行Job的创建与执行，以及执行结果的确认。
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# Comannd vs Job
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)Job在使用上更接近于运维的例行作业，而Comand则一般是人工手动执行的命令级别。
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)|概念|说明|
|---|---|
|Jobs|Job是相关操作的步骤与设定选项以及执行Job的Node所组成，而在实际的场景中，很多运维的例行操作都可以在Rundeck中以job的方式进行定义。|
|Commands|相较于Job，Command是可以在Node上进行单次执行的可执行的命令，通过Rundeck在指定的Node上进行此命令的执行。|
# 创建Job
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)使用左侧导航菜单中的Jobs菜单选项，可以进行菜单的创建
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190410161234465.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
这里示例仅简单地设定一下job的描述
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190410161546303.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
然后设立运维操作的步骤，因为一项操作可能是有多个步骤完成的，而每个步骤也可是通过command或者脚本的执行等来完成，具有多种可选择的余地。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190410161757436.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
定义三个步骤，步骤1 执行一条命令
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019041016370459.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
步骤2 执行一个脚本（多行命令）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190410163811518.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
步骤3再执行一条命令
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190410163837526.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
设定执行对象Node
缺省是在Local的那台Node上执行，这里选择两个Node，其余缺省
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190410163854946.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
发送通知
可以以多种方式进行设定，这里选择缺省的No
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190410164009132.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
排期设定
类似Crontab的主要功能，可以在这里设定具体的执行排期，当然还需要考虑到时区等信息，这里也一起考虑了。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190410164115639.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)[还有其他设定诸如超时设定，失败时最大执行次数以及重新执行的Sleep间隔等也都非常有用，可根据需要进行设定。](https://img-blog.csdnimg.cn/20190410164115639.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190410164240582.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
创建完毕之后则可以看到如下信息，还没有相关的统计信息。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190410164446420.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
Run Job Later可以设定后续执行的时间，点击Run Job Now则会立即执行，可以看到执行的实施结果如下所示
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190410164541472.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
全部执行完毕后，可以确认详细的执行信息
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190410164723921.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
同时还可以看到简单的统计信息
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190410164805965.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
而Dashboard的功能页面，CE版本只能看到这种程度，提示执行了多少次（3次执行0次失败）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190410164849748.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
其他内容
[https://liumiaocn.blog.csdn.net/article/details/89135773](https://liumiaocn.blog.csdn.net/article/details/89135773)
参考内容
[https://www.rundeck.com/open-source](https://www.rundeck.com/open-source)
[
						](https://img-blog.csdnimg.cn/20190410164805965.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
[
	](https://img-blog.csdnimg.cn/20190410164805965.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
