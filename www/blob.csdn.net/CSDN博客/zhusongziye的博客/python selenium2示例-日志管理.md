# python selenium2示例 - 日志管理 - zhusongziye的博客 - CSDN博客





2017年12月03日 20:30:16[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：64
个人分类：[Python语言](https://blog.csdn.net/zhusongziye/article/category/7131144)









## 前言


       在自动化测试实践过程中，必不可少的就是进行日志管理，方便调试和生产问题追踪，python提供了logging模块来进行日志的管理。下面我们就logging模块的学习和使用进行一个层层推进演示学习。


      Python的logging模块提供了通用的日志系统，可以方便第三方模块或应用使用。这个模块提供了不同的日志级别，并可以采用不同的方式进行日志记录，比如文件，HTTP GET/POST， SMTP， socket等等，甚至可以自定实现具体的日志记录方式。


    logging模块与java的log4j的机制是一样的，只是具体的语言实现细节有些不同。python logging模块提供了logger、handler、filter、formatter等基础类。

> 

1、logger： 提供日志接口，供应用程序调用。logger最常用的操作有两大类：配置和发送日志消息。



2、handler：将日志记录发送到合适的目的，比如文件、socket等等。一个logger对象可以通过addhandler方法添加0到N个handler，每个hangdler又可以定义不同的日志级别，以实现日志分级过滤。



3、filter：提供了一种优雅的方式决定一个日志记录是否发送到handler。



4、formatter：指定日志记录的输出格式。formatter的构造方法需要两个参数：消息的格式字符串和日期字符串，这两个参数是可选的。




默认情况下，logging将日志输出至console，日志级别为WARNING。



logging中按日志级别大小关系为CRITICAL > ERROR > WARNING > INFO >DEBUG > NOTSET，当然也可以自定义日志级别。




## 简单日志


下面我们看一下一个简单的日志示例，将日志记录输出到console：

> 

#-*- coding:utf-8 -*-



import logging


if __name__ == '__main__':


    logging.debug(u'这是bug级别日志记录')



    logging.info(u'这是提示信息级别日志记录')



    logging.warning(u'这是警告级别日志记录')



在console中将输出一下信息：



WARNING:root:这是警告级别日志记录





为什么只输出了一条呢？因为logging默认情况下的日志输出级别是：WANRING




## 日志格式和级别控制


接下来我们看看如何控制日志的输出格式和日志级别。代码示例如下：

> 

#-*- coding:utf-8 -*-



import logging


if __name__ == '__main__':


    logging.basicConfig(level=logging.DEBUG,  # 日志级别设置

        format="%(asctime)s  %(filename)s [line: %(lineno)d] %(levelname)s %(message)s",

        datefmt='%a, %d %b %Y %H:%M:%S',

        filename='mylog.log',

        filemode='w'

    )

    logging.debug(u'这是debug级别日志记录')

    logging.info(u'这是信息级别日志记录')

    logging.warning(u'这是警告级别日志记录')




在当前目录下mylog.log文件中的内容为：

Mon, 20 Mar 2017 16:21:28 log.py [line: 14] DEBUG 这是debug级别日志记录

Mon, 20 Mar 2017 16:21:28 log.py [line: 15] INFO 这是信息级别日志记录

Mon, 20 Mar 2017 16:21:28 log.py [line: 16] WARNING 这是警告级别日志记录





logging.basicConfig函数各参数说明


filename： 指定日志输出文件名


filemode：和file函数的意义相同，指定日志文件的打开模式，‘w或a’


format：指定日志输出格式和内容，format可以输出很多有用的信息，如上例所示：


    %(levelno)s: 打印日志级别的数值



    %(levelname)s: 打印日志级别名称


    %(pathname)s: 打印当前执行程序的路径，其实就是sys.argv[0]


    %(filename)s: 打印当前执行程序名


    %(funcName)s: 打印日志的当前函数


    %(lineno)d: 打印日志的当前行号


    %(asctime)s: 打印日志的时间


    %(thread)d: 打印线程ID


    %(threadName)s: 打印线程名称


    %(process)d: 打印进程ID


    %(message)s: 打印日志信息


datefmt：指定时间格式，同time.strtime()


level：指定日志级别，默认为logging.WARNING


stream：指定日志的输出流，可以指定输出到sys.stderr, sys.stdout或文件，默认输出到sys.stderr，当stream和filename同时指定时，stream被忽略。




## 日志输入定向


下面我们来看看如何把日志同时输出到console和文件中，代码示例如下：

> 

#-*- coding:utf-8 -*-



import logging


if __name__ == '__main__':


    logging.basicConfig(level=logging.DEBUG,  # 日志级别设置


        format="%(asctime)s  %(filename)s [line: %(lineno)d] %(levelname)s %(message)s",


        datefmt='%a, %d %b %Y %H:%M:%S',


        filename='mylog.log',


        filemode='w')


    #####################################################

    # 定义一个StreamHandler，将info级别的或更高级别的日志输出到标错错误

    # 并将其添加到当前的日志处理对象

    console = logging.StreamHandler()

    console.setLevel(logging.INFO)

    formatter = logging.Formatter('%(name)-12s: %(levelname)-8s %(message)s')

    console.setFormatter(formatter)

    logging.getLogger('').addHandler(console)

    ####################################################

    logging.debug(u"这是debug日志记录")

    logging.info(u'这是info日志记录')

    logging.warning(u'这是warning日志记录')






在console中输出以下日志记录：

root        : INFO     这是info日志记录

root        : WARNING  这是warning日志记录


在当前目录下mylog.log文件中内容为：

Mon, 20 Mar 2017 17:32:43  log.py [line: 26] DEBUG 这是debug日志记录

Mon, 20 Mar 2017 17:32:43  log.py [line: 27] INFO 这是info日志记录

Mon, 20 Mar 2017 17:32:43  log.py [line: 28] WARNING 这是warning日志记录



在本示例中实现了根据不同需要，将不同级别的日志重定向输出至不同的目标。




## 日志配置


在上述所有的示例中，日志的配置都是在代码中实现，但在实际的应用过程中，我们一般都需要动态的配置日志信息，或是满足自定义的需要，下面我们就自定义日志配置进行示例演示：


# 定义一个配置文件，这里命名为logger.conf，为标准的INI格式的文件，内容如下


> 

###############################################



###### 下面定义了三个logger： root,demo01,demo01



[loggers]


keys=root,demo01,demo01


[logger_root]


level=DEBUG


handlers=hand01,hand02


[logger_demo01]


handlers=hand01,hand02


qualname=demo01


propagate=0


[logger_demo02]


handlers=hand01,hand03


qualname=demo02


propagate=0


###############################################


#### 下面定义了三个handler： hand01,hand02,hand03



[handlers]


keys=hand01,hand02,hand03


[handler_hand01]


class=StreamHandler


level=INFO


formatter=form02


args=(sys.stderr,)


[handler_hand02]


class=FileHandler


level=DEBUG


formatter=form01


args=('mylog.log', 'a')


[handler_hand03]


class=handlers.RotatingFileHandler


level=INFO


formatter=form02


args=('mylog.log', 'a', 10*1024*1024, 5)


###############################################


### 下面定义了两种formatter： form01,form02



[formatters]


keys=form01,form02


[formatter_form01]


format=%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s %(message)s


datefmt=%a, %d %b %Y %H:%M:%S


[formatter_form02]


format=%(name)-12s: %(levelname)-8s %(message)s


datefmt=





使用demo01 logger代码示例：

> 

#-*- coding:utf-8 -*-



import logging


import logging.config


if __name__ == '__main__':


    logging.config.fileConfig("logger.conf")


    logger = logging.getLogger("demo01")


    logger.debug(u'这是demo01 debug日志记录')


    logger.info('u'这是demo01 info日志记录')


   logger.warning(u'这是demo01 warning日志记录')





下面是使用demo02 logger代码示例：

> 

#-*- coding:utf-8 -*-



import logging


import logging.config


if __name__ == '__main__':


    logging.config.fileConfig("logger.conf")


    logger = logging.getLogger("demo02")


    logger.debug(u'这是demo02 debug日志记录')


    logger.info('u'这是demo02 info日志记录')


    logger.warning(u'这是demo02 warning日志记录')


## 结束语


本文从日志的基本应用到更高级的应用方式层层推进进行演示，当然了在实际的自动化测试实践中，还需要对logging模块进行更高级的封装以提高其复用性，达成高可用的目的。对于测试人员而言更需要加强编程基本功，提升测试技术能力，更加灵活的应用各种基础技术。



