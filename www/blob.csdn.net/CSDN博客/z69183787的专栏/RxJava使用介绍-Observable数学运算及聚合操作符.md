# RxJava使用介绍-Observable数学运算及聚合操作符 - z69183787的专栏 - CSDN博客
2018年08月09日 18:09:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：72
个人分类：[Java-RxJava异步框架](https://blog.csdn.net/z69183787/article/category/7921321)
[https://blog.csdn.net/jdsjlzx/article/details/51489793](https://blog.csdn.net/jdsjlzx/article/details/51489793)
RxJava系列教程：
[1. RxJava使用介绍](http://blog.csdn.net/jdsjlzx/article/details/51485778)[【视频教程】](http://www.stay4it.com/course/27/learn#lesson/360)
[2. RxJava操作符](http://www.stay4it.com/course/27)
[• Creating Observables(Observable的创建操作符)](http://blog.csdn.net/jdsjlzx/article/details/51485861)[【视频教程】](http://www.stay4it.com/course/27/learn#lesson/359)
[• Transforming Observables(Observable的转换操作符)](http://blog.csdn.net/jdsjlzx/article/details/51493772)[【视频教程】](http://www.stay4it.com/course/27/learn#lesson/369)
[• Filtering Observables(Observable的过滤操作符)](http://blog.csdn.net/jdsjlzx/article/details/51489061)[【视频教程】](http://www.stay4it.com/course/27/learn#lesson/371)
[• Combining Observables(Observable的组合操作符)](http://blog.csdn.net/jdsjlzx/article/details/52415615)[【视频教程】](http://www.stay4it.com/course/27/learn#lesson/370)
[• Error Handling Operators(Observable的错误处理操作符)](http://blog.csdn.net/jdsjlzx/article/details/52448173)[【视频教程】](http://www.stay4it.com/course/27/learn#lesson/372)
[• Observable Utility Operators(Observable的辅助性操作符)](http://blog.csdn.net/jdsjlzx/article/details/54768360)[【视频教程】](http://www.stay4it.com/course/27/learn#lesson/364)
[• Conditional and Boolean Operators(Observable的条件和布尔操作符)](http://blog.csdn.net/jdsjlzx/article/details/54767751)[【视频教程】](http://www.stay4it.com/course/27/learn#lesson/374)
[• Mathematical and Aggregate Operators(Observable数学运算及聚合操作符)](http://blog.csdn.net/jdsjlzx/article/details/51489793)[【视频教程】](http://www.stay4it.com/course/27/learn#lesson/368)
[• 其他如observable.toList()、observable.connect()、observable.publish()等等；](http://blog.csdn.net/jdsjlzx/article/details/53365839)[【视频教程】](http://www.stay4it.com/course/27/learn#lesson/366)
[3. RxJava Observer与Subcriber的关系](http://blog.csdn.net/jdsjlzx/article/details/51534504)[【视频教程】](http://www.stay4it.com/course/27/learn#lesson/367)
[4. RxJava线程控制（Scheduler）](http://blog.csdn.net/jdsjlzx/article/details/51685769)[【视频教程】](http://www.stay4it.com/course/27/learn#lesson/365)
[5. RxJava 并发之数据流发射太快如何办（背压（Backpressure））](http://blog.csdn.net/jdsjlzx/article/details/51868640)[【视频教程】](http://www.stay4it.com/course/27/learn#lesson/373)
## 算术操作符
- averageInteger( ) — 求序列平均数并发射
- averageLong( ) — 求序列平均数并发射
- averageFloat( ) — 求序列平均数并发射
- averageDouble( ) — 求序列平均数并发射
- max( ) — 求序列最大值并发射
- maxBy( ) — 求最大key对应的值并发射
- min( ) — 求最小值并发射
- minBy( ) — 求最小Key对应的值并发射
- sumInteger( ) — 求和并发射
- sumLong( ) — 求和并发射
- sumFloat( ) — 求和并发射
- sumDouble( ) — 求和并发射
需要说明的是算术操作符是属于可选的rxjava-math模块，需要单独下载jar包： 
* [http://search.maven.org/#search%7Cga%7C1%7Cio.reactivex.rxjava-math](http://search.maven.org/#search%7Cga%7C1%7Cio.reactivex.rxjava-math)
下面我们简单举例介绍下。
Average
计算原始Observable发射数字的平均值并发射它。Average操作符操作符一个发射数字的Observable，并发射单个值：原始Observable发射的数字序列的平均值。
它被实现为四个操作符：averageDouble, averageFloat, averageInteger, averageLong。 
示例代码：
```
MathObservable.averageInteger(Observable.just(1, 2, 3, 4))//  10/4=2
        .subscribe(new Action1<Integer>() {
            @Override
            public void call(Integer value) {
                System.out.println("onSuccess value = " + value);
            }
        });
```
输出如下：
`onSuccess value = 2`
## 聚合操作符
- concat( ) — 顺序连接多个Observables,并且严格按照发射顺序，前一个没有发射完，是不能发射后面的。
- count( ) and countLong( ) — 计算数据项的个数并发射结果
- reduce( ) — Reduce操作符应用一个函数接收Observable发射的数据和函数的计算结果作为下次计算的参数，输出最后的结果。 
	跟scan操作符很类似，只是scan会输出每次计算的结果，而reduce只会输出最后的结果。
- collect(Func0,Action2) — 将原始Observable发射的数据放到一个单一的可变的数据结构中，然后返回一个发射这个数据结构的Observable 。 
	下面几个to方法，也属于辅助操作符.
- toList( ) — 收集原始Observable发射的所有数据到一个列表，然后返回这个列表。
- toSortedList( ) — 收集原始Observable发射的所有数据到一个有序列表，然后返回这个列表。
- toMap( ) — 将序列数据转换为一个Map，Map的key是根据一个函数计算的。
- toMultiMap( ) — 将序列数据转换为一个列表，同时也是一个Map，Map的key是根据一个函数计算的。
Concat
Concat操作符将多个Observable结合成一个Observable并发射数据，并且严格按照先后顺序发射数据，前一个Observable的数据没有发射完，是不能发射后面Observable的数据的。 
有两个操作符跟它类似，但是有区别，分别是 
1.startWith：仅仅是在前面插上一个数据。 
2.merge:其发射的数据是无序的。 
![这里写图片描述](http://reactivex.io/documentation/operators/images/concat.png)
示例代码：
```
//还有一个实例方法叫concatWith，这两者是等价的：Observable.concat(a,b)和a.concatWith(b)
Observable.concat(
        Observable.interval(100,TimeUnit.MILLISECONDS).take(4),
        Observable.interval(200,TimeUnit.MILLISECONDS).take(5))
        .subscribe(aLong -> Log.d(TAG, "concat:"+aLong));
```
输出如下：
```
concat:0 
concat:1 
concat:2 
concat:3
concat:0 
concat:1 
concat:2 
concat:3 
concat:4
```
### Count
Count操作符将一个Observable转换成一个发射单个值的Observable，这个值表示原始Observable发射的数据的数量。
如果原始Observable发生错误终止，Count不发射数据而是直接传递错误通知。如果原始Observable永远不终止，Count既不会发射数据也不会终止。
示例代码：
```
Observable.from(new String[] { "one", "two", "three" })
        .count()
        .subscribe(integer->Log.d(TAG, "count:"+integer));
Observable.from(new String[] { "one", "two", "three" })
        .countLong()
        .subscribe(aLong->Log.d(TAG, "countLong:"+aLong));
```
运行结果如下
```
count:3 
countLong:3
```
Reduce、Collect
Reduce操作符应用一个函数接收Observable发射的数据和函数的计算结果作为下次计算的参数，输出最后的结果。跟前面我们了解过的scan操作符很类似，只是scan会输出每次计算的结果，而reduce只会输出最后的结果。
> 
注意：如果原始Observable没有发射任何数据，reduce抛出异常IllegalArgumentException。
在其它场景中，这种操作有时被称为累积，聚集，压缩，折叠，注射等。
![这里写图片描述](http://reactivex.io/documentation/operators/images/reduce.png)
示例代码：
```
Observable.just(1,2,3,4)
        .reduce(new Func2<Integer, Integer, Integer>() {
            @Override
            public Integer call(Integer integer, Integer integer2) {//integer为前面几项只和，integer2为当前发射的数据
                Log.d(TAG, "integer:"+integer+"  integer2:"+integer2);
                return integer+integer2;
            }
        }).subscribe(integer -> Log.d(TAG, "reduce:"+integer));
```
输出如下：
```
integer:1 integer2:2 
integer:3 integer2:3 
integer:6 integer2:4 
reduce:10
```
Collect操作符类似于Reduce，但是其目的不同，collect用来将源Observable发射的数据给收集到一个数据结构里面，需要使用两个参数： 
1.一个产生收集数据结构的函数。 
2.一个接收第一个函数产生的数据结构和源Observable发射的数据作为参数的函数。 
![这里写图片描述](http://reactivex.io/documentation/operators/images/collect.png)
聚合操作符比较少，就介绍到这里了。
