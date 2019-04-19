# STL-vector容器 - KinboSong的博客 - CSDN博客
2017年03月25日 21:53:52[songjinbo3](https://me.csdn.net/KinboSong)阅读数：217
个人分类：[数据结构与算法](https://blog.csdn.net/KinboSong/article/category/6791889)
vector、list和deque的优缺点比较：
参考：[http://blog.csdn.net/ariessurfer/article/details/8135761](http://blog.csdn.net/ariessurfer/article/details/8135761)
vector的物理结构是数组（顺序表），所以其具有顺序表的优缺点
vector优点：
（1）可以随机存取：知道下表后会访问很快。
vector缺点：
（1）插入、删除效率低：因为用数组存储，所以在中间或者开头插入（或删除）数据时会移动后面的数据，效率低下。
参考：[http://blog.csdn.net/jenny_84/article/details/45009381](http://blog.csdn.net/jenny_84/article/details/45009381)
以vector<int> vec;为例：
1、vector初始化
（1）vector<int> a(10)：定义了含有10个整形元素的vector，但是没有初始化；
（2）vector<int> a(10,1)：定义了含有10个1的vector，值都初始化为1；
（3）vector<int> a(vec)：定义a并将a初始化为vec
（4）vector<int> a(vec.begin()+1,a.end())：将vec从第二个位置到最后的值都赋给a
（5）int b[] = {1，2，3，4，5，6，7}
vector<int> a(b+1,b+3)：把b+1，b+2位置的元素赋给a。
(6) vector<int> a={1,2,3,4,5};
2、赋值
（1）vector<int> a;
a = c;
(2) int b[] = {1，2，3，4，5，6，7};
a.assign(b+1,b+3)：清除a的内容，将b+1,b+2位置的元素赋给a
a.assign(vec.begin()+1,vec.end())：清除a的内容，将vec从第二个位置到最后的值都赋给a
a.assign(4,2)：令a只含有4个元素，并且每个元素的值都为2；
2、访问vec的元素
（1）下表访问：vec[i]
（2）迭代器访问：
vector<int>::iterator iter = vec.begin();
*iter;
（3）at访问：vec.at(i)
vec.at(i)与vec[i]的不同就是前者会执行越界检查，当访问越界时会报错。
3、向vector中添加元素
（1）尾部添加元素：vec.push_back(i);
（2）vec.insert(vec.begin()+1,5)：在vec的第二个位置处插入5；
vec.insert(vec.begin()+1,3,5)：在vec的第二个位置处插入3个5；
int b[] = {1,2,3,4,5,6,7};
vec.insert(vec.begin()+1,b+3,b+6)：在vec的第二个位置处插入b[3],b[4],b[5],注意没有b[6];
vec.insert(vec.begin+1,a.begin()+1,a.begin()+6)：在vec的第二个位置处插入a.begin()+1到a,begin()+6位置上的元素（不包括a.begin()+6位置上的元素）
4、删除vector中的元素
（1）删除尾部元素：vvec.pop_back(i);
（2）vec.erase(vec.begin()+3,vec.begin()+6);删除vec.begin()+3 ,vec.begin()+4 ,vec.begin()+5位置上的元素。
（3）删除所有元素：vec.clear();
5、其他常用函数
vec.size()：获取vec的大小
vec.begin()：返回vec的第一个元素的首地址
vec.end()：返回vec的最后一个元素的下一个位置的地址
vec.front()：返回vec的第一个元素值
vec.back()：返回vec的最后一个元素值
