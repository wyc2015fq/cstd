# jmeter分布式测试 - _天枢 - 博客园
## [jmeter分布式测试](https://www.cnblogs.com/yhleng/p/8743897.html)
2018-04-08 11:24 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8743897)
**何为jmeter分布式测试：**
**通熟易懂点，做压测过程中，我们一台终端机，对服务器发压，有可能服务器压力没上来，终端，先承受不住了。**
**那么，分布式就来了，我们可以，把终端任务，分配到多个终端机，对服务器进行发压。**
**Jmeter的分布式测试（优缺点）－转**
下面是分布式测试的使用方法，即使用一台机器控制多台机器给目标机器产生压力。由于Linux和Windows上的方法有些不一样，这里说下Linux下是怎么来配置的。
首先总控的机器我们叫master,产生压力的肉鸡我们叫JmeterServer，待我们测试的系统我们叫Target.
第一步：启动JmeterServer。我们可以找到Jmeter/bin 目录下的jmeter-server这个脚本，运行即可，当然如果你需要调整这个测试负载产生服务器的内存等性能参数的话，我们修改这个脚本jmeter中的内存参数。
第二步 : 修改Master机器上jmeter/bin目录下的jmeter.properties文件，修改remote_hosts=localhost:1099,localhost:2010这个为你自己的JmeterServer的实际IP，如果有多台可以用逗号分开。
第三步 ： 启动master jmeter，运行测试脚本的时候选：运行 --> 远程运行 --> 选择IP 或者 运行所有远程。JmeterServer就会运行Master机器上设定的运行脚本了。
问题 ： 我在实际使用中发现使用这种模式下的Jmeter在Master观测实际测试结果，发现TPS远远低于使用传统的一台Jmeter来测试所能达到的TPS，我感觉可能是由于Jmeter这种多机器协作中传递信息是使用RMI技术，可能这种技术本身还对Jmeter的性能产生了影响导致压力上去的很慢。该问题还没有得到完全认证，所以我现在如果在一台机器测试产生的压力不够的情况下我直接使用多台机器测把TPS加起来就是结果，比较傻，期待大侠指点迷津。
