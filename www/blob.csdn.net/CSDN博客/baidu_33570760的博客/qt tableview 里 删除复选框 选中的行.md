# qt  tableview 里 删除复选框  选中的行 - baidu_33570760的博客 - CSDN博客
2016年05月27日 10:21:20[carman_风](https://me.csdn.net/baidu_33570760)阅读数：1801
     由于在tableview理通过代理  绘制了 复选框  （详细绘制 过程 参考qt群主的http://blog.csdn.net/liang19890820/article/details/50721200）  现在我想实现 删除在tableview 里复选框选中的行；如图 我想删除选中的2.4.5 行 在删除按钮的点击事件   关键代码如下
intnCount=ui->tableView->model()->rowCount();//model
 里总行数
QModelIndexindex;QVariantok;intj=0;for(inti=0;i<nCount;++i){//判断是否勾选上index=ui->tableView->model()->index(i-j,0,QModelIndex());
             //i-j 非常重要 因为在下面的代码中会删除行 而造成model里的总行数变小造成相对位置的变化, 0 表示复选框在的列数 根据需要自己改变;
             //通过data函数 取得该 位置的值 通过tobool 判断是否勾选
ok=ui->tableView->model()->data(index,Qt::UserRole);if(ok.toBool()){//如果勾选则删除改行boole=ui->tableView->model()->removeRow(i-j);++j;}}
