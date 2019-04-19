# qt treewidget 的模糊查询 - baidu_33570760的博客 - CSDN博客
2016年05月23日 16:31:23[carman_风](https://me.csdn.net/baidu_33570760)阅读数：2230
###  主要思路是 逐步遍历所有的根节点 以及节点下的子节点，然后利用QString类型的contains函数 进行匹配  若包含则 显示该节点与其父节点 否则隐藏
主要函数如下：
voidOrganizationManagement::on_btn_search_clicked()
{QStringsearchword=ui->lineEdit->text();//查找的内容for(intm=0;m<ui->treeWidget->topLevelItemCount();++m)//遍历所有父节点{QTreeWidgetItem*CurTopLevItm=ui->treeWidget->topLevelItem(m);//显示与关键字匹配的馈线QStringTopNodText=CurTopLevItm->text(0);boolIsContain=TopNodText.contains(searchword,Qt::CaseInsensitive);//节点名字是否包含关键字if(IsContain){if(CurTopLevItm->isHidden())CurTopLevItm->setHidden(false);//显示此节点}else{CurTopLevItm->setHidden(true);}//遍历该父节点下的子节点for(intn=0;n<CurTopLevItm->childCount();++n){QTreeWidgetItem*CurChidItm=CurTopLevItm->child(n);QStringChilNodText=CurChidItm->text(0);boolIsContain=ChilNodText.contains(searchword,Qt::CaseInsensitive);//节点名字是否包含关键字if(!IsContain)//与关键字不匹配{CurChidItm->setHidden(true);continue;}CurChidItm->setHidden(false);//与关键字匹配if(CurTopLevItm->isHidden())//显示其父节点CurTopLevItm->setHidden(false);if(!CurTopLevItm->isExpanded())CurTopLevItm->setExpanded(true);//展开父节点}}}
