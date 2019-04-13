
# java千万级别数据生成文件思路和优化 - 萌萌的It人 www.itmmd.com - CSDN博客


置顶2012年04月19日 16:40:46[Jlins](https://me.csdn.net/dyllove98)阅读数：5709标签：[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)[xml																](https://so.csdn.net/so/search/s.do?q=xml&t=blog)[数据库																](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)[框架																](https://so.csdn.net/so/search/s.do?q=框架&t=blog)[object																](https://so.csdn.net/so/search/s.do?q=object&t=blog)[string																](https://so.csdn.net/so/search/s.do?q=string&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=object&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=框架&t=blog)个人分类：[千万级数据处理																](https://blog.csdn.net/dyllove98/article/category/1128357)
[
																								](https://so.csdn.net/so/search/s.do?q=框架&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=xml&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=java&t=blog)
**iteye/csdn 个人原创，转载请标明出处**

一年前写过一个百万级别数据库数据生成配置xml文件的程序,程序目的是用来把数据库里面的数据生成xml文件.程序可以配置多少文件生成到一个文件中去.
程序刚开始设计的时候说的是最多百万级别数据，最多50W数据生成到一个xml文件里面去,所以在做测试的时候自己也只是造了100W的数据并没有做过多数据量的测试,然后问题就来了....由于程序使用的局点数据量巨大，需要生成xml文件的客户资料接近千万级别的程度,而现场对程序的配置大约是100W条数据生成一个xml文件里面去，程序在这样的大数据量下面偶尔会有崩溃.

最近几天现场催的比较紧，最近抽空把这个问题处理了一下,在解决问题的过程中我把解决的步骤和方法记录了下来，正好和大家共享一下
**现场提的问题概况：**
数据量：生成xml，每个文件100W+ 条的数据
内存控制：最好不要超过512M
问题详情：在处理70W左右的时候内存溢出

**一、先来看一下程序要生成的xml文件的结构**
Xml代码![收藏代码](http://jlins.iteye.com/images/icon_star.png)
<File>
<FileType>1</FileType>
<RType>12</RType>
<Version>03</Version>
<BNo>004</BNo>
<FileQ>5</FileQ>
<FNo>0006</FNo>
<RecordNum>1000000</RecordNum>
<!-- 上面是文件头  下面是百万个<RecordList>  -->
<RecordList>
<Msisdn>10350719507</Msisdn>
<State>1</State>
<StartDate>20110303</StartDate>
<Date>20110419</Date>
<Balance>45000</Balance>
</RecordList>
...<!-- 可能百万个  <RecordList> 块-->
</File>

**二、给大家说一下如何把大数据生成xml文件**

**1、小数据量的情况下    <  1W条数据**
比较好用的方法是使用开源框架，比如XStream 直接把javabean 生成 xml
优点：api操作简单，方便维护
缺点：数据量大的情况下太消耗内存
**2、大数据量生成一个xml文件(本程序采用的方法)**
自己做的一个可以使用极少的内存生成无限制大的xml文件框架由3部分生成xml文件
第一部分：生成文件头
例如： xxx.toXML(Object obj, String fileName)
第二部分：通过每次向文件里面追加3000（可配置）条数据的形式生成文件块
例如：xxx.appendXML(Object object);  //object 可以是ArrayList 或者一个单独的javaBean
第三部分：生成xml文件尾巴
例如：xxx.finishXML();
程序中的调用：调用xxx.toXML(Object obj, String fileName) 生成文件头之后，可以循环从数据库中读取数据生成ArrayList，通过xxx.appendXML(Object object) 方法追加到xml文件里面,xxx.finishXML() 对文件进行收尾

对框架说明：我上面提供的例子有文件头 + 文件块 + 文件尾巴. 如果和你们的实际使用文件不太一致的话，可以参考上面提供的思路修改一下即可，主要的方法是把相同的文件块部分分离出来通过追加的形式写入xml文件.
**有了思路之后，大家可以尝试着自己写一个类似的大数据处理框架(千万级别以上),如何有什么需要帮助的可以直接联系我，因为是公司的程序，不太敢放出来，怕......**
**三、我是如何测试性能和优化的**
**1、手动排除**
根据文件崩溃时候的日志发现是在生成xml的框架里面报的错误，第一想到的是框架有些资源没有释放.于是把自己做的文件生成框架整体的排查了一遍,并且自己写个简单程序生成200万条数据，使用xml框架生成一个xml文件,整个生成过程中任务管理器(xp)查看程序对应的java进程使用的内存基本在20M左右,因此排除框架的问题.怀疑是数据库查询和调用框架的部门出现问题.
检测了一遍主程序的关键部分代码,优化了一下字符串处理.手动的释放一些对象的内存(例如：调用ArrayList.clear()，或者把对象置空等)，分配512内存后运行程序,60万数据的时候内存溢出，因为能主动释放的对象都已经释放掉了,还是没有解决,果断放弃看代码,准备使用JProfile进行内存检测.
**2、手动排除没有解决，借助内存分析工具JProfile进行排除**
通过在数据库中生成300W条数据，在JProfile上面多跑程序，一边运行，一边调用JProfile 提供的执行GC按钮主动运行垃圾回收，运行50W数据后，通过检测中发现 java.long.String[] 和 oracle.jdbc.driver.Binder[] 两个对象的数目一直保持在自增状态，而且数目基本上差不多，对象数目 都在200W以上，由于java.long.String[]对象是需要依赖对象而存在的，因此断定问题就出在oracle.jdbc.driver.Binder[]上面，由于改对象存在引用导致String[]不能正常回收.
**3、通过在JProfile对象查看对象的管理**
检测到oracle.jdbc.driver.Binder 被 oracle.jdbc.driver.T4CPreparedStatement 引起,而T4CPreparedStatement正好是Oracle对jdbc OraclePreparedStatement的具体实现,因此断定是在数据库处理方面出现的问题导致oracle.jdbc.driver.Binder对象不能正常释放，通过再一次有目的的检测代码，排查jdbc数据查询的问题,把问题的矛头直至数据库的批处理和事务处理.因此程序是每生成一个文件成功后，会把已经处理的数据转移到对应的历史表中进行备份,而再个表操作的过程中使用了批处理和事务，使用批处理主要是保证执行速度，使用事务主要是保证同时成功和失败。
**4、又因此程序每次从数据库中查询3000条数据处理，**所以准备监控oracle.jdbc.driver.Binder的对象数目是否和查询次数对应.,通过在程序中Sysout输出查询次数 + JProfile运行GC测试 Binder，数据匹配，证实是java在数据库批处理的过程中有些问题.
**5、专门把批处理代码提取出来通过JProfile内存分析.最终问题定位完毕.**
原因如下：100W数据生成一个文件的过程中，等文件生成完毕之后才能把数据库中的数据备份到历史表中，这个时候才能进行事务的提交，也就是执行commit()， 并且删除原表数据，100W数据按照3000一批写入文件，每批次只是通过 PreparedStatement.addBatch();加入到批次里面去,并没有执行PreparedStatement.executeBatch(),而是在commit()之前统一调用的PreparedStatement.executeBatch()，这样的话PreparedStatement就会缓存100W条数据信息，造成了内存溢出.
错误的方法如下：
Java代码![收藏代码](http://jlins.iteye.com/images/icon_star.png)
try{
conn.setAutoCommit(false);
pst = conn.prepareStatement(insertSql);
pstDel = conn.prepareStatement(delSql);
pstUpdate = conn.prepareStatement(sql);
...
//totalSize = 100W数据 / 3000一批次
for(inti =1; i <= totalSize; i++) {
client.appendXML(list);
}
// 错误的使用方法
client.finishXML();
pst.executeBatch();
pstDel.executeBatch();
}
...
finally{
try{
if(isError) {
conn.rollback();
}
else
conn.commit();
...
}
...
}
正确的方法如下
try{
Java代码![收藏代码](http://jlins.iteye.com/images/icon_star.png)
conn.setAutoCommit(false);
pst = conn.prepareStatement(insertSql);
pstDel = conn.prepareStatement(delSql);
pstUpdate = conn.prepareStatement(sql);
...
//totalSize = 100W数据 / 3000一批次
for(inti =1; i <= totalSize; i++) {
list = 从数据库中查询3000条数据
client.appendXML(list);
pst.executeBatch();
pstDel.executeBatch();
}
client.finishXML();

...
inally {
try{
if(isError) {
conn.rollback();
}
else
conn.commit();
...
}
...

如果碰到和我一样的需要给大家一个提醒.
oracle在每次执行executeBatch();进行批处理的时候，当前connection对应的rownum会根据操作的结果发生变化.

在执行pst.executeBatch(); 之后，当前连接的 rownum 数就会发生变化. 因此凡是通过rownum查询数据的程序都要小心这一点


下一篇将整理写java大数据(千万级别以上的)处理，包括 ftp大数据处理、文件生成大数据处理、数据库转移大数据处理、文件读取大数据处理等等.

<千万级别数据生成xml文件> by dyllove98 @[http://jlins.iteye.com](http://jlins.iteye.com/)
转载请表明出处。
<script type="text/javascript" src="http://pagead2.googlesyndication.com/pagead/show_ads.js"></script>

