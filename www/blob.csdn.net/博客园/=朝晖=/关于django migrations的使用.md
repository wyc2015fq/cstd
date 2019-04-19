# 关于django migrations的使用 - =朝晖= - 博客园
# [关于django migrations的使用](https://www.cnblogs.com/dhcn/p/7124693.html)
     django 1.8之后推出的migrations机制使django的数据模式管理更方便容易，现在简单谈谈他的机制和一些问题的解决方法：
1、谈谈机制:migrations机制有两个指令，第一个是makemigrations，第二个是migrate，生成migrations代码的makemigrations指令是用models里面的model和当前的migrations代码里面的model做对比，如果有新的修改，就生成新的migrations代码，migrate指令是用migrations目录中代码文件和django[数据库](http://lib.csdn.net/base/mysql)djaong_migrations表中的代码文件做对比，如果表中没有，那就对这些没有的文件按顺序及依赖关系做migrate apply，然后再把代码文件名加进migrations表中。
一些问题的解决方法：
1、旧的项目如果升级到migrations机制：项目当前状态，应该是数据库和model定义一致，然后makemigtations，把生成的migrations initial文件全部加进django_migtrations表中，剩下的工作就和正常使用一样了。
2、managed=Fasle表的[测试](http://lib.csdn.net/base/softwaretest)处理，空库测试，这部分表也需要做进测试数据库，这部分表，可以先在False状态下makemigrations，然后migrate一次，这样那些migrations代码已经属于已执行状态，剩下的就是把model和migrations代码中的manged全部切为true，这样这些代码就可以在空库测试中可用，同时不会再向数据库同步，当然这些表加字段就需要你自己先makemigrations一次，然后手动merge新旧migratons脚本，当然最后最后这部分新的migrations代码需要删除掉。
3、关于migrations Schema学习的问题，migrations代码实际叫Schema，这个代码也是[Python](http://lib.csdn.net/base/python)代码，非常易读，稍微学一下，以后可以帮助你解决不少类似问题2这样的问题。

