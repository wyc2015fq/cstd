# Asio基本接口 - xcw0754 - 博客园
# [Asio基本接口](https://www.cnblogs.com/xcw0754/p/8848375.html)
Asio是C++的网络库，有boost和非boost这两种版本，这里涉及的都是非boost的版本。[Asio官方文档](http://think-async.com/Asio/asio-1.10.6/doc/asio/reference.html)
在使用Asio时可以只包含头文件`asio.hpp`，如果知道所用接口具体在哪个头文件中定义，也可以直接包含该头文件。
### buffer
头文件`asio/buffer.hpp`
`asio::buffer`的接口很多，下面这些都是：
```
mutable_buffers_1 buffer(const mutable_buffer & b);
mutable_buffers_1 buffer(const mutable_buffer & b, std::size_t max_size_in_bytes);
const_buffers_1 buffer(const const_buffer & b);
const_buffers_1 buffer(const const_buffer & b, std::size_t max_size_in_bytes);
mutable_buffers_1 buffer(void * data, std::size_t size_in_bytes);
const_buffers_1 buffer(const void * data, std::size_t size_in_bytes);
// 模板
template< typename PodType, std::size_t N> 
mutable_buffers_1 buffer( PodType (&data)[N]);
template<typename PodType, std::size_t N>
mutable_buffers_1 buffer(PodType (&data)[N], std::size_t max_size_in_bytes);
template<typename PodType, std::size_t N>
const_buffers_1 buffer(const PodType (&data)[N]);
template<typename PodType, std::size_t N>
const_buffers_1 buffer(const PodType (&data)[N], std::size_t max_size_in_bytes);
template<typename PodType, std::size_t N>
mutable_buffers_1 buffer(boost::array< PodType, N > & data);
template<typename PodType, std::size_t N>
mutable_buffers_1 buffer(boost::array< PodType, N > & data, std::size_t max_size_in_bytes);
template<typename PodType, std::size_t N>
const_buffers_1 buffer(boost::array< const PodType, N > & data);
template<typename PodType, std::size_t N>
const_buffers_1 buffer(boost::array< const PodType, N > & data, std::size_t max_size_in_bytes);
template<typename PodType, std::size_t N>
const_buffers_1 buffer(const boost::array< PodType, N > & data);
template<typename PodType, std::size_t N>
const_buffers_1 buffer(const boost::array< PodType, N > & data, std::size_t max_size_in_bytes);
template<typename PodType, std::size_t N>
mutable_buffers_1 buffer(std::array< PodType, N > & data);
template<typename PodType, std::size_t N>
mutable_buffers_1 buffer(std::array< PodType, N > & data, std::size_t max_size_in_bytes);
template<typename PodType, std::size_t N>
const_buffers_1 buffer(std::array< const PodType, N > & data);
template<typename PodType, std::size_t N>
const_buffers_1 buffer(std::array< const PodType, N > & data, std::size_t max_size_in_bytes);
template<typename PodType, std::size_t N>
const_buffers_1 buffer(const std::array< PodType, N > & data);
template<typename PodType, std::size_t N>
const_buffers_1 buffer(const std::array< PodType, N > & data, std::size_t max_size_in_bytes);
template<typename PodType, typename Allocator>
mutable_buffers_1 buffer(std::vector< PodType, Allocator > & data);
template<typename PodType, typename Allocator>
mutable_buffers_1 buffer(std::vector< PodType, Allocator > & data, std::size_t max_size_in_bytes);
template<typename PodType, typename Allocator>
const_buffers_1 buffer(const std::vector< PodType, Allocator > & data);
template<typename PodType, typename Allocator>
const_buffers_1 buffer(const std::vector< PodType, Allocator > & data, std::size_t max_size_in_bytes);
template<typename Elem, typename Traits, typename Allocator>
const_buffers_1 buffer(const std::basic_string< Elem, Traits, Allocator > & data);
template<typename Elem, typename Traits, typename Allocator>
const_buffers_1 buffer(const std::basic_string< Elem, Traits, Allocator > & data, std::size_t max_size_in_bytes);
```
一般常用的是这几个：
`mutable_buffers_1 buffer(void * data, std::size_t size_in_bytes);`
一般用带有指定大小的比较方便，这样可以重复利用buffer，不需要去整一个恰好大小的buffer。
官方的例子：
```
char d1[128];
size_t bytes_transferred = sock.receive(asio::buffer(d1));
std::vector<char> d2(128);
bytes_transferred = sock.receive(asio::buffer(d2));
std::array<char, 128> d3;
bytes_transferred = sock.receive(asio::buffer(d3));
boost::array<char, 128> d4;
bytes_transferred = sock.receive(asio::buffer(d4));
```
注意，如果用的是STL容器，如vector，虽然它是可以动态调整大小的，但是buffer不会去调整它，所以传进去的vector的size决定了这个buffer所能容纳的数据量，而不是capacity决定的。buffer的其他接口还有`buffer_size`获取大小、`buffer_cast`类型转换、`buffer_copy`拷贝，这里不谈了。
### write
写的接口比较少，只有如下四个：
```
template<typename AsyncWriteStream, typename ConstBufferSequence, typename WriteHandler>
void-or-deduced async_write(AsyncWriteStream & s, const ConstBufferSequence & buffers, WriteHandler handler);
template<typename AsyncWriteStream, typename ConstBufferSequence, typename CompletionCondition, typename WriteHandler>
void-or-deduced async_write(AsyncWriteStream & s, const ConstBufferSequence & buffers, CompletionCondition completion_condition, WriteHandler handler);
template<typename AsyncWriteStream, typename Allocator, typename WriteHandler>
void-or-deduced async_write(AsyncWriteStream & s, basic_streambuf< Allocator > & b, WriteHandler handler);
template<typename AsyncWriteStream, typename Allocator, typename CompletionCondition, typename WriteHandler>
void-or-deduced async_write(AsyncWriteStream & s, basic_streambuf< Allocator > & b, CompletionCondition completion_condition, WriteHandler handler);
```
一般可以这样用：
```
char data[size] = ...;
asio::async_write(sock, asio::buffer(data, size), handler);
```

