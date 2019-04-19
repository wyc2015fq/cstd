# Qt动态删除 - fanyun的博客 - CSDN博客
2017年11月26日 21:51:53[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：183
     //m_gLayout->removeWidget(widget);
    //delete widget;
    //ui->widget->children(i);   
int num = ui->m_gLayout->count();//获取gridLayout中item的数目
    //(!num) return;
    for(int i; i<num; i++){
    QWidget *p = ui->m_gLayout->itemAt(num-1)->widget();//获取最后一个控件的指针
    ui->m_gLayout->removeWidget(p);//移除控件
    delete p;//释放资源
    }
  int num = m_gLayout->count();//获取gridLayout中item的数目
    //(!num) return;
    for(int i; i<num; i++){
    QWidget *p = m_gLayout->itemAt(num-1)->widget();//获取最后一个控件的指针
    ui->m_gLayout->removeWidget(p);//移除控件
    delete p;//释放资源
    }
