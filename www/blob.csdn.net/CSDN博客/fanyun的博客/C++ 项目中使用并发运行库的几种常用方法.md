# C++ 项目中使用并发运行库的几种常用方法 - fanyun的博客 - CSDN博客
2017年06月18日 15:06:48[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：187
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
         在VS2013中，使用该 API 的方式和是并行模式库 （PPL）、 异步代理库和并发运行的现有项目中的部分的类。 我将介绍四种常见方案，开发人员面临在多线程应用程序开发过程中，并描述如何可以提高工作效率立即，使用
 PPL 和代理库进行多线程的程序，更有效和更具可伸缩性。
## 1、将工作从 UI 线程移动到后台任务
您正在以作为 Windows 开发人员避免告诉在首要任务之一悬挂在 UI 线程。 鏃犳硶鍝嶅簲窗口的最终用户的体验非常不无论是否开发人员提供他们的客户等待鼠标指针或
 Windows 为它们提供了挂起 UI frosted 玻璃窗口的形式。 我们正在提供本指南通常是而不是简洁的：不在 UI 线程上执行任何阻塞的调用，但而移动在后台线程对这些调用。
将一个长时间运行的串行操作移动一个后台线程
因此意味着什么？它移动到一个后台线程的工作如果有正在运行一个长时间，或可能阻止操作的某些函数，并且想要将该函数移后台线程，一个很好的交易的样板代码参与的实际移动的工作机制，即使对于一个函数一样简单内容调用像如下所示：
void SomeFunction(int x, int y){
       LongRunningOperation(x, y);
}
第一次，您需要打包要使用的任何状态。此处我时只打包一个对整数，因此我可以使用内置容器类似于一个 std::vector，一个 std::pair，或一个 std::tuple，但更通常我曾看到过人员执行包在他们自己结构或类，如下值：
struct LongRunningOperationParams{
       LongRunningOperationParams(int x_, int y_):x(x_),y(y_){}
       int x;
       int y;
}
您需要创建一个全局或静态函数，匹配的线程池 CreateThread 签名 unpackages 的状态（通常通过取消引用一个 void * 指针）、执行函数，然后删除数据（如果适用。 示例如下：
DWORD WINAPILongRunningOperationThreadFunc(void* data){
                LongRunningOperationParams*pData =
          (LongRunningOperationParams*) data;
               LongRunningOperation(pData->x,pData->y);
                //delete the data ifappropriate
        delete pData;
}
现在可以最后获取周围实际的日程排定线程与数据如下所示：
void SomeFunction(int x, int y){
       //package up our thread state
       //and store it on the heap
       LongRunningOperationParams* threadData =
       new LongRunningOperationParams(x,y);
       //now schedule the thread with the data
       CreateThread(NULL,NULL,&LongRunningOperationThreadFunc,
         (void*) pData,NULL);
}
这不似乎类似的更多的代码。从技术上讲，我添加了只有两个代码行的 SomeFunction、四行，我们的类和线程函数的三行。但这实际上是四倍的代码。我们采用 12 行只是为了计划一个函数调用具有两个参数的代码从三行代码。我必须做如下，最后一次我认为我不得不捕获大约八个变量，并捕获并将所有此状态设置成为很乏味和容易出错。如果我撤回正确，我发现并修复至少两个错误，只需的过程中捕获状态，并生成构造函数。
我还没有涉及上所需等待线程完成，通常涉及到创建的事件和跟踪该句柄 WaitForSingleObject 调用和操作完成之后使用它当然，清理句柄。它的至少三个更多行代码的仍然留出处理异常和返回代码。
CreateThread 的替代方法：task_group 类
第一种我要介绍的方法使用从该 PPL task_group 类。如果您不熟悉 task_group 类，它提供生成通过 task_group::run 异步任务和等待 task_group::wait 通过完成其任务的方法。此外提供了取消的任务，还没有被启动，并包括打包 std::exception_ptr 的异常，重新引发它的功能。
您将看到的明显较少的代码此处涉及比与 CreateThread 方法从可读性的角度来看，代码更接近串行示例。第一步是创建 task_group 对象。该对象需要存储位置可以管理其生命周期内的位置— — 渚嬪的方式  在堆上或在类中的一个成员变量。 接下来使用task_group::run 计划任务（不是线程） 执行该操作。Task_group::run 采用一个 functor 作为参数，并为您管理的 functor 的生存期。通过使用一个 C
 + + 打包状态，0x lambda 这有效地是两行更改程序。如下代码如下所示：
//a task_group member variable
task_group backgroundTasks;
void SomeFunction(int x, int y){
       backgroundTasks.run([x,y](){LongRunningOperation(x, y);});
}
**2、使工作异步与代理程序库**
另一种方法是使用代理为库包括基于传递的邮件的方法。代码更改的量是有关相同，但键语义差异值得指出使用的基于代理的方法。而不是计划任务，您可以生成消息传递管道，异步发送一个包含仅在数据依赖于自身以处理该消息管道。在最前面的情况下，我应该发送一个消息包含 x 和 y。工作仍会发生在另一个的线程上，但随后对同一管道的调用进行排队，并顺序（相对于 task_group，它不提供排序保证）处理该邮件。
第一次，您需要一个结构来包含消息。您可以实际上，使用与早期的相同的结构，但我将其重命名，如下所示：
struct LongRunningOperationMsg{
       LongRunningOperationMsg (int x, int y):m_x(x),m_y(y){}
       int m_x;
       int m_y;
}
下一步是声明一个发送到的位置。代理库中可以将消息发送到任何消息的接口的"目标，但在这种特定情况下最适合 < T > 调用。调用 < T >一条消息，并使用一个 functor 采用该消息作为参数的构造。声明和调用的结构可能如下（使用 lambdas）所示：
call<LongRunningOperationMsg>*LongRunningOperationCall = new
  call<LongRunningOperationMsg>([]( LongRunningOperationMsg msg)
{
LongRunningOperation(msg.x, msg.y);
})
修改 SomeFunction 现在是轻微。目标是构造一条消息并发送它以异步方式调用的对象。收到邮件时，将在单独的线程上调用调用：
void SomeFunction(int x, int y){
       asend(LongRunningOperationCall, LongRunningOperationMsg(x,y));
}
返回到 UI 线程上的工作
关闭在 UI 线程的工作是只有一半问题。大概 LongRunningOperation 结尾处，要得到一些有意义的结果和下一步通常获取返回到 UI 线程上的工作。执行方法因您的应用程序，但最简单的 Visual Studio 2010 中提供的库中实现此方法是从代理库中使用的 API 和消息块的另一对：try_receive 和 unbounded_buffer < T >。
一个 unbounded_buffer < T >可用于存储数据，并且可能需要在 UI 线程上运行的代码包含一条消息。 Try_receive 是一个非阻止调用 API 可用于查询是否存在要显示的数据。
渚嬪的方式 如果您是在您的 UI 线程上的呈现图像，可以使用类似于下面的代码后 InvalidateRect 调用获取数据返回到 UI 线程上：
unbounded_buffer<ImageClass>*ImageBuffer;
LONG APIENTRY MainWndProc(HWND hwnd, UINTuMsg,
 WPARAM wParam, LPARAM lParam)
{
   RECT rcClient;
   int i;
   ...
  ImageClass image;
  //check the buffer for images and if there is one there, display it.
   if(try_receive(ImageBuffer,image))
      DisplayImage(image);
  ...
}
此处，省略了类似于在邮件循环的实现某些详细信息，但我希望这一节已足够有益，来演示此方法。我建议您检查项目有一个完整的示例代码使用示例，这些方法的每个。
图 1 的非的线程安全类
samclass
Widget{
size_t m_width;
size_t m_height;
public:
Widget(size_t w, size_th):m_width(w),m_height(h){};
size_t GetWidth(){
return m_width;
}
size_t GetHeight(){
return m_height;
}
void SetWidth(size_t width){
m_width = width;
}
void SetHeight(size_t height){
m_height = height;
}
};
：管理消息块与代理的共享的状态
在多线程应用程序开发中的另一个常见情况管理共享的状态。专门，只要您尝试进行通信或线程之间共享数据，快速管理共享的状态成为您要处理的问题。我经常看到该方法是只需将关键节添加到要保护其数据成员和公共的接口的对象，但这很快成为维护问题，有时会变得以及性能问题。鍦 ㄨ 繖绉嶆儏鍐典笅，我将介绍一个串行和简单的示例使用锁，，然后我将显示代理库使用消息块的替代方法。
锁定一个简单的小工具类
图 1 演示非线程安全 Widget 类带有宽度和高度数据成员和改变其状态的简单方法。
使 Widget 类线程的安全而简单的方法是保护其方法与关键节或读取器-编写器锁。在 PPL 包含一个 reader_writer_lock，并的图 2 提供了第一个看明显解决方案的最简单的方法：使用该 PPL 在 reader_writer_lock。
图 2 使用该 reader_writer_lock 从并行模式库
class LockedWidget{
size_t m_width;
size_t m_height;
reader_writer_lock lock;
public:
LockedWidget (size_t w, size_th):m_width(w),m_height(h){};
size_t GetWidth(){
auto lockGuard =reader_writer::scoped_lock_read(lock);
return m_width;
}
size_t GetHeight(){
auto lockGuard =reader_writer::scoped_lock_read(lock);
return m_height;
}
void SetWidth(size_t width){
auto lockGuard =reader_writer::scoped_lock(lock);
m_width = width;
}
void SetHeight(size_t height){
auto lockGuard =reader_writer::scoped_lock(lock)
m_height = height;
}
};
什么我做此处是作为成员变量添加一个 read_writer_lock 然后装饰与读取器或编写器锁定版本的所有相应的方法。我还使用 scoped_lock 对象确保锁不保持期间的异常。获取的所有方法现在都获取该读者锁，并设置方法都获取写入锁定。从技术上讲，这种方法看起来像是正确的但设计是实际上不正确而且是脆弱总体因为组合时, 其接口不是线程安全。特别是，如果具有下面的代码我可能已损坏的状态：
Thread1{
                SharedWidget.GetWidth();
                SharedWidget.GetHeight();
}
Thread2{
                SharedWidget.SetWidth();
                SharedWidget.SetHeight();
}
Thread1 和 Thread2 调用可以交错，因为Thread1 可以获取读取的锁定的 GetWidth，然后 GetHeight 称为之前，SetWidth 和 SetHeight 可能同时执行。因此除了保护数据，您必须确保该数据接口也是正确的；这是最害虫争用条件的类型之一，因为代码看起来正确，错误很难跟踪中。简单解决方案，我看到这种情况下通常涉及引入锁定方法在对象自身上的或者更糟糕的是，锁存储其他地方，开发人员需要获得访问该小工具时，请记住。有时，将使用这两种方法。
一个更容易的方法是确保接口可以交错安全而不公开此能够消除之间交错调用对象的状态。您可能决定发展您的界面提供 GetDimensions 和 UpdateDimensions 方法的图 3 所示。此接口现在是不太可能导致令人惊讶的行为，因为该方法不允许公开 interleavings 不安全的。
图 3 的接口以 GetDimensions 和 UpdateDimensions 方法 A 鐗堟湰
struct WidgetDimensions
{
size_t width;
size_t height;
};
class LockedWidgetEx{
WidgetDimensions m_dimensions;
reader_writer_lock lock;
public:
LockedWidgetEx(size_t w, size_t h):
m_dimensions.width(w),m_dimensions.height(h){};
WidgetDimensions GetDimensions(){
auto lockGuard =reader_writer::scoped_lock_read(lock);
return m_dimensions;
}
void UpdateDimensions(size_t width, size_theight){
auto lockGuard =reader_writer::scoped_lock(lock);
m_dimensions.width = width;
m_dimensions.height = height;
}
};
管理共享的状态的消息块
现在让我们采用一看如何库代理可使管理共享状态更容易和代码有点更可靠。称为密钥从代理库类对于管理共享的变量 overwrite_buffer < T >，存储单个的更新值并接收的返回的最新副本的值时非常有用的 ；哪些存储和返回值的单个副本，时接收的 single_assignment < T >，调用，但是，如常量，可以分配一次 ；存储无限 （允许内存） 的项目和，像一个 FIFO 队列unbounded_buffer < T >，dequeues
 最旧项和时接收调用。
我首先使用一个 overwrite_buffer < T >。 在小部件类，我将首先将m_dimensions 成员变量替换 overwrite_buffer < WidgetDimensions >，然后我将删除方法的显式锁和它们替换为相应的发送和接收呼叫。仍需要担心安全，我们的接口，但不能再有记住要锁定数据。下面是此代码中的显示方式。这是代码的实际稍少比锁定的版本和为串行版本相同的行数：
class AgentsWidget{
   overwrite_buffer<WidgetDimensions> m_dimensionBuf;
public:
   AgentsWidget(size_t w, size_t h){
       send(&m_dimensionBuf,WidgetDimensions(w,h));
   };
   WidgetDimensions GetDimensions(){
       return receive(&m_dimensionBuf);
    }
   void UpdateDimensions(size_t width, size_t height){
       send(&m_dimensionBuf,WidgetDimensions(w,h));
    }
};
还有一个细微语义区别此处 reader_writer 锁实现。在 overwrite_buffer 允许 UpdateDimensions 调用调用维度的过程中发生。这样实际上不阻止这些的调用的过程中，但 GetDimensions 调用可能会稍有过期。值得指出问题以及，锁定版本中存在因为一旦您获得维度，它们可能已过期。已在此处做的一切。移除阻塞的调用
一个 unbounded_buffer 也很有用的 Widget 类。设想一下刚才描述的语义不同之处细微是非常重要。渚嬪的方式  如果您有您希望确保的对象的实例被访问只有一个线程一次，您可以使用 unbounded_buffer 为管理该对象的访问权限的对象持有者。若要将此应用于 Widget 类，您可以删除 m_dimensions 并替换为 unbounded_buffer < WidgetDimension >然后使用通过GetDimensions
 UpdateDimensions 来调用该缓冲区。此处挑战是确保没有一个可以获得一个值，从我们的小工具正在更新时。这是通过清空该 unbounded_buffer 以便 GetDimension 对的调用将阻止发生更新等待实现的。如图 4 所示。 GetDimensions 和 UpdateDimensions 块，等待该维度的变量的独占访问。
图 4 清空 Unbounded_Buffer
class AgentsWidget2{
unbounded_buffer<WidgetDimensions>m_dimensionBuf;
public:
AgentsWidget2(size_t w, size_t h){
send(&m_dimensionBuf,WidgetDimensions(w,h));
};
WidgetDimensions GetDimensions(){
//get the current value
WidgetDimensions value =receive(&m_dimensionBuf);
//and put a copy of it right back in theunbounded_buffer
send(&m_dimensionBuf,value);
//return a copy of the value
return WidgetDimensions(value);
}
void UpdateDimensions (size_t width, size_theight){
WidgetDimensions oldValue =receive(&m_dimensionBuf);
send(&m_dimensionBuf,WidgetDimensions(width,height));
}
};
它实际上有关 Coordinating 访问的数据
我想要压力 Widget 类有关一件事情：确保的方法和可以同时访问数据的工作"安全"一起至关重要。通常，这是通过可以获得协调状态而不是通过锁定的方法或对象的访问。从一个纯"系列的代码"角度来看，您不会看到一个大赢得通过锁定的示例中，并且特别，第二个示例甚至可能有点更多的代码。什么，获得但是，是一个更安全的设计，并与一些想法，您可以通常修改串行接口，以便该对象的内部状态不是"数据被破坏。小工具的示例中我做这通过使用消息块，并我能够保护这种方式它是安全的状态。添加方法或
 Widget 类功能在以后就很难破坏我们设置了该内部同步。使用一个成员锁可以很容易非常只是在类上添加一个方法时锁定忘了。但移动到消息传递模型的操作和使用消息块，如其自然的方式覆盖缓冲区数据和类通常可以将保持同步。
3、使用 Combinable 的线程本地 Accumulations 和初始化
在受保护访问的对象的锁或消息块，该第二个方案很适用于更大的粗细对象不经常访问的。如果读取该示例时，您认为有可能是性能问题如果紧密和并行循环中使用同步的小工具，您可能不正确。这是因为，保护共享的状态可能有问题，而且完全通用算法和真正共享状态的对象中，遗憾的是没有大量的选项以外的其他协调访问或引入一个锁。您几乎总是能找到一种重构代码或放宽依赖项取决共享的状态的算法的方法，但一次您做这几个特定但常见模式中的对象调用可组合 < T >在并行模式库中可以真正帮助出。
组合 < T >是为这三种广泛使用的情况下提供了支持的并发容器：按住线程本地变量或执行线程的线程本地初始化，线程本地变量执行关联的二进制文件操作（如求和、 最小值和组合） 和组合它们，和访问 （如一起 splicing列表） 的操作与每个线程的本地副本。 在本节中，我将解释每个这种情况下，提供了如何使用它们的示例。
控制线程的线程本地变量或正在执行线程的线程本地初始化
第一个用例提到的组合 < T >用于存放一个线程的线程本地变量。是一种相对较常见存储全局状态的线程的本地副本。渚嬪的方式  在彩色的射线 tracers 在应用程序像我们的示例包 （code.msdn.microsoft.com/concrtextras） 中或在与.NET 4.0 （code.msdn.microsoft.com/ParExtSamples） 有并行开发的示例是可视化，并行线程标记每个行的选项。 在演示的本机版本，这是通过使用一个包含线程本地颜色的组合对象。 
您可以通过使用线程本地存储区 (TLS)，当然，保存一个线程的线程本地变量但存在一些缺点，最值得注意的是生存期管理和可见性，并将这些手动在手动定位。若要用于 TLS 需要首先分配 TlsAlloc 具有索引、分配您的对象和然后将一个指向您对象的指针存储在与 TlsSetValue 索引。然后时您的线程正在退出，, 您需要确保被释放的对象。 (TlsFree 称为自动。执行此操作线程，并确保每一次或两次没有任何泄漏因提前退出或异常不是该挑战但是，如果您的应用程序需要数十或数百个这些项，一个不同的方法很可能更好地。
组合 < T >可以使用保存以及，一个线程的本地值，但各个对象的生存期密切相关的可组合 < T > 生存期被自动的项，和初始化的很多。您只需通过调用 combinable::local 方法返回一个对本地对象的引用的访问线程本地值。下面是示例使用 task_group，但可以进行此操作与 Win32 线程以及：
combinable<int> values;
auto task = [&](){
                values.local() =GetCurrentThreadId();
                printf("hello from thread:%d\n",values.local());
};
task_group tasks;
tasks.run(task);
//run a copy of the task on the main thread
task();
tasks.wait();
我提到过该线程的线程本地初始化还可以获得与组合。如果渚嬪的方式  需要初始化它使用的每个线程上调用库，您可以创建执行它的构造函数初始化的类。 然后，在每个线程在首次使用，将对进行库调用，但它将在后续使用跳过。示例如下：
class ThreadInitializationClass
{
public:
     ThreadInitializationClass(){
           ThreadInitializationRoutine();
     };
};
...
//a combinable object will initialize these
combinable<ThreadInitializationClass>libraryInitializationToken;
...
//initialize the library if it hasn't beenalready on this thread
ThreadInitializationClass& threadInit =libraryInitalizationToken.local();
在并行循环中执行缩减
组合对象的另一个主要方案是执行线程的线程本地缩减，或线程本地 accumulations。特别是，可以避免特定类型的争用条件时，并行化循环或递归使用组合的并行遍历。下面是一个非常简单示例的不希望显示 speed-ups。下面的代码演示一个简单的循环看起来像它可以被 parallelized parallel_for_each，除了用于访问和变量的：
int sum = 0;
for (vector<int>::iterator it =myVec.begin(); it != myVec.end(); ++it) {
   int element = *it;
   SomeFineGrainComputation(element);
   sum += element;
}
现在，而不是锁置于我们 parallel_for_each 销毁我们 speed-ups 的任何机会的我们可以使用一个可组合的对象来计算线程本地总和： 
combinable<int> localSums;
parallel_for_each(myVec.begin(),  myVec.end(), [&localSums] (int element) {
  SomeFineGrainComputation(element);
  localSums.local() += element;
});
我们已经现在成功地避免将争用情形，但有的线程本地存储在该 localSums 对象中的总和的集合，我们仍然需要提取最后的值。我们可以与合并方法采用类似于下面的一个二进制 functor 执行此操作：
int sum =localSums.combine(std::plus<int>);
在第三个用例的组合 < T >，涉及到使用该 combine_each 方法是当您需要访问每个线程的本地副本，并执行在其上的某些操作（例如，清除或错误检查下）时。 您可组合的对象是一个组合< 列表 < T > >，并且在线程中您正在构建 std::lists 或 std::sets 时另一个，更有趣的示例。在 std::lists 的情况下它们可以很容易地被 spliced 一起 list::splice ；std::sets，使用它们可以与 set::insert
 插入。
**4、转换一个现有的后台线程代理程序或任务**
假设您已经有应用程序中的背景色或辅助线程。有一些很好的原因，为什么您可能希望将该背景线程转换为任务在 PPL，或将代理程序，并这样做是相对简单。一些执行此操作的主要优点包括：
可组合性和性能。如果您的工作线程都计算占用，并且您正在考虑使用 PPL 或代理库中的其他线程将在后台线程转换工作任务使得协作与运行库中的其他任务，以避免在系统上的 oversubscription。
取消和异常处理。如果您希望能够方便地取消工作线程上的或具有一个 well-described 的异常处理机制，一个 task_group 将具有内置的这些功能。
控制流和状态管理。如果要管理您的线程（启动或已完成的示例）的状态或状态是从辅助线程有效不可分割的对象实现的代理可能很有用。
Task_group 提供了取消和异常处理
在第一个的情况下我们探讨了所需安排一个 task_group 使用：实质上打包到（使用一个 lambda，一个 std::bind 或一个自定义函数的对象）一个 functor 工作和计划它与 task_group::run 方法。取消，实际上，相关的异常处理语义，我没有描述的内容。
图 5 MyAgentClass 的实现
class MyAgentClass : public agent{
public:
MyAgentClass (){
}
AgentsWidget widget;
void run(){
//run is started asynchronously whenagent::start is called
//...
//set status to complete
agent::done();
}
};
第一次，我将说明简单的语义。如果您的代码调用 task_group::cancel，或者某个任务引发未捕获的异常取消是有效的 task_group。时取消实际上，还没有启动的 task_group 上的任务不会启动，允许取消上一个 task_group 方便而快速的计划的工时。取消不会中断正在运行或阻止的任务，使正在运行的任务可以查询取消状态与 task_group::is_canceling 方法帮助器函数
is_current_task_group_canceling。 下面是一个简短的示例：
task_group tasks;   
tasks.run([](){
   ...
   if(is_current_task_group_canceling())
    {
       //cleanup then return
       ...
       return;
    }
});
tasks.cancel();
tasks.wait();
异常处理会影响取消，因为在一个 task_group 捕获的异常触发的 task_group 上取消。如果未捕获的异常，task_group 实际用于将 std::exception_ptr 打包在线程引发异常。以后，调用 task_group::wait 时, 引发异常调用等待的线程上。
实现异步代理
代理库提供了使用一个 task_group 的替代方法：替换线程代理程序的基类。如果您的线程中有大量的特定于线程的状态和对象的代理可能方案的一个更好地适应。抽象的代理类是参与者模式； 的实现预期的用途是实现您自己代理从派生的类，然后将任何状态可能您的参与者（或线程） 封装到该代理。 如果都应是可公开访问的字段本指南是将它们公开为消息块或源和目标，并使用消息传递与该代理进行通讯。
实现的代理程序将需要从代理基类派生类，然后重写虚拟方法运行。然后可以通过调用 agent::start，作为线程类似的任务中生成运行的方法来启动代理。优点是线程本地状态现在可以存储在类中。这样更容易的同步状态，在线程间的特别是如果状态存储在消息块中。图 5 显示具有公开的成员变量的类型 AgentsWidget 实现的示例。
请注意，我已设置代理的状态为运行的方法退出操作完成。这样不仅能启动，但也在等待代理。此外，可以通过调用 agent::status 查询代理的当前状态。启动并等待代理类是很简单如下面的代码所示：
MyAgentClass MyAgent;
//start the agent
MyAgent.start();
//do something else
...
//wait for the agent to finish
MyAgent.wait(&MyAgent);
额外项:与并行 parallel_sort 进行排序
最后，希望建议的并行化，这次不在 PPL 或在代理库但从我们的示例包在 code.msdn.microsoft.com/concrtextras 另一个可能很容易的点。 并行快速排序是我们用于两个示例说明如何在并行化任务，递归除和征服算法，并该示例包中包含的并行快速排序实现。并行的排序可以显示 speed-ups 是否大量的有些昂贵，作为使用字符串比较操作所在的项目进行排序。它可能不会显示 speed-ups 小编号或项目的排序内置类型，如整数和双精度数。下面是可以如何使用它的一个示例：
//from the sample pack
#include "parallel_algorithms.h"
int main()
using namespace concurrency_extras;
{
                vector<string> strings;
                //populate the strings
                ...
               parallel_sort(strings.begin(),strings.end());
}
