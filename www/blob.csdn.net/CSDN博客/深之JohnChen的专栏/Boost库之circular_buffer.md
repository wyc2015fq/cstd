# Boost库之circular_buffer - 深之JohnChen的专栏 - CSDN博客

2017年05月11日 15:13:49[byxdaz](https://me.csdn.net/byxdaz)阅读数：1125


         Boost.Circular_buffer维护了一块连续内存块作为缓存区，当缓存区内的数据存满时，继续存入数据就覆盖掉旧的数据。Boost.Circular_buffer维护了一块连续内存块作为缓存区，当缓存区内的数据存满时，继续存入数据就覆盖掉旧的数据。它是一个与STL兼容的容器，类似于
 std::list或std::deque,并且支持随机存取。circular_buffer被特别设计为提供固定容量的存储大小。当其容量被用完时，新插入的元素会覆盖缓冲区头部或尾部(取决于使用何种插入操作)的元素。

```cpp
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/circular_buffer.hpp>
typedef unsigned int size_type;

template<typename T>
class SafeCircularBuffer
{
public:
	SafeCircularBuffer(int size) :
	  buffer_(size)
	  {
	  };
	  size_type capacity()
	  {
		  boost::shared_lock< boost::shared_mutex > read_lock(buffer_mutex_);
		  return buffer_.capacity();
	  };
	  void set_capacity(size_type new_capacity)
	  {
		  boost::unique_lock< boost::shared_mutex > write_lock(buffer_mutex_);
		  buffer_.set_capacity(new_capacity);
	  };
	  size_type size() const
	  {
		  boost::shared_lock< boost::shared_mutex > read_lock(buffer_mutex_);
		  return buffer_.size();
	  };
	  bool push_back(const T& item)
	  {
		  boost::unique_lock< boost::shared_mutex > write_lock(buffer_mutex_);
		  if (buffer_.capacity() == 0)
			  return false;

		  if (!buffer_.full())
		  {
			  buffer_.push_back(item);
			  return true;
		  }

		  return false;
	  };
	  bool push_front(const T& item)
	  {
		  boost::unique_lock< boost::shared_mutex > write_lock(buffer_mutex_);
		  if (buffer_.capacity() == 0)
			  return false;
		  buffer_.push_front(item);
		  return true;
	  };
	  void clear()
	  {
		  boost::unique_lock< boost::shared_mutex > write_lock(buffer_mutex_);
		  buffer_.clear();
	  };
	  T& front()
	  {
		  boost::shared_lock< boost::shared_mutex > read_lock(buffer_mutex_);
		  return buffer_.front();
	  };
	  void pop_front()
	  {
		  boost::unique_lock< boost::shared_mutex > write_lock(buffer_mutex_);
		  buffer_.pop_front();
	  };
	  void pop_back()
	  {
		  boost::unique_lock< boost::shared_mutex > write_lock(buffer_mutex_);
		  buffer_.pop_back();
	  };
	  unsigned int size()
	  {
		  boost::shared_lock< boost::shared_mutex > read_lock(buffer_mutex_);
		  return buffer_.size();
	  };
	  bool empty()
	  {
		  boost::shared_lock< boost::shared_mutex > read_lock(buffer_mutex_);
		  return buffer_.empty();
	  };
	  bool full()
	  {
		  boost::shared_lock< boost::shared_mutex > read_lock(buffer_mutex_);
		  return buffer_.full();
	  };
	  T& at(size_t index)
	  {
		  boost::shared_lock< boost::shared_mutex > read_lock(buffer_mutex_);
		  try
		  {
			  buffer_.at(index);
		  }
		  catch(std::out_of_range& ex)
		  {
			  throw(ex);
		  }
		  return buffer_[index];
	  };
	  T& operator[](size_t index)
	  {
		  boost::shared_lock< boost::shared_mutex > read_lock(buffer_mutex_);
		  return buffer_[index];
	  };
private:
	boost::circular_buffer<T> buffer_;
	boost::shared_mutex  buffer_mutex_;			//读写锁
};

#include <boost/circular_buffer.hpp>
#include <iostream>
using namespace std;

int main()
{
	//非线程安全circular_buffer
	typedef boost::circular_buffer<int> circular_buffer;
	circular_buffer cb(3);
	cout << cb.capacity() <<endl;
	cout << cb.size() <<endl;

	cb.push_back(0);
	cb.push_back(1);
	cb.push_back(2);
	cout << cb.size() <<endl;

	//设置新容量大小
	cb.set_capacity(6);
	cb.push_back(3);
	cb.push_back(4);
	cb.push_back(5);
	cout << cb.size() << endl;
	
	//数据循环（从某个位置开始）
	for(int i=0; i<cb.size(); i++)
	{
		cout << cb[i];
		if(i != cb.size()-1)
		{
			cout << ",";
		}
	}
	cout<<endl;
	cb.rotate(cb.begin()+1);
	for(int i=0; i<cb.size(); i++)
	{
		cout << cb[i];
		if(i != cb.size()-1)
		{
			cout << ",";
		}
	}
	cout<<endl;

	//线程安全circular_buffer
	SafeCircularBuffer<double> scbCircularBuffer(3);
	cout << scbCircularBuffer.capacity() <<endl;
	cout << scbCircularBuffer.size() <<endl;

	scbCircularBuffer.push_back(1.0999f);
	scbCircularBuffer.push_back(1.1f);
	scbCircularBuffer.push_back(2.2f);

	cout << scbCircularBuffer.size() <<endl;
	scbCircularBuffer.push_back(3.3f);
	cout << scbCircularBuffer[0]<<endl;
	int k = 0;
}
```

