# 面试|return 和finally那些事儿 - Spark高级玩法 - CSDN博客
2018年12月18日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：108
前些天有朋友在微信群里问了一个比较有意思的问题：
try/catch/finally语句块的finally和return谁先执行呢？也即是我们在try内部调用return，然后finally内部又去修改return返回的内容会怎么样呢？
废话少数先上代码:
publicclassReturnAndFinally {
privatestaticint i =10;
privatestaticintReturnIntValue(){
        i++;
try {
return i;
        }catch (Exception e){
            e.printStackTrace();
        }
finally {
            i++;
        }
return0;
    }
publicstaticvoidmain(String args[]){
        System.out.println(ReturnIntValue());
    }
}
大家在运行之前可以先猜一下返回值是多少？
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUf7YIMeGGut4ibhrliablySsNEFm9RZBhB351etjxPa0QyV9ibwEzq7fKibfqYTfE5E5x2rUxLTAKRfA/640)
这时候大家可能会认为return执行完之后，finally就没有执行了，要知道finally定义是无论什么情况都会最后执行finally语句。那么是不是由于int类型返回的时候直接返回值，那么finally内部再加的话也是无效的呢？
我们换成StringBuilder来验证一下，要知道这种我们的变量名实际上是对象地址，对象实体存储于堆中。
那么我们换成StringBuilder来看一下结果：
package bigdata.spark.app;
publicclassReturnAndFinally {
privatestatic StringBuilder str = new StringBuilder("hello ");
privatestatic StringBuilder ReturnStringValue(){
try {
return str.append("word");
        }catch (Exception e){
            e.printStackTrace();
        }
finally {
            str.append("!!!!!");
        }
return str.append("+++++++++");
    }
publicstaticvoidmain(String args[]){
        System.out.println(ReturnStringValue());
    }
}
执行结果是：
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUf7YIMeGGut4ibhrliablySsrTwE3hYnZDeaz0XMiaLC4aoo8Q07pfoI7MmftLp60bkYrsYxJCjmyFg/640)
这个结果证明是即使是调用了return，finally也会执行。
微信交流，可以加浪尖微信。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWg6f2VhvNHVlBNaqibGBo9dCZRanTib2HX4WseDSibzQquwkYFWPBpF89ZClkdeZeuWMAiamAwP28wCQ/640)
RDD扩展第三弹已经更新至星球。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFUibDu2gaX89M600Ip3MDrVY0ibqsWg1gHuTdGCX1QfbyyCPlf31gLQna9OvNyRsBaByXsZrZeqibtVQ/640)
