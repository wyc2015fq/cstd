# caffe源码阅读——Blob类

2016年01月02日 17:36:14 [鱼蛋蛋哥](https://me.csdn.net/u010327085) 阅读数：2339



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/u010327085/article/details/50448912

最近，在windows下用vs2013编译通过了caffe。现想在vs2013中来阅读学习caffe的源码，边学边记录。caffe的windows版本，我是参考了这里。 
<http://blog.csdn.net/happynear/article/details/45372231>。 
顺便来以此项目学习C++。

作为一个小白，对这么大的工程，感觉完全无法下手，参考了知乎<https://www.zhihu.com/question/27982282>中甘宇飞的回答。按Blob，Layer，Net，Solver的顺序来熟悉。

## Blob

**成员变量**

```cpp
protected:
  shared_ptr<SyncedMemory> data_;//data数据的智能指针
  shared_ptr<SyncedMemory> diff_;//差分的智能指针
  shared_ptr<SyncedMemory> shape_data_;//数据结构的智能指针
  vector<int> shape_;//数据结构
  int count_;数据的大小
  int capacity_;目前分配空间的大小，可以理解为存放数据的能力
```

关于智能指针，我也是刚学，理解不到位的地方欢迎指正。我理解就是能够自动释放它指向的对象。这里的shared_ptr主要区别于unique_ptr，能够被多个指针所指向。 
关于shape_data_ 使用来存放data_和diff_ 数据结构用的，其实和shape_中的内容是一样的。 
关于count_，它是只当前data数据结构的大小，是各个维度值得乘积。而由于在这里面当存储空间不够时会重新分配添加存储空间。相反的，却不会减小存储空间，所以这里的capacity_ 表示的是存储的能力。 
在看一下之前的版本，这里与之前有了一些修改，主要就是将以前的具体的num，weight，dim，height等换了成shape_和shape_data_。感觉主要是为了提高程序的可扩展性。

**SyncedMemory类**

SyncedMemory负责简单的管理分配内存，并且负责同步CPU和GPU。具体可见<http://blog.csdn.net/u010327085/article/details/50449728>。

**关于template**

在blob中用到了template，很是不解，看的晕晕乎乎，看了<http://blog.sina.com.cn/s/blog_74a271040100tkmj.html>后明白了一点，感觉写的还算清楚。

**正餐开始**

在blob中有一些舍弃的函数，这里就不再进行介绍了。 
构造函数 
第一个简单构造函数

```cpp
Blob():data_(), diff_(), count_(0), capacity_(0) {}1
```

第二个构造函数

```cpp
explicit Blob(const vector<int>& shape);1
```

根据shape来初始化shape_和shape_data_，以及为data_ 和diff_ 分配空间。 
三个Reshape函数

```cpp
void Reshape(const vector<int>& shape);
void Reshape(const BlobShape& shape);
void ReshapeLike(const Blob& other);
```

构造函数其实就是先初始化capacity_然后再调用Reshape函数。该函数的功能是改变blob的维，如果需要新存储，则重新分配内存。如之前所说，不会减小内存。 
这三个函数主要是在于参数的数据类型不同，第一个没什么好说的，第二个中的BlobShape是通过protobuf通过caffe.proto生成的caffe.pb.h头文件中的一种数据类型，具体可以去看，这里为不详细说明，只当是一种数据类型。第三个是使当前的blob的结构与参数other的结构相同。

一些获取成员变量函数

```cpp
inline string shape_string() const;//以string形式获取shape_
inline const vector<int>& shape() const;//获取shape_
inline int shape(int index) const;//获取index维的大小
inline int num_axes() const;//获取维的个数
inline int count() const;//获取当前data的大小
inline int count(int start_axis, int end_axis) const;//获取某几维数据的大小
inline int count(int start_axis) const;//获取某一维到结束数据的大小
inline int CanonicalAxisIndex(int axis_index) const;//标准化索引，主要是对参数索引进行标准化，以满足要求
inline int LegacyShape(int index) const;//data_维数不大于4时才能使用，功能同shape()类似。
inline int offset(const int n, const int c = 0, \
            const int h = 0,const int w = 0) const;
            //获取某位置为第多少个。
inline int offset(const vector<int>& indices) const;//同上，只是参数不同
inline Dtype data_at(const int n, const int c, const int h,
      const int w) const;//获取某位置的data_数据
inline Dtype data_at(const vector<int>& index);//同上
inline Dtype diff_at(const int n, const int c, const int h,
      const int w) const;//获取某位置的diff_数据
inline Dtype diff_at(const vector<int>& index) const;//同上
inline const shared_ptr<SyncedMemory>& data() const;//获取data_
inline const shared_ptr<SyncedMemory>& diff() const;//获取diff_
const Dtype* cpu_data() const;//获取data_ cpu指针
const int* gpu_shape() const;//获取shape_data_的gpu指针
const Dtype* gpu_data() const;//获取data_的gpu指针
const Dtype* cpu_diff() const;//获取diff_的cpu指针
const Dtype* gpu_diff() const;//获取diff_的gpu指针
```

一些功能函数

```cpp
void CopyFrom(const Blob<Dtype>& source, bool copy_diff =false,bool reshape = false);
/**功能：由source Blob拷贝到本Blob。
  参数：source 源Blob
       copy_diff 如果是false，拷贝data_,如果是true，拷贝diff_
       reshape 如果是false，则需要源Blob与本Blob形状相同，如果是
**/
void set_cpu_data(Dtype* data);//设置data_的cpu指针，只是修改了指针
Dtype* mutable_cpu_data();//见SyncedMemory的mutable_cpu_data();
Dtype* mutable_gpu_data();//见SyncedMemory的mutable_gpu_data();
Dtype* mutable_cpu_diff();//见SyncedMemory的mutable_cpu_data();
Dtype* mutable_gpu_diff();//见SyncedMemory的mutable_gpu_data();
void Update();
/**
其中用到math_functions.hpp中的函数caffe_axpy(),该函数封装了cblas_saxpy，实现的是Y=alpha*X+Y。由此，知该函数的功能是data_=(data_-diff_)。
另外，该函数只实现了对double和float型数据，对于unsigned int和int由于该函数主要是在Net中被调用，只有Blob<float>和Blob<double>型式，因此没有定义unsigned int和int。
**/
void FromProto(const BlobProto& proto, bool reshape = true);
//由BlobProto对Blob进行赋值操作。reshape代表是否允许修改shape_的大小。需要注意的是再这里有double和float两种类型的数据 ，在代码中可以看到具体的体现
void ToProto(BlobProto* proto, bool write_diff = false) const;//针对double和float有两个实现的函数，这里只举例说明其中的一个。将Blob中的数据存入BlobProto中，write_diff表示是否存diff_。
Dtype asum_data() const;
/**
功能：计算L1范数
说明：其中用到了math_function.hpp中的函数caffe_cpu_asum()和caffe_gpu_asum，实现的功能是对向量X求其每个元素绝对值的和，不同的是X分别在cpu和gpu中。
**/
Dtype asum_diff() const;//同上，不同的是针对的是diff_
Dtype sumsq_data() const;
/**
功能：计算L2范数。
说明：用到了math_function.hpp中的caffe_cpu_dot(),caffe_cpu_strided_dot(),caffe_gpu_dot(), caffe_gpu_strided_dot()。具体就是就向量X的平方和。
**/
Dtype sumsq_diff() const;//同上，不同的是针对的是diff_。
void scale_data(Dtype scale_factor);
/**
功能：正规化data_。
说明：用到math_function.hpp中的caffe_scal()和caffe_gpu_scal()函数，我的理解就是对向量X乘上一个因子。
**/
void scale_diff(Dtype scale_factor);//同上，不同在于针对diff_。
void ShareData(const Blob& other);//本Blob共享other的data_
void ShareDiff(const Blob& other);//本Blob共享other的diff_
bool ShapeEquals(const BlobProto& other);//判断other与本Blob形状是否相同。
```

**一些细节** 
关于cpu_data()和mutable_cpu_data(),还有其他的几个类似函数，主要是注意返回指针，cpu_data()返回的是const，也即不能修改指针指向的值。mutable_cpu_data()则可以。

**总结** 
终于写完了，真是曲折。感觉看完了每一个函数的具体实现，有点晕乎乎，但还是看完了，对于每一个函数都做了总结。总体上，感觉由于caffe版本的更新问题，为了使得caffe的扩展性更强，其中很多地方都用的vector代替以前的四个维度num，channel，width和height。 
另外，对于math_function.hpp中的数学矩阵操作的函数真是太多了，准备遇见一个看一个吧。。。 
准备要看layer了，好激动有木有。