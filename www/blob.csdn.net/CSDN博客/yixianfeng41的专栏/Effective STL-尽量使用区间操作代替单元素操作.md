# Effective STL-尽量使用区间操作代替单元素操作 - yixianfeng41的专栏 - CSDN博客
2017年02月23日 10:37:02[第2梦](https://me.csdn.net/yixianfeng41)阅读数：652标签：[Stl尽量使用区间操](https://so.csdn.net/so/search/s.do?q=Stl尽量使用区间操&t=blog)
个人分类：[c++](https://blog.csdn.net/yixianfeng41/article/category/5618607)
所属专栏：[C++编程](https://blog.csdn.net/column/details/14514.html)
### **一、原因：**
1、代码更少，更简洁； 
2、 对序列容器，还可以避免单个单个申请造成多次分配内存（拷贝开销大）
### **二、区间操作：**
#### 1）区间构造：
`container::container(InputIterator begin,InputIterator end);`
#### 2）区间插入：
对于序列容器：
```
void Container::insert(Iterator positon,//插入位置
                       InputIterator begin, //目标起点位置
                       InputIterator end);  //目标终点
```
对于关联容器（他们不需要指定位置，位置有他们自己机制决定）
` void Container::Insert(InputIterator begin,InputIterator end);`
#### 3）区间删除：
对于序列容器：
`iterator Container:erase(Iteraor begin,Iterator);`
对于关联容器：
`void Container::erase(Iterator begin,Iterator end);`
#### 4）区间赋值
`void Container::assign(InputIterator begin,InputIterator end);`
### **三、 实例**
给两个vector,v1和v2，使v1得内容为v2后半部分。
#### 方法一：
`v1.assign(v2.begin()+v2.size()/2,v2.end());`
#### 方法二：
```
v1.clear();
v1.insert(v2.begin()+v2.size()/2,v2.end());
```
#### 方法三：
```
v1.clear();
copy(v2.begin()+v2.size()/2,v2.end(),back_insert(v1);
```
