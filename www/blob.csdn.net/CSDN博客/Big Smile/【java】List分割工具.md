# 【java】List分割工具 - Big Smile - CSDN博客
2019年04月16日 14:14:18[王啸tr1912](https://me.csdn.net/tr1912)阅读数：14标签：[java																[list](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Java学习](https://blog.csdn.net/tr1912/article/category/6453602)
我们在使用mybatis批量插入数据库的时候，在数据量过大的时候，肯定碰到过一个错误，详见另外一个老兄的博客
[https://blog.csdn.net/qq_36773257/article/details/79895819](https://blog.csdn.net/qq_36773257/article/details/79895819)
        在mybatis中使用foreach拼接sql语句，很常见了，在这里就不多提了，如果遇到了这种错误，要怎么办呢？一种解决方案就是提高数据库设置的sql语句的容量（mysql默认为4mb），这样可以多拼接一些些数据进去，但是如果真的是数据很多的情况需要怎么办呢？
        这个时候就需要用到List分割的程序了，我写了一段程序分享给大家，这个使用了最基本的数学运算解决了list的均分问题，然后返回了一个嵌套的list，外层list就是我们需要的分割结束的list集合，很好的采用了分治的策略，把大list分解为小list进行sql操作，解决报错的问题。
```
import java.util.ArrayList;
import java.util.List;
/**
 * @author sam
 */
public class ListUtil {
    /**
     * 将一组数据平均分成n组
     *
     * @param source 要分组的数据源
     * @param n      平均分成n组
     * @param <T> 构造型
     * @return 返回list集合
     */
    public static <T> List<List<T>> averageAssign(List<T> source, int n) {
        List<List<T>> result = new ArrayList<>();
        int remainder = source.size() % n;  //(计算出余数)
        int number = source.size() / n;  //商
        int offset = 0;//偏移量
        for (int i = 0; i < n; i++) {
            List<T> value = null;
            if (remainder > 0) {
                value = source.subList(i * number + offset, (i + 1) * number + offset + 1);
                remainder--;
                offset++;
            } else {
                value = source.subList(i * number + offset, (i + 1) * number + offset);
            }
            result.add(value);
        }
        return result;
    }
}
```
