# 用Arrays.sort()对数字排序 - 右上角同学的专栏 - CSDN博客
2016年08月27日 17:00:52[右上角徐](https://me.csdn.net/u011032983)阅读数：2162
       有的时候需要对数组里的element进行排序。当然可以自己编写合适的排序方法，但既然[java包](https://www.baidu.com/s?wd=java%E5%8C%85&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1d9mvn4uh7bnH-WPvcYm1wW0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHTvPWcdnHTvnHcLPjbLnWnYn0)里有自带的[Arrays.sort](https://www.baidu.com/s?wd=Arrays.sort&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1d9mvn4uh7bnH-WPvcYm1wW0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHTvPWcdnHTvnHcLPjbLnWnYn0)排序方法，在
 数组元素比较少的时候可以使用。
　　Sorting an Array 
      1. 数字排序  int[] intArray = new int[] { 4, 1, 3, -23 };
[Arrays.sort](https://www.baidu.com/s?wd=Arrays.sort&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1d9mvn4uh7bnH-WPvcYm1wW0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHTvPWcdnHTvnHcLPjbLnWnYn0)(intArray);
　　输出： [-23, 1, 3, 4]
　　2. 字符串排序，先大写后小写 String[] strArray = new String[] { "z", "a", "C" };
[Arrays.sort](https://www.baidu.com/s?wd=Arrays.sort&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1d9mvn4uh7bnH-WPvcYm1wW0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHTvPWcdnHTvnHcLPjbLnWnYn0)(strArray);
　　输出： [C, a, z]
　　3. 严格按字母表顺序排序，也就是忽略大小写排序 Case-insensitive sort
　　Arrays.sort(strArray, String.CASE_INSENSITIVE_ORDER);
　　输出： [a, C, z]
　　4. 反向排序， Reverse-order sort
　　Arrays.sort(strArray, Collections.reverseOrder());
　　输出：[z, a, C]
　　5. 忽略大小写反向排序 Case-insensitive reverse-order sort
　　Arrays.sort(strArray, String.CASE_INSENSITIVE_ORDER);
　　Collections.reverse(Arrays.asList(strArray));
　　输出： [z, C, a]
　　数组排序功能，在java的api里面早已实现，我们没有 必要去重复制造轮子。
      Java Arrays中提供了对所有类型的排序。其中主要分为Primitive(8种基本类型)和Object两大类。
　　基本类型：采用调优的快速排序；
　　对象类型：采用改进的归并排序。
具体改进分析见博客：http://www.cnblogs.com/gw811/archive/2012/10/04/2711746.html
