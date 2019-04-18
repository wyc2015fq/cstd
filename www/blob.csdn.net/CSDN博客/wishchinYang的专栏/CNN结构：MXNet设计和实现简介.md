# CNN结构：MXNet设计和实现简介 - wishchinYang的专栏 - CSDN博客
2017年11月10日 14:48:32[wishchin](https://me.csdn.net/wishchin)阅读数：733
         对原文有大量修改，如有疑惑，请移步原文。
         参考链接：[MXNet设计和实现简介](https://github.com/apache/incubator-mxnet/issues/797)
         文章翻译于：[https://mxnet.incubator.apache.org/architecture/index.html](https://mxnet.incubator.apache.org/architecture/index.html)
**一、MXNet System Architecture系统概览**
![](https://img-blog.csdn.net/20171110103519271?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
This figure shows the major modules and components of the MXNet system and their interaction. The modules are:
- Runtime Dependency Engine: Schedules and executes theoperations according to their read/write dependency.
- Storage Allocator: Efficiently allocates and recycles memory blocks on host (CPU) and devices (GPUs).
- Resource Manager: Manages global resources, such as the random number generatorand temporal space.
- NDArray: Dynamic, asynchronous n-dimensional arrays,which provide flexible imperative programs for MXNet.
- Symbolic Execution: Static symbolic graph executor,which provides efficient symbolic graph execution and optimization.
- Operator: Operators that define static forward and gradientcalculation (backprop).
- SimpleOp: Operators that extend NDArray operators and symbolic operators in a unified fashion.
- Symbol Construction: Symbolic construction, which provides a way to constructa computation graph (net configuration).
- KVStore: Key-value store interface for efficient parameter synchronization.
- Data Loading(IO): Efficient distributed data loading and augmentation.
这个表格显示了MxNet的主要模块和系统内部交互组元。模块拆分为：
- 实时的计算引擎，根据硬件定制的计划性的操作运算。
- 存储分配操作：高效的CPU和GPU内存分配和回收机制。
- Resource Manager: 全局资源管理器，例如随机生成的数据和临时空间。
- NDArray: 同步动态的N维数组，可提供弹性规划的MxNet程序。.
- Symbolic Execution: 静态的符号图，提供高效的符号图的执行和优化。
- Operator: 一个前向计算和回溯优化的操作符。
- SimpleOp: 一个标准样式的扩展了NDArray的操作符系列。
- Symbol Construction: 网络配置-符号表的构建。用以构建计算图的一种通路。
- KVStore: 键-值存储接口，给出了参数同步的有效方式。
- Data Loading(IO): 分布式的IO系统.
# MXNet System Components
## Execution Engine
     You can use MXNet’s engine not only for deep learning,but for any domain-specific problem.It’s designed to solve a general problem:execute a bunch of functions following their dependencies.Execution of any two functions with dependencies should be serialized.
     To boost performance, functions with no dependencies *can* be executed in parallel.For a general discussion of this topic,see our
[*notes on the dependency engine*](https://mxnet.incubator.apache.org/architecture/note_engine.html).
    专注于问题解决而不是构架设计。为了提升速度，可以使用并行方法计算。详情请看...............
### Interface
The following API is the core interface for the execution engine:
virtualvoidPushSync( Fnexec_fun,Contextexec_ctx,std::vector<VarHandle>const&const_vars,std::vector<VarHandle>const&mutate_vars)=;
This API allows you to push a function (`exec_fun`),along with its context information and dependencies, to the engine.`exec_ctx` is
 the context information in which the `exec_fun` should be executed,` const_vars` denotes the variables that the function reads from,and
`mutate_vars` are the variables to be modified.The engine provides the following guarantee:
> 
*The execution of any two functionsthat modify a common variableis serialized in their push order.*
这个API作用是 绑定一个函数到设备，.`exec_ctx` 是设备信息。mutate_vars必须是一个变量![疑问](http://static.blog.csdn.net/xheditor/xheditor_emot/default/doubt.gif)
### Function
The function type of the engine is:
usingFn=std::function<void(RunContext)>;
`RunContext` contains runtime information, which is determined by the engine:
structRunContext{// stream pointer which could be safely cast to// cudaStream_t* typevoid*stream;};
Alternatively, you could use `mxnet::engine::DAGEngine::Fn`, which has the same type definition.
All of the functions are executed by the engine’s internal threads.In such a model, it’s usually not a good idea to push
*blocking* functionsto the engine (usually for dealing with I/O tasks like disk, web service, UI, etc.)because it will occupy the execution thread and reduce total throughput.In that case, we provide another
*asynchronous* function type:
usingCallback=std::function<void()>;usingAsyncFn=std::function<void(RunContext,Callback)>;
In the `AsyncFn` function, you can pass the heavy part to your own threadsand safely exit the body of the function.The engine doesn’t consider the function finisheduntil the
`Callback` function is called.
### Context
     You can specify the `Context` of the function to be executed within.This usually includes whether the function should be run on a CPU or a GPU,and if you specify a GPU, which GPU to use.`Context`
 is different from `RunContext`.` Context` contains device type (GPU/CPU) and device id,while
`RunContext` contains information that can be decided only during runtime,for example, on which stream the function should be executed.
     配置上下文，确定 函数是运行在哪个地方，CPU还是GPU。上下文包含设备类型和设备ID，`RunContext` 包含运行时设备信息，例如指定哪个/哪些流处理器执行函数。
### VarHandle
`VarHandle` is used to specify the dependencies of functions.The MXNet engine is designed to be decoupled from other MXNet modules.So
`VarHandle` is like an engine-provided token you use to represent the external resources the functions can use or modify. It’s designed to be lightweight, so creating,deleting, or copying a variable
 incurs little overhead. Upon pushing the functions, you need to specify the variables that will be used (immutable) in the
`const_vars` vector, and the variables that will be modified (mutable) in the
`mutate_vars` vector.The engine uses one rule for resolving the dependencies among functions:
> 
*The execution of any two functions when one of them modifies at least one common variable is serialized in their push order.*
For example, if `Fn1` and
`Fn2` both mutate 
```
V2
```
 then 
```
Fn2
```
is guaranteed to be executed after `Fn1` if
`Fn2` is pushed after
`Fn1`.On the other hand, if
`Fn1` and 
```
Fn2
```
 both use `V2`,their actual execution order could be random.
This design allows the engine to schedule *state-mutating* operations in a manner that minimizes calls to allocate new memory. For example, the weight update function in DNN can now use the
`+=` operatorto update the weights in place,rather than generating a new weight array each time.
To create a variable, use the `NewVar()` API.To delete a variable, use the
`PushDelete` API.
       变量标识符：MxNet设计为引擎和模块解耦的策略，VarHandle用于标识函数依赖。........................................... 
`mutate_vars`向量.........................
       两个函数对变量的改变取决于它们的计算顺序
例如：若Fn1和Fn2 同时锁住了V2，Fn2确定是在Fn1执行之后执行。而实际上，这种有序性是不能保证的。这种设计可以使变量使用+=这种运算更新权值，而不是重新声明一个变量。
      在此，变量的创建和销毁使用专门的API，如`NewVar()` 和
`PushDelete` API.
### Push and Wait
*All `Push` APIs are asynchronous.* The API call returns immediately regardless of whether the pushed
`Fn` is finished or not. This allows the engine to start computing at the same timeas the user thread is pushing functions.
`Push` APIs are not thread-safe.To be specific, only one thread should make engine API calls at a time.
If you want to wait for a specific `Fn` to finish,include a callback function in the closure,and call the function at the end of your
`Fn`.
If you want to wait for all `Fn`s that involve (use or mutate) a certain variable to finish,use the
`WaitForVar(var)` API.
If you want to wait for all pushed `Fn`s to finish,use the
`WaitForAll()` API.
顺序和等待：所有的push操作是异步的。API回调忽略变量是否被函数Fn操作完成。因此push API不是线程安全的。因此，引擎API的calls操作必须是单线程的。
若是期待特定Fn完成，使用Fn的闭环，等待Fn完成。
若是期待所有Fns完成对变量var操作，使用`WaitForVar(var)` API。
若是期待所有Fns完成操作，使用`WaitForAll``()` API。
### Save Object Creation Cost
In some cases, you need to push several functions to the engine for a long period of time. If the computation of these functions is light,the overhead of copying lambdas and creating use/mutate variable lists becomes relatively high.We provide an API to
 create an `OprHandle` beforehand:
virtualOprHandleNewOperator(AsyncFnfn,std::vector<VarHandle>const&const_vars,std::vector<VarHandle>const&mutate_vars)=;
You can keep pushing the `OprHandle` without repeatedly creating them:
virtualvoidPush(OprHandleop,Contextexec_ctx)=;
To delete it, call the 
```
DeleteOperator(OprHandle
op)
```
 API.Ensure that the operator has finished computing before calling this API.
对象创建的代价：有时，需要一段长的时间排列几个函数到引擎。若这些函数其实是轻量计算，那么复制正则表达式和变量维持表变得代价相对巨大。为此提供了专用的API 
```
OprHandle
```
。
...........................
对应的删除操作是：
```
DeleteOperator(OprHandle
op)
```
 API。但是使用之前一定要确定所有函数执行完成。
### API手册-API Reference
> 
- *class*`mxnet::``Engine`
Dependency engine that schedules operations. 
Public Types
- *typedef*engine::CallbackOnComplete `CallbackOnComplete`
callback on complete 
- *typedef*std::function(RunContext)> 
```
SyncFn
```
Synchronous operation to pass to engine. 
- *typedef*std::function(RunContext, [CallbackOnComplete](https://mxnet.incubator.apache.org/architecture/overview.html#_CPPv2N5mxnet6Engine18CallbackOnCompleteE))> `AsyncFn`
Asynchronous operation to pass to engine. 
- *typedef*engine::VarHandle `VarHandle`
Variable pointer. 
- *typedef*engine::OprHandle `OprHandle`
Operator pointer. 
Public Functions
- *virtual* void
`NotifyShutdown`() = 0
Notify the engine about a shutdown, This can help engine to print less messages into display.
User do not have to call this function. 
- **Return**
0 when success, -1 when failure happens. - *virtual*[VarHandle](https://mxnet.incubator.apache.org/architecture/overview.html#_CPPv2N5mxnet6Engine9VarHandleE)`NewVariable`() = 0
Allocate a new variable, the variable can then be used to schedule the operation concurrently via dependency patterns.
- **Return**
The new variable allocated. - *virtual*[OprHandle](https://mxnet.incubator.apache.org/architecture/overview.html#_CPPv2N5mxnet6Engine9OprHandleE)`NewOperator`([AsyncFn](https://mxnet.incubator.apache.org/architecture/overview.html#_CPPv2N5mxnet6Engine7AsyncFnE)*fn*, std::vector<[VarHandle](https://mxnet.incubator.apache.org/architecture/overview.html#_CPPv2N5mxnet6Engine9VarHandleE)>
*const* &*const_vars*, std::vector<[VarHandle](https://mxnet.incubator.apache.org/architecture/overview.html#_CPPv2N5mxnet6Engine9VarHandleE)>
*const* &*mutable_vars*, FnProperty *prop* = FnProperty::kNormal,
*const* char **opr_name* = nullptr) = 0
Create a new operator. The returned operator could be saved externally so that it could be resued for scheduling.
- **Return**
The new operator allocated. - **Parameters**
- `fn`: The execution function.
- `const_vars`: The variables that current operation will use but not mutate.
- `mutable_vars`: The variables that current operation will mutate.
- `prop`: Property of the function.
- `opr_name`: The operator name.
- *virtual*
 void `DeleteOperator`([OprHandle](https://mxnet.incubator.apache.org/architecture/overview.html#_CPPv2N5mxnet6Engine9OprHandleE)*op*) = 0
Delete the given operator. 
The delete will not happen immediately, but will wait until all the operations using this operator are completed.
- **Parameters**
- `op`: The operator to delete.
- *virtual*
 void `Push`([OprHandle](https://mxnet.incubator.apache.org/architecture/overview.html#_CPPv2N5mxnet6Engine9OprHandleE)*op*, Context *exec_ctx*, int *priority* = 0, bool *profiling* = false) = 0
Push an operator to the engine. 
- **Parameters**
- `op`: The operator to push.
- `exec_ctx`: Execution context.
- `priority`: Priority of the action, as hint to the engine.
- `profiling`: The variable indicate whether to profile this operator.
- *virtual* void
`PushAsync`([AsyncFn](https://mxnet.incubator.apache.org/architecture/overview.html#_CPPv2N5mxnet6Engine7AsyncFnE)*exec_fun*, Context *exec_ctx*, std::vector<[VarHandle](https://mxnet.incubator.apache.org/architecture/overview.html#_CPPv2N5mxnet6Engine9VarHandleE)>
*const* &*const_vars*, std::vector<[VarHandle](https://mxnet.incubator.apache.org/architecture/overview.html#_CPPv2N5mxnet6Engine9VarHandleE)>
*const* &*mutable_vars*, FnProperty *prop* = FnProperty::kNormal, int
*priority* = 0, *const* char **opr_name* = nullptr) = 0
Push an asynchronous operation to the engine. 
- **Parameters**
- `exec_fun`: Execution function, this function takes a parameter on_complete that must be called when the execution completes.
- `exec_ctx`: Execution context.
- `const_vars`: The variables that current operation will use but not mutate.
- `mutable_vars`: The variables that current operation will mutate.
- `prop`: Property of the function.
- `priority`: Priority of the action, as hint to the engine.
- `opr_name`: The operator name.
- *virtual* void
`DeleteVariable`([SyncFn](https://mxnet.incubator.apache.org/architecture/overview.html#_CPPv2N5mxnet6Engine6SyncFnE)*delete_fn*, Context *exec_ctx*, [VarHandle](https://mxnet.incubator.apache.org/architecture/overview.html#_CPPv2N5mxnet6Engine9VarHandleE)*var*) = 0
Schedule the deletion of a variable. 
The delete will not happen immediately, but will wait until all the operations depending on var are completed.
- **Parameters**
- `delete_fn`: A function that will be called after the variable is deleted.
- `exec_ctx`: Execution context.
- `var`: The variable to be deleted.
- *virtual* void
`WaitForVar`([VarHandle](https://mxnet.incubator.apache.org/architecture/overview.html#_CPPv2N5mxnet6Engine9VarHandleE)*var*) = 0
Wait for a variable. 
- **Parameters**
- `var`: The variable we should wait for. This function returns when the variable is ready.
- *virtual* void
`WaitForAll`() = 0
Wait until all the activity of engine finishes. 
- *virtual*`~Engine`()
virtual destructor 
- void
`PushSync`([SyncFn](https://mxnet.incubator.apache.org/architecture/overview.html#_CPPv2N5mxnet6Engine6SyncFnE)*exec_fn*, Context *exec_ctx*, std::vector<[VarHandle](https://mxnet.incubator.apache.org/architecture/overview.html#_CPPv2N5mxnet6Engine9VarHandleE)>
*const* &*const_vars*, std::vector<[VarHandle](https://mxnet.incubator.apache.org/architecture/overview.html#_CPPv2N5mxnet6Engine9VarHandleE)>
*const* &*mutable_vars*, FnProperty *prop* = FnProperty::kNormal, int
*priority* = 0, *const* char **opr_name* = nullptr)
Push an synchronous operation to the engine. 
- **Parameters**
- `exec_fn`: Execution function that executes the operation.
- `exec_ctx`: Execution context.
- `const_vars`: The variables that current operation will use but not mutate.
- `mutable_vars`: The variables that current operation will mutate.
- `prop`: Property of the function.
- `priority`: Priority of the action, as hint to the engine.
- `opr_name`: The operator name.
- **Template Parameters**
- `SyncFn`: the synchronous function to be pushed.
- [CallbackOnComplete](https://mxnet.incubator.apache.org/architecture/overview.html#_CPPv2N5mxnet6Engine18CallbackOnCompleteE)`CreateCallback`(void (**callback*)([Engine](https://mxnet.incubator.apache.org/architecture/overview.html#_CPPv2N5mxnet6EngineE)
 *, void *), void **param*, 
)
factory function to create OnComplete callback. 
- **Parameters**
- `callback`: th static callback function.
- `param`: the paramter passed to callback.
- *virtual* int
`num_omp_threads_per_worker`()*const* = 0
Return the number of OMP threads that should be used per worker. 
- **Return**
Number of OMP threads that should be used per worker - *virtual* void
`set_num_omp_threads_per_worker`(int
*num_omp_threads_per_worker*) = 0
Set the number of OMP threads that should be used per worker. 
- **Parameters**
- `num_threads_per_worker`: Number of OMP threads to be used per worker
Public Static Functions
- *static*[Engine](https://mxnet.incubator.apache.org/architecture/overview.html#_CPPv2N5mxnet6EngineE) *`Get`()
- **Return**
[Engine](https://mxnet.incubator.apache.org/architecture/overview.html#classmxnet_1_1Engine) singleton.
- *static* std::shared_ptr<[Engine](https://mxnet.incubator.apache.org/architecture/overview.html#_CPPv2N5mxnet6EngineE)>
`_GetSharedRef`()
Get shared pointer reference to engine singleton. Most user should not call this function. This function is called by another singleton X who requires engine to be destructed after X.
- **Return**
A shared pointer to [Engine](https://mxnet.incubator.apache.org/architecture/overview.html#classmxnet_1_1Engine) singleton. - 
## 算子-Operators in MXNet
In MXNet, an operator is a class that contains both actual computation logicand auxiliary information that can aid the system in performing optimizations,like in-place updates and auto-derivatives.To understand the remainder of the document,we recommend
 that you familiarize yourself with the `mshadow` library,because all operators compute on the tensor-like structure
`mshadow::TBlob`provided by the system during runtime.
MXNet’s operator interface allows you to:
- Reduce memory allocation cost by specifying in-place updates.
- Hide some internal arguments from Python to make it cleaner.
- Define the relationships among input tensors and output tensors,which allows the system to perform shape checking for you.
- Acquire additional temporary spaces from the systemto perform computation (e.g., calling
`cudnn` routines).
### Operator Interface
`Forward` is the core operator interface:
virtualvoidForward(constOpContext&ctx,conststd::vector<TBlob>&in_data,conststd::vector<OpReqType>&req,conststd::vector<TBlob>&out_data,conststd::vector<TBlob>&aux_states)=;
The `OpContext` structure is:
structOpContext{intis_train;RunContextrun_ctx;std::vector<Resource>requested;}
It describes whether the operator is in the train or test phase,which device the operator should be run on (in
`run_ctx`),and requested resources (covered in the following sections).
- `in_data` and 
```
out_data
```
 represent the input and output tensors, respectively.All of the tensor spaces have been allocated by the system.
- `req` denotes how the computation results are written into the
`out_data`.In other words,
```
req.size() 
== out_data.size()
```
 and 
```
req[i]
```
correspond to the write type of 
```
out_data[i]
```
.
- The `OpReqType` is defined as:
enumOpReqType{kNullOp,kWriteTo,kWriteInplace,kAddTo};
Normally, the types of all `out_data` should be
`kWriteTo`,meaning that the provided
`out_data` tensor is a
*raw* memory block,so the operator should write results directly into it.In some cases, for example when calculating the
`gradient` tensor,it would be great if we could accumulate the result,rather than directly overwrite the tensor contentsso that no extra space needs to be created each time.In such a case, the corresponding
`req` type is set as
`kAddTo`,indicating that a
`+=` should be called.
- `aux_states` is intentionally designed for auxiliary tensors used to help computation. Currently, it is useless.
Aside from the `Forward` operator, you could optionally implement the
`Backward` interface:
virtualvoidBackward(constOpContext&ctx,conststd::vector<TBlob>&out_grad,conststd::vector<TBlob>&in_data,conststd::vector<TBlob>&out_data,conststd::vector<OpReqType>&req,conststd::vector<TBlob>&in_grad,conststd::vector<TBlob>&aux_states);
This interface follows the same design principle as the 
```
Forward
```
 interface,except that 
```
out_grad
```
, `in_data`, and
`out_data` are given,and the operator computes
`in_grad` as the results.The naming strategy is similar to Torch’s convention,and can be summarized in following figure:
[input/output semantics figure]
Some operators might not require all of the following:`out_grad`,
`in_data` and 
```
out_data
```
.You can specify these dependencies with the
`DeclareBackwardDependency` interface in
`OperatorProperty`.
### Operator Property
One convolution might have several implementations,and you might want to switch among them to achieve the best performance.Therefore, we separate the operator
*semantic* interfacesfrom the implementation interface (`Operator` class)into the
`OperatorProperty` class.The
`OperatorProperty` interface consists of:
- **InferShape:**
virtualboolInferShape(std::vector<TShape>*in_shape,std::vector<TShape>*out_shape,std::vector<TShape>*aux_shape)const=;
This interface has two purposes:
- Tell the system the size of each input and output tensor,so it can allocate space for them before the
`Forward` and 
```
Backward
```
 call.
- Perform a size check to make sure that there isn’t an obvious error before running.The shape in
`in_shape` is set by the system(from the
`out_shape` of the previous operators).It returns
`false` when there is not enough informationto infer shapes or throws an error when the shape is inconsistent.
- **Request Resources:** Operations like 
```
cudnnConvolutionForward
```
 need a work space for computation.If the system can manage that, it could then perform optimizations,like reuse the space, and so on.MXNet defines two interfaces to achieve this:
virtualstd::vector<ResourceRequest>ForwardResource(conststd::vector<TShape>&in_shape)const;virtualstd::vector<ResourceRequest>BackwardResource(conststd::vector<TShape>&in_shape)const;
The `ResourceRequest` structure (in
`resource.h`) currently contains only a type flag:
structResourceRequest{enumType{kRandom,// get a mshadow::Random objectkTempSpace,// request temporary space};Typetype;};
If `ForwardResource` and
`BackwardResource` return non-empty arrays,the system offers the corresponding resources through the
`ctx` parameterin the
`Forward` and 
```
Backward
```
 interface of 
```
Operator
```
.Basically, to access those resources, simply write:
autotmp_space_res=ctx.requested[kTempSpace].get_space(some_shape,some_stream);autorand_res=ctx.requested[kRandom].get_random(some_stream);
For an example, see `src/operator/cudnn_convolution-inl.h`.
- **Backward dependency:** Let’s look at two different operator signatures(we name all of the arguments for demonstration purposes):
voidFullyConnectedForward(TBlobweight,TBlobin_data,TBlobout_data);voidFullyConnectedBackward(TBlobweight,TBlobin_data,TBlobout_grad,TBlobin_grad);voidPoolingForward(TBlobin_data,TBlobout_data);voidPoolingBackward(TBlobin_data,TBlobout_data,TBlobout_grad,TBlobin_grad);
Note that `out_data` in
`FullyConnectedForward`is not used by
`FullyConnectedBackward`,while
`PoolingBackward` requires all of the arguments of
`PoolingForward`.Therefore, for
`FullyConnectedForward`,the
`out_data` tensor once consumed could be safely freedbecause the backward function will not need it.This provides a chance for the system to collect some tensorsas garbage as soon as possible.To specify
 this situation, we provide an interface:
virtualstd::vector<int>DeclareBackwardDependency(conststd::vector<int>&out_grad,conststd::vector<int>&in_data,conststd::vector<int>&out_data)const;
The `int` element of the argument vector is an IDto distinguish different arrays.Let’s see how this interface specifies different dependenciesfor
`FullyConnected` and
`Pooling`:
std::vector<int>FullyConnectedProperty::DeclareBackwardDependency(conststd::vector<int>&out_grad,conststd::vector<int>&in_data,conststd::vector<int>&out_data)const{return{out_grad[],in_data[]};// NOTE: out_data[0] is NOT included}std::vector<int>PoolingProperty::DeclareBackwardDependency(conststd::vector<int>&out_grad,conststd::vector<int>&in_data,conststd::vector<int>&out_data)const{return{out_grad[],in_data[],out_data[]};}
- **In place Option:** To further save the cost of memory allocation,you can use in-place updates.They are appropriate for element-wise operationswhen the input tensor and output tensor have the same shape.You specify and in-place update with
 the following interface:
virtualstd::vector<std::pair<int,void*>>ElewiseOpProperty::ForwardInplaceOption(conststd::vector<int>&in_data,conststd::vector<void*>&out_data)const{return{{in_data[],out_data[]}};}virtualstd::vector<std::pair<int,void*>>ElewiseOpProperty::BackwardInplaceOption(conststd::vector<int>&out_grad,conststd::vector<int>&in_data,conststd::vector<int>&out_data,conststd::vector<void*>&in_grad)const{return{{out_grad[],in_grad[]}}}
This tells the system that the `in_data[0]` and
`out_data[0]` tensors could share the same memory spaces during
`Forward`, and so do
`out_grad[0]` and 
```
in_grad[0]
```
 during 
```
Backward
```
.
> 
**Important:** Even if you use the preceding specification, it’s
*not* guaranteed that the input and output tensors will share the same space. In fact, this is only a suggestion for the system, which makes the final decision. However, in either case, the decision is completely transparent to you, so the actual
`Forward` and 
```
Backward
```
 implementation does not need to consider that.
- **Expose Operator to Python:** Because of the restrictions of C++, you need user to implement following interfaces:
// initial the property class from a list of key-value string pairsvirtualvoidInit(constvector<pair<string,string>>&kwargs)=;// return the parameters in a key-value string mapvirtualmap<string,string>GetParams()const=;// return the name of arguments (for generating signature in python)virtualvector<string>ListArguments()const;// return the name of output valuesvirtualvector<string>ListOutputs()const;// return the name of auxiliary statesvirtualvector<string>ListAuxiliaryStates()const;// return the number of output valuesvirtualintNumOutputs()const;// return the number of visible outputsvirtualintNumVisibleOutputs()const;
### Create an Operator from the Operator Property
`OperatorProperty` includes all
*semantic* attributes of an operation. It’s also responsible for creating the
`Operator` pointer for actual computation.
#### Create Operator
Implement the following interface in `OperatorProperty`:
virtualOperator*CreateOperator(Contextctx)const=;
For example:
classConvolutionOp{public:voidForward(...){...}voidBackward(...){...}};classConvolutionOpProperty:publicOperatorProperty{public:Operator*CreateOperator(Contextctx)const{returnnewConvolutionOp;}};
#### Parametrize Operator
When implementing a convolution operator, you need to know the kernel size,the stride size, padding size, and so on.These parameters should be passed to the operatorbefore any
`Forward` or 
```
Backward
```
 interface is called.To do so, you could define a
`ConvolutionParam` structure, as follows:
#includestructConvolutionParam:publicdmlc::Parameter<ConvolutionParam>{TShapekernel,stride,pad;uint32_tnum_filter,num_group,workspace;boolno_bias;};
Put it in `ConvolutionOpProperty`, and pass it to the operator class during construction:
classConvolutionOp{public:ConvolutionOp(ConvolutionParamp):param_(p){}voidForward(...){...}voidBackward(...){...}private:ConvolutionParamparam_;};classConvolutionOpProperty:publicOperatorProperty{public:voidInit(constvector<pair<string,string>&kwargs){// initialize param_ using kwargs}Operator*CreateOperator(Contextctx)const{returnnewConvolutionOp(param_);}private:ConvolutionParamparam_;};
#### Register the Operator Property Class and the Parameter Class to MXNet
Use the following macros to register the parameter structure and the operator property class to MXNet:
DMLC_REGISTER_PARAMETER(ConvolutionParam);MXNET_REGISTER_OP_PROPERTY(Convolution,ConvolutionOpProperty);
The first argument is the name string, the second is the property class name.
### Interface Summary
We’ve almost covered the entire interface required to define a new operator. Let’s do a recap:
- Use the `Operator` interface to write your computation logic (`Forward` and
`Backward`).
- Use the `OperatorProperty` interface to:
- Pass the parameter to the operator class (you can use the 
```
Init
```
 interface).
- Create an operator using the `CreateOperator` interface.
- Correctly implement the operator description interface, such as the names of arguments, etc.
- Correctly implement the `InferShape` interface to set the output tensor shape.
- [Optional] If additional resources are needed, check 
```
ForwardResource
```
 and 
```
BackwardResource
```
.
- [Optional] If `Backward` doesn’t need all of the input and output of
`Forward`, check 
```
DeclareBackwardDependency
```
.
- [Optional] If in-place update is supported, check 
```
ForwardInplaceOption
```
 and 
```
BackwardInplaceOption
```
.
- Register the `OperatorProperty` class and the parameter class.
## Unifying the NDArray Operator and Symbolic Operator
NDArray operations are similar to symbolic operations,except that sometimes you can’t write in place to the operandswithout a complete dependency graph.However, the logic underlying NDArray and symbolic operations are almost identical.*SimpleOp*,
 a new unified operator API,unifies different invoking processesand returns to the fundamental elements of operators.Because most mathematical operators attend to one or two operands,and more operands make dependency-related optimization useful,the unified
 operator is specifically designed for unary and binary operations.
Consider the elements of an operation.Ideally, you need only functions and derivativesto describe an operation.Let’s restrict that to the space of unary and binary operations.How do we classify all operations to maximize the possibilityof in-place write
 optimization?Note that you can separate functions by the number of operands.Derivatives are a bit more complex.To construct a dependency graph, you need to know whether output value,input data, or neither are needed alongside head gradient.Gradient functions
 in the unified API are differentiatedby the types of operands it takes for calculation.
Before you learn more about the SimpleOp interface,we recommend that you review the[mshadow library guide](https://github.com/dmlc/mshadow/tree/master/guide)because calculations will be done in the
`mshadow::TBlob` structure.
In the following example, we’ll create an operatorfunctioning as a smooth l1 loss,which is a mixture of l1 loss and l2 loss. The loss itself can be written as:
    loss = outside_weight .* f(inside_weight .* (data - label))
    grad = outside_weight .* inside_weight .* f'(inside_weight .* (data - label))
`.*` stands for element-wise multiplication, and
`f`, 
```
f'
```
 is the smooth l1 loss function,which we are assuming is in
`mshadow` for now.At first glance, it’s impossible to implementthis particular loss as a unary or binary operator.But we have automatic differentiation in symbolic execution.That simplifies the loss
 to `f` and 
```
f'
```
 directly.This loss is no more complex than a 
```
sin
```
 or an `abs` function,and can certainly be implemented as a unary operator.
## SimpleOp: The Unified Operator API
### Define Shapes
The `mshadow` library requires explicit memory allocation.As a consequence, all data shapesmust be provided before any calculation occurs.Before we proceed with defining functions and gradient,let’s
 check input data shape consistency and provide output shape.
typedefTShape(*UnaryShapeFunction)(constTShape&src,constEnvArguments&env);typedefTShape(*BinaryShapeFunction)(constTShape&constTShape&rhs,lhs,constEnvArguments&env);
You can use `mshadow::TShape` to check input data shape and designate output data shape.If you don’t define this function, the default output shape is the same as the input shape.In the case of
 a binary operator, the shape of `lhs` and
`rhs` is checked as the same by default.
You can also use shape functions to check if any additional arguments and resources are present.Refer to the additional usages of
`EnvArguments` to accomplish this.
Before we start on our smooth l1 loss example, we define a 
```
XPU
```
 to `cpu` or
`gpu` in the header`smooth_l1_unary-inl.h` implementation so that we reuse the same code in
`smooth_l1_unary.cc` and`smooth_l1_unary.cu`.
#include#if defined(__CUDACC__)#define XPU gpu#else#define XPU cpu#endif
In our smooth l1 loss example, it’s okay to use the default behavior whereby the output has the same shape as the source.Written explicitly, it is:
inlineTShapeSmoothL1Shape_(constTShape&src,constEnvArguments&env){returnTShape(src);
### Define Functions
Create a unary or binary function with one output: 
```
mshadow::TBlob
```
.
typedefvoid(*UnaryFunction)(constTBlob&src,constEnvArguments&env,TBlob*ret,OpReqTypereq,RunContextctx);typedefvoid(*BinaryFunction)(constTBlob&lhs,constTBlob&rhs,constEnvArguments&env,TBlob*ret,OpReqTypereq,RunContextctx);
- Functions are differentiated by the types of input arguments.
- 
```
RunContext 
ctx
```
 contains information needed during runtime for execution.
structRunContext{void*stream;// the stream of the device, can be NULL or Stream* in GPU modetemplate<typenamexpu>inlinemshadow::Stream<xpu>*get_stream()// get mshadow stream from Context}// namespace mxnet
```
mshadow::stream 
*s = ctx.get_stream();
```
 is an example of obtaining a stream from
`ctx`.
- 
```
OpReqType 
req
```
 denotes how computation results are written into 
```
ret
```
.
enumOpReqType{kNullOp,// no operation, do not write anythingkWriteTo,// write gradient to provided spacekWriteInplace,// perform an in-place writekAddTo// add to the provided space};
A macro is defined in `operator_util.h` for a simplified use of
`OpReqType`.
```
ASSIGN_DISPATCH(out,
req, exp)
```
 checks 
```
req
```
 and performs an assignment.
In our smooth l1 loss example, we use `UnaryFunction` to define the function of this operator.
template<typenamexpu>voidSmoothL1Forward_(constTBlob&src,constEnvArguments&env,TBlob*ret,OpReqTypereq,RunContextctx){usingnamespacemshadow;usingnamespacemshadow::expr;mshadow::Stream<xpu>*s=ctx.get_stream<xpu>();real_tsigma2=env.scalar*env.scalar;MSHADOW_TYPE_SWITCH(ret->type_flag_,DType,{mshadow::Tensor<xpu,,DType>out=ret->get<xpu,,DType>(s);mshadow::Tensor<xpu,,DType>in=src.get<xpu,,DType>(s);ASSIGN_DISPATCH(out,req,F<mshadow_op::smooth_l1_loss>(in,ScalarExp<DType>(sigma2)));});}
After obtaining `mshadow::Stream` from
`RunContext`, we get
`mshadow::Tensor` from
`mshadow::TBlob`.`mshadow::F` is a shortcut to initiate a
`mshadow` expression. The macro
```
MSHADOW_TYPE_SWITCH(type,
DType, ...)
```
handles details on different types, and the macro
```
ASSIGN_DISPATCH(out, 
req, exp)
```
 checks 
```
OpReqType
```
 andperforms actions accordingly. 
```
sigma2
```
 is a special parameter in this loss, which we will cover later.
### Define Gradients (Optional)
定义梯度-可选方案.
Create a gradient function with various types of inputs.
// depending only on out_gradtypedefvoid(*UnaryGradFunctionT0)(constOutputGrad&out_grad,constEnvArguments&env,TBlob*in_grad,OpReqTypereq,RunContextctx);// depending only on out_valuetypedefvoid(*UnaryGradFunctionT1)(constOutputGrad&out_grad,constOutputValue&out_value,constEnvArguments&env,TBlob*in_grad,OpReqTypereq,RunContextctx);// depending only on in_datatypedefvoid(*UnaryGradFunctionT2)(constOutputGrad&out_grad,constInput0&in_data0,constEnvArguments&env,TBlob*in_grad,OpReqTypereq,RunContextctx);
Gradient functions of binary operators have similar structures, except that 
```
Input
```
, `TBlob`, and
`OpReqType`are doubled.
`GradFunctionArgument`
`Input0`, 
```
Input
```
, `OutputValue`, and
`OutputGrad` all share the structure of
`GradFunctionArgument`,which is defined as:
structGradFunctionArgument{TBlobdata;}
In our smooth l1 loss example, note that it’s an 
```
f'(x)
```
,which utilizes input for the gradient calculation,so the
`UnaryGradFunctionT2` is suitable.To enable the chain rule of the gradient,we also need to multiply
`out_grad` from the top to the result of
`in_grad`.
template<typenamexpu>voidSmoothL1BackwardUseIn_(constOutputGrad&out_grad,constInput0&in_data0,constEnvArguments&env,TBlob*in_grad,OpReqTypereq,RunContextctx){usingnamespacemshadow;usingnamespacemshadow::expr;mshadow::Stream<xpu>*s=ctx.get_stream<xpu>();real_tsigma2=env.scalar*env.scalar;MSHADOW_TYPE_SWITCH(in_grad->type_flag_,DType,{mshadow::Tensor<xpu,,DType>src=in_data0.data.get<xpu,,DType>(s);mshadow::Tensor<xpu,,DType>ograd=out_grad.data.get<xpu,,DType>(s);mshadow::Tensor<xpu,,DType>igrad=in_grad->get<xpu,,DType>(s);ASSIGN_DISPATCH(igrad,req,ograd*F<mshadow_op::smooth_l1_gradient>(src,ScalarExp<DType>(sigma2)));});}
### Register SimpleOp to MXNet
After creating the shape, function, and gradient, restore them into both an NDArray operator anda symbolic operator. To simplify this process, use the registration macro defined in
`operator_util.h`.
MXNET_REGISTER_SIMPLE_OP(Name,DEV).set_shape_function(Shape).set_function(DEV::kDevMask,Function<XPU>,SimpleOpInplaceOption).set_gradient(DEV::kDevMask,Gradient<XPU>,SimpleOpInplaceOption).describe("description");
`SimpleOpInplaceOption` is defined as:
enumSimpleOpInplaceOption{kNoInplace,// do not allow inplace in argumentskInplaceInOut,// allow inplace in with out (unary)kInplaceOutIn,// allow inplace out_grad with in_grad (unary)kInplaceLhsOut,// allow inplace left operand with out (binary)kInplaceOutLhs// allow inplace out_grad with lhs_grad (binary)};
In our example, we have a gradient function that relies on input data, so the function can’t be written inplace. The output gradient has no purpose after gradient computation, so the gradient can be written in place.
MXNET_REGISTER_SIMPLE_OP(smooth_l1,XPU).set_function(XPU::kDevMask,SmoothL1Forward_<XPU>,kNoInplace).set_gradient(XPU::kDevMask,SmoothL1BackwardUseIn_<XPU>,kInplaceOutIn).set_enable_scalar(true).describe("Calculate Smooth L1 Loss(lhs, scalar)");
Remember from the discussion of shape functions that a default behavior without
`set_shape_function` forces the inputs(if they’re binary) to be the same shape and yield the same shape for output. We’ll discuss
`set_enable_scalar` later.
### NDArray Operator Summary
- Create a shape function for determining the output shape.
- Create a function as the forward routine by choosing a suitable function type.
- Create a gradient as the backward routine by choosing a suitable gradient type.
- Register the operator using the registration process.
## Additional Information on SimpleOp
### Using SimpleOp on EnvArguments
Some operations might need a scalar as input, such as a gradient scale, a set of keyword argumentscontrolling behavior, or a temporary space to speed up calculations.`EnvArguments` provides additional
 arguments and resources to make calculations more scalableand efficient.
structEnvArguments{real_tscalar;// scalar argument, if enabledstd::vector<std::pair<std::string,std::string>>kwargs;// keyword argumentsstd::vector<Resource>resource;// pointer to the resources requested};
More registration parameters are required to enable these additional features. To prevent confusion on parameters,
`scalar` and 
```
kwargs
```
can’t be present at the same time. To enable
`scalar`, use
```
set_enable_scalar(bool
enable_scalar)
```
 in registration. Then, in forward functions and gradients, the
`scalar` can be accessed from
`env.scalar` as in the function parameter
```
EnvArguments 
env
```
.
To enable `kwargs`, use
```
set_enable_kwargs(bool 
enable_kwargs)
```
 in registration. Then, in forwardfunctions and gradients, additional arguments are contained in
`env.kwarg`, which is defined as
```
std::vector
std::string> >
```
. Use the DMLC parameter structure tosimplify parsing keyword arguments. For more details, see the
[guide on parameter structure](https://github.com/dmlc/dmlc-core/blob/master/doc/parameter.md).
Additional resources like `mshadow::Random` and temporary memory space can also be requested andaccessed from
`EnvArguments.resource`. The registration routine is
```
set_resource_request(ResourceRequest
req)
```
or 
```
set_resource_request(const
std::vector)
```
, where 
```
mxnet::ResourceRequest
```
 is defined as:
structResourceRequest{enumType{// Resource type, indicating what the pointer type iskRandom,// mshadow::Random objectkTempSpace// A dynamic temp space that can be arbitrary size};Typetype;// type of resources};
Registration will request the declared resource requests from 
```
mxnet::ResourceManager
```
, and place resourcesin 
```
std::vector resource
```
 in 
```
EnvArguments
```
. To access resources, use the following:
autotmp_space_res=env.resources[].get_space(some_shape,some_stream);autorand_res=env.resources[].get_random(some_stream);
For an example, see `src/operator/loss_binary_op-inl.h`.
In our smooth l1 loss example, a scalar input is needed to mark the turning point of a loss function. Therefore,in the registration process, we use
`set_enable_scalar(true)`, and use
`env.scalar` in function and gradientdeclarations.
### Crafting a Tensor Operation
Because computation utilizes the `mshadow` library and we sometimes don’t have functions readily available, wecan craft tensor operations in operator implementations. If you define such functions
 as element-wise, youcan implement them as a `mxnet::op::mshadow_op`.
`src/operator/mshadow_op.h` that contains a lot of
`mshadow_op`,for example.
`mshadow_op` are expression mappers. They deal with the scalar case of desired functions. For details, see[mshadow
 expression API guide](https://github.com/dmlc/mshadow/tree/master/doc).
If an operation can’t be done in an element-wise way, like the softmax loss and gradient, then you need to create a new tensor operation. You need to create as
`mshadow` function and as
`mshadow::cuda`function directly. For details, see the
`mshadow` library. For an example, see
`src/operator/roi_pooling.cc`.
In our smooth l1 loss example, we create two mappers, namely the scalar cases of smooth l1 loss and gradient.
namespacemshadow_op{structsmooth_l1_loss{// a is x, b is sigma2MSHADOW_XINLINEstaticreal_tMap(real_ta,real_tb){if(a>1.0f/b){returna-0.5f/b;}elseif(a<-1.0f/b){return-a-0.5f/b;}else{return0.5f*a*a*b;}}};}
The gradient, which can be found in `src/operator/smooth_l1_unary-inl.h`, is similar.
### Beyond Two Operands
The new unified API is designed to fulfill the fundamentals of an operation. For operators with more than two inputs,more than one output, or that need more features, see the original
[Operator API](https://mxnet.incubator.apache.org/architecture/overview.html#operators-in-mxnet).
