# Object::connect: No such signal - fanyun的博客 - CSDN博客
2018年05月06日 10:31:05[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：251标签：[Qt](https://so.csdn.net/so/search/s.do?q=Qt&t=blog)
个人分类：[Qt](https://blog.csdn.net/fanyun_01/article/category/6429937)
RecUdpThread  *receive = new RecUdpThread  ();
    connect(receive , SIGNAL(sendData(char* Data,int DataLen), this, SLOT(RevData(char * Data,int DataLen)));
去掉参数名， 保留类型即可:
RecUdpThread  *receive = new RecUdpThread  ();
    connect(receive , SIGNAL(sendData(char* ,int )), this, SLOT(RevData(char *,int)));
1、看看你的类声明中有没有Q_OBJECT
2、你声明的函数要加声明:
     private slots:
        void xxxx();
3、检查槽函数名拼写是否有误，以及对应的signal和slot的参数是否一致,如果还不行的话,清理项目,删掉原有的moc_xxx.cpp,重新执行qmake.
还有一点要注意的是:signal和slots的参数要一样;在自定义信号 signal 和 槽函数 slot 是一定要严格对应好写法,举个例子:
        在某个QObject对象的头文件里面是这么定义signal和slot的
signals:
   void on_sendSignal(QString)
slots:
   void rec_getSignal(QString)
       此时,我们的connect函数这么写:connect(this,SIGNAL(on_sendSignal(QString)),this,SLOT(rec_getSignal(QString)));那么这个时候就会链接成功.
       但是如果我们的signal和slot是这么定义的:
signals:
   void on_sendSignal(QString str);
slots:
   void rec_getSignal(QString str);
       我们之前写的那句connect函数就没有作用了,运行的时候会告诉你slot和signal都不存在.
