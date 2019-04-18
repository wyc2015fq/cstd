# RxJava使用（二）filter 操作符 - z69183787的专栏 - CSDN博客
2018年08月09日 14:34:43[OkidoGreen](https://me.csdn.net/z69183787)阅读数：58
个人分类：[Java-RxJava异步框架](https://blog.csdn.net/z69183787/article/category/7921321)
[https://blog.csdn.net/tangxl2008008/article/details/51334295](https://blog.csdn.net/tangxl2008008/article/details/51334295)
    filter()操作符是可以对Observable流程的数据进行一层过滤处理，filter() 返回为 false 的值将不会发出到 Subscriber。
        基本用法如下，下面的例子是过滤只保留偶数：
Observable.just(1, 2, 3, 4)
			        .filter(new Func1<Integer, Boolean>() {
			            @Override
			            public Boolean call(Integer integer) {
			                return integer % 2 == 0;
			            }
			        })
			        .subscribe(new Action1<Integer>() {
			            @Override
			            public void call(Integer integer) {
			                Log.v(TAG, "data=" + integer);
			            }
			        });
         输出结果：
         data=2
         data=4
       说明：filter的实现是lift()的变换处理，详细请参考lift变换部分。另外，filter()返回了一个新的Observable，因此若不是采用上面这种直接流方式，而是分步调用方式，需要将新返回的Observable赋给原来的Observable。
