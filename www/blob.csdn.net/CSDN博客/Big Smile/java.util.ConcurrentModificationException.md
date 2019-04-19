# java.util.ConcurrentModificationException - Big Smile - CSDN博客
2018年06月29日 23:12:45[王啸tr1912](https://me.csdn.net/tr1912)阅读数：71标签：[for](https://so.csdn.net/so/search/s.do?q=for&t=blog)
个人分类：[Java学习](https://blog.csdn.net/tr1912/article/category/6453602)
所属专栏：[java学习](https://blog.csdn.net/column/details/26775.html)
有这么一段代码：
```java
for (QuestionMainModel questionMainModel : questionMainModels) {
    if (detailEntity.getQuestionMainId().equals(questionMainModel.getId())) {
           questionMainModel.setScore(getItemScore);
           questionMainList.add(questionMainModel);//加入已抽题list
           questionCount--;//需要抽题的个数减一
           questionMainModels.remove(questionMainModel);//从可选抽题list中去除,避免抽出的题目重复.
    }
}
```
这段代码执行的时候会有概率的报如标题一般的错误：
java.util.ConcurrentModificationException
这究竟是什么原因呢？
我们可以看到，我们在通过for遍历questionMainModels这个集合的时候，在里面可能会经过判断去直接操作questionMainModels中的元素进行添加或者删除操作，导致遍历集合的对象不稳定性。
我的猜测：
在我们执行for的时候，会预先把集合中的元素地址放在一个临时内存中，然后我们每遍历一个元素就会在其中拿一个元素的地址去访问，一旦我们删除其中的元素，或者让元素的排序不同之后，我们就通过地址取不到应得的元素了，从而导致报错。
网上的一篇相似情况的博客：
[https://www.cnblogs.com/snowater/p/8024776.html](https://www.cnblogs.com/snowater/p/8024776.html)
总之，我们在遍历集合的时候，尽量不要对集合进行增删的操作，所以上述代码修改如下：
```java
List<QuestionMainModel> questionMainModels = mainMap.get(key);
List<QuestionMainModel> delQuestionMainModel=new ArrayList<>();
for (QuestionMainModel questionMainModel : questionMainModels) {
    if (detailEntity.getQuestionMainId().equals(questionMainModel.getId())) {
       questionMainModel.setScore(getItemScore);
       questionMainList.add(questionMainModel);//加入已抽题list
       questionCount--;//需要抽题的个数减一
       //questionMainModels.remove(questionMainModel);//从可选抽题list中去除,避免抽出的题目重复.
       delQuestionMainModel.add(questionMainModel);
    }
}
for (QuestionMainModel questionMainModel : delQuestionMainModel) {
    questionMainModels.remove(questionMainModel);
}
```
