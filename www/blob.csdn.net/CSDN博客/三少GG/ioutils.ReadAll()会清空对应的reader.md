# ioutils.ReadAll()会清空对应的reader - 三少GG - CSDN博客
2015年12月28日 21:19:33[三少GG](https://me.csdn.net/scut1135)阅读数：1801
解决办法：
```cpp
//copy first, or Boby will be empty
	reqBytes, err := ioutil.ReadAll(c.Request.Body)
	if err != nil {
		log.Error("fail to read requset data")
		return 0, err
	}
	log.Infof("%s", reqBytes)
	buf := bytes.NewBuffer(reqBytes)
	c.Request.Body = ioutil.NopCloser(buf)
```
**********************************************************************************
原因分析：
http://stackoverflow.com/questions/30910487/why-an-io-reader-after-read-it-became-empty-in-golang
Reading from a [`bytes.Buffer`](https://golang.org/pkg/bytes/#Buffer) drains
 or consumes the bytes that were read. This means if you try to read again those will not be returned.
[`Buffer.Bytes()`](https://golang.org/pkg/bytes/#Buffer.Bytes) returns
 the unread portion of the buffer so it is the expected result for you to see `0` length
 after everything has been read (this is exactly what [`ioutil.ReadAll()`](https://golang.org/pkg/io/ioutil/#ReadAll) does).
What if you just want to "peek" and not really "read" bytes?
There is no "peek" functionality in `bytes.Buffer`.
 The easiest would be to get the bytes of the buffer, and construct another `bytes.Buffer` from
 it and read from the new buffer.
It could look something like this:
```
func peek(buf *bytes.Buffer, b []byte) (int, error) {
    buf2 := bytes.NewBuffer(buf.Bytes())
    return buf2.Read(b)
}
```
实现代码分析: http://www.cnphp6.com/archives/110001
解决思路来源：
# [Reading body of http.Request without modifying request state?](http://stackoverflow.com/questions/23070876/reading-body-of-http-request-without-modifying-request-state)
http://stackoverflow.com/questions/23070876/reading-body-of-http-request-without-modifying-request-state
