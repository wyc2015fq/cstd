
# JDK源码研究Jstack，JMap，threaddump，dumpheap的原理 - 无界 - CSDN博客

置顶2019年03月13日 19:53:11[21aspnet](https://me.csdn.net/21aspnet)阅读数：207个人分类：[Java																](https://blog.csdn.net/21aspnet/article/category/1877583)



openjdk源码地址：
[https://jdk.java.net/java-se-ri/8](https://jdk.java.net/java-se-ri/8)
[https://download.java.net/openjdk/jdk8u40/ri/openjdk-8u40-src-b25-10_feb_2015.zip](https://download.java.net/openjdk/jdk8u40/ri/openjdk-8u40-src-b25-10_feb_2015.zip)
如果国外网速不行，这里有下好放csdn上的：[ JDK源码 openjdk-8u40-src-b25-10_feb_2015.zip](https://download.csdn.net/download/21aspnet/11028742)
线上源码：[http://hg.openjdk.java.net/jdk8u/jdk8u/hotspot/file/6e2900603bc6/](http://hg.openjdk.java.net/jdk8u/jdk8u/hotspot/file/6e2900603bc6/)
如果官网很慢，可以直接CSDN下载：[https://download.csdn.net/download/21aspnet/11028742](https://download.csdn.net/download/21aspnet/11028742)
**JEP 0：JEP指数：**[http://openjdk.java.net/jeps/0](http://openjdk.java.net/jeps/0)
## [Java8官方文档总目录](https://docs.oracle.com/javase/8/)
[Java8语言规范](https://docs.oracle.com/javase/specs/jls/se8/html/index.html)[Java8虚拟机规范](https://docs.oracle.com/javase/specs/jvms/se8/html/index.html)[HotSpot虚拟机垃圾收集调整指南](https://docs.oracle.com/javase/8/docs/technotes/guides/vm/gctuning)
[Java8 API](https://docs.oracle.com/javase/8/docs/api/)
--------------
[https://jdk.java.net/java-se-ri/12](https://jdk.java.net/java-se-ri/12)
[https://download.java.net/openjdk/jdk12/ri/openjdk-12+32_src.zip](https://download.java.net/openjdk/jdk12/ri/openjdk-12+32_src.zip)
----------------
## [Java语言和虚拟机规范[各语言总目录]](https://docs.oracle.com/javase/specs/index.html)
其他jdk文档地址：[https://docs.oracle.com/en/java/javase/index.html](https://docs.oracle.com/en/java/javase/index.html)
----
扩展阅读：虽然是历史资源，但是还是闪烁着智慧的
[Oracle JRockit文档](https://www.oracle.com/technetwork/cn/middleware/jrockit/overview/index.html)[Oracle JRockit在线文档](https://docs.oracle.com/cd/E13150_01/jrockit_jvm/jrockit/webdocs/index.html)【有参考价值】
[Oracle JRockit联机文档库4.0版](https://docs.oracle.com/cd/E15289_01/index.htm)【很有价值】
----
## [Java12【总目录】](https://docs.oracle.com/en/java/javase/12/)
[Java™教程](https://docs.oracle.com/javase/tutorial/tutorialLearningPaths.html)【有参考价值】：
[学习Java语言](https://docs.oracle.com/javase/tutorial/java/index.html)[异常](https://docs.oracle.com/javase/tutorial/essential/exceptions/index.html)[基本I/O](https://docs.oracle.com/javase/tutorial/essential/io/index.html)[并发](https://docs.oracle.com/javase/tutorial/essential/concurrency/index.html)[泛型](https://docs.oracle.com/javase/tutorial/extra/generics/index.html)[反射](https://docs.oracle.com/javase/tutorial/reflect/index.html)[集合](https://docs.oracle.com/javase/tutorial/collections/index.html)[序列化](https://docs.oracle.com/en/java/javase/12/docs/specs/serialization/index.html)
[Lambda表达式](https://docs.oracle.com/javase/tutorial/java/javaOO/lambdaexpressions.html)[聚合操作](https://docs.oracle.com/javase/tutorial/collections/streams/index.html)
[垃圾收集调整](https://www.oracle.com/pls/topic/lookup?ctx=javase12&id=JSGCT-GUID-326EB4CF-8C8C-4267-8355-21AB04F0D304)【重要】[Java虚拟机指南](https://www.oracle.com/pls/topic/lookup?ctx=javase12&id=JSJVM-GUID-982B244A-9B01-479A-8651-CB6475019281)【很重要】[JRockit到HotSpot迁移指南](https://www.oracle.com/pls/topic/lookup?ctx=javase12&id=JRHMG107)【有参考价值】
[故障排除指南](https://www.oracle.com/pls/topic/lookup?ctx=javase12&id=JSTGD107)【重要】
[https://docs.oracle.com/en/java/javase/11/](https://docs.oracle.com/en/java/javase/11/)和12类似
----
# 分析Jstack源码
这是起点>>>
\openjdk\jdk\src\share\classes\sun\tools目录下
![](https://img-blog.csdnimg.cn/20190313201539123.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
常见的jvm命令jstack  jmap   jps都在这里
```python
package sun.tools.jstack;
import java.lang.reflect.Method;
import java.lang.reflect.Constructor;
import java.io.IOException;
import java.io.InputStream;
import com.sun.tools.attach.VirtualMachine;
import com.sun.tools.attach.AttachNotSupportedException;
import sun.tools.attach.HotSpotVirtualMachine;
/*
 * This class is the main class for the JStack utility. It parses its arguments
 * and decides if the command should be executed by the SA JStack tool or by
 * obtained the thread dump from a target process using the VM attach mechanism
 */
public class JStack {
    public static void main(String[] args) throws Exception {
        if (args.length == 0) {
            usage(1); // no arguments
        }
        boolean useSA = false;
        boolean mixed = false;
        boolean locks = false;
        // Parse the options (arguments starting with "-" )
        int optionCount = 0;
        while (optionCount < args.length) {
            String arg = args[optionCount];
            if (!arg.startsWith("-")) {
                break;
            }
            if (arg.equals("-help") || arg.equals("-h")) {
                usage(0);
            }
            else if (arg.equals("-F")) {
                useSA = true;
            }
            else {
                if (arg.equals("-m")) {
                    mixed = true;
                } else {
                    if (arg.equals("-l")) {
                       locks = true;
                    } else {
                        usage(1);
                    }
                }
            }
            optionCount++;
        }
        // mixed stack implies SA tool
        if (mixed) {
            useSA = true;
        }
        // Next we check the parameter count. If there are two parameters
        // we assume core file and executable so we use SA.
        int paramCount = args.length - optionCount;
        if (paramCount == 0 || paramCount > 2) {
            usage(1);
        }
        if (paramCount == 2) {
            useSA = true;
        } else {
            // If we can't parse it as a pid then it must be debug server
            if (!args[optionCount].matches("[0-9]+")) {
                useSA = true;
            }
        }
        // now execute using the SA JStack tool or the built-in thread dumper
        if (useSA) {
            // parameters (<pid> or <exe> <core>
            String params[] = new String[paramCount];
            for (int i=optionCount; i<args.length; i++ ){
                params[i-optionCount] = args[i];
            }
            runJStackTool(mixed, locks, params);
        } else {
            // pass -l to thread dump operation to get extra lock info
            String pid = args[optionCount];
            String params[];
            if (locks) {
                params = new String[] { "-l" };
            } else {
                params = new String[0];
            }
            runThreadDump(pid, params);
        }
    }
```
根据传入参数的不同，有两种实现机制，一种是基于SA，一种是通过attach。
下面是jmap部分代码下面是用的最多的：
```python
// Invoke SA tool  with the given arguments
    private static void runTool(String option, String args[]) throws Exception {
        String[][] tools = {
            { "-pmap",          "sun.jvm.hotspot.tools.PMap"             },
            { "-heap",          "sun.jvm.hotspot.tools.HeapSummary"      },
            { "-heap:format=b", "sun.jvm.hotspot.tools.HeapDumper"       },
            { "-histo",         "sun.jvm.hotspot.tools.ObjectHistogram"  },
            { "-clstats",       "sun.jvm.hotspot.tools.ClassLoaderStats" },
            { "-finalizerinfo", "sun.jvm.hotspot.tools.FinalizerInfo"    },
        };
```
-------------------
都是通过**executeCommand**来实现的，例如：datadump、threaddump、dumpheap、inspectheap、jcmd等，而最终的execute()在Linux上是由类LinuxVirtualMachine来完成。
```python
public abstract class HotSpotVirtualMachine extends VirtualMachine {
...
    // --- HotSpot specific methods ---
    // same as SIGQUIT
    public void localDataDump() throws IOException {
        executeCommand("datadump").close();
    }
    // Remote ctrl-break. The output of the ctrl-break actions can
    // be read from the input stream.
    public InputStream remoteDataDump(Object ... args) throws IOException {
        return executeCommand("threaddump", args);
    }
    // Remote heap dump. The output (error message) can be read from the
    // returned input stream.
    public InputStream dumpHeap(Object ... args) throws IOException {
        return executeCommand("dumpheap", args);
    }
    // Heap histogram (heap inspection in HotSpot)
    public InputStream heapHisto(Object ... args) throws IOException {
        return executeCommand("inspectheap", args);
    }
    // set JVM command line flag
    public InputStream setFlag(String name, String value) throws IOException {
        return executeCommand("setflag", name, value);
    }
    // print command line flag
    public InputStream printFlag(String name) throws IOException {
        return executeCommand("printflag", name);
    }
    public InputStream executeJCmd(String command) throws IOException {
        return executeCommand("jcmd", command);
    }
    // -- Supporting methods
```
-----------------------------------
jstack命令首先会attach到目标jvm进程，产生VirtualMachine类；
linux系统下，其实现类为LinuxVirtualMachine，调用其remoteDataDump方法，打印堆栈信息；
VirtualMachine是如何连接到目标JVM进程的呢？
具体的实现逻辑在sun.tools.attach.LinuxVirtualMachine的构造函数：
```python
// The patch to the socket file created by the target VM
    String path;
    /**
     * Attaches to the target VM
     */
    LinuxVirtualMachine(AttachProvider provider, String vmid)
        throws AttachNotSupportedException, IOException
    {
        super(provider, vmid);
        // This provider only understands pids
        int pid;
        try {
            pid = Integer.parseInt(vmid);
        } catch (NumberFormatException x) {
            throw new AttachNotSupportedException("Invalid process identifier");
        }
        // Find the socket file. If not found then we attempt to start the
        // attach mechanism in the target VM by sending it a QUIT signal.
        // Then we attempt to find the socket file again.
        path = findSocketFile(pid);
        if (path == null) {
            File f = createAttachFile(pid);
            try {
                // On LinuxThreads each thread is a process and we don't have the
                // pid of the VMThread which has SIGQUIT unblocked. To workaround
                // this we get the pid of the "manager thread" that is created
                // by the first call to pthread_create. This is parent of all
                // threads (except the initial thread).
                if (isLinuxThreads) {
                    int mpid;
                    try {
                        mpid = getLinuxThreadsManager(pid);
                    } catch (IOException x) {
                        throw new AttachNotSupportedException(x.getMessage());
                    }
                    assert(mpid >= 1);
                    sendQuitToChildrenOf(mpid);
                } else {
                    sendQuitTo(pid);
                }
                // give the target VM time to start the attach mechanism
                int i = 0;
                long delay = 200;
                int retries = (int)(attachTimeout() / delay);
                do {
                    try {
                        Thread.sleep(delay);
                    } catch (InterruptedException x) { }
                    path = findSocketFile(pid);
                    i++;
                } while (i <= retries && path == null);
                if (path == null) {
                    throw new AttachNotSupportedException(
                        "Unable to open socket file: target process not responding " +
                        "or HotSpot VM not loaded");
                }
            } finally {
                f.delete();
            }
        }
        // Check that the file owner/permission to avoid attaching to
        // bogus process
        checkPermissions(path);
        // Check that we can connect to the process
        // - this ensures we throw the permission denied error now rather than
        // later when we attempt to enqueue a command.
        int s = socket();
        try {
            connect(s, path);
        } finally {
            close(s);
        }
    }
    /**
     * Detach from the target VM
     */
    public void detach() throws IOException {
        synchronized (this) {
            if (this.path != null) {
                this.path = null;
            }
        }
    }
```
查找/tmp目录下是否存在".java_pid"+pid文件；
如果文件不存在，则首先创建"/proc/" + pid + "/cwd/" + ".attach_pid" + pid文件，然后通过kill命令发送SIGQUIT信号给目标JVM进程；
目标JVM进程接收到信号之后，会在/tmp目录下创建".java_pid"+pid文件
当发现/tmp目录下存在".java_pid"+pid文件，LinuxVirtualMachine会通过connect系统调用连接到该文件描述符，后续通过该fd进行双方的通讯；
JVM接受SIGQUIT信号的相关逻辑在os.cpp文件的os::signal_init方法：
jstack是通过调用remoteDataDump方法实现的，该方法就是通过往前面提到的fd中写入threaddump指令，读取返回结果，从而得到目标JVM的堆栈信息。
----------------------------------
jstack等命令会与jvm进程建立socket连接，发送对应的指令(jstack发送了threaddump指令)，然后再读取返回的数据。
```python
/**
     * Execute the given command in the target VM.
     */
    InputStream execute(String cmd, Object ... args) throws AgentLoadException, IOException {
        assert args.length <= 3;                // includes null
        // did we detach?
        String p;
        synchronized (this) {
            if (this.path == null) {
                throw new IOException("Detached from target VM");
            }
            p = this.path;
        }
        // create UNIX socket
        int s = socket();
        // connect to target VM
        try {
            connect(s, p);
        } catch (IOException x) {
            close(s);
            throw x;
        }
        IOException ioe = null;
        // connected - write request
        // <ver> <cmd> <args...>
        try {
            writeString(s, PROTOCOL_VERSION);
            writeString(s, cmd);
            for (int i=0; i<3; i++) {
                if (i < args.length && args[i] != null) {
                    writeString(s, (String)args[i]);
                } else {
                    writeString(s, "");
                }
            }
        } catch (IOException x) {
            ioe = x;
        }

        // Create an input stream to read reply
        SocketInputStream sis = new SocketInputStream(s);
        // Read the command completion status
        int completionStatus;
        try {
            completionStatus = readInt(sis);
        } catch (IOException x) {
            sis.close();
            if (ioe != null) {
                throw ioe;
            } else {
                throw x;
            }
        }
        if (completionStatus != 0) {
            // read from the stream and use that as the error message
            String message = readErrorMessage(sis);
            sis.close();
            // In the event of a protocol mismatch then the target VM
            // returns a known error so that we can throw a reasonable
            // error.
            if (completionStatus == ATTACH_ERROR_BADVERSION) {
                throw new IOException("Protocol mismatch with target VM");
            }
            // Special-case the "load" command so that the right exception is
            // thrown.
            if (cmd.equals("load")) {
                throw new AgentLoadException("Failed to load agent library");
            } else {
                if (message == null) {
                    throw new AttachOperationFailedException("Command failed in target VM");
                } else {
                    throw new AttachOperationFailedException(message);
                }
            }
        }
        // Return the input stream so that the command output can be read
        return sis;
    }
```
-----------------
下面是C++部分
\openjdk\hotspot\src\share\vm\services\attachListener.hpp
```python
// Table to map operation names to functions.
// names must be of length <= AttachOperation::name_length_max
static AttachOperationFunctionInfo funcs[] = {
  { "agentProperties",  get_agent_properties },
  { "datadump",         data_dump },
  { "dumpheap",         dump_heap },
  { "load",             JvmtiExport::load_agent_library },
  { "properties",       get_system_properties },
  { "threaddump",       thread_dump },
  { "inspectheap",      heap_inspection },
  { "setflag",          set_flag },
  { "printflag",        print_flag },
  { "jcmd",             jcmd },
  { NULL,               NULL }
};
```
\openjdk\hotspot\src\os\linux\vm\attachListener_linux.cpp
侦听socket
```python
// The attach mechanism on Linux uses a UNIX domain socket. An attach listener
// thread is created at startup or is created on-demand via a signal from
// the client tool. The attach listener creates a socket and binds it to a file
// in the filesystem. The attach listener then acts as a simple (single-
// threaded) server - it waits for a client to connect, reads the request,
// executes it, and returns the response to the client via the socket
// connection.
//
// As the socket is a UNIX domain socket it means that only clients on the
// local machine can connect. In addition there are two other aspects to
// the security:
// 1. The well known file that the socket is bound to has permission 400
// 2. When a client connect, the SO_PEERCRED socket option is used to
//    obtain the credentials of client. We check that the effective uid
//    of the client matches this process.
....
// Initialization - create a listener socket and bind it to a file
int LinuxAttachListener::init() {
  char path[UNIX_PATH_MAX];          // socket file
  char initial_path[UNIX_PATH_MAX];  // socket file during setup
  int listener;                      // listener socket (file descriptor)
  // register function to cleanup
  ::atexit(listener_cleanup);
  int n = snprintf(path, UNIX_PATH_MAX, "%s/.java_pid%d",
                   os::get_temp_directory(), os::current_process_id());
  if (n < (int)UNIX_PATH_MAX) {
    n = snprintf(initial_path, UNIX_PATH_MAX, "%s.tmp", path);
  }
  if (n >= (int)UNIX_PATH_MAX) {
    return -1;
  }
  // create the listener socket
  listener = ::socket(PF_UNIX, SOCK_STREAM, 0);
  if (listener == -1) {
    return -1;
  }
  // bind socket
  struct sockaddr_un addr;
  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path, initial_path);
  ::unlink(initial_path);
  int res = ::bind(listener, (struct sockaddr*)&addr, sizeof(addr));
  if (res == -1) {
    ::close(listener);
    return -1;
  }
  // put in listen mode, set permissions, and rename into place
  res = ::listen(listener, 5);
  if (res == 0) {
      RESTARTABLE(::chmod(initial_path, S_IREAD|S_IWRITE), res);
      if (res == 0) {
          res = ::rename(initial_path, path);
      }
  }
  if (res == -1) {
    ::close(listener);
    ::unlink(initial_path);
    return -1;
  }
  set_path(path);
  set_listener(listener);
  return 0;
}
....
```
再就是一个个命令对应去看具体代码实现，以dumpheap为例：
\openjdk\hotspot\src\share\vm\services\heapDumper.cpp
```python
// The VM operation that dumps the heap. The dump consists of the following
// records:
//
//  HPROF_HEADER
//  [HPROF_UTF8]*
//  [HPROF_LOAD_CLASS]*
//  [[HPROF_FRAME]*|HPROF_TRACE]*
//  [HPROF_GC_CLASS_DUMP]*
//  HPROF_HEAP_DUMP
//
// The HPROF_TRACE records represent the stack traces where the heap dump
// is generated and a "dummy trace" record which does not include
// any frames. The dummy trace record is used to be referenced as the
// unknown object alloc site.
//
// The HPROF_HEAP_DUMP record has a length following by sub-records. To allow
// the heap dump be generated in a single pass we remember the position of
// the dump length and fix it up after all sub-records have been written.
// To generate the sub-records we iterate over the heap, writing
// HPROF_GC_INSTANCE_DUMP, HPROF_GC_OBJ_ARRAY_DUMP, and HPROF_GC_PRIM_ARRAY_DUMP
// records as we go. Once that is done we write records for some of the GC
// roots.

// HPROF_TRACE记录表示堆转储的堆栈跟踪
//生成并且“虚拟跟踪”记录不包括
//任何帧 虚拟跟踪记录用于引用
//未知对象分配站点。
//
// HPROF_HEAP_DUMP记录的子记录长度如下。 允许
//在一次传递中生成堆转储，我们记住了它的位置
//转储长度并在写完所有子记录后修复它。
//为了生成子记录，我们迭代堆，写
// HPROF_GC_INSTANCE_DUMP，HPROF_GC_OBJ_ARRAY_DUMP和HPROF_GC_PRIM_ARRAY_DUMP
//我们去的记录。 完成后，我们会为某些GC编写记录
//根
void VM_HeapDumper::doit() {
  HandleMark hm;
  CollectedHeap* ch = Universe::heap();
  ch->ensure_parsability(false); // must happen, even if collection does
                                 // not happen (e.g. due to GC_locker)
  if (_gc_before_heap_dump) {
    if (GC_locker::is_active()) {
      warning("GC locker is held; pre-heapdump GC was skipped");
    } else {
      ch->collect_as_vm_thread(GCCause::_heap_dump);
    }
  }
  // At this point we should be the only dumper active, so
  // the following should be safe.
  set_global_dumper();
  set_global_writer();
  // Write the file header - use 1.0.2 for large heaps, otherwise 1.0.1
  size_t used = ch->used();
  const char* header;
  if (used > (size_t)SegmentedHeapDumpThreshold) {
    set_segmented_dump();
    header = "JAVA PROFILE 1.0.2";
  } else {
    header = "JAVA PROFILE 1.0.1";
  }
  // header is few bytes long - no chance to overflow int
  writer()->write_raw((void*)header, (int)strlen(header));
  writer()->write_u1(0); // terminator
  writer()->write_u4(oopSize);
  writer()->write_u8(os::javaTimeMillis());
  // HPROF_UTF8 records
  SymbolTableDumper sym_dumper(writer());
  SymbolTable::symbols_do(&sym_dumper);
  // write HPROF_LOAD_CLASS records
  ClassLoaderDataGraph::classes_do(&do_load_class);
  Universe::basic_type_classes_do(&do_load_class);
  // write HPROF_FRAME and HPROF_TRACE records
  // this must be called after _klass_map is built when iterating the classes above.
  dump_stack_traces();
  // write HPROF_HEAP_DUMP or HPROF_HEAP_DUMP_SEGMENT
  write_dump_header();
  // Writes HPROF_GC_CLASS_DUMP records
  ClassLoaderDataGraph::classes_do(&do_class_dump);
  Universe::basic_type_classes_do(&do_basic_type_array_class_dump);
  check_segment_length();
  // writes HPROF_GC_INSTANCE_DUMP records.
  // After each sub-record is written check_segment_length will be invoked. When
  // generated a segmented heap dump this allows us to check if the current
  // segment exceeds a threshold and if so, then a new segment is started.
  // The HPROF_GC_CLASS_DUMP and HPROF_GC_INSTANCE_DUMP are the vast bulk
  // of the heap dump.
  HeapObjectDumper obj_dumper(this, writer());
  Universe::heap()->safe_object_iterate(&obj_dumper);
  // HPROF_GC_ROOT_THREAD_OBJ + frames + jni locals
  do_threads();
  check_segment_length();
  // HPROF_GC_ROOT_MONITOR_USED
  MonitorUsedDumper mon_dumper(writer());
  ObjectSynchronizer::oops_do(&mon_dumper);
  check_segment_length();
  // HPROF_GC_ROOT_JNI_GLOBAL
  JNIGlobalsDumper jni_dumper(writer());
  JNIHandles::oops_do(&jni_dumper);
  check_segment_length();
  // HPROF_GC_ROOT_STICKY_CLASS
  StickyClassDumper class_dumper(writer());
  SystemDictionary::always_strong_classes_do(&class_dumper);
  // fixes up the length of the dump record. In the case of a segmented
  // heap then the HPROF_HEAP_DUMP_END record is also written.
  end_of_dump();
  // Now we clear the global variables, so that a future dumper might run.
  clear_global_dumper();
  clear_global_writer();
}
```
`void VM_HeapDumper::dump_stack_traces() {
  // write a HPROF_TRACE record without any frames to be referenced as object alloc sites
  DumperSupport::write_header(writer(), HPROF_TRACE, 3*sizeof(u4));
  writer()->write_u4((u4) STACK_TRACE_ID);
  writer()->write_u4(0);                    // thread number
  writer()->write_u4(0);                    // frame count
  _stack_traces = NEW_C_HEAP_ARRAY(ThreadStackTrace*, Threads::number_of_threads(), mtInternal);
  int frame_serial_num = 0;
  for (JavaThread* thread = Threads::first(); thread != NULL ; thread = thread->next()) {
    oop threadObj = thread->threadObj();
    if (threadObj != NULL && !thread->is_exiting() && !thread->is_hidden_from_external_view()) {
      // dump thread stack trace
      ThreadStackTrace* stack_trace = new ThreadStackTrace(thread, false);
      stack_trace->dump_stack_at_safepoint(-1);
      _stack_traces[_num_threads++] = stack_trace;
      // write HPROF_FRAME records for this thread's stack trace
      int depth = stack_trace->get_stack_depth();
      int thread_frame_start = frame_serial_num;
      int extra_frames = 0;
      // write fake frame that makes it look like the thread, which caused OOME,
      // is in the OutOfMemoryError zero-parameter constructor
      if (thread == _oome_thread && _oome_constructor != NULL) {
        int oome_serial_num = _klass_map->find(_oome_constructor->method_holder());
        // the class serial number starts from 1
        assert(oome_serial_num > 0, "OutOfMemoryError class not found");
        DumperSupport::dump_stack_frame(writer(), ++frame_serial_num, oome_serial_num,
                                        _oome_constructor, 0);
        extra_frames++;
      }
      for (int j=0; j < depth; j++) {
        StackFrameInfo* frame = stack_trace->stack_frame_at(j);
        Method* m = frame->method();
        int class_serial_num = _klass_map->find(m->method_holder());
        // the class serial number starts from 1
        assert(class_serial_num > 0, "class not found");
        DumperSupport::dump_stack_frame(writer(), ++frame_serial_num, class_serial_num, m, frame->bci());
      }
      depth += extra_frames;
      // write HPROF_TRACE record for one thread
      DumperSupport::write_header(writer(), HPROF_TRACE, 3*sizeof(u4) + depth*oopSize);
      int stack_serial_num = _num_threads + STACK_TRACE_ID;
      writer()->write_u4(stack_serial_num);      // stack trace serial number
      writer()->write_u4((u4) _num_threads);     // thread serial number
      writer()->write_u4(depth);                 // frame count
      for (int j=1; j <= depth; j++) {
        writer()->write_id(thread_frame_start + j);
      }
    }
  }
}`
```python
// dump the heap to given path.
PRAGMA_FORMAT_NONLITERAL_IGNORED_EXTERNAL
int HeapDumper::dump(const char* path) {
  assert(path != NULL && strlen(path) > 0, "path missing");
  // print message in interactive case
  if (print_to_tty()) {
    tty->print_cr("Dumping heap to %s ...", path);
    timer()->start();
  }
  // create the dump writer. If the file can be opened then bail
  DumpWriter writer(path);
  if (!writer.is_open()) {
    set_error(writer.error());
    if (print_to_tty()) {
      tty->print_cr("Unable to create %s: %s", path,
        (error() != NULL) ? error() : "reason unknown");
    }
    return -1;
  }
  // generate the dump
  VM_HeapDumper dumper(&writer, _gc_before_heap_dump, _oome);
  if (Thread::current()->is_VM_thread()) {
    assert(SafepointSynchronize::is_at_safepoint(), "Expected to be called at a safepoint");
    dumper.doit();
  } else {
    VMThread::execute(&dumper);
  }
  // close dump file and record any error that the writer may have encountered
  writer.close();
  set_error(writer.error());
  // print message in interactive case
  if (print_to_tty()) {
    timer()->stop();
    if (error() == NULL) {
      char msg[256];
      sprintf(msg, "Heap dump file created [%s bytes in %3.3f secs]",
        JLONG_FORMAT, timer()->seconds());
PRAGMA_DIAG_PUSH
PRAGMA_FORMAT_NONLITERAL_IGNORED_INTERNAL
      tty->print_cr(msg, writer.bytes_written());
PRAGMA_DIAG_POP
    } else {
      tty->print_cr("Dump file is incomplete: %s", writer.error());
    }
  }
  return (writer.error() == NULL) ? 0 : -1;
}
```
说明：本文参考了《[jstack是如何获取threaddump的](https://blog.csdn.net/myle69/article/details/81463598)》和《[java attach机制源码阅读](https://www.jianshu.com/p/96b825c50423)》这两篇都是java部分的缺少C++，然后C++部分是我加上的。
--------------------
《[OpenJDK源码学习-加载本地库](https://www.jianshu.com/p/fdf50516faa8)》
从整个加载本地库的流程来看，基本上还是调用和平台有关的函数来完成的，并在加载和卸载的时候分别调用了两个生命周期回调函数`JNI_OnLoad`和`JNI_OnUnLoad`。
以linux平台为例，简单总结一下整个so库的加载流程：
首先System.loadLibrary()被调用，开始整个加载过程。
其中调用ClassLoader对象来完成主要工作，将每个本地库封装成NativeLibrary对象，并以静态变量存到已经加载过的栈中。
执行NativeLibrary类的loadnative方法，来交给native层去指向具体的加载工作。
native层ClassLoader.c中的Java_java_lang_ClassLoader_00024NativeLibrary_load函数被调用。
在native load函数中首先使用dlopen来加载so本地库文件，并将返回的handle保存到NativeLibrary对象中。
接着查找已经加载的so本地库中的JNI_OnLoad函数，并执行它。
整个so本地库的加载流程完毕。
只有在`NativeLibrary`对象被GC回收的时候，其`finalize`方法被调用了，对应加载的本地库才会被 unload 。这种情况一般来说并不会发生，因为`NativeLibrary`对象是以静态变量的形式被保存的，而静态变量是 GC roots，一般来说都不会被回收掉的。

