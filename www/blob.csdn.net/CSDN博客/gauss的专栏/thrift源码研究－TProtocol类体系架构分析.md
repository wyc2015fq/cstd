# thrift源码研究－TProtocol类体系架构分析 - gauss的专栏 - CSDN博客
2015年05月03日 23:33:55[gauss](https://me.csdn.net/mathlmx)阅读数：409
个人分类：[开源库](https://blog.csdn.net/mathlmx/article/category/944352)
转自:http://blog.csdn.net/whycold/article/details/8531561
　这里的协议是指数据格式的打包和解包方式。
　　TProtocol类体系属于thrift框架中的协议层，它提供多种协议，如TBinaryProtocolT、TCompactProtocolT、TJsonPotocolT和TDenseProtocolT等，以支持不同应用场景的数据传输，它的类图(转载）如下：
![](http://pic002.cnblogs.com/images/2012/162856/2012060423574661.png)
**架构分析**
由以上类图可以发现所有的协议类都从TProtocol类直接或间接继承，这为上层提供统一访问接口；每一个协议类都有一个对应的生产对象工厂（协议工厂）。从架构图可以看出类之前的继承关系比较复杂，这是为了提供更好地扩展性，从抽象类TProtocol到具体协议类之间多加了二层继承关系，第一层是TProtocolDefault，它直接继承TProtocol并实现所有虚方法，但都是空实现，第二层是TVirtualProtocol类，它是一个模板类，带有两个模板参数，一个表示数据传输的真正协议，一个是用来继承的，它本身没有对协议具体内容做实现，所以说它是一个虚的协议类。
（1）抽象类TProtocol
一些纯虚函数（未完列出）：
 
 virtual uint32_t writeMessageBegin_virt(const std::string& name,
                                          const TMessageType messageType,
                                          const int32_t seqid) = 0;
  virtual uint32_t writeMessageEnd_virt() = 0;
  virtual uint32_t writeStructBegin_virt(const char* name) = 0;
  virtual uint32_t writeStructEnd_virt() = 0;
  virtual uint32_t writeFieldBegin_virt(const char* name,
                                        const TType fieldType,
                                        const int16_t fieldId) = 0;
一些供用户调用的函数（未完列出）：
uint32_t
 writeMessageBegin(const std::string& name,
                             const TMessageType messageType,
                             const int32_t seqid) {
    T_VIRTUAL_CALL();
    return writeMessageBegin_virt(name, messageType, seqid);
  }
  uint32_t writeMessageEnd() {
    T_VIRTUAL_CALL();
    return writeMessageEnd_virt();
  }
  uint32_t writeStructBegin(const char* name) {
    T_VIRTUAL_CALL();
    return writeStructBegin_virt(name);
  }
  uint32_t writeStructEnd() {
    T_VIRTUAL_CALL();
    return writeStructEnd_virt();
  }
  uint32_t writeFieldBegin(const char* name,
                           const TType fieldType,
                           const int16_t fieldId) {
    T_VIRTUAL_CALL();
    return writeFieldBegin_virt(name, fieldType, fieldId);
  }
可见，它们都调用对应的纯虚函数。
（2）TProtocolDefault类
一些供用户调用的函数实现（未完列出）：
  uint32_t readMessageBegin(std::string& name,
                            TMessageType& messageType,
                            int32_t& seqid) {
    (void) name;
    (void) messageType;
    (void) seqid;
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
                             "this protocol does not support reading (yet).");
  }
  uint32_t readMessageEnd() {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
                             "this protocol does not support reading (yet).");
  }
  uint32_t readStructBegin(std::string& name) {
    (void) name;
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
                             "this protocol does not support reading (yet).");
  }
  uint32_t readStructEnd() {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
                             "this protocol does not support reading (yet).");
  }
  uint32_t readFieldBegin(std::string& name,
                          TType& fieldType,
                          int16_t& fieldId) {
    (void) name;
    (void) fieldType;
    (void) fieldId;
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
                             "this protocol does not support reading (yet).");
  }
可见，它们只是抛出异常，这一来告诉上层用户不可直接使用该协议类，二、它作为TVirtualProtocol模板类的默认参数存在，可以告诉实现具体协议类的人要实现这些函数和避免开发人员在未实现其中某个函数时使用中出现死循环，可见该类在该架构中还是相当重要的。
（3）TVirtualProtocol模板类
template
 <class Protocol_, class Super_=TProtocolDefaults>
class TVirtualProtocol : public Super_ {
 
 virtual uint32_t writeMessageBegin_virt(const std::string& name,
                                          const TMessageType messageType,
                                          const int32_t seqid) {
    return static_cast<Protocol_*>(this)->writeMessageBegin(name, messageType,
                                                            seqid);
  }
  virtual uint32_t writeMessageEnd_virt() {
    return static_cast<Protocol_*>(this)->writeMessageEnd();
  }
  virtual uint32_t writeStructBegin_virt(const char* name) {
    return static_cast<Protocol_*>(this)->writeStructBegin(name);
  }
  virtual uint32_t writeStructEnd_virt() {
    return static_cast<Protocol_*>(this)->writeStructEnd();
  }
...........
}
具体协议类都会继承TVirtualProtocol，并将自己作为第一个参数传给TVirtualProtocol，从上面的虚成员函数实现可以看出：都是调用对应的由具体协议类的非虚函数。
以下以具体协议类TBinaryProtocolT为例：
template
 <class Transport_>
class TBinaryProtocolT
  : public TVirtualProtocol< TBinaryProtocolT<Transport_> > {
...........
}
可见TBinaryProtocolT是模板类，接收Transport层的类。
通过上面的分析，可以知道用户是怎样通过父类TProtocol来使用具体协议类，以writeMessageBegin函数为例，用户调用TProtocol的writeMessageBegin函数，该函数调用子类TVirtualProtocol的writeMessageBegin_virt函数，writeMessageBegin_virt函数又调用具体协议类的writeMessageBegin函数，若具体协议类没有覆盖实现writeMessageBegin函数，则会调用TProtocolDefaults的writeMessageBegin函数（前提是TVirtualProtocol的第二模板参数是TProtocolDefaults），它会抛出异常，故可提醒开发者要覆盖实现writeMessageBegin函数。
由于有四层的继承层次，加上采用模板方式实现，对TProtocol类体系架构提供极大的灵活性。
