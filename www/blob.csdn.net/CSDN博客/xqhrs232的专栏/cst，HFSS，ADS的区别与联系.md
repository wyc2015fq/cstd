# cst，HFSS，ADS的区别与联系 - xqhrs232的专栏 - CSDN博客
2015年09月29日 22:52:20[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1283
原文地址::[http://blog.sina.com.cn/s/blog_791753310100s54x.html](http://blog.sina.com.cn/s/blog_791753310100s54x.html)
相关文章
1、HFSS、ADS、CST各自优缺点及应用范围----[http://wenku.baidu.com/link?url=BkdAK3TSkitPpz2pUSIFo-MFlkjuecYLWCoVXkpXU2r72MAQ862sv7PsmgctZdoagIPDtsylTyxGKT0lprC6IgiobYy0swTnJV5cf2NIT13](http://wenku.baidu.com/link?url=BkdAK3TSkitPpz2pUSIFo-MFlkjuecYLWCoVXkpXU2r72MAQ862sv7PsmgctZdoagIPDtsylTyxGKT0lprC6IgiobYy0swTnJV5cf2NIT13)
2、ADS和HFSS联合仿真----[http://download.csdn.net/detail/u010915735/5797723](http://download.csdn.net/detail/u010915735/5797723)
3、**ads仿真与hfss，cst仿真出来的结果完全不同，求帮助！----[http://www.mwtee.com/forum.php?mod=viewthread&tid=676995](http://www.mwtee.com/forum.php?mod=viewthread&tid=676995)**
4、ADS和CST联合仿真差别怎么这么大?效果太差了----[http://zhidao.baidu.com/link?url=OJhpo_6Dc-yNa9hTr9QwW2wyG1pcErf65xpmMXuyTOSGwWIco3mfK9eeA0fLyMub7-SVW39dSnP956yjfa-oS_](http://zhidao.baidu.com/link?url=OJhpo_6Dc-yNa9hTr9QwW2wyG1pcErf65xpmMXuyTOSGwWIco3mfK9eeA0fLyMub7-SVW39dSnP956yjfa-oS_)
CST是基于FDTD（时域有限积分法）电磁 场求解算法的仿真器，适合仿真宽带频谱结果，因为只需要输入一个时域脉冲就可以覆盖宽频带。
HFSS是基于FEM（有限元法）电磁场求解算法的仿真器，适合仿真三维复杂结果，但是电长度较小。
ADS内含momentum (基于第三种经典算法－矩量法），是一种对第三维度进行简化的电磁场仿真器，非常适合仿真第三维度上均匀变化的结构，例如 电路多
 层板，如PCB，陶瓷等电路板，常见无源电路，如滤波器等结构。仿真速度极快，同时保证和HFSS相同的精度。因此作为板级和IC级电路设计师，ADS
 momentum是最好的仿真工具，其效率远炒股HFSS和CST。但是如果要仿真天线，键合线等第三维度上非均匀延展的结构，就需要全波三维求解器。
今年安捷伦推出了其基于有限元算法的电磁场仿真器－－－EMDS，并且和嵌入到ADS中，我使用后感觉非常好，完全解决了业界最好的路仿真软件ADS与全波三维电磁场仿真器之间的连接。Ansoft 的HFSS已经不再是有限元的唯一选择了。。。
安捷伦同时也推出了基于有限时域差分法的工具 AMDS，可以做天线仿真，因为听说是收购来的，所以技术也非常成熟，同时听说也能跟ADS连接！！！我拿了一个试用版用了一下，收购的东西果然是成熟啊。。。。。。CST也不再是唯一选择。
总之，对于三维电磁场来说，没有最好的工具，只有用户根据实际需要做的东西去选择一个最合适自己的工具，那才是最好的工具－－－对你来说。
简单归纳如下：
1。混合集成电路设计，PCB板级设计，无源板级器件设计，RFIC/MMIC设计－－－ ADS＋ momentu
2. 天线设计。  首推 Agilent   AMDS ，CAD导入，建模很方便。。。CST备选
3。微波腔体，衰减器，微波转接头，波导滤波 器等设计 －－－－ Agilent EMDS or HFSS ，有限元法的最佳发挥场所！
永远没有最佳的三维电磁场工具，只有针对你自 己的产品的最佳工具！ 深刻了解工具的原理，深刻了解自己所开发产品的特性，是做好工作的第一步！
工具就象螺丝刀，十字螺丝刀跟一字螺丝刀本身没法比较，但是当你家的电器上用的是一字螺丝的时候，哪怕十字螺丝刀是用钻石做的都没用。
备注::
1>[ADS](https://www.baidu.com/s?wd=ADS&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1d9mvNBmW-9PjbsrHT3nj630ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6K1TL0qnfK1TL0z5HD0IgF_5y9YIZ0lQzqlpA-bmyt8mh7GuZR8mvqVQL7dugPYpyq8Q1Rsnjb1rHcLrf)主要用来仿真电路，[HFSS](https://www.baidu.com/s?wd=HFSS&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1d9mvNBmW-9PjbsrHT3nj630ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6K1TL0qnfK1TL0z5HD0IgF_5y9YIZ0lQzqlpA-bmyt8mh7GuZR8mvqVQL7dugPYpyq8Q1Rsnjb1rHcLrf)主要用来仿真器件，比如滤波器、天线等等~
