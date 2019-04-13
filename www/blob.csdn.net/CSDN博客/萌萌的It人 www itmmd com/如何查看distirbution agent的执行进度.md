
# 如何查看distirbution agent的执行进度 - 萌萌的It人 www.itmmd.com - CSDN博客


2013年08月07日 18:35:12[Jlins](https://me.csdn.net/dyllove98)阅读数：1275


在transactional replication troubleshooting的过程中，经常会遇到下面的场景：
客户在发布端执行了一个几百万行的更新，结果导致性能下降。客户很想知道目前distribution agent的进度，完成的百分比，决定是等下去还是跳过这个过程。如果已经完成了90%，那么贸然停止就非常可惜了，并且rollback的操作也是要很长时间的。
下面介绍如何查看进度。
如果distribution agent已经启用了verbose log，可以通过verbose log来查看进度.
 Command id代表已经执行过的数量;transaction seqno表示正在进行的事务的xact_seqno。然后在distribution数据库执行selectcount(*)Fromdistribution..msrepl_commandswith(nolock)wherexact_seqno=@xact_seqno.
对比结果就可以知道进度了。
![](https://img-blog.csdn.net/20130807183451250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHlsbG92ZTk4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如果没有启用verbose log，就比较麻烦了，下面是具体的步骤。
找到相应的distribution agent名称和publisher_database_id
select*Fromdistribution..msdistribution_agents
![](https://img-blog.csdn.net/20130807183451656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHlsbG92ZTk4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
通过名称就可以找到distribution agent进行的process id.在distributor上执行下面的语句。
selecthostprocessfromsys.sysprocesseswhereprogram_name=@mergeAgentName
同一个distribution agent进程的process id是相同的，所以可以通过这个process id(对应trace里的client
 process id)，使用sql server trace得到distribution agent正在subscriber端执行的语句.
假设我们得到了下面这个语句exec [dbo].[sp_MSupd_dbota] default,511,4,0x02
根据这个存储过程，我们可以得到相应的aritlce_id。
在subscription database执行sp_helptext，得到表的名称
在distribution数据库查询得出article_id.selectarticle_idfrommsarticleswheredestination_object=@tablename
在subscriber上执行下面的语句,得到subscription数据库当前当xact_seqno.（请将第一步得到的distribution name带入@distribution_agent）
selecttransaction_timestamp,*FromMSreplication_subscriptionswheredistribution_agent=@distribution_agent
接下来就可找到distribution agent当前正在执行的xact_seqno了.将第一步得到的publisher_database_id,第5步得到的article_id和上一步得到的xact_seqno带入下面的查询
selectxact_seqno,count(*)asnumberFromdistribution..msrepl_commandswith(nolock)
wherepublisher_database_id=@publisher_database_idandarticle_id=@article_id
xact_seqno>@xact_seqnogroupbyxact_seqnoorderbyxact_seqno
顺序靠前，并且number较大的就是正在执行的事务了。您可能会问，为什么不是第六步得到的xact_seqno的下一个呢(select min(xact_seqno)From distribution..msrepl_commands with(nolock)where publisher_database_id=@publisher_database_id and xact_seqno>@xact_seqno).
因为distribution并不是每一个事务都单独提交的，而是根据CommitBatchSize和CommitBatchThreshold来提交的，这样可以提高性能。具体请参见http://www.kendalvandyke.com/2008/11/how-commitbatchsize-and.html
在distribution数据执行sp_browsereplcmds @xact_seqno,
 @xact_seqno
![](https://img-blog.csdn.net/20130807183452312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHlsbG92ZTk4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
用第四步得到的语句去查找，这样就可以知道当前执行到了什么位置


