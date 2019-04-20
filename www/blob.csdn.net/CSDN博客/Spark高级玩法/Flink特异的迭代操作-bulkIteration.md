# Flink特异的迭代操作-bulkIteration - Spark高级玩法 - CSDN博客
2019年01月13日 22:30:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：197
迭代算法在很多数据分析领域会用到，比如机器学习或者图计算。为了从大数据中抽取有用信息，这个时候往往会需要在处理的过程中用到迭代计算。大数据处理框架很多，比如spark，mr。实际上这些实现迭代计算都是很困难的。
Flink神奇之处就是它直接支持迭代计算。Flink实现迭代的思路也是很简单，就是实现一个step函数，然后将其嵌入到迭代算子中去。有两种迭代操作算子:Iterate和Delta Iterate。两个操作算子都是在未收到终止迭代信号之前一直调用step函数。
本小节是主要是讲解理论。
迭代操作算子包括了简单的迭代形式：每次迭代，step函数会消费全量数据(本次输入和上次迭代的结果)，然后计算得到下轮迭代的输出(例如，map，reduce，join等)
1.迭代输入(Iteration Input)
第一次迭代的初始输入，可能来源于数据源或者先前的操作算子。
2. Step函数
每次迭代都会执行step函数。其是由map，reduce，join等算子组成的数据流，根据业务定制的。
3. 下次迭代的部分结果(Next Partial Solution):
每次迭代，step函数的输出结果会有部分返回参与继续迭代。
4. 最大迭代次数
如果没有其他终止条件，就会在聚合次数达到该值的情况下终止。
5. 自定义聚合器收敛：
迭代允许指定自定义聚合器和收敛标准，如sum会聚合要发出的记录数（聚合器），如果此数字为零则终止（收敛标准）。
**案例：累加计数**
这个例子主要是给定数据输入，每次增加一，输出结果。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXUXNzXOmibyB39wmEFNQEkIzn6zC4A4ZStibbicymM6sbPYszjGTDWibpZfcm4x1hiaAAZnxibuEgCpljQ/640?wx_fmt=png)
- 
迭代输入：输入是1-5的数字。
- 
step函数：给数字加一操作。
- 
部分结果：实际上就是一个map函数。
- 
迭代结果：最大迭代次数是十次，所以最终输出是11-15.
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXUXNzXOmibyB39wmEFNQEkI5yygllxZN0mRKJCXlnZREQQRnOBO3Itb1cCMJn8og5GZkZMhU19qZQ/640?wx_fmt=png)
**代码操作**
编程的时候，本文说的这种迭代方式叫做bulk Iteration，需要调用iterate(int),该函数返回的是一个IterativeDataSet，当然我们可以对他进行一些操作，比如map等。Iterate函数唯一的参数是代表最大迭代次数。
迭代是一个环有前面的图可以看到，我们需要进行闭环操作，那么这时候就要用到closeWith(Dataset)操作了，参数就是需要循环迭代的dataset。也可以可选的指定一个终止标准，操作closeWith(DataSet, DataSet)，可以通过判断第二个dataset是否为空，来终止迭代。如果不指定终止迭代条件，迭代就会在迭代了最大迭代次数后终止。
下面就是通过迭代计算pi的例子。
package Streaming.iteration;
import org.apache.flink.api.common.functions.MapFunction;
import org.apache.flink.api.java.DataSet;
import org.apache.flink.api.java.ExecutionEnvironment;
import org.apache.flink.api.java.operators.IterativeDataSet;
publicclassIteratePi{
publicstaticvoidmain(String[] args)throws Exception{
final ExecutionEnvironmentenv = ExecutionEnvironment.getExecutionEnvironment();
// Create initialIterativeDataSet
      IterativeDataSet<Integer> initial= env.fromElements(0).iterate(100);
      DataSet<Integer> iteration= initial.map(new MapFunction<Integer, Integer>(){
@Override
publicIntegermap(Integer i)throws Exception{
double x = Math.random();
double y = Math.random();
return i + ((x * x + y * y < 1) ? 1 : 0);
         }
      });
// Iterativelytransform the IterativeDataSet
      DataSet<Integer> count = initial.closeWith(iteration);
      count.map(new MapFunction<Integer, Double>(){
@Override
public Double map(Integercount)throws Exception {
return count /(double) 10000 * 4;
         }
      }).print();
// execute theprogram
      env.execute("IterativePi Example");
   }
}
推荐阅读：
[调试flink源码](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485969&idx=1&sn=38a5dc0465a96573e81037343e377417&chksm=9f38e939a84f602f4e1d90d7ebcd2cabb52532c9c8b6d1b553d95806df5f46f5a4e3f5e6d7ac&scene=21#wechat_redirect)
[Flink异步IO第一讲](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485776&idx=1&sn=bb78ed1f45c7509688526cbd29686f4f&chksm=9f38ea78a84f636e468765b2f870e5f78afe5063f6a94b96cca540c9116fc39d7e4a123b6b6e&scene=21#wechat_redirect)
[flink的神奇分流器-sideoutput](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486266&idx=1&sn=d9e5b585ea70a1715514f621b4acda11&chksm=9f38e812a84f6104609912a96a41c80259ad2c4dab24303c667e6de85e4133ea7ed218e617e7&scene=21#wechat_redirect)
[Structured Streaming VS Flink](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486237&idx=1&sn=427e452f78f725c535b450eb759173ea&chksm=9f38e835a84f6123c697e76bf4a3a2e77ddb09af433e958f1206a2aba897608675eccb973f93&scene=21#wechat_redirect)
更详细的flink内容分享，欢迎加入浪尖知识星球，与470人一起学习～
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFUibDu2gaX89M600Ip3MDrVY0ibqsWg1gHuTdGCX1QfbyyCPlf31gLQna9OvNyRsBaByXsZrZeqibtVQ/640?wx_fmt=jpeg)
