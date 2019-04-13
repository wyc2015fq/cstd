
# C++ 资源管理 —— RAII - Zhang's Wikipedia - CSDN博客


2018年01月29日 23:29:28[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：128


RAII：在构造函数中申请资源，在析构函数中释放资源。
## 1. RAII 自动实现锁资源的释放
```python
void
```
```python
bad() {
    m.
```
```python
lock
```
```python
();
    f();
```
```python
if
```
```python
(COND)
```
```python
return
```
```python
;
    m.unlock();
}
```
显然如果提前返回，会导致死锁。
```python
class SafeMutex {
```
```python
public
```
```python
:
```
```python
SafeMutex
```
```python
(std::mutex& m) : _
```
```python
m
```
```python
(m) {
        m.
```
```python
lock
```
```python
();
    }
    ~SafeMutex() {
        _m.unlock();
    }
```
```python
private
```
```python
:
    std::mutex& _m;
}
```
```python
void
```
```python
good() {
    SafeMutex mutex(m);
    foo();
```
```python
if
```
```python
(COND)
```
```python
return
```
```python
;
```
```python
// 异常发生时，依然会自动调用锁资源的自动释放；
```
```python
}
```

