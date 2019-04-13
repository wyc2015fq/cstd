
# java的Array工具类 - 李昆鹏的博客 - CSDN博客


2018年03月25日 10:32:49[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：92


**-----------------------java的Array工具类--------------------------**
**工具类****Arrays**
**数组的工具类，这里的方法都是静态的**
**1.****把数组转换成字符串**
**2.****对任意数组排序**
**3.****对任意的数组做二分法的查找**
**4.****把数组转换成****List**
**java.util**
**类 Arrays**
java.lang.Object
**java.util.Arrays**
---

public class**Arrays**
extendsObject
此类包含用来操作数组（比如排序和搜索）的各种方法。此类还包含一个允许将数组作为列表来查看的静态工厂。
除非特别注明，否则如果指定数组引用为null，则此类中的方法都会抛出 NullPointerException。
此类中所含方法的文档都包括对*实现*的简短描述。应该将这些描述视为*实现注意事项*，而不应将它们视为*规范*的一部分。实现者应该可以随意替代其他算法，只要遵循规范本身即可。（例如，sort(Object[]) 使用的算法不必是一个合并排序算法，但它必须是*稳定的*。）
此类是Java Collections Framework的成员。

**--------------------------------**
**例子：**
**package**cn.tx.set;
**import**java.util.Arrays;
**import**java.util.List;
**public****class**ArraysDemo {
**public****static****void**main(String[]args) {
**int**[]arr={12,123,55,6,88,99,555,8445};
//将数组转换成字符串
*arrayToString*(arr);
Person[]p= {**new**Person(1,12,"李昆鹏"),**new**Person(2,12,"李昆鹏")};
*arrayToString*(p);
//对数组进行排序
//Arrays.sort(arr,1,3)对数组进行局部排序
*arraySort*(arr);
//对数组进行二分查找，前提必须排好升序
System.**out**.println(*binarySearch*(arr, 88));
//把数组转换成List集合
Integer[]arr1= {6,12,123,555,666};
List<Integer>list= Arrays.*asList*(arr1);
System.**out**.println(list);
//从数组转换过来的list不能添加和删除，但是可以修改
//list.add(1231);
//list.remove(12);
list.set(2, 345);
System.**out**.println(list);

}
/**
*将数组转换成字符串
***@param**array
*/
**public****static****void**arrayToString(**int**[]array) {
System.**out**.println(Arrays.*toString*(array));
}
/**
*将Person类数组转换成字符串
***@param**array
*/
**public****static****void**arrayToString(Person[]array) {
System.**out**.println(Arrays.*toString*(array));
}
/**
*对数组进行升序排序
***@param**array
*/
**public****static****void**arraySort(**int**[]array) {
//对对象进行排序的时候依照对象的compareTo方法进行的
Arrays.*sort*(array);
*arrayToString*(array);
}
/**
*使用二分查找法
*/
**public****static****int**binarySearch(**int**[]arr,**int**val) {
**return**Arrays.*binarySearch*(arr,val);
}

}
**结果：**
[12, 123, 55, 6, 88,99, 555, 8445]
[Person [id=1,age=12, name=李昆鹏], Person [id=2, age=12, name=李昆鹏]]
[6, 12, 55, 88, 99,123, 555, 8445]
3
[6, 12, 123, 555,666]
[6, 12, 345, 555,666]


