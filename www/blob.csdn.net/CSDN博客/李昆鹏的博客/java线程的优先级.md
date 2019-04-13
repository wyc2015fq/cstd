
# java线程的优先级 - 李昆鹏的博客 - CSDN博客


2018年04月20日 16:25:14[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：36标签：[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)个人分类：[JAVASE																](https://blog.csdn.net/weixin_41547486/article/category/7484968)


----------------------------------------java线程的优先级----------------------------------------------
我们可以通过public final void setPriority（int  newPriority）方法来设置线程的优先级，但是优先级并不是绝对的，只是相对来说比其他线程得到CPU的资源机会多些。
示例代码
**public****class**PrioThread**extends**Thread{
@Override
**public****void**run() {
**for**(**int**i= 0;i<50;i++) {
System.**out**.println(**this**.getName() +"正在运行"+i);
}
}
}
---------------------------------------------
**public****class**PrioTest {
**public****static****void**main(String[]args) {
PrioThreadpt=**new**PrioThread();
PrioThreadpt1=**new**PrioThread();
PrioThreadpt2=**new**PrioThread();
//查看优先级，最大优先级为10，最小为1，正常为5
System.**out**.println(pt.getPriority());
System.**out**.println(pt1.getPriority());
System.**out**.println(pt2.getPriority());
//输出5
//给线程设置优先级
pt.setPriority(10);
//      pt.start();
//      pt1.start();
//      pt2.start();
}
}


