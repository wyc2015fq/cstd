# Tensorflow源码解析





# 1 -- 内核架构和源码结构



## 1 主流深度学习框架对比

当今的软件开发基本都是分层化和模块化的，应用层开发会基于框架层。比如开发Linux Driver会基于Linux kernel，开发Android app会基于Android Framework。深度学习也不例外，框架层为上层模型开发提供了强大的多语言接口、稳定的运行时、高效的算子，以及完备的通信层和设备层管理层。因此，各大公司早早的就开始了深度学习框架的研发，以便能占领市场。当前的框架有数十种之多，主流的如下（截止到2018年11月）

![img](https://img.alicdn.com/tfs/TB1cTE.pxjaK1RjSZFAXXbdLFXa-1080-693.png)

显然TensorFlow是独一无二的王者。第二名Keras，它是对TensorFlow或Theano接口的二次封装，严格意义上并不是一个独立的深度学习框架。TensorFlow目前也已经集成了Keras，使得安装了TensorFlow的用户就可以直接使用Keras了。

TensorFlow之所以能够从数十种框架中脱颖而出，主要优点有

1. 出身高贵，是谷歌出品的。但其他很多框架出身也不差，例如PyTorch之于Facebook，MXNET之于Amazon
2. 2015年就开源了，比较早的俘获了一大批开发者。这个确实是tf的一大先发优势，但PyTorch的前身Caffe，以及MXNET开源时间都不晚，而且Caffe流行时间比tf早，后来才被赶超的。更有Theano这样的绝对老前辈。由此可见，软件开源是多么重要。目前流行的深度学习框架也基本都开源了。
3. 支持的开发语言多，支持Python Java Go C++等多种流行语言。相比某些框架，确实是优势很大。相比MXNET则小巫见大巫了。MXNET早期发展的一个主要方向就是前端多语言的支持，连MATLAB R Julia等语言都支持了。
4. 运行效率高。早期的时候，其实tf的运行效率比很多框架都要低一些的。
5. 安装容易，用户上手快，文档齐全，社区活跃。这个是tf的一个较大优势，特别是社区方面，也就是我们常说的生态优势。互联网头部集中效应十分明显，体现在开源软件上也是一样。这也是我认为最大的一个优势。

总结起来，TensorFlow虽然每个方面都不是绝对领先的优势，但贵在每个方面都做的不错，因此最终能够一骑绝尘，独领风骚。

学习Tensorflow框架内核，可以理解前端接口语言的支持，session生命周期，graph的构建、分裂和执行，operation的注册和运行，模块间数据通信，本地运行和分布式运行模式，以及CPU GPU TPU等异构设备的封装支持等。学习这些，对于模型的压缩 加速 优化等都是大有裨益的。



## 2 TensorFlow系统架构

TensorFlow设计十分精巧，基于分层和模块化的设计思想进行开发的。框架如下图

![img](https://img.alicdn.com/tfs/TB19glXpG6qK1RjSZFmXXX0PFXa-339-302.png)

整个框架以C API为界，分为前端和后端两大部分。

1. 前端：提供编程模型，多语言的接口支持，比如Python Java C++等。通过C API建立前后端的连接，后面详细讲解。
2. 后端：提供运行环境，完成计算图的执行。进一步分为4层
   1. 运行时：分为分布式运行时和本地运行时，负责计算图的接收，构造，编排等。
   2. 计算层：提供各op算子的内核实现，例如conv2d, relu等
   3. 通信层：实现组件间数据通信，基于GRPC和RDMA两种通信方式
   4. 设备层：提供多种异构设备的支持，如CPU GPU TPU FPGA等

### 模型构造和执行流程

TensorFlow的一大特点是，图的构造和执行相分离。用户添加完算子，构建好整图后，才开始进行训练和执行，也就是图的执行。大体流程如下

1. 图构建：用户在client中基于TensorFlow的多语言编程接口，添加算子，完成计算图的构造。

2. 图传递：client开启session，通过它建立和master之间的连接。执行session.run()时，将构造好的graph序列化为graphDef后，以protobuf的格式传递给master。

3. 图剪枝：master根据session.run()传递的fetches和feeds列表，反向遍历全图full graph，实施剪枝，得到最小依赖子图

4. 图分裂：master将最小子图分裂为多个Graph Partition，并注册到多个worker上。一个worker对应一个Graph Partition。

5. 图二次分裂：worker根据当前可用硬件资源，如CPU GPU，将Graph Partition按照op算子设备约束规范（例如tf.device(’/cpu:0’)，二次分裂到不同设备上。每个计算设备对应一个Graph Partition。

6. 图运行：对于每一个计算设备，worker依照op在kernel中的实现，完成op的运算。设备间数据通信可以使用send/recv节点，而worker间通信，则使用GRPC或RDMA协议。

   ![img](https://img.alicdn.com/tfs/TB1NMs.pCrqK1RjSZK9XXXyypXa-300-103.png)

## 3 前端多语言实现 - swig包装器

TensorFlow提供了很多种语言的前端接口，使得用户可以通过多种语言来完成模型的训练和推断。其中Python支持得最好。这也是TensorFlow之所以受欢迎的一大原因。前端多语言是怎么实现的呢？这要归功于swig包装器。

swig是个帮助使用C或者C++编写的软件能与其它各种高级编程语言进行嵌入联接的开发工具。在TensorFlow使用bazel编译时，swig会生成两个wrapper文件

1. pywrap_tensorflow_internal.py：对接上层Python调用
2. pywrap_tensorflow_internal.cc：对接底层C API调用。

pywrap_tensorflow_internal.py 模块被导入时，会加载_pywrap_tensorflow_internal.so动态链接库，它里面包含了所有运行时接口的符号。而pywrap_tensorflow_internal.cc中，则注册了一个函数符号表，实现Python接口和C接口的映射。运行时，就可以通过映射表，找到Python接口在C层的实现了。

![img](https://img.alicdn.com/tfs/TB1KiVFpH2pK1RjSZFsXXaNlXXa-1340-1440.png)

## 4 tensorflow 源码结构

TensorFlow源码基本也是按照框架分层来组织文件的。如下

![img](https://img.alicdn.com/tfs/TB1gbpnpQPoK1RjSZKbXXX1IXXa-1442-996.png)

其中core为tf的核心，它的源码结构如下

![img](https://img.alicdn.com/tfs/TB1mM4spFzqK1RjSZFoXXbfcXXa-1150-730.png)

## 5 总结

TensorFlow框架设计精巧，代码量也很大，我们可以从以下部分逐步学习

1. TensorFlow内核架构和源码结构。先从全局上对框架进行理解。
2. 前后端连接的桥梁–Session，重点理解session的生命周期，并通过相关源码可以加深理解Python前端如何调用底层C实现。
3. TensorFlow核心对象—Graph。图graph是TensorFlow最核心的对象，基本都是围绕着它来进行的。graph的节点为算子operation，边为数据tensor。
4. TensorFlow图的节点 – Operation。operation是图graph的节点，承载了计算算子。
5. TensorFlow图的边 – Tensor。Tensor是图graph的边，承载了计算的数据。
6. TensorFlow本地运行时。
7. TensorFlow分布式运行时。和本地运行时有一些共用的接口，但区别也很大。
8. TensorFlow设备层。主要了解设备层的定义规范，以及实现。
9. TensorFlow队列和并行运算。
10. TensorFlow断点检查checkpoint，模型保存Saver，以及可视化tensorboard。这三个为TensorFlow主要的工具。









# 2 -- 前后端连接的桥梁 - Session



## 1 Session概述

Session是TensorFlow前后端连接的桥梁。用户利用session使得client能够与master的执行引擎建立连接，并通过session.run()来触发一次计算。它建立了一套上下文环境，封装了operation计算以及tensor求值的环境。

session创建时，系统会分配一些资源，比如graph引用、要连接的计算引擎的名称等。故计算完毕后，需要使用session.close()关闭session，避免引起内存泄漏，特别是graph无法释放的问题。可以显式调用session.close(),或利用with上下文管理器，或者直接使用InteractiveSession。

session之间采用共享graph的方式来提高运行效率。一个session只能运行一个graph实例，但一个graph可以运行在多个session中。一般情况下，创建session时如果不指定Graph实例，则会使用系统默认Graph。常见情况下，我们都是使用一个graph，即默认graph。当session创建时，不会重新创建graph实例，而是默认graph引用计数加1。当session close时，引用计数减1。只有引用计数为0时，graph才会被回收。这种graph共享的方式，大大减少了graph创建和回收的资源消耗，优化了TensorFlow运行效率。



## 2 默认session

op运算和tensor求值时，如果没有指定运行在哪个session中，则会运行在默认session中。通过session.as_default()可以将自己设置为默认session。但个人建议最好还是通过session.run(operator)和session.run(tensor)来进行op运算和tensor求值。

### operation.run()

operation.run()等价于tf.get_default_session().run(operation)

```python
@tf_export("Operation")
class Operation(object):
    # 通过operation.run()调用，进行operation计算
    def run(self, feed_dict=None, session=None):
        _run_using_default_session(self, feed_dict, self.graph, session)
        
    def _run_using_default_session(operation, feed_dict, graph, session=None):
          # 没有指定session，则获取默认session
          if session is None:
    		session = get_default_session()
            
		 # 最终还是通过session.run()进行运行的。tf中任何运算，都是通过session来run的。
          # 通过session来建立client和master的连接，并将graph发送给master，master再进行执行
  		 session.run(operation, feed_dict)
1234567891011121314
```

### tensor.eval()

tensor.eval()等价于tf.get_default_session().run(tensor), 如下

```python
@tf_export("Tensor")
class Tensor(_TensorLike):
    # 通过tensor.eval()调用，进行tensor运算
	def eval(self, feed_dict=None, session=None):
		return _eval_using_default_session(self, feed_dict, self.graph, session)
		
	def _eval_using_default_session(tensors, feed_dict, graph, session=None):
           # 如果没有指定session，则获取默认session
		  if session is None:
    		session = get_default_session()
  		  return session.run(tensors, feed_dict)
1234567891011
```

### 默认session的管理

tf通过运行时维护的session本地线程栈，来管理默认session。故不同的线程会有不同的默认session，默认session是线程作用域的。

```python
# session栈
_default_session_stack = _DefaultStack()

# 获取默认session的接口
@tf_export("get_default_session")
def get_default_session():
  return _default_session_stack.get_default()

# _DefaultStack默认session栈是线程相关的
class _DefaultStack(threading.local):
      # 默认session栈的创建，其实就是一个list
      def __init__(self):
    	super(_DefaultStack, self).__init__()
    	self._enforce_nesting = True
    	self.stack = []  
        
      # 获取默认session
      def get_default(self):
    	return self.stack[-1] if len(self.stack) >= 1 else None
12345678910111213141516171819
```

## 3 前端Session类型

### session类图

会话Session的UML类图如下

![img](https://img.alicdn.com/tfs/TB1gbOcpMTqK1RjSZPhXXXfOFXa-1210-452.png)

分为两种类型，普通Session和交互式InteractiveSession。InteractiveSession和Session基本相同，区别在于

1. InteractiveSession创建后，会将自己替换为默认session。使得之后operation.run()和tensor.eval()的执行通过这个默认session来进行。特别适合Python交互式环境。
2. InteractiveSession自带with上下文管理器。它在创建时和关闭时会调用上下文管理器的enter和exit方法，从而进行资源的申请和释放，避免内存泄漏问题。这同样很适合Python交互式环境。

Session和InteractiveSession的代码逻辑不多，主要逻辑均在其父类BaseSession中。主要代码如下

```python
@tf_export('Session')
class Session(BaseSession):
    def __init__(self, target='', graph=None, config=None):
        # session创建的主要逻辑都在其父类BaseSession中
        super(Session, self).__init__(target, graph, config=config)
    	self._default_graph_context_manager = None
    	self._default_session_context_manager = None
1234567
@tf_export('InteractiveSession')
class InteractiveSession(BaseSession):
    def __init__(self, target='', graph=None, config=None):
        self._explicitly_closed = False
        
        # 将自己设置为default session
    	self._default_session = self.as_default()
    	self._default_session.enforce_nesting = False
        
        # 自动调用上下文管理器的__enter__()方法
    	self._default_session.__enter__()
    	self._explicit_graph = graph
       
    def close(self):
    	super(InteractiveSession, self).close()
    	## 省略无关代码
        ## 自动调用上下文管理器的__exit__()方法，避免内存泄漏
        self._default_session.__exit__(None, None, None)
        self._default_session = None
12345678910111213141516171819
```

### BaseSession

BaseSession基本包含了所有的会话实现逻辑。包括会话的整个生命周期，也就是创建 执行 关闭和销毁四个阶段。生命周期后面详细分析。BaseSession包含的主要成员变量有graph引用，序列化的graph_def, 要连接的tf引擎target，session配置信息config等。



## 4 后端Session类型

在后端master中，根据前端client调用tf.Session(target=’’, graph=None, config=None)时指定的target，来创建不同的Session。target为要连接的tf后端执行引擎，默认为空字符串。Session创建采用了抽象工厂模式，如果为空字符串，则创建本地DirectSession，如果以grpc://开头，则创建分布式GrpcSession。类图如下

![img](https://img.alicdn.com/tfs/TB1wzqEpOrpK1RjSZFhXXXSdXXa-1262-538.png)

DirectSession只能利用本地设备，将任务创建到本地的CPU GPU上。而GrpcSession则可以利用远端分布式设备，将任务创建到其他机器的CPU GPU上，然后通过grpc协议进行通信。grpc协议是谷歌发明并开源的远程通信协议。



## 5 Session生命周期

Session作为前后端连接的桥梁，以及上下文运行环境，其生命周期尤其关键。大致分为4个阶段

1. 创建：通过tf.Session()创建session实例，进行系统资源分配，特别是graph引用计数加1
2. 运行：通过session.run()触发计算的执行，client会将整图graph传递给master，由master进行执行
3. 关闭：通过session.close()来关闭，会进行系统资源的回收，特别是graph引用计数减1.
4. 销毁：Python垃圾回收器进行GC时，调用`session.__del__()`进行回收。

生命周期方法入口基本都在前端Python的BaseSession中，它会通过swig自动生成的函数符号映射关系，调用C层的实现。

### 5.1 创建

先从BaseSession类的init方法看起，只保留了主要代码。

```python
def __init__(self, target='', graph=None, config=None):
    # graph表示构建的图。TensorFlow的一个session会对应一个图。这个图包含了所有涉及到的算子
    # graph如果没有设置（通常都不会设置），则使用默认graph
    if graph is None:
      self._graph = ops.get_default_graph()
    else:
      self._graph = graph

    self._opened = False
    self._closed = False

    self._current_version = 0
    self._extend_lock = threading.Lock()

    # target为要连接的tf执行引擎
    if target is not None:
      self._target = compat.as_bytes(target)
    else:
      self._target = None

    self._delete_lock = threading.Lock()
    self._dead_handles = []

    # config为session的配置信息
    if config is not None:
      self._config = config
      self._add_shapes = config.graph_options.infer_shapes
    else:
      self._config = None
      self._add_shapes = False

    self._created_with_new_api = ops._USE_C_API

    # 调用C层来创建session
    self._session = None
    opts = tf_session.TF_NewSessionOptions(target=self._target, config=config)
    self._session = tf_session.TF_NewSession(self._graph._c_graph, opts, status)
        
1234567891011121314151617181920212223242526272829303132333435363738
```

BaseSession先进行成员变量的赋值，然后调用TF_NewSession来创建session。TF_NewSession()方法由swig自动生成，在bazel-bin/tensorflow/python/pywrap_tensorflow_internal.py中

```python
def TF_NewSession(graph, opts, status):
    return _pywrap_tensorflow_internal.TF_NewSession(graph, opts, status)
12
```

_pywrap_tensorflow_internal包含了C层函数的符号表。在swig模块import时，会加载pywrap_tensorflow_internal.so动态链接库，从而得到符号表。在pywrap_tensorflow_internal.cc中，注册了供Python调用的函数的符号表，从而实现Python到C的函数映射和调用。

```c
// c++函数调用的符号表，Python通过它可以调用到C层代码。符号表和动态链接库由swig自动生成
static PyMethodDef SwigMethods[] = {
      // .. 省略其他函数定义
	 // TF_NewSession的符号表，通过这个映射，Python中就可以调用到C层代码了。
	 { (char *)"TF_NewSession", _wrap_TF_NewSession, METH_VARARGS, NULL},
     // ... 省略其他函数定义
}
1234567
```

最终调用到c_api.c中的TF_NewSession()

```c
// TF_NewSession创建session的新实现，在C层后端代码中
TF_Session* TF_NewSession(TF_Graph* graph, const TF_SessionOptions* opt,
                          TF_Status* status) {
  Session* session;
  // 创建session
  status->status = NewSession(opt->options, &session);
  if (status->status.ok()) {
    TF_Session* new_session = new TF_Session(session, graph);
    if (graph != nullptr) {
      // 采用了引用计数方式，多个session共享一个图实例，效率更高。
      // session创建时，引用计数加1。session close时引用计数减1。引用计数为0时，graph才会被回收。
      mutex_lock l(graph->mu);
      graph->sessions[new_session] = Status::OK();
    }
    return new_session;
  } else {
    DCHECK_EQ(nullptr, session);
    return nullptr;
  }
}
1234567891011121314151617181920
```

session创建时，并创建graph，而是采用共享方式，只是引用计数加1了。这种方式减少了session创建和关闭时的资源消耗，提高了运行效率。NewSession()根据前端传递的target，使用sessionFactory创建对应的TensorFlow::Session实例。

```c
Status NewSession(const SessionOptions& options, Session** out_session) {
  SessionFactory* factory;
  const Status s = SessionFactory::GetFactory(options, &factory);

  // 通过sessionFactory创建多态的Session。本地session为DirectSession，分布式为GRPCSession
  *out_session = factory->NewSession(options);
  if (!*out_session) {
    return errors::Internal("Failed to create session.");
  }
  return Status::OK();
}
1234567891011
```

创建session采用了抽象工厂模式。根据client传递的target，来创建不同的session。如果target为空字符串，则创建本地DirectSession。如果以grpc://开头，则创建分布式GrpcSession。TensorFlow包含本地运行时和分布式运行时两种运行模式。

下面来看DirectSessionFactory的NewSession()方法

```c
class DirectSessionFactory : public SessionFactory {
 public:
  Session* NewSession(const SessionOptions& options) override {
    std::vector<Device*> devices;
      
    // job在本地执行
    const Status s = DeviceFactory::AddDevices(
        options, "/job:localhost/replica:0/task:0", &devices);
    if (!s.ok()) {
      LOG(ERROR) << s;
      return nullptr;
    }

    DirectSession* session =
        new DirectSession(options, new DeviceMgr(devices), this);
    {
      mutex_lock l(sessions_lock_);
      sessions_.push_back(session);
    }
    return session;
  }
123456789101112131415161718192021
```

GrpcSessionFactory的NewSession()方法就不详细分析了，它会将job任务创建在分布式设备上，各job通过grpc协议通信。

### 5.2 运行

通过session.run()可以启动graph的执行。入口在BaseSession的run()方法中, 同样只列出关键代码

```python
class BaseSession(SessionInterface):
    def run(self, fetches, feed_dict=None, options=None, run_metadata=None):
        # fetches可以为单个变量，或者数组，或者元组。它是图的一部分，可以是操作operation，也可以是数据tensor，或者他们的名字String
        # feed_dict为对应placeholder的实际训练数据，它的类型为字典
        result = self._run(None, fetches, feed_dict, options_ptr,run_metadata_ptr)
        return result
    
    def _run(self, handle, fetches, feed_dict, options, run_metadata):
    	# 创建fetch处理器fetch_handler
        fetch_handler = _FetchHandler(
            self._graph, fetches, feed_dict_tensor, feed_handles=feed_handles)

        # 经过不同类型的fetch_handler处理，得到最终的fetches和targets
        # targets为要执行的operation，fetches为要执行的tensor
        _ = self._update_with_movers(feed_dict_tensor, feed_map)
        final_fetches = fetch_handler.fetches()
        final_targets = fetch_handler.targets()

        # 开始运行
        if final_fetches or final_targets or (handle and feed_dict_tensor):
          results = self._do_run(handle, final_targets, final_fetches,
                                 feed_dict_tensor, options, run_metadata)
        else:
          results = []

        # 输出结果到results中
        return fetch_handler.build_results(self, results)

    def _do_run(self, handle, target_list, fetch_list, feed_dict, options, run_metadata):
        # 将要运行的operation添加到graph中
        self._extend_graph()
        
        # 执行一次运行run，会调用底层C来实现
        return tf_session.TF_SessionPRunSetup_wrapper(
              session, feed_list, fetch_list, target_list, status)
      
    # 将要运行的operation添加到graph中
    def _extend_graph(self):
        with self._extend_lock:
          if self._graph.version > self._current_version:
            # 生成graph_def对象，它是graph的序列化表示
            graph_def, self._current_version = self._graph._as_graph_def(
                from_version=self._current_version, add_shapes=self._add_shapes)

            # 通过TF_ExtendGraph将序列化后的graph，也就是graph_def传递给后端
            with errors.raise_exception_on_not_ok_status() as status:
              tf_session.TF_ExtendGraph(self._session,
                                        graph_def.SerializeToString(), status)
            self._opened = True
12345678910111213141516171819202122232425262728293031323334353637383940414243444546474849
```

逻辑还是十分复杂的，主要有一下几步

1. 入参处理，创建fetch处理器fetch_handler，得到最终要执行的operation和tensor
2. 对graph进行序列化，生成graph_def对象
3. 将序列化后的grap_def对象传递给后端master。
4. 通过后端master来run。

我们分别来看extend和run。

#### 5.2.1 extend添加节点到graph中

TF_ExtendGraph()会调用到c_api中，这个逻辑同样通过swig工具自动生成。下面看c_api.cc中的TF_ExtendGraph()方法

```c
// 增加节点到graph中，proto为序列化后的graph
void TF_ExtendGraph(TF_DeprecatedSession* s, const void* proto,
                    size_t proto_len, TF_Status* status) {
  GraphDef g;
  // 先将proto反序列化，得到client传递的graph，放入g中
  if (!tensorflow::ParseProtoUnlimited(&g, proto, proto_len)) {
    status->status = InvalidArgument("Invalid GraphDef");
    return;
  }

  // 再调用session的extend方法。根据创建的不同session类型，多态调用不同方法。
  status->status = s->session->Extend(g);
}
12345678910111213
```

后端系统根据生成的Session类型，多态的调用Extend方法。如果是本地session，则调用DirectSession的Extend()方法。如果是分布式session，则调用GrpcSession的相关方法。下面来看GrpcSession的Extend方法。

```c
Status GrpcSession::Extend(const GraphDef& graph) {
  CallOptions call_options;
  call_options.SetTimeout(options_.config.operation_timeout_in_ms());
  return ExtendImpl(&call_options, graph);
}

Status GrpcSession::ExtendImpl(CallOptions* call_options,
                               const GraphDef& graph) {
  bool handle_is_empty;
  {
    mutex_lock l(mu_);
    handle_is_empty = handle_.empty();
  }
  if (handle_is_empty) {
    // 如果graph句柄为空，则表明graph还没有创建好，此时extend就等同于create
    return Create(graph);
  }
  mutex_lock l(mu_);
  ExtendSessionRequest req;
  req.set_session_handle(handle_);
  *req.mutable_graph_def() = graph;
  req.set_current_graph_version(current_graph_version_);
  ExtendSessionResponse resp;
    
  // 调用底层实现，来添加节点到graph中
  Status s = master_->ExtendSession(call_options, &req, &resp);
  if (s.ok()) {
    current_graph_version_ = resp.new_graph_version();
  }
  return s;
}

1234567891011121314151617181920212223242526272829303132
```

Extend()方法中要注意的一点是，如果是首次执行Extend(), 则要先调用Create()方法进行graph的注册。否则才是执行添加节点到graph中。

#### 5.2.2 run执行图的计算

同样，Python通过swig自动生成的代码，来实现对C API的调用。C层实现在c_api.cc的TF_Run()中。

```c
// session.run()的C层实现
void TF_Run(TF_DeprecatedSession* s, const TF_Buffer* run_options,
            // Input tensors，输入的数据tensor
            const char** c_input_names, TF_Tensor** c_inputs, int ninputs,
            // Output tensors，运行计算后输出的数据tensor
            const char** c_output_names, TF_Tensor** c_outputs, int noutputs,
            // Target nodes，要运行的节点
            const char** c_target_oper_names, int ntargets,
            TF_Buffer* run_metadata, TF_Status* status) {
  // 省略一段代码
  TF_Run_Helper(s->session, nullptr, run_options, input_pairs, output_names,
                c_outputs, target_oper_names, run_metadata, status);
}

// 真正的实现了session.run()
static void TF_Run_Helper() {
    RunMetadata run_metadata_proto;
    // 调用不同的session实现类的run方法，来执行
    result = session->Run(run_options_proto, input_pairs, output_tensor_names,
                          target_oper_names, &outputs, &run_metadata_proto);
    // 省略代码
}
12345678910111213141516171819202122
```

最终会调用创建的session来执行run方法。DirectSession和GrpcSession的Run()方法会有所不同。后面很复杂，就不接着分析了。

### 5.3 关闭session

通过session.close()来关闭session，释放相关资源，防止内存泄漏。

```python
class BaseSession(SessionInterface):
	def close(self):
        tf_session.TF_CloseSession(self._session, status)
123
```

会调用到C API的TF_CloseSession()方法。

```c
void TF_CloseSession(TF_Session* s, TF_Status* status) {
  status->status = s->session->Close();
}
123
```

最终根据创建的session，多态的调用其Close()方法。同样分为DirectSession和GrpcSession两种。

```c
::tensorflow::Status DirectSession::Close() {
    cancellation_manager_->StartCancel();
  {
    mutex_lock l(closed_lock_);
    if (closed_) return ::tensorflow::Status::OK();
    closed_ = true;
  }
    
  // 注销session
  if (factory_ != nullptr) factory_->Deregister(this);
  return ::tensorflow::Status::OK();
}
123456789101112
```

DirectSessionFactory中的Deregister()方法如下

```c
void Deregister(const DirectSession* session) {
    mutex_lock l(sessions_lock_);
    // 释放相关资源
    sessions_.erase(std::remove(sessions_.begin(), sessions_.end(), session),
                    sessions_.end());
  }
123456
```

### 5.4 销毁session

session的销毁是由Python的GC自动执行的。python通过引用计数方法来判断是否回收对象。当对象的引用计数为0，且虚拟机触发了GC时，会调用对象的`__del__()`方法来销毁对象。引用计数法有个很致命的问题，就是无法解决循环引用问题，故会存在内存泄漏。Java虚拟机采用了调用链分析的方式来决定哪些对象会被回收。

```python
class BaseSession(SessionInterface):  
  def __del__(self):
    # 先close，防止用户没有调用close()
    try:
      self.close()
	
    # 再调用c api的TF_DeleteSession来销毁session
    if self._session is not None:
      try:
        status = c_api_util.ScopedTFStatus()
        if self._created_with_new_api:
          tf_session.TF_DeleteSession(self._session, status)
123456789101112
```

c_api.cc中的相关逻辑如下

```c
void TF_DeleteSession(TF_Session* s, TF_Status* status) {
  status->status = Status::OK();
  TF_Graph* const graph = s->graph;
  if (graph != nullptr) {
    graph->mu.lock();
    graph->sessions.erase(s);
      
    // 如果graph的引用计数为0，也就是graph没有被任何session持有，则考虑销毁graph对象
    const bool del = graph->delete_requested && graph->sessions.empty();
    graph->mu.unlock();
    
    // 销毁graph对象
    if (del) delete graph;
  }
    
  // 销毁session和TF_Session 
  delete s->session;
  delete s;
}
12345678910111213141516171819
```

TF_DeleteSession()会判断graph的引用计数是否为0，如果为0，则会销毁graph。然后销毁session和TF_Session对象。通过Session实现类的析构函数，来销毁session，释放线程池Executor，资源管理器ResourceManager等资源。

```c
DirectSession::~DirectSession() {
  for (auto& it : partial_runs_) {
    it.second.reset(nullptr);
  }
  // 释放线程池Executor
  for (auto& it : executors_) {
    it.second.reset();
  }

  
  for (auto d : device_mgr_->ListDevices()) {
    d->op_segment()->RemoveHold(session_handle_);
  }

  // 释放ResourceManager
  for (auto d : device_mgr_->ListDevices()) {
    d->ClearResourceMgr();
  }
 
  // 释放CancellationManager实例
  functions_.clear();
  delete cancellation_manager_;

  // 释放ThreadPool 
  for (const auto& p_and_owned : thread_pools_) {
    if (p_and_owned.second) delete p_and_owned.first;
  }

  execution_state_.reset(nullptr);
  flib_def_.reset(nullptr);
}
```

## 6 总结

Session是TensorFlow的client和master连接的桥梁，client任何运算也是通过session来run。它是client端最重要的对象。在Python层和C++层，均有不同的session实现。session生命周期会经历四个阶段，create run close和del。四个阶段均由Python前端开始，最终调用到C层后端实现。由此也可以看到，TensorFlow框架的前后端分离和模块化设计是多么的精巧。















# 3 -- TensorFlow核心对象 - Graph





# 1 Graph概述

计算图Graph是TensorFlow的核心对象，TensorFlow的运行流程基本都是围绕它进行的。包括图的构建、传递、剪枝、按worker分裂、按设备二次分裂、执行、注销等。因此理解计算图Graph对掌握TensorFlow运行尤为关键。



# 2 默认Graph

### 默认图替换

之前讲解Session的时候就说过，一个Session只能run一个Graph，但一个Graph可以运行在多个Session中。常见情况是，session会运行全局唯一的隐式的默认的Graph，operation也是注册到这个Graph中。

也可以显示创建Graph，并调用as_default()使他替换默认Graph。在该上下文管理器中创建的op都会注册到这个graph中。退出上下文管理器后，则恢复原来的默认graph。一般情况下，我们不用显式创建Graph，使用系统创建的那个默认Graph即可。

```python
print tf.get_default_graph()

with tf.Graph().as_default() as g:
    print tf.get_default_graph() is g
    print tf.get_default_graph()

print tf.get_default_graph()
1234567
```

输出如下

```
<tensorflow.python.framework.ops.Graph object at 0x106329fd0>
True
<tensorflow.python.framework.ops.Graph object at 0x18205cc0d0>
<tensorflow.python.framework.ops.Graph object at 0x10d025fd0>
1234
```

由此可见，在上下文管理器中，当前线程的默认图被替换了，而退出上下文管理后，则恢复为了原来的默认图。

### 默认图管理

默认graph和默认session一样，也是线程作用域的。当前线程中，永远都有且仅有一个graph为默认图。TensorFlow同样通过栈来管理线程的默认graph。

```python
@tf_export("Graph")
class Graph(object):
    # 替换线程默认图
    def as_default(self):
        return _default_graph_stack.get_controller(self)
    
    # 栈式管理，push pop
    @tf_contextlib.contextmanager
    def get_controller(self, default):
        try:
          context.context_stack.push(default.building_function, default.as_default)
        finally:
          context.context_stack.pop()
12345678910111213
```

替换默认图采用了堆栈的管理方式，通过push pop操作进行管理。获取默认图的操作如下，通过默认graph栈_default_graph_stack来获取。

```python
@tf_export("get_default_graph")
def get_default_graph():
  return _default_graph_stack.get_default()
123
```

下面来看_default_graph_stack的创建

```python
_default_graph_stack = _DefaultGraphStack()
class _DefaultGraphStack(_DefaultStack):  
  def __init__(self):
    # 调用父类来创建
    super(_DefaultGraphStack, self).__init__()
    self._global_default_graph = None
    
class _DefaultStack(threading.local):
  def __init__(self):
    super(_DefaultStack, self).__init__()
    self._enforce_nesting = True
    # 和默认session栈一样，本质上也是一个list
    self.stack = []
12345678910111213
```

_default_graph_stack的创建如上所示，最终和默认session栈一样，本质上也是一个list。



# 3 前端Graph数据结构

### Graph数据结构

理解一个对象，先从它的数据结构开始。我们先来看Python前端中，Graph的数据结构。Graph主要的成员变量是Operation和Tensor。Operation是Graph的节点，它代表了运算算子。Tensor是Graph的边，它代表了运算数据。

```python
@tf_export("Graph")
class Graph(object):
    def __init__(self):
   	    # 加线程锁，使得注册op时，不会有其他线程注册op到graph中，从而保证共享graph是线程安全的
        self._lock = threading.Lock()
        
        # op相关数据。
        # 为graph的每个op分配一个id，通过id可以快速索引到相关op。故创建了_nodes_by_id字典
        self._nodes_by_id = dict()  # GUARDED_BY(self._lock)
        self._next_id_counter = 0  # GUARDED_BY(self._lock)
        # 同时也可以通过name来快速索引op，故创建了_nodes_by_name字典
        self._nodes_by_name = dict()  # GUARDED_BY(self._lock)
        self._version = 0  # GUARDED_BY(self._lock)
        
        # tensor相关数据。
        # 处理tensor的placeholder
        self._handle_feeders = {}
        # 处理tensor的read操作
        self._handle_readers = {}
        # 处理tensor的move操作
        self._handle_movers = {}
        # 处理tensor的delete操作
        self._handle_deleters = {}
1234567891011121314151617181920212223
```

下面看graph如何添加op的，以及保证线程安全的。

```python
  def _add_op(self, op):
    # graph被设置为final后，就是只读的了，不能添加op了。
    self._check_not_finalized()
    
    # 保证共享graph的线程安全
    with self._lock:
      # 将op以id和name分别构建字典，添加到_nodes_by_id和_nodes_by_name字典中，方便后续快速索引
      self._nodes_by_id[op._id] = op
      self._nodes_by_name[op.name] = op
      self._version = max(self._version, op._id)
12345678910
```

### GraphKeys 图分组

每个Operation节点都有一个特定的标签，从而实现节点的分类。相同标签的节点归为一类，放到同一个Collection中。标签是一个唯一的GraphKey，GraphKey被定义在类GraphKeys中，如下

```python
@tf_export("GraphKeys")
class GraphKeys(object):
    GLOBAL_VARIABLES = "variables"
    QUEUE_RUNNERS = "queue_runners"
    SAVERS = "savers"
    WEIGHTS = "weights"
    BIASES = "biases"
    ACTIVATIONS = "activations"
    UPDATE_OPS = "update_ops"
    LOSSES = "losses"
    TRAIN_OP = "train_op"
    # 省略其他
123456789101112
```

### name_scope 节点命名空间

使用name_scope对graph中的节点进行层次化管理，上下层之间通过斜杠分隔。

```python
# graph节点命名空间
g = tf.get_default_graph()
with g.name_scope("scope1"):
    c = tf.constant("hello, world", name="c")
    print c.op.name

    with g.name_scope("scope2"):
        c = tf.constant("hello, world", name="c")
        print c.op.name

12345678910
```

输出如下

```python
scope1/c
scope1/scope2/c  # 内层的scope会继承外层的，类似于栈，形成层次化管理
12
```

# 4 后端Graph数据结构

### Graph

先来看graph.h文件中的Graph类的定义，只看关键代码

```c
 class Graph {
     private:
      // 所有已知的op计算函数的注册表
      FunctionLibraryDefinition ops_;

      // GraphDef版本号
      const std::unique_ptr<VersionDef> versions_;

      // 节点node列表，通过id来访问
      std::vector<Node*> nodes_;

      // node个数
      int64 num_nodes_ = 0;

      // 边edge列表，通过id来访问
      std::vector<Edge*> edges_;

      // graph中非空edge的数目
      int num_edges_ = 0;

      // 已分配了内存，但还没使用的node和edge
      std::vector<Node*> free_nodes_;
      std::vector<Edge*> free_edges_;
 }
123456789101112131415161718192021222324
```

后端中的Graph主要成员也是节点node和边edge。节点node为计算算子Operation，边为算子所需要的数据，或者代表节点间的依赖关系。这一点和Python中的定义相似。边Edge的持有它的源节点和目标节点的指针，从而将两个节点连接起来。下面看Edge类的定义。

### Edge

```c
class Edge {
     private:
      Edge() {}

      friend class EdgeSetTest;
      friend class Graph;
      // 源节点, 边的数据就来源于源节点的计算。源节点是边的生产者
      Node* src_;

      // 目标节点，边的数据提供给目标节点进行计算。目标节点是边的消费者
      Node* dst_;

      // 边id，也就是边的标识符
      int id_;

      // 表示当前边为源节点的第src_output_条边。源节点可能会有多条输出边
      int src_output_;

      // 表示当前边为目标节点的第dst_input_条边。目标节点可能会有多条输入边。
      int dst_input_;
};
123456789101112131415161718192021
```

Edge既可以承载tensor数据，提供给节点Operation进行运算，也可以用来表示节点之间有依赖关系。对于表示节点依赖的边，其`src_output_, dst_input_`均为-1，此时边不承载任何数据。

下面来看Node类的定义。

### Node

```c
class Node {
 public:
    // NodeDef,节点算子Operation的信息，比如op分配到哪个设备上了，op的名字等，运行时有可能变化。
  	const NodeDef& def() const;
    
    // OpDef, 节点算子Operation的元数据，不会变的。比如Operation的入参列表，出参列表等
  	const OpDef& op_def() const;
 private:
  	// 输入边，传递数据给节点。可能有多条
  	EdgeSet in_edges_;

  	// 输出边，节点计算后得到的数据。可能有多条
  	EdgeSet out_edges_;
}
1234567891011121314
```

节点Node中包含的主要数据有输入边和输出边的集合，从而能够由Node找到跟他关联的所有边。Node中还包含NodeDef和OpDef两个成员。NodeDef表示节点算子的信息，运行时可能会变，创建Node时会new一个NodeDef对象。OpDef表示节点算子的元信息，运行时不会变，创建Node时不需要new OpDef，只需要从OpDef仓库中取出即可。因为元信息是确定的，比如Operation的入参个数等。

由Node和Edge，即可以组成图Graph，通过任何节点和任何边，都可以遍历完整图。Graph执行计算时，按照拓扑结构，依次执行每个Node的op计算，最终即可得到输出结果。入度为0的节点，也就是依赖数据已经准备好的节点，可以并发执行，从而提高运行效率。

系统中存在默认的Graph，初始化Graph时，会添加一个Source节点和Sink节点。Source表示Graph的起始节点，Sink为终止节点。Source的id为0，Sink的id为1，其他节点id均大于1.



# 5 Graph运行时生命周期

Graph是TensorFlow的核心对象，TensorFlow的运行均是围绕Graph进行的。运行时Graph大致经过了以下阶段

1. 图构建：client端用户将创建的节点注册到Graph中，一般不需要显示创建Graph，使用系统创建的默认的即可。
2. 图发送：client通过session.run()执行运行时，将构建好的整图序列化为GraphDef后，传递给master
3. 图剪枝：master先反序列化拿到Graph，然后根据session.run()传递的fetches和feeds列表，反向遍历全图full graph，实施剪枝，得到最小依赖子图。
4. 图分裂：master将最小子图分裂为多个Graph Partition，并注册到多个worker上。一个worker对应一个Graph Partition。
5. 图二次分裂：worker根据当前可用硬件资源，如CPU GPU，将Graph Partition按照op算子设备约束规范（例如tf.device(’/cpu:0’)，二次分裂到不同设备上。每个计算设备对应一个Graph Partition。
6. 图运行：对于每一个计算设备，worker依照op在kernel中的实现，完成op的运算。设备间数据通信可以使用send/recv节点，而worker间通信，则使用GRPC或RDMA协议。

这些阶段根据TensorFlow运行时的不同，会进行不同的处理。运行时有两种，本地运行时和分布式运行时。故Graph生命周期到后面分析本地运行时和分布式运行时的时候，再详细讲解。







# 4 -- 图的节点 - Operation





# 1 概述

上文讲述了TensorFlow的核心对象，计算图Graph。Graph包含两大成员，节点和边。节点即为计算算子Operation，边则为计算数据Tensor。由起始节点Source出发，按照Graph的拓扑顺序，依次执行节点的计算，即可完成整图的计算，最后结束于终止节点Sink，并输出计算结果。

本文会对节点Operation进行详细讲解。



# 2 前端节点数据结构

在Python前端中，Operation表示Graph的节点，Tensor表示Graph的边。Operation包含OpDef和NodeDef两个主要成员变量。其中OpDef描述了op的静态属性信息，例如op入参列表，出参列表等。而NodeDef则描述op的动态属性信息，例如op运行的设备信息，用户给op设置的name等。

先来看Operation的数据结构，只列出重要代码。

```python
@tf_export("Operation")
class Operation(object):
  def __init__(self,
               node_def,
               g,
               inputs=None,
               output_types=None,
               control_inputs=None,
               input_types=None,
               original_op=None,
               op_def=None):
     # graph引用，通过它可以拿到Operation所注册到的Graph
     self._graph = g
    
    # inputs
    if inputs is None:
      inputs = []

    #  input types
    if input_types is None:
      input_types = [i.dtype.base_dtype for i in inputs]

    # control_input_ops
    control_input_ops = []
    
    # node_def和op_def是两个最关键的成员
    if not self._graph._c_graph:
      self._inputs_val = list(inputs)  # Defensive copy.
      self._input_types_val = input_types
      self._control_inputs_val = control_input_ops
      
      # NodeDef，深复制
      self._node_def_val = copy.deepcopy(node_def)
        
      # OpDef
      self._op_def_val = op_def
      
    # outputs输出
    self._outputs = [
        Tensor(self, i, output_type)
        for i, output_type in enumerate(output_types)
    ]
```

下面来看Operation的属性方法，通过属性方法我们可以拿到Operation的两大成员，即OpDef和NodeDef。

```python
  @property
  def name(self):
    # Operation的name，注意要嵌套name_scope
	return self._node_def_val.name

  @property
  def _id(self):
    # Operation的唯一标示，id
    return self._id_value

  @property
  def device(self):
    # Operation的设备信息
    return self._node_def_val.device
    
  @property
  def graph(self):
    # graph引用
    return self._graph

  @property
  def node_def(self):
    # NodeDef成员，获取Operation的动态属性信息，例如Operation分配到的设备信息，Operation的name等
    return self._node_def_val

  @property
  def op_def(self):
    # OpDef，获取Operation的静态属性信息，例如Operation入参列表，出参列表等
    return self._op_def_val
```

# 3 后端节点数据结构

在C++后端中，Graph图也包含两部分，即边Edge和节点Node。同样，节点Node用来表示计算算子，而边Edge则表示计算数据或者Node间依赖关系。Node数据结构如下所示。

```c
class Node {
 public:
    // NodeDef,节点算子Operation的信息，比如op分配到哪个设备上了等，运行时有可能变化。
  	const NodeDef& def() const;
    
    // OpDef, 节点算子Operation的元数据，不会变的。比如Operation的入参个数，名字等
  	const OpDef& op_def() const;
 private:
  	// 输入边，传递数据给节点。可能有多条
  	EdgeSet in_edges_;

  	// 输出边，节点计算后得到的数据。可能有多条
  	EdgeSet out_edges_;
}
```

节点Node中包含的主要数据有输入边和输出边的集合，从而能够由Node找到跟他关联的所有边。Node中还包含NodeDef和OpDef两个成员。NodeDef表示节点算子的动态属性，创建Node时会new一个NodeDef对象。OpDef表示节点算子的静态属性，运行时不会变，创建Node时不需要new OpDef，只需要从OpDef仓库中取出即可。因为元信息是确定的，比如Operation的入参列表，出参列表等。









# 5 -- 图的边 - Tensor







# 1 概述

前文两篇文章分别讲解了TensorFlow核心对象Graph，和Graph的节点Operation。Graph另外一大成员，即为其边Tensor。边用来表示计算的数据，它经过上游节点计算后得到，然后传递给下游节点进行运算。本文讲解Graph的边Tensor，以及TensorFlow中的变量。



# 2 前端边Tensor数据结构

Tensor作为Graph的边，使得节点Operation之间建立了连接。上游源节点Operation经过计算得到数据Tensor，然后传递给下游目标节点，是一个典型的生产者-消费者关系。下面来看Tensor的数据结构

```python
@tf_export("Tensor")
class Tensor(_TensorLike):
  def __init__(self, op, value_index, dtype):
    # 源节点，tensor的生产者，会计算得到tensor
    self._op = op

    # tensor在源节点的输出边集合中的索引。源节点可能会有多条输出边
    # 利用op和value_index即可唯一确定tensor。
    self._value_index = value_index

    # tensor中保存的数据的数据类型
    self._dtype = dtypes.as_dtype(dtype)

    # tensor的shape，可以得到张量的rank，维度等信息
    self._shape_val = tensor_shape.unknown_shape()

    # 目标节点列表，tensor的消费者，会使用该tensor来进行计算
    self._consumers = []

    #
    self._handle_data = None
    self._id = uid()
12345678910111213141516171819202122
```

Tensor中主要包含两类信息，一个是Graph结构信息，如边的源节点和目标节点。另一个则是它所保存的数据信息，例如数据类型，shape等。



# 3 后端边Edge数据结构

后端中的Graph主要成员也是节点node和边edge。节点node为计算算子Operation，边Edge为算子所需要的数据，或者代表节点间的依赖关系。这一点和Python中的定义相似。边Edge的持有它的源节点和目标节点的指针，从而将两个节点连接起来。下面看Edge类的定义。

```c
class Edge {
   private:
      Edge() {}

      friend class EdgeSetTest;
      friend class Graph;
      // 源节点, 边的数据就来源于源节点的计算。源节点是边的生产者
      Node* src_;

      // 目标节点，边的数据提供给目标节点进行计算。目标节点是边的消费者
      Node* dst_;

      // 边id，也就是边的标识符
      int id_;

      // 表示当前边为源节点的第src_output_条边。源节点可能会有多条输出边
      int src_output_;

      // 表示当前边为目标节点的第dst_input_条边。目标节点可能会有多条输入边。
      int dst_input_;
};
```

Edge既可以承载tensor数据，提供给节点Operation进行运算，也可以用来表示节点之间有依赖关系。对于表示节点依赖的边，其`src_output_, dst_input_`均为-1，此时边不承载任何数据。



# 4 常量constant

TensorFlow的常量constant，最终包装成了一个Tensor。通过tf.constant(10)，返回一个Tensor对象。

```python
@tf_export("constant")
def constant(value, dtype=None, shape=None, name="Const", verify_shape=False):
  # 算子注册到默认Graph中
  g = ops.get_default_graph()
    
  # 对常量值value的处理
  tensor_value = attr_value_pb2.AttrValue()
  tensor_value.tensor.CopyFrom(
      tensor_util.make_tensor_proto(
          value, dtype=dtype, shape=shape, verify_shape=verify_shape))

  # 对常量值的类型dtype进行处理
  dtype_value = attr_value_pb2.AttrValue(type=tensor_value.tensor.dtype)

  # 构造并注册类型为“Const”的算子到Graph中，从算子的outputs中取出输出的tensor。
  const_tensor = g.create_op(
      "Const", [], [dtype_value.type],
      attrs={"value": tensor_value,
             "dtype": dtype_value},
      name=name).outputs[0]
  return const_tensor
```

tf.constant的过程为

1. 获取默认graph
2. 对常量值value和常量值的类型dtype进行处理
3. 构造并注册类型为“Const”的算子到默认graph中，从算子的outputs中取出输出的tensor。

此时只是图的构造过程，tensor并未承载数据，仅表示Operation输出的一个符号句柄。经过tensor.eval()或session.run()后，才会启动graph的执行，并得到数据。



# 5 变量Variable

### Variable构造器

通过tf.Variable()构造一个变量，代码如下，我们仅分析入参。

```python
@tf_export("Variable")
class Variable(object):
  def __init__(self,
               initial_value=None,
               trainable=True,
               collections=None,
               validate_shape=True,
               caching_device=None,
               name=None,
               variable_def=None,
               dtype=None,
               expected_shape=None,
               import_scope=None,
               constraint=None):
# initial_value: 初始值，为一个tensor，或者可以被包装为tensor的值
# trainable：是否可以训练，如果为false，则训练时不会改变
# collections：变量要加入哪个集合中，有全局变量集合、本地变量集合、可训练变量集合等。默认加入全局变量集合中
# dtype：变量的类型
```

主要的入参代码中已经给出了注释。Variable可以接受一个tensor或者可以被包装为tensor的值，来作为初始值。事实上，Variable可以看做是Tensor的包装器，它重载了Tensor的几乎所有操作，是对Tensor的进一步封装。

### Variable初始化

变量只有初始化后才能使用，初始化时将initial_value初始值赋予Variable内部持有的Tensor。通过运行变量的初始化器可以对变量进行初始化，也可以执行全局初始化器。如下

```python
y = tf.Variable([5.3])

with tf.Session() as sess:
    initialization = tf.global_variables_initializer()
    print sess.run(y)
```

### Variable集合

Variable被划分到不同的集合中，方便后续操作。常见的集合有

1. 全局变量：全局变量可以在不同进程中共享，可运用在分布式环境中。变量默认会加入到全局变量集合中。通过tf.global_variables()可以查询全局变量集合。其op标示为GraphKeys.GLOBAL_VARIABLES

   ```python
   @tf_export("global_variables")
   def global_variables(scope=None):
     return ops.get_collection(ops.GraphKeys.GLOBAL_VARIABLES, scope)
   ```

2. 本地变量：运行在进程内的变量，不能跨进程共享。通常用来保存临时变量，如训练迭代次数epoches。通过tf.local_variables()可以查询本地变量集合。其op标示为GraphKeys.LOCAL_VARIABLES

   ```python
   @tf_export("local_variables")
   def local_variables(scope=None):
   	return ops.get_collection(ops.GraphKeys.LOCAL_VARIABLES, scope)
   ```

3. 可训练变量：一般模型参数会放到可训练变量集合中，训练时，做这些变量会得到改变。不在这个集合中的变量则不会得到改变。默认会放到此集合中。通过tf.trainable_variables()可以查询。其op标示为GraphKeys.TRAINABLE_VARIABLES

   ```python
   @tf_export("trainable_variables")
   def trainable_variables(scope=None):
     return ops.get_collection(ops.GraphKeys.TRAINABLE_VARIABLES, scope)
   ```

其他集合还有model_variables，moving_average_variables。











# 6 -- TensorFlow本地运行时







# 1 概述

TensorFlow后端分为四层，运行时层、计算层、通信层、设备层。运行时作为第一层，实现了session管理、graph管理等很多重要的逻辑，是十分关键的一层。根据任务分布的不同，运行时又分为本地运行时和分布式运行时。本地运行时，所有任务运行于本地同一进程内。而分布式运行时，则允许任务运行在不同机器上。

Tensorflow的运行，通过session搭建了前后端沟通的桥梁，前端几乎所有操作都是通过session进行。session的生命周期由创建、运行、关闭、销毁组成，前文已经详细讲述过。可以将session看做TensorFlow运行的载体。而TensorFlow运行的核心对象，则是计算图Graph。它由计算算子和计算数据两部分构成，可以完整描述整个计算内容。Graph的生命周期包括构建和传递、剪枝、分裂、执行等步骤，本文会详细讲解。理解TensorFlow的运行时，重点就是理解会话session和计算图Graph。

本地运行时，client master和worker都在本地机器的同一进程内，均通过DirectSession类来描述。由于在同一进程内，三者间可以共享内存，通过DirectSession的相关函数实现调用。

client前端直接面向用户，负责session的创建，计算图Graph的构造。并通过session.run()将Graph序列化后传递给master。master收到后，先反序列化得到Graph，然后根据反向依赖关系，得到几个最小依赖子图，这一步称为剪枝。之后master根据可运行的设备情况，将子图分裂到不同设备上，从而可以并发执行，这一步称为分裂。最后，由每个设备上的worker并行执行分裂后的子图，得到计算结果后返回。



# 2 Graph构建和传递

session.run()开启了后端Graph的构建和传递。在前文session生命周期的讲解中，session.run()时会先调用_extend_graph()将要运行的Operation添加到Graph中，然后再启动运行过程。extend_graph()会先将graph序列化，得到graph_def，然后调用后端的TF_ExtendGraph()方法。下面我们从c_api.cc中的TF_ExtendGraph()看起。

```c
// 增加节点到graph中，proto为序列化后的graph
void TF_ExtendGraph(TF_DeprecatedSession* s, const void* proto,
                    size_t proto_len, TF_Status* status) {
  GraphDef g;
  // 先将proto转换为GrapDef。graphDef是图的序列化表示，反序列化在后面。
  if (!tensorflow::ParseProtoUnlimited(&g, proto, proto_len)) {
    status->status = InvalidArgument("Invalid GraphDef");
    return;
  }

  // 再调用session的extend方法。根据创建的不同session类型，多态调用不同方法。
  status->status = s->session->Extend(g);
}
```

后端系统根据生成的Session类型，多态的调用Extend方法。如果是本地session，则调用DirectSession的Extend()方法。下面看DirectSession的Extend()方法。

```c
Status DirectSession::Extend(const GraphDef& graph) {
  // 保证线程安全，然后调用ExtendLocked()
  mutex_lock l(graph_def_lock_);
  return ExtendLocked(graph);
}

// 主要任务就是创建GraphExecutionState对象。
Status DirectSession::ExtendLocked(const GraphDef& graph) {
  bool already_initialized;

  if (already_initialized) {
    TF_RETURN_IF_ERROR(flib_def_->AddLibrary(graph.library()));

    // 创建GraphExecutionState
    std::unique_ptr<GraphExecutionState> state;
    TF_RETURN_IF_ERROR(execution_state_->Extend(graph, &state));
    execution_state_.swap(state);
  }
  return Status::OK();
}
```

最终创建了GraphExecutionState对象。它主要工作有

1. 负责将GraphDef反序列化为graph，从而构造出graph。在初始化方法InitBaseGraph()中
2. 执行部分op编排工作，在初始化方法InitBaseGraph()中

```c
Status GraphExecutionState::InitBaseGraph(const BuildGraphOptions& options) {
  const GraphDef* graph_def = &original_graph_def_;

  // graphDef反序列化得到graph
  std::unique_ptr<Graph> new_graph(new Graph(OpRegistry::Global()));
  GraphConstructorOptions opts;
  TF_RETURN_IF_ERROR(ConvertGraphDefToGraph(opts, *graph_def, new_graph.get()));

  // 恢复有状态的节点
  RestoreStatefulNodes(new_graph.get());

  // 构造优化器的选项 optimization_options
  GraphOptimizationPassOptions optimization_options;
  optimization_options.session_options = session_options_;
  optimization_options.graph = &new_graph;
  optimization_options.flib_def = flib_def_.get();
  optimization_options.device_set = device_set_;

  TF_RETURN_IF_ERROR(OptimizationPassRegistry::Global()->RunGrouping(
      OptimizationPassRegistry::PRE_PLACEMENT, optimization_options));

  // plaer执行op编排
  Placer placer(new_graph.get(), device_set_, session_options_);
  TF_RETURN_IF_ERROR(placer.Run());

  TF_RETURN_IF_ERROR(OptimizationPassRegistry::Global()->RunGrouping(
      OptimizationPassRegistry::POST_PLACEMENT, optimization_options));

  // 报春状态节点
  SaveStatefulNodes(new_graph.get());
  graph_ = new_graph.release();
  return Status::OK();
}
```

### 构造Graph：反序列化GraphDef为Graph

由于client传递给master的是序列化后的计算图，所以master需要先反序列化。通过ConvertGraphDefToGraph实现。代码在graph_constructor.cc中，如下

```c
Status ConvertGraphDefToGraph(const GraphConstructorOptions& opts,
                              const GraphDef& gdef, Graph* g) {
  ShapeRefiner refiner(gdef.versions().producer(), g->op_registry());
  return GraphConstructor::Construct(
      opts, gdef.node(), &gdef.versions(), &gdef.library(), g, &refiner,
      /*return_tensors=*/nullptr, /*return_nodes=*/nullptr,
      /*missing_unused_input_map_keys=*/nullptr);
}
```

### 编排OP

Operation编排的目的是，将op以最高效的方式，放在合适的硬件设备上，从而最大限度的发挥硬件能力。通过Placer的run()方法进行，算法很复杂，在placer.cc中，我也看得不大懂，就不展开了。



# 3 Graph剪枝

反序列化构建好Graph，并进行了Operation编排后，master就开始对Graph剪枝了。剪枝就是根据Graph的输入输出列表，反向遍历全图，找到几个最小依赖的子图，从而方便并行计算。

```c
Status GraphExecutionState::BuildGraph(const BuildGraphOptions& options,
                                       std::unique_ptr<ClientGraph>* out) {

  std::unique_ptr<Graph> ng;
  Status s = OptimizeGraph(options, &ng);
  if (!s.ok()) {
    // 1 复制一份原始的Graph
    ng.reset(new Graph(flib_def_.get()));
    CopyGraph(*graph_, ng.get());
  }

  // 2 剪枝，根据输入输出feed fetch，对graph进行增加节点或删除节点等操作。通过RewriteGraphForExecution()方法
  subgraph::RewriteGraphMetadata rewrite_metadata;
  if (session_options_ == nullptr ||
      !session_options_->config.graph_options().place_pruned_graph()) {
    TF_RETURN_IF_ERROR(subgraph::RewriteGraphForExecution(
        ng.get(), options.feed_endpoints, options.fetch_endpoints,
        options.target_nodes, device_set_->client_device()->attributes(),
        options.use_function_convention, &rewrite_metadata));
  }

  // 3 处理优化选项optimization_options
  GraphOptimizationPassOptions optimization_options;
  optimization_options.session_options = session_options_;
  optimization_options.graph = &ng;
  optimization_options.flib_def = flib.get();
  optimization_options.device_set = device_set_;

  TF_RETURN_IF_ERROR(OptimizationPassRegistry::Global()->RunGrouping(
      OptimizationPassRegistry::POST_REWRITE_FOR_EXEC, optimization_options));

  // 4 复制一份ClientGraph
  std::unique_ptr<ClientGraph> dense_copy(
      new ClientGraph(std::move(flib), rewrite_metadata.feed_types,
                      rewrite_metadata.fetch_types));
  CopyGraph(*ng, &dense_copy->graph);

  *out = std::move(dense_copy);
  return Status::OK();
}
```

剪枝的关键在RewriteGraphForExecution()方法中，在subgraph.cc文件中。

```c
Status RewriteGraphForExecution(
    Graph* g, const gtl::ArraySlice<string>& fed_outputs,
    const gtl::ArraySlice<string>& fetch_outputs,
    const gtl::ArraySlice<string>& target_node_names,
    const DeviceAttributes& device_info, bool use_function_convention,
    RewriteGraphMetadata* out_metadata) {

  std::unordered_set<string> endpoints;

  // 1 构建节点的name_index，从而快速索引节点。为FeedInputs，FetchOutputs等步骤所使用
  NameIndex name_index;
  name_index.reserve(g->num_nodes());
  for (Node* n : g->nodes()) {
    name_index[n->name()] = n;
  }

  // 2 FeedInputs，添加输入节点
  if (!fed_outputs.empty()) {
    FeedInputs(g, device_info, fed_outputs, use_function_convention, &name_index, &out_metadata->feed_types);
  }

  // 3 FetchOutputs，添加输出节点
  std::vector<Node*> fetch_nodes;
  if (!fetch_outputs.empty()) {
    FetchOutputs(g, device_info, fetch_outputs, use_function_convention, &name_index, &fetch_nodes, &out_metadata->fetch_types);
  }

  // 4 剪枝，形成若干最小依赖子图
  if (!fetch_nodes.empty() || !target_node_names.empty()) {
    PruneForTargets(g, name_index, fetch_nodes, target_node_names);
  }

  return Status::OK();
}
```

主要有4步

1. 构建节点的name_index，从而快速索引节点。为FeedInputs，FetchOutputs等步骤所使用
2. FeedInputs，添加输入节点。输入节点的数据来源于session.run()时的feed列表。
3. FetchOutputs，添加输出节点。输出节点在session.run()时通过fetches所给出
4. 剪枝PruneForTargets，形成若干最小依赖子图。这是剪枝算法最关键的一步。

PruneForTargets()从输出节点反向搜索，按照BFS广度优先算法，找到若干个最小依赖子图。

```c
static Status PruneForTargets(Graph* g, const subgraph::NameIndex& name_index,
                              const std::vector<Node*>& fetch_nodes,
                              const gtl::ArraySlice<string>& target_nodes) {
  string not_found;
  std::unordered_set<const Node*> targets;

  // 1 AddNodeToTargets添加节点到targets中，从输出节点按照BFS反向遍历。
  for (Node* n : fetch_nodes) {
    AddNodeToTargets(n->name(), name_index, &targets);
  }

  // 2 剪枝，得到多个最小依赖子图子图
  PruneForReverseReachability(g, targets);

  // 修正Source和Sink节点的依赖边，将没有输出边的节点连接到sink node上
  FixupSourceAndSinkEdges(g);

  return Status::OK();
}
```

主要有3步

1. AddNodeToTargets，从输出节点按照BFS反向遍历图的节点，添加到targets中。
2. PruneForReverseReachability，剪枝，得到多个最小依赖子图子图
3. FixupSourceAndSinkEdges，修正Source和Sink节点的依赖边，将没有输出边的节点连接到sink node上

PruneForReverseReachability()在algorithm.cc文件中，算法就不分析了，总体是按照BFS广度优先算法搜索的。

```c
bool PruneForReverseReachability(Graph* g,
                                 std::unordered_set<const Node*> visited) {
  // 按照BFS广度优先算法，从输出节点开始，反向搜索节点的依赖关系
  std::deque<const Node*> queue;
  for (const Node* n : visited) {
    queue.push_back(n);
  }
  while (!queue.empty()) {
    const Node* n = queue.front();
    queue.pop_front();
    for (const Node* in : n->in_nodes()) {
      if (visited.insert(in).second) {
        queue.push_back(in);
      }
    }
  }

  // 删除不在"visited"列表中的节点，说明最小依赖子图不依赖此节点
  std::vector<Node*> all_nodes;
  all_nodes.reserve(g->num_nodes());
  for (Node* n : g->nodes()) {
    all_nodes.push_back(n);
  }

  bool any_removed = false;
  for (Node* n : all_nodes) {
    if (visited.count(n) == 0 && !n->IsSource() && !n->IsSink()) {
      g->RemoveNode(n);
      any_removed = true;
    }
  }

  return any_removed;
}
```

# 4 Graph分裂

剪枝完成后，master即得到了最小依赖子图ClientGraph。然后根据本地机器的硬件设备，以及op所指定的运行设备等关系，将图分裂为多个Partition Graph，传递到相关设备的worker上，从而进行并行运算。这就是Graph的分裂。

Graph分裂的算法在graph_partition.cc的Partition()方法中。算法比较复杂，我们就不分析了。图分裂有两种

1. splitbydevice按设备分裂，也就是将Graph分裂到本地各CPU GPU上。本地运行时只使用按设备分裂。

   ```c
   static string SplitByDevice(const Node* node) {
     return node->assigned_device_name();
   }
   ```

2. splitByWorker 按worker分裂, 也就是将Graph分裂到各分布式任务上，常用于分布式运行时。分布式运行时，图会经历两次分裂。先splitByWorker分裂到各分布式任务上，一般是各分布式机器。然后splitbydevice二次分裂到分布式机器的CPU GPU等设备上。

   ```c
   static string SplitByWorker(const Node* node) {
     string task;
     string device;
     DeviceNameUtils::SplitDeviceName(node->assigned_device_name(), &task, &device);
     return task;
   }
   ```

# 5 Graph执行

Graph经过master剪枝和分裂后，就可以在本地的各CPU GPU设备上执行了。这个过程的管理者叫worker。一般一个worker对应一个分裂后的子图partitionGraph。每个worker启动一个执行器Executor，入度为0的节点数据依赖已经ready了，故可以并行执行。等所有Executor执行完毕后，通知执行完毕。

各CPU GPU设备间可能需要数据通信，通过创建send/recv节点来解决。数据发送方创建send节点，将数据放在send节点内，不阻塞。数据接收方创建recv节点，从recv节点中取出数据，recv节点中如果没有数据则阻塞。这又是一个典型的生产者-消费者关系。

Graph执行的代码逻辑在direct_session.cc文件的DirectSession::Run()方法中。代码逻辑很长，我们抽取其中的关键部分。

```c
Status DirectSession::Run(const RunOptions& run_options,
                          const NamedTensorList& inputs,
                          const std::vector<string>& output_names,
                          const std::vector<string>& target_nodes,
                          std::vector<Tensor>* outputs,
                          RunMetadata* run_metadata) {

  // 1 将输入tensor的name取出，组成一个列表，方便之后快速索引输入tensor
  std::vector<string> input_tensor_names;
  input_tensor_names.reserve(inputs.size());
  for (const auto& it : inputs) {
    input_tensor_names.push_back(it.first);
  }

  // 2 传递输入数据给executor，通过FunctionCallFrame方式。
  // 2.1 创建FunctionCallFrame，用来输入数据给executor，并从executor中取出数据。
  FunctionCallFrame call_frame(executors_and_keys->input_types,
                               executors_and_keys->output_types);
  // 2.2 构造输入数据feed_args
  gtl::InlinedVector<Tensor, 4> feed_args(inputs.size());
  for (const auto& it : inputs) {
    if (it.second.dtype() == DT_RESOURCE) {
      Tensor tensor_from_handle;
      ResourceHandleToInputTensor(it.second, &tensor_from_handle);
      feed_args[executors_and_keys->input_name_to_index[it.first]] = tensor_from_handle;
    } else {
      feed_args[executors_and_keys->input_name_to_index[it.first]] = it.second;
    }
  }

  // 2.3 将feed_args输入数据设置到Arg节点上
  const Status s = call_frame.SetArgs(feed_args);


  // 3 开始执行executor
  // 3.1 创建run_state, 和IntraProcessRendezvous
  RunState run_state(args.step_id, &devices_);
  run_state.rendez = new IntraProcessRendezvous(device_mgr_.get());
  CancellationManager step_cancellation_manager;
  args.call_frame = &call_frame;

  // 3.2 创建ExecutorBarrier，它是一个执行完成的计数器。同时注册执行完成的监听事件executors_done.Notify()
  const size_t num_executors = executors_and_keys->items.size();
  ExecutorBarrier* barrier = new ExecutorBarrier(
      num_executors, run_state.rendez, [&run_state](const Status& ret) {
        {
          mutex_lock l(run_state.mu_);
          run_state.status.Update(ret);
        }
        // 所有线程池计算完毕后，会触发Notify，发送消息。
        run_state.executors_done.Notify();
      });

  args.rendezvous = run_state.rendez;
  args.cancellation_manager = &step_cancellation_manager;
  args.session_state = &session_state_;
  args.tensor_store = &run_state.tensor_store;
  args.step_container = &run_state.step_container;
  args.sync_on_finish = sync_on_finish_;

  // 3.3 创建executor的运行器Runner
  Executor::Args::Runner default_runner = [this,
                                           pool](Executor::Args::Closure c) {
    SchedClosure(pool, std::move(c));
  };

  // 3.4 依次启动所有executor，开始运行
  for (const auto& item : executors_and_keys->items) {
    item.executor->RunAsync(args, barrier->Get());
  }

  // 3.5 阻塞，收到所有executor执行完毕的通知
  WaitForNotification(&run_state, &step_cancellation_manager, operation_timeout_in_ms_);

  // 4 接收执行器执行完毕的输出值
  if (outputs) {
    // 4.1 从RetVal节点中得到输出值sorted_outputs
    std::vector<Tensor> sorted_outputs;
    const Status s = call_frame.ConsumeRetvals(&sorted_outputs);

    // 4.2 处理原始输出sorted_outputs，保存到最终的输出outputs中
    outputs->clear();
    outputs->reserve(sorted_outputs.size());
    for (int i = 0; i < output_names.size(); ++i) {
      const string& output_name = output_names[i];
      if (first_indices.empty() || first_indices[i] == i) {
        outputs->emplace_back(
            std::move(sorted_outputs[executors_and_keys->output_name_to_index[output_name]]));
      } else {
        outputs->push_back((*outputs)[first_indices[i]]);
      }
    }
  }

  // 5 保存输出的tensor
  run_state.tensor_store.SaveTensors(output_names, &session_state_));

  return Status::OK();
}
```

主要步骤如下

1. 将输入tensor的name取出，组成一个列表，方便之后快速索引输入tensor

2. 传递输入数据给executor，通过FunctionCallFrame方式。本地运行时因为在同一个进程中，我们采用FunctionCallFrame函数调用的方式来实现数据传递。将输入数据传递给Arg节点，从RetVal节点中取出数据。

3. 开始执行executor，并注册监听器。所有executor执行完毕后，会触发executors_done.Notify()事件。然后当前线程wait阻塞，等待收到执行完毕的消息。

4. 收到执行完毕的消息后，从RetVal节点中取出输出值，经过简单处理后，就可以最终输出了

5. 保存输出的tensor，方便以后使用。

   

# 6 总结

本文主要讲解了TensorFlow的本地运行时，牢牢抓住session和graph两个对象即可。Session的生命周期前文讲解过，本文主要讲解了Graph的生命周期，包括构建与传递，剪枝，分裂和执行。Graph是TensorFlow的核心对象，很多问题都是围绕它来进行的，理解它有一定难度，但十分关键。文章中可能有一些理解不正确的地方，希望小伙伴们不吝赐教。









# 7 -- TensorFlow分布式运行时







# 1 概述

TensorFlow架构设计精巧，在后端运行时这一层，除了提供本地运行时外，还提供了分布式运行时。通过分布式训练，在多台机器上并行执行，大大提高了训练速度。前端用户通过session.run()启动系统执行时，target默认为空字符串""，对应的是本地运行模式。若target以"grpc://"开头，则对应的是分布式运行模式，target指定了要连接的TensorFlow执行引擎。

分布式运行时同样分为client master和worker，只是三者不在同一进程内。分布式运行时同样是围绕计算图Graph来进行的，流程也与本地运行时几乎相同。client负责图的构造，并传递给master。master接收后，启动图的剪枝和分裂，将分裂后的子图发送给多个worker进程。worker进程负责执行计算子图，它会先按照自己所在机器包含的设备，先按照设备进行子图的二次分裂，然后在每个设备上进行子图执行。所有设备执行完毕后，从计算图的终止节点sink中取出数据。

本地运行时通过DirectSession同时管理client master和worker，而分布式运行时则不同。client对应GrpcSession，master对应MasterSession，worker对应WorkerSession。三者使用同一个句柄session_handle进行协同工作。



# 2 数据交换

和本地运行时类似，分布式运行时也存在跨设备的数据依赖。对于跨设备的数据边，将其分裂，在发送方插入send节点，接收方插入recv节点。如果二者跨进程通信（比如两台不同的服务器），则通过GrpcRemoteRendezvous进行数据交换。如果二者是进程内通信（比如同一台服务器的CPU0和CPU1），则通过IntraProcessRendezvous进行数据交换。上节讲过的本地运行时在运行前，就创建了一个IntraProcessRendezvous对象。



# 3 分布式集群结构

TensorFlow为分布式运行时，设计了一个精巧的结构。共分为三级。

1. 集群cluster，可包含多台服务器，通过ClusterSpec对象描述。它包含多个job，一个job又包含多个Task。一个Task对应一个server。

2. Job。将目的相同的Task划归为一个job，使用job_id唯一标示。一般存在两种job

   1. ps：数据存储，负责存储和更新模型的参数，比如w和b。比较适合CPU
   2. worker：数据计算，负责train和inference时的数据计算工作。比较适合GPU

   一般ps将数据发送给worker，待worker运算完毕后再返回给ps，ps再进行数据更新。

3. Task。Task是提供服务的最小单位，它一般单独在一个进程内，通过job_id:task_index唯一标示。一个Task对应一个server，提供MasterService和WorkerService两种服务。

下面是一个集群配置的例子。

```python
tf.train.ClusterSpec({
    "worker": [
        "worker0:1111", # /job:worker/task:0
        "worker1:2222", # /job:worker/task:1
        "worker2:3333" # /job:worker/task:2
    ],
    "ps": [
        "ps0:1111", # /job:ps/task:0
        "ps1:2222" # /job:ps/task:1
]})
```

这个集群cluster内包含2个job，一个ps和一个worker。ps又包含2个task，worker则包含3个task，共计5个task。

