# PCB布线中，重要的信号线进行包地处理 - xqhrs232的专栏 - CSDN博客
2014年07月17日 17:44:47[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：6831
原文地址::[http://zhenshanmcu.blog.163.com/blog/static/16545766220137755225723/](http://zhenshanmcu.blog.163.com/blog/static/16545766220137755225723/)
相关网帖
1、在PADS2007中，地包线怎么画，主要针对时钟信号？----[http://zhidao.baidu.com/link?url=Fau1TwzcLlHxP07V_0GXXcIxDZ4OrrjwQPL01UYd8TTVyjYPwqlT8law-mCNBVMq-V78-2_oef-DCsiYsUKHqq](http://zhidao.baidu.com/link?url=Fau1TwzcLlHxP07V_0GXXcIxDZ4OrrjwQPL01UYd8TTVyjYPwqlT8law-mCNBVMq-V78-2_oef-DCsiYsUKHqq)
2、在PADS中怎样才能在弱小信号线周围画包地线啊？我刚学PADS，请说详细点，最好有详细步骤，谢了----[http://zhidao.baidu.com/link?url=w5wYhNkHpKCscl_DNgylhhWTZlQFL11nrBUCMVjtIZC4uIiBunjv4hd7eKkg-PsXob7FAz1omEeLADHm-R_ZBa](http://zhidao.baidu.com/link?url=w5wYhNkHpKCscl_DNgylhhWTZlQFL11nrBUCMVjtIZC4uIiBunjv4hd7eKkg-PsXob7FAz1omEeLADHm-R_ZBa)

  信号线两边的地包还是不包是个问题。在平时做设计的时候经常看到有人纠结于包地问题。可能受到板子大小的限制，又听说包地能让信号屏蔽更好，于是在重要的时钟线差分信号两边都尽量画上两条细细的地线。实际上这种做法反而增加了对附近信号的干扰。  
        包地主要的作用是为了减小串扰。那么除了包地以外还有什么方法能减小串扰呢？增加信号间距还有让信号和参考平面紧耦合。如果是多层板，减小参考平面和信号层的距离，可以更好的控制阻抗的同时能够让信号与参考平面紧耦合，减少信号对附近信号的干扰。在通过增加信号线间距就能很好的减小串扰，这时候对信号包地的作用就不明显了。尤其是空间比较小的情况下，加一根细细的地线，相当于在两根信号线之间又增加了一根信号线，起到了一个桥的作用，把信号的干扰又传导到下一根信号。去掉这跟地线减小串扰的效果应该会更好。
        有人说，不光要加地线包地还要在地线上多打地孔。当然，这样的效果会比较好。但是既然能打地孔说明包地线宽最小也要有十几个mil了，再加上线间距，原有两根信号线间距都足够满足4W了，这样串扰本身就很小了，去掉包地信号也不会增加多少串扰。
        如果是两层板，没有参考平面，那么重要信号的包地就很重要。包地线的宽度要尽量宽，最好在信号宽度的两倍以上。同时多打过孔，过孔间距小于信号线上信号波长1/5。
       在一些非高频的单片机布线中，晶振、串口、重要的信号线、中断信号等进行包地处理。
