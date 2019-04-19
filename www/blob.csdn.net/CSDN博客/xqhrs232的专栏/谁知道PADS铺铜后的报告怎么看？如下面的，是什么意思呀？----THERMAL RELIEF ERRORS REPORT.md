# 谁知道PADS铺铜后的报告怎么看？如下面的，是什么意思呀？----THERMAL RELIEF ERRORS REPORT - xqhrs232的专栏 - CSDN博客
2015年04月04日 17:55:33[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1625
原文地址::[http://zhidao.baidu.com/link?url=npVXQCE-LAwIrYG1p5qT8VT-sA0917jOXeYkho26f9TzNP0tXfoGh6dBBkLSlsPPM5Q7Q2a676_2B1OzorNo6_](http://zhidao.baidu.com/link?url=npVXQCE-LAwIrYG1p5qT8VT-sA0917jOXeYkho26f9TzNP0tXfoGh6dBBkLSlsPPM5Q7Q2a676_2B1OzorNo6_)
相关文章
1、在pads中对地铺铜时,有以下提示生成----[http://zuoye.baidu.com/question/8ab97d1baad79020f209c273a5fa5a1a.html](http://zuoye.baidu.com/question/8ab97d1baad79020f209c273a5fa5a1a.html)
THERMAL RELIEF ERRORS REPORT -- BIM8DITPUAT2().pcb -- Thu Dec 27 10:14:15 2012
       Drilled pads with                    Nondrilled pads with
less than 50% thermal extensions      less than 50% thermal extensions
Report of Thermal Spokes Generator.
On Top:
                                        D2.16   (988.63, 2505.86) # = 0
                                        D2.10   (1106.74, 2505.86) # = 1
                                        D2.25   (894.14, 2332.63) # = 0
                                        U5.13   (2194.87, 1950.96) # = 0
                                        U5.48   (2005.89, 2321.04) # = 1
                                        U5.14   (2210.61, 1950.96) # = 0
        (2212, 1852) # = 1
                                        D2.33   (1047.69, 2238.14) # = 0
Total Drilled pads:    1               Total Nondrilled pads:    7

就是有的焊盘（有孔和无孔焊盘两类）没有按设置的规则连接覆铜：单个焊盘有4边可接地，如少于2个（less than 50%），就会报错。 
实际上，如果这些个焊盘已走线连通地，则报警没有影响，可忽略之。
