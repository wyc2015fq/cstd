
# Java实用方法整理（七）——增强for - manduner的博客 - CSDN博客


2018年05月27日 13:55:09[Manduner_TJU](https://me.csdn.net/manduner)阅读数：77


# 1，描述
是for循环的一种
# 2，格式
for(元素数据类型 变量 ： 数组或者Collection集合) {
循环体中直接使用变量即可，变量就是元素
}
# 3，优缺点
优点：简化了数组和集合的遍历，是迭代的一种替换方法
缺点：增强for的目标不能为null，在使用之前需要判断一下目标是否为空
# 4，实例
```python
//增强for遍历数组
int[] arr = {11,22,33,44,55}
for(int x : arr){
  System.out.println(x);//输出的就是数组中的元素
}
```
```python
//假设已经建立好Student类，成员变量包含“id，name”,且已经建立好了包含该对象的ArryList类
//增强for遍历包含Student对象的集合
for(Student s : arry){
  System.out.println(s.getId()+"---"+s.getName());
}
```


