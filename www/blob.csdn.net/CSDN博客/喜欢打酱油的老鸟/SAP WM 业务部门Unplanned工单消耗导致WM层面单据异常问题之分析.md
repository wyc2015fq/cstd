
# SAP WM 业务部门Unplanned工单消耗导致WM层面单据异常问题之分析 - 喜欢打酱油的老鸟 - CSDN博客


2018年09月18日 14:05:49[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：82


业务部门报如下工单的261消耗后WM层面的有异常，
![](https://img-blog.csdn.net/20180918140342262?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
其261发货过账后的物料凭证：4900023991/2018
![](https://img-blog.csdn.net/20180918140355713?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180918140407658?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
261过账后，LT06创建的WM 层面的TO单，
![](https://img-blog.csdn.net/20180918140420371?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
为什么这个TO单里是物料从100转入914?  而不是261之后，系统直接从100/PSA下扣线边仓的库存？
比如如下的一个正常的261货物移动，
![](https://img-blog.csdn.net/20180918140437256?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
系统直接从100/PSA下扣减库存。
为什么这2个发货到工单过账后，SAP系统会在WM层面有两种不同的行为？
经查，业务报异常的这个物料凭证4900023991/2018 （261）是一个未计划的发货(unplanned goods issue)，
![](https://img-blog.csdn.net/20180918140500590?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这是一个非正常的操作 !
而4900024537/2018这个261发货操作是Planned GI,
![](https://img-blog.csdn.net/20180918140515449?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Planned GI的方式，不会导致WM层面的行为与storage type 914有关的。
解决方案：业务人员做工单消耗的时候，不要做unplanned goods issue。
2018-09-18 写于杭州市.


