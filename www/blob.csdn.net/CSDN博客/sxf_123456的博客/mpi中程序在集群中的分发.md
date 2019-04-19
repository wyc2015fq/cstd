# mpi中程序在集群中的分发 - sxf_123456的博客 - CSDN博客
2017年04月24日 21:27:22[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：793
我们在开发mpi程序时，由于其是分布式程序，我们在单个节点上完成编码后，需要将代码拷贝到整个集群进行[测试](http://lib.csdn.net/base/softwaretest)。集群之间的文件拷贝可以通过scp命令完成。但是scp命令是针对两个节点之间文件互传设计，为了将代码拷贝到所有的机器，我们需要多次调用scp命令。一个好的方法是将拷贝命令写成脚本，让其自动完成。
       首次拷贝时，我们可以让scp拷贝整个工作目录到集群中，当项目较小时这种方法没有影响，但是当项目规模变大，拷贝将变得很费时。其实我们每次只对某个文件进行修改，然后重新编译进行测试。所以这时就没有必要将整个项目进行拷贝，只需要拷贝最新修改的几个文件即可。一种方法是将要拷贝的文件以参数形式传递给脚本，然后启动拷贝任务。
       但是修改一个源程序文件，往往涉及多个文件的修改，例如修改一个cpp文件，重新编译之后对应的.o文件和可执行文件都被修改。如果源文件之间有依赖关系，修改的文件将会更多。因而通过参数形式声明要拷贝的文件还是有点麻烦。在实际情况中，我们往往按照这样的流程开发程序：修改一个文件，编译，然后立即将其分发到集群中进行测试。根据这个习惯，我们可以通过检查文件的修改时间来确定系统中哪些文件刚被修改，然后只拷贝这些与当前时间一样或者很接近的文件即可。下面的代码给出了一个简单的例子，程序在node1上编写，然后拷贝到集群中的node2-4中。判断文件被修改的方法就是检查文件的修改时间和当前时间是否一样，一样说明该文件刚被修改过。
**[plain]**[view
 plain](http://blog.csdn.net/yutianzuijin/article/details/9360281#)[copy](http://blog.csdn.net/yutianzuijin/article/details/9360281#)
[print](http://blog.csdn.net/yutianzuijin/article/details/9360281#)[?](http://blog.csdn.net/yutianzuijin/article/details/9360281#)
- #!/bin/sh  
- 
- echo "copy to Node2, Node3, Node4"  
- 
- current_time=$(date | cut -d' ' -f4|cut -d':' -f1,2)  
- for file in $(ls ~/ygch/mpi/qbh/); do  
-     m_time=$(ls -l $file |cut -d' ' -f8)  
- 
-     if [ $m_time = $current_time ]; then  
-         scp ~/ygch/mpi/qbh/$file root@Node2:~/ygch/mpi/qbh/  
-         scp ~/ygch/mpi/qbh/$file root@Node3:~/ygch/mpi/qbh/  
-         scp ~/ygch/mpi/qbh/$file root@Node4:~/ygch/mpi/qbh/  
-     fi  
- done  
获取文件访问时间可以通过ls –l命令获得，运行该命令的结果是：
![](https://img-blog.csdn.net/20130717210241796)
红色方框里面的即是文件的修改时间，然后我们通过cut命令提取该列即可获得文件的修改时间。当前时间可以利用date命令获得，运行结果是：
![](https://img-blog.csdn.net/20130717210255593)
然后我们也通过cut命令获取当前时间，与ls –l命令稍微有点不同，我们需要两次利用cut命令获取和ls –l一样的时间。最后查找目录中两个时间一致的文件拷贝到集群中所有其他节点中。
       在上例中文件被修改的标志是文件修改时间和当前时间一致，这个是最简单的方法，我们可以依照实际情况调整判别依据，但是上述方法足以应付绝大多数情况。
