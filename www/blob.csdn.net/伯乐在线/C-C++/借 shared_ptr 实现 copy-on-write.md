# 借 shared_ptr 实现 copy-on-write - 文章 - 伯乐在线
原文出处： [陈硕](http://blog.csdn.net/solstice/article/details/3351751)
场景：
一个多线程的C++程序，24h x 5.5d运行。有几个工作线程ThreadW{0,1,2,3}，处理客户发过来的交易请求，另外有一个背景线程ThreadB，不定期更新程序内部的参考数据。这些线程都跟一个hash表打交道，工作线程只读，背景线程读写，必然要用到一些同步机制，防止数据损坏。
这里的示例代码用std::map代替hash表，意思是一样的：
typedef map<string, vector<pair<string, int> > > Map;
map 的 key 是用户名，value 是一个vector，里边存的是不同stock的最小交易间隔，vector已经排好序，可以用二分查找。
我们的系统要求工作线程的延迟尽可能小，可以容忍背景线程的延迟略大。一天之内，背景线程对数据更新的次数屈指可数，最多一小时一次，更新的数据来自于网络，所以对更新的及时性不敏感。Map的数据量也不大，大约一千多条数据。
最简单的同步办法，用读写锁，工作线程加读锁，背景线程加写锁。但是读写锁的开销比普通mutex要大，如果工作线程能用最普通的非重入Mutex实现同步，就不必用读写锁，性能较高。我们借助shared_ptr实现了这一点：


```
class Mutex;
class MutexLockGuard;
class CustomerData
{
 public:
  CustomerData() : data_(new Map)
  { }
  ~CustomerData();
  int query(const string& customer, const string& stock) const
  {
    MapPtr data = getData();
    // data 一旦拿到，就不再需要锁了。取数据的时候只有getData()内部有锁，多线程并发读的性能很好。
    // 假设用户肯定存在
    const EntryList& entries = (*data)[customer];
    return findEntry(entries, stock);
  }
  void update(const string& customer, const EntryList& entries );
 private:
  typedef vector<string, int> EntryList;
  typedef map<string, EntryList> Map;
  typedef tr1::shared_ptr<Map> MapPtr;
  static int findEntry(const EntryList& entries, const string& key) const
  { /* 用 lower_bound 在 entries 里找 key */ }
  MapPtr getData() const
  {
    MutexLockGuard lock(dataMutex_);
    return data_;
  }
  MapPtr data_;
  mutable Mutex dataMutex_;
};
```
关键看CustomerData::update()怎么写。既然要更新数据，那肯定得加锁，如果这时候其他线程正在读，那么不能在原来的数据上修改，得创建一个副本，在副本上修改，修改完了再替换。如果没有用户在读，那么就能直接修改，节约一次拷贝。


```
void CustomerData::update(const string& customer, const EntryList& entries )
{
  MutexLockGuard lock(dataMutex_);
  if (!data_.unique())
  {
    MapPtr newData(new Map(*data_));
    data_.swap(newData);
  }
  assert(data_.unique());
  (*data_)[customer] = entries;
}
```
注意其中用了shared_ptr::unique()来判断是不是有人在读，如果有人在读，那么我们不能直接修改，因为query()并没有全程加锁，只在getData()内部有锁。shared_ptr::swap()把data_替换为新副本，而且我们还在锁里，不会有别的线程来读，可以放心地更新。
据我们测试，大多数情况下更新都是在原来数据上进行的，拷贝的比例还不到1%，很高效。更准确的说，这不是copy-on-write，而是copy-on-other-reading。
我们将来可能会采用无锁数据结构，不过目前这个实现已经非常好，满足我们的要求。
