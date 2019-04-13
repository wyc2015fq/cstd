
# SAP MM 如何查询所有未审批完的采购订单列表？ - 喜欢打酱油的老鸟 - CSDN博客


2018年07月23日 08:42:06[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：2123


SAP MM如何查询所有未审批完的采购订单列表？
TCode:ME2N,
![](https://img-blog.csdn.net/20180723084128128?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180723084128215?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180723084128352?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
双击release status字段， 这个字段就出现在右边，
![](https://img-blog.csdn.net/20180723084128185?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
鼠标放在白色输入框里，双击，
![](https://img-blog.csdn.net/20180723084128249?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
选择Not equal to,回车，
![](https://img-blog.csdn.net/20180723084128163?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
输入框里输入XX，（不等于XX，代表没有完成2级审批，假定项目上采购订单都是需要2个审批级别），执行，得到如下list，这些PO就是尚未完成审批的所有采购订单了，
![](https://img-blog.csdn.net/20180723084128324?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

Another method:T_Code ME80FN, Select Document Category as "F" , click on Dynamic Selections ( Shift+F4), select Purchasing Document Header, Double Click on Release indicator , select Blocked (or 0) and Execute. The layout of this report is more friendly than ME2N.
2018-03-13 写于无锡市新吴区

