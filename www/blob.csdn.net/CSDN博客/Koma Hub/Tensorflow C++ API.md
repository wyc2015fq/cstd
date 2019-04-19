# Tensorflow C++ API - Koma Hub - CSDN博客
2019年03月27日 22:11:53[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：40
**Table of Contents**
[Class tensorflow::Env](#class-tensorflowenv-)
[Member Summary](#member-summary-)
[Member Details](#member-details-)
[Class tensorflow::EnvWrapper](#class-tensorflowenvwrapper-)
[Member Summary](#member-summary-)
[Member Details](#member-details-)
[Class tensorflow::RandomAccessFile](#class-tensorflowrandomaccessfile-)
[Member Summary](#member-summary-)
[Member Details](#member-details-)
[Class tensorflow::Session](#class-tensorflowsession-)
[Member Summary](#member-summary-)
[Member Details](#member-details-)
[Class tensorflow::Status](#class-tensorflowstatus-)
[Member Summary](#member-summary-)
[Member Details](#member-details-)
[Class tensorflow::Tensor](#class-tensorflowtensor-)
[Member Summary](#member-summary-)
[Member Details](#member-details-)
[Class tensorflow::TensorBuffer](#class-tensorflowtensorbuffer-)
[Member Summary](#member-summary-)
[Member Details](#member-details-)
[Class tensorflow::TensorShape](#class-tensorflowtensorshape-)
[Member Summary](#member-summary-)
[Member Details](#member-details-)
[Class tensorflow::TensorShapeIter](#class-tensorflowtensorshapeiter-)
[Member Summary](#member-summary-)
[Member Details](#member-details-)
[Class tensorflow::TensorShapeUtils](#class-tensorflowtensorshapeutils-)
[Member Summary](#member-summary-)
[Member Details](#member-details-)
[Class tensorflow::Thread](#class-tensorflowthread-)
[Member Summary](#member-summary-)
[Member Details](#member-details-)
[Class tensorflow::WritableFile](#class-tensorflowwritablefile-)
[Member Summary](#member-summary-)
[Member Details](#member-details-)
[Struct tensorflow::SessionOptions](#struct-tensorflowsessionoptions-)
[Member Summary](#member-summary-)
[Member Details](#member-details-)
[Struct tensorflow::Status::State](#struct-tensorflowstatusstate-)
[Member Summary](#member-summary-)
[Member Details](#member-details-)
[Struct tensorflow::TensorShapeDim](#struct-tensorflowtensorshapedim-)
[Member Summary](#member-summary-)
[Member Details](#member-details-)
[Struct tensorflow::ThreadOptions](#struct-tensorflowthreadoptions-)
[Member Summary](#member-summary-)
[Member Details](#member-details-)
[TensorFlow C++ Session API reference documentation](#tensorflow-c-session-api-reference-documentation-)
[Classes](#classes-)
[Structs](#structs-)
Webset：[http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnv.html](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnv.html)
# Class `tensorflow::Env`
An interface used by the tensorflow implementation to access operating system functionality like the filesystem etc.
Callers may wish to provide a custom Env object to get fine grain control.
All Env implementations are safe for concurrent access from multiple threads without any external synchronization.
## Member Summary
- [`tensorflow::Env::Env()`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnv.html#tensorflow_Env_Env)
- [`virtual tensorflow::Env::~Env()`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnv.html#virtual_tensorflow_Env_Env)
- [`virtual Status tensorflow::Env::NewRandomAccessFile(const string &fname, RandomAccessFile **result)=0`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnv.html#virtual_Status_tensorflow_Env_NewRandomAccessFile)- Creates a brand new random access read-only file with the specified name.
- [`virtual Status tensorflow::Env::NewWritableFile(const string &fname, WritableFile **result)=0`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnv.html#virtual_Status_tensorflow_Env_NewWritableFile)- Creates an object that writes to a new file with the specified name.
- [`virtual Status tensorflow::Env::NewAppendableFile(const string &fname, WritableFile **result)=0`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnv.html#virtual_Status_tensorflow_Env_NewAppendableFile)- Creates an object that either appends to an existing file, or writes to a new file (if the file does not exist to begin with).
- [`virtual bool tensorflow::Env::FileExists(const string &fname)=0`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnv.html#virtual_bool_tensorflow_Env_FileExists)- Returns true iff the named file exists.
- [`virtual Status tensorflow::Env::GetChildren(const string &dir, std::vector< string > *result)=0`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnv.html#virtual_Status_tensorflow_Env_GetChildren)- Stores in *result the names of the children of the specified directory. The names are relative to "dir".
- [`virtual Status tensorflow::Env::DeleteFile(const string &fname)=0`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnv.html#virtual_Status_tensorflow_Env_DeleteFile)- Deletes the named file.
- [`virtual Status tensorflow::Env::CreateDir(const string &dirname)=0`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnv.html#virtual_Status_tensorflow_Env_CreateDir)- Creates the specified directory.
- [`virtual Status tensorflow::Env::DeleteDir(const string &dirname)=0`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnv.html#virtual_Status_tensorflow_Env_DeleteDir)- Deletes the specified directory.
- [`virtual Status tensorflow::Env::GetFileSize(const string &fname, uint64 *file_size)=0`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnv.html#virtual_Status_tensorflow_Env_GetFileSize)- Stores the size of fname in *file_size.
- [`virtual Status tensorflow::Env::RenameFile(const string &src, const string &target)=0`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnv.html#virtual_Status_tensorflow_Env_RenameFile)- Renames file src to target. If target already exists, it will be replaced.
- [`virtual uint64 tensorflow::Env::NowMicros()=0`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnv.html#virtual_uint64_tensorflow_Env_NowMicros)- Returns the number of micro-seconds since some fixed point in time. Only useful for computing deltas of time.
- [`virtual void tensorflow::Env::SleepForMicroseconds(int micros)=0`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnv.html#virtual_void_tensorflow_Env_SleepForMicroseconds)- Sleeps/delays the thread for the prescribed number of micro-seconds.
- [`virtual Thread* tensorflow::Env::StartThread(const ThreadOptions &thread_options, const string &name, std::function< void()> fn) TF_MUST_USE_RESULT=0`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnv.html#virtual_Thread_tensorflow_Env_StartThread)- Returns a new thread that is running fn() and is identified (for debugging/performance-analysis) by "name".
- [`static Env* tensorflow::Env::Default()`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnv.html#static_Env_tensorflow_Env_Default)- Returns a default environment suitable for the current operating system.
## Member Details
`tensorflow::Env::Env()`
`virtual tensorflow::Env::~Env()`
`virtual Status tensorflow::Env::NewRandomAccessFile(const string &fname, RandomAccessFile **result)=0`
Creates a brand new random access read-only file with the specified name.
On success, stores a pointer to the new file in *result and returns OK. On failure stores NULL in *result and returns non-OK. If the file does not exist, returns a non-OK status.
The returned file may be concurrently accessed by multiple threads.
`virtual Status tensorflow::Env::NewWritableFile(const string &fname, WritableFile **result)=0`
Creates an object that writes to a new file with the specified name.
Deletes any existing file with the same name and creates a new file. On success, stores a pointer to the new file in *result and returns OK. On failure stores NULL in *result and returns non-OK.
The returned file will only be accessed by one thread at a time.
`virtual Status tensorflow::Env::NewAppendableFile(const string &fname, WritableFile **result)=0`
Creates an object that either appends to an existing file, or writes to a new file (if the file does not exist to begin with).
On success, stores a pointer to the new file in *result and returns OK. On failure stores NULL in *result and returns non-OK.
The returned file will only be accessed by one thread at a time.
`virtual bool tensorflow::Env::FileExists(const string &fname)=0`
Returns true iff the named file exists.
`virtual Status tensorflow::Env::GetChildren(const string &dir, std::vector< string > *result)=0`
Stores in *result the names of the children of the specified directory. The names are relative to "dir".
Original contents of *results are dropped.
`virtual Status tensorflow::Env::DeleteFile(const string &fname)=0`
Deletes the named file.
`virtual Status tensorflow::Env::CreateDir(const string &dirname)=0`
Creates the specified directory.
`virtual Status tensorflow::Env::DeleteDir(const string &dirname)=0`
Deletes the specified directory.
`virtual Status tensorflow::Env::GetFileSize(const string &fname, uint64 *file_size)=0`
Stores the size of fname in *file_size.
`virtual Status tensorflow::Env::RenameFile(const string &src, const string &target)=0`
Renames file src to target. If target already exists, it will be replaced.
`virtual uint64 tensorflow::Env::NowMicros()=0`
Returns the number of micro-seconds since some fixed point in time. Only useful for computing deltas of time.
`virtual void tensorflow::Env::SleepForMicroseconds(int micros)=0`
Sleeps/delays the thread for the prescribed number of micro-seconds.
`virtual Thread* tensorflow::Env::StartThread(const ThreadOptions &thread_options, const string &name, std::function< void()> fn) TF_MUST_USE_RESULT=0`
Returns a new thread that is running fn() and is identified (for debugging/performance-analysis) by "name".
Caller takes ownership of the result and must delete it eventually (the deletion will block until fn() stops running).
`static Env* tensorflow::Env::Default()`
Returns a default environment suitable for the current operating system.
Sophisticated users may wish to provide their own Env implementation instead of relying on this default environment.
The result of Default() belongs to this library and must never be deleted.
# Class `tensorflow::EnvWrapper`
An implementation of Env that forwards all calls to another Env .
May be useful to clients who wish to override just part of the functionality of another Env .
## Member Summary
- [`tensorflow::EnvWrapper::EnvWrapper(Env *t)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnvWrapper.html#tensorflow_EnvWrapper_EnvWrapper)- Initializes an EnvWrapper that delegates all calls to *t.
- [`virtual tensorflow::EnvWrapper::~EnvWrapper()`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnvWrapper.html#virtual_tensorflow_EnvWrapper_EnvWrapper)
- [`Env* tensorflow::EnvWrapper::target() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnvWrapper.html#Env_tensorflow_EnvWrapper_target)- Returns the target to which this Env forwards all calls.
- [`Status tensorflow::EnvWrapper::NewRandomAccessFile(const string &f, RandomAccessFile **r) override`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnvWrapper.html#Status_tensorflow_EnvWrapper_NewRandomAccessFile)- Creates a brand new random access read-only file with the specified name.
- [`Status tensorflow::EnvWrapper::NewWritableFile(const string &f, WritableFile **r) override`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnvWrapper.html#Status_tensorflow_EnvWrapper_NewWritableFile)- Creates an object that writes to a new file with the specified name.
- [`Status tensorflow::EnvWrapper::NewAppendableFile(const string &f, WritableFile **r) override`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnvWrapper.html#Status_tensorflow_EnvWrapper_NewAppendableFile)- Creates an object that either appends to an existing file, or writes to a new file (if the file does not exist to begin with).
- [`bool tensorflow::EnvWrapper::FileExists(const string &f) override`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnvWrapper.html#bool_tensorflow_EnvWrapper_FileExists)- Returns true iff the named file exists.
- [`Status tensorflow::EnvWrapper::GetChildren(const string &dir, std::vector< string > *r) override`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnvWrapper.html#Status_tensorflow_EnvWrapper_GetChildren)- Stores in *result the names of the children of the specified directory. The names are relative to "dir".
- [`Status tensorflow::EnvWrapper::DeleteFile(const string &f) override`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnvWrapper.html#Status_tensorflow_EnvWrapper_DeleteFile)- Deletes the named file.
- [`Status tensorflow::EnvWrapper::CreateDir(const string &d) override`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnvWrapper.html#Status_tensorflow_EnvWrapper_CreateDir)- Creates the specified directory.
- [`Status tensorflow::EnvWrapper::DeleteDir(const string &d) override`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnvWrapper.html#Status_tensorflow_EnvWrapper_DeleteDir)- Deletes the specified directory.
- [`Status tensorflow::EnvWrapper::GetFileSize(const string &f, uint64 *s) override`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnvWrapper.html#Status_tensorflow_EnvWrapper_GetFileSize)- Stores the size of fname in *file_size.
- [`Status tensorflow::EnvWrapper::RenameFile(const string &s, const string &t) override`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnvWrapper.html#Status_tensorflow_EnvWrapper_RenameFile)- Renames file src to target. If target already exists, it will be replaced.
- [`uint64 tensorflow::EnvWrapper::NowMicros() override`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnvWrapper.html#uint64_tensorflow_EnvWrapper_NowMicros)- Returns the number of micro-seconds since some fixed point in time. Only useful for computing deltas of time.
- [`void tensorflow::EnvWrapper::SleepForMicroseconds(int micros) override`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnvWrapper.html#void_tensorflow_EnvWrapper_SleepForMicroseconds)- Sleeps/delays the thread for the prescribed number of micro-seconds.
- [`Thread* tensorflow::EnvWrapper::StartThread(const ThreadOptions &thread_options, const string &name, std::function< void()> fn) override`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnvWrapper.html#Thread_tensorflow_EnvWrapper_StartThread)- Returns a new thread that is running fn() and is identified (for debugging/performance-analysis) by "name".
## Member Details
`tensorflow::EnvWrapper::EnvWrapper(Env *t)`
Initializes an EnvWrapper that delegates all calls to *t.
`virtual tensorflow::EnvWrapper::~EnvWrapper()`
`Env* tensorflow::EnvWrapper::target() const`
Returns the target to which this Env forwards all calls.
`Status tensorflow::EnvWrapper::NewRandomAccessFile(const string &f, RandomAccessFile **r) override`
Creates a brand new random access read-only file with the specified name.
On success, stores a pointer to the new file in *result and returns OK. On failure stores NULL in *result and returns non-OK. If the file does not exist, returns a non-OK status.
The returned file may be concurrently accessed by multiple threads.
`Status tensorflow::EnvWrapper::NewWritableFile(const string &f, WritableFile **r) override`
Creates an object that writes to a new file with the specified name.
Deletes any existing file with the same name and creates a new file. On success, stores a pointer to the new file in *result and returns OK. On failure stores NULL in *result and returns non-OK.
The returned file will only be accessed by one thread at a time.
`Status tensorflow::EnvWrapper::NewAppendableFile(const string &f, WritableFile **r) override`
Creates an object that either appends to an existing file, or writes to a new file (if the file does not exist to begin with).
On success, stores a pointer to the new file in *result and returns OK. On failure stores NULL in *result and returns non-OK.
The returned file will only be accessed by one thread at a time.
`bool tensorflow::EnvWrapper::FileExists(const string &f) override`
Returns true iff the named file exists.
`Status tensorflow::EnvWrapper::GetChildren(const string &dir, std::vector< string > *r) override`
Stores in *result the names of the children of the specified directory. The names are relative to "dir".
Original contents of *results are dropped.
`Status tensorflow::EnvWrapper::DeleteFile(const string &f) override`
Deletes the named file.
`Status tensorflow::EnvWrapper::CreateDir(const string &d) override`
Creates the specified directory.
`Status tensorflow::EnvWrapper::DeleteDir(const string &d) override`
Deletes the specified directory.
`Status tensorflow::EnvWrapper::GetFileSize(const string &f, uint64 *s) override`
Stores the size of fname in *file_size.
`Status tensorflow::EnvWrapper::RenameFile(const string &s, const string &t) override`
Renames file src to target. If target already exists, it will be replaced.
`uint64 tensorflow::EnvWrapper::NowMicros() override`
Returns the number of micro-seconds since some fixed point in time. Only useful for computing deltas of time.
`void tensorflow::EnvWrapper::SleepForMicroseconds(int micros) override`
Sleeps/delays the thread for the prescribed number of micro-seconds.
`Thread* tensorflow::EnvWrapper::StartThread(const ThreadOptions &thread_options, const string &name, std::function< void()> fn) override`
Returns a new thread that is running fn() and is identified (for debugging/performance-analysis) by "name".
Caller takes ownership of the result and must delete it eventually (the deletion will block until fn() stops running).
# Class `tensorflow::RandomAccessFile`
A file abstraction for randomly reading the contents of a file.
## Member Summary
- [`tensorflow::RandomAccessFile::RandomAccessFile()`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassRandomAccessFile.html#tensorflow_RandomAccessFile_RandomAccessFile)
- [`virtual tensorflow::RandomAccessFile::~RandomAccessFile()`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassRandomAccessFile.html#virtual_tensorflow_RandomAccessFile_RandomAccessFile)
- [`virtual Status tensorflow::RandomAccessFile::Read(uint64 offset, size_t n, StringPiece *result, char *scratch) const =0`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassRandomAccessFile.html#virtual_Status_tensorflow_RandomAccessFile_Read)- Reads up to "n" bytes from the file starting at "offset".
## Member Details
`tensorflow::RandomAccessFile::RandomAccessFile()`
`virtual tensorflow::RandomAccessFile::~RandomAccessFile()`
`virtual Status tensorflow::RandomAccessFile::Read(uint64 offset, size_t n, StringPiece *result, char *scratch) const =0`
Reads up to "n" bytes from the file starting at "offset".
"scratch[0..n-1]" may be written by this routine. Sets "*result" to the data that was read (including if fewer than "n" bytes were successfully read). May set "*result" to point at data in "scratch[0..n-1]", so "scratch[0..n-1]" must be live when "*result" is used.
On OK returned status: "n" bytes have been stored in "*result". On non-OK returned status: [0..n] bytes have been stored in "*result".
Returns `OUT_OF_RANGE` if fewer than n bytes were stored in "*result" because of EOF.
Safe for concurrent use by multiple threads.
# Class `tensorflow::Session`
A Session instance lets a caller drive a TensorFlow graph computation.
When a Session is created with a given target, a new Session object is bound to the universe of resources specified by that target. Those resources are available to this session to perform computation described in the GraphDef. After extending the session with a graph, the caller uses the Run() API to perform the computation and potentially fetch outputs as Tensors.
Example:
```c++ tensorflow::GraphDef graph; // ... Create or load graph into "graph".
// This example uses the default options which connects // to a local runtime. tensorflow::SessionOptions options; std::unique_ptr session(tensorflow::NewSession(options));
// Create the session with this graph. tensorflow::Status s = session->Create(graph); if (!s.ok()) { ... }
// Run the graph and fetch the first output of the "output" // operation, and also run to but do not return anything // for the "update_state" operation. std::vector outputs; s = session->Run({}, {"output:0"}, {"update_state"}, &outputs); if (!s.ok()) { ... }
// Map the output as a flattened float tensor, and do something // with it. auto output_tensor = outputs[0].flat(); if (output_tensor(0) > 0.5) { ... }
// Close the session to release the resources associated with // this session. session->Close()
```
A Session allows concurrent calls to Run() , though a Session must be created / extended by a single thread.
Only one thread must call Close() , and Close() must only be called after all other calls to Run() have returned.
## Member Summary
- [`virtual Status tensorflow::Session::Create(const GraphDef &graph)=0`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassSession.html#virtual_Status_tensorflow_Session_Create)- Create the graph to be used for the session.
- [`virtual Status tensorflow::Session::Extend(const GraphDef &graph)=0`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassSession.html#virtual_Status_tensorflow_Session_Extend)- Adds operations to the graph that is already registered with the Session .
- [`virtual Status tensorflow::Session::Run(const std::vector< std::pair< string, Tensor > > &inputs, const std::vector< string > &output_tensor_names, const std::vector< string > &target_node_names, std::vector< Tensor > *outputs)=0`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassSession.html#virtual_Status_tensorflow_Session_Run)- Runs the graph with the provided input tensors and fills `outputs` for the endpoints specified in `output_tensor_names`. Runs to but does not return Tensors for the nodes in `target_node_names`.
- [`virtual Status tensorflow::Session::Close()=0`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassSession.html#virtual_Status_tensorflow_Session_Close)- Closes this session.
- [`virtual tensorflow::Session::~Session()`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassSession.html#virtual_tensorflow_Session_Session)
## Member Details
`virtual Status tensorflow::Session::Create(const GraphDef &graph)=0`
Create the graph to be used for the session.
Returns an error if this session has already been created with a graph. To re-use the session with a different graph, the caller must Close() the session first.
`virtual Status tensorflow::Session::Extend(const GraphDef &graph)=0`
Adds operations to the graph that is already registered with the Session .
The names of new operations in "graph" must not exist in the graph that is already registered.
`virtual Status tensorflow::Session::Run(const std::vector< std::pair< string, Tensor > > &inputs, const std::vector< string > &output_tensor_names, const std::vector< string > &target_node_names, std::vector< Tensor > *outputs)=0`
Runs the graph with the provided input tensors and fills `outputs` for the endpoints specified in `output_tensor_names`. Runs to but does not return Tensors for the nodes in `target_node_names`.
The order of tensors in `outputs` will match the order provided by `output_tensor_names`.
If `Run` returns `OK()`, then `outputs->size()` will be equal to `output_tensor_names.size()`. If `Run` does not return `OK()`, the state of `outputs` is undefined.
REQUIRES: The name of each Tensor of the input or output must match a "Tensor endpoint" in the `GraphDef` passed to `Create()`.
REQUIRES: outputs is not nullptr if `output_tensor_names` is non-empty.
`virtual Status tensorflow::Session::Close()=0`
Closes this session.
Closing a session releases the resources used by this session on the TensorFlow runtime (specified during session creation by the `SessionOptions::target` field).
`virtual tensorflow::Session::~Session()`
# Class `tensorflow::Status`
## Member Summary
- [`tensorflow::Status::Status()`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassStatus.html#tensorflow_Status_Status)- Create a success status.
- [`tensorflow::Status::~Status()`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassStatus.html#tensorflow_Status_Status)
- [`tensorflow::Status::Status(tensorflow::error::Code code, tensorflow::StringPiece msg)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassStatus.html#tensorflow_Status_Status)- Create a status with the specified error code and msg as a human-readable string containing more detailed information.
- [`tensorflow::Status::Status(const Status &s)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassStatus.html#tensorflow_Status_Status)- Copy the specified status.
- [`void tensorflow::Status::operator=(const Status &s)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassStatus.html#void_tensorflow_Status_operator_)
- [`bool tensorflow::Status::ok() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassStatus.html#bool_tensorflow_Status_ok)- Returns true iff the status indicates success.
- [`tensorflow::error::Code tensorflow::Status::code() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassStatus.html#tensorflow_error_Code_tensorflow_Status_code)
- [`const string& tensorflow::Status::error_message() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassStatus.html#const_string_tensorflow_Status_error_message)
- [`bool tensorflow::Status::operator==(const Status &x) const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassStatus.html#bool_tensorflow_Status_operator_)
- [`bool tensorflow::Status::operator!=(const Status &x) const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassStatus.html#bool_tensorflow_Status_operator_)
- [`void tensorflow::Status::Update(const Status &new_status)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassStatus.html#void_tensorflow_Status_Update)- If `ok()`, stores `new_status` into `*this`. If `!ok()`, preserves the current status, but may augment with additional information about `new_status`.
- [`string tensorflow::Status::ToString() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassStatus.html#string_tensorflow_Status_ToString)- Return a string representation of this status suitable for printing. Returns the string `"OK"` for success.
- [`static Status tensorflow::Status::OK()`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassStatus.html#static_Status_tensorflow_Status_OK)
## Member Details
`tensorflow::Status::Status()`
Create a success status.
`tensorflow::Status::~Status()`
`tensorflow::Status::Status(tensorflow::error::Code code, tensorflow::StringPiece msg)`
Create a status with the specified error code and msg as a human-readable string containing more detailed information.
`tensorflow::Status::Status(const Status &s)`
Copy the specified status.
`void tensorflow::Status::operator=(const Status &s)`
`bool tensorflow::Status::ok() const`
Returns true iff the status indicates success.
`tensorflow::error::Code tensorflow::Status::code() const`
`const string& tensorflow::Status::error_message() const`
`bool tensorflow::Status::operator==(const Status &x) const`
`bool tensorflow::Status::operator!=(const Status &x) const`
`void tensorflow::Status::Update(const Status &new_status)`
If `ok()`, stores `new_status` into `*this`. If `!ok()`, preserves the current status, but may augment with additional information about `new_status`.
Convenient way of keeping track of the first error encountered. Instead of: `if (overall_status.ok()) overall_status = new_status` Use: `overall_status.Update(new_status);`
`string tensorflow::Status::ToString() const`
Return a string representation of this status suitable for printing. Returns the string `"OK"` for success.
`static Status tensorflow::Status::OK()`
# Class `tensorflow::Tensor`
Represents an n-dimensional array of values.
## Member Summary
- [`tensorflow::Tensor::Tensor()`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#tensorflow_Tensor_Tensor)- Default Tensor constructor. Creates a 1-dimension, 0-element float tensor.
- [`tensorflow::Tensor::Tensor(DataType type, const TensorShape &shape)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#tensorflow_Tensor_Tensor)- Creates a Tensor of the given `type` and `shape`.
- [`tensorflow::Tensor::Tensor(Allocator *a, DataType type, const TensorShape &shape)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#tensorflow_Tensor_Tensor)- Creates a tensor with the input `type` and `shape`, using the allocator `a` to allocate the underlying buffer.
- [`tensorflow::Tensor::Tensor(DataType type)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#tensorflow_Tensor_Tensor)- Creates an uninitialized Tensor of the given data type.
- [`tensorflow::Tensor::Tensor(const Tensor &other)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#tensorflow_Tensor_Tensor)
- [`tensorflow::Tensor::~Tensor()`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#tensorflow_Tensor_Tensor)- Copy constructor.
- [`DataType tensorflow::Tensor::dtype() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#DataType_tensorflow_Tensor_dtype)- Returns the data type.
- [`const TensorShape& tensorflow::Tensor::shape() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#const_TensorShape_tensorflow_Tensor_shape)- Returns the shape of the tensor.
- [`int tensorflow::Tensor::dims() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#int_tensorflow_Tensor_dims)- Convenience accessor for the tensor shape.
- [`int64 tensorflow::Tensor::dim_size(int d) const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#int64_tensorflow_Tensor_dim_size)- Convenience accessor for the tensor shape.
- [`int64 tensorflow::Tensor::NumElements() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#int64_tensorflow_Tensor_NumElements)- Convenience accessor for the tensor shape.
- [`bool tensorflow::Tensor::IsSameSize(const Tensor &b) const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#bool_tensorflow_Tensor_IsSameSize)
- [`bool tensorflow::Tensor::IsInitialized() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#bool_tensorflow_Tensor_IsInitialized)- Has this Tensor been initialized?
- [`size_t tensorflow::Tensor::TotalBytes() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#size_t_tensorflow_Tensor_TotalBytes)- Returns the estimated memory usage of this tensor.
- [`Tensor& tensorflow::Tensor::operator=(const Tensor &other)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#Tensor_tensorflow_Tensor_operator_)- Assign operator. This tensor shares other's underlying storage.
- [`bool tensorflow::Tensor::CopyFrom(const Tensor &other, const TensorShape &shape) TF_MUST_USE_RESULT`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#bool_tensorflow_Tensor_CopyFrom)- Copy the other tensor into this tensor and reshape it.
- [`Tensor tensorflow::Tensor::Slice(int64 dim0_start, int64 dim0_limit) const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#Tensor_tensorflow_Tensor_Slice)- Slice this tensor along the 1st dimension.
- [`bool tensorflow::Tensor::FromProto(const TensorProto &other) TF_MUST_USE_RESULT`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#bool_tensorflow_Tensor_FromProto)- Parse `other` and construct the tensor.
- [`bool tensorflow::Tensor::FromProto(Allocator *a, const TensorProto &other) TF_MUST_USE_RESULT`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#bool_tensorflow_Tensor_FromProto)
- [`void tensorflow::Tensor::AsProtoField(TensorProto *proto) const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#void_tensorflow_Tensor_AsProtoField)- Fills in `proto` with `*this` tensor's content.
- [`void tensorflow::Tensor::AsProtoTensorContent(TensorProto *proto) const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#void_tensorflow_Tensor_AsProtoTensorContent)
- [`TTypes<T>::Vec tensorflow::Tensor::vec()`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#TTypes_T_Vec_tensorflow_Tensor_vec)- Return the tensor data as an `Eigen::Tensor` with the type and sizes of this `Tensor`.
- [`TTypes<T>::Matrix tensorflow::Tensor::matrix()`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#TTypes_T_Matrix_tensorflow_Tensor_matrix)
- [`TTypes< T, NDIMS >::Tensor tensorflow::Tensor::tensor()`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#TTypes_T_NDIMS_Tensor_tensorflow_Tensor_tensor)
- [`TTypes<T>::Flat tensorflow::Tensor::flat()`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#TTypes_T_Flat_tensorflow_Tensor_flat)- Return the tensor data as an `Eigen::Tensor` of the data type and a specified shape.
- [`TTypes<T>::UnalignedFlat tensorflow::Tensor::unaligned_flat()`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#TTypes_T_UnalignedFlat_tensorflow_Tensor_unaligned_flat)
- [`TTypes<T>::Matrix tensorflow::Tensor::flat_inner_dims()`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#TTypes_T_Matrix_tensorflow_Tensor_flat_inner_dims)
- [`TTypes<T>::Matrix tensorflow::Tensor::flat_outer_dims()`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#TTypes_T_Matrix_tensorflow_Tensor_flat_outer_dims)
- [`TTypes< T, NDIMS >::Tensor tensorflow::Tensor::shaped(gtl::ArraySlice< int64 > new_sizes)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#TTypes_T_NDIMS_Tensor_tensorflow_Tensor_shaped)
- [`TTypes< T, NDIMS >::UnalignedTensor tensorflow::Tensor::unaligned_shaped(gtl::ArraySlice< int64 > new_sizes)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#TTypes_T_NDIMS_UnalignedTensor_tensorflow_Tensor_unaligned_shaped)
- [`TTypes< T >::Scalar tensorflow::Tensor::scalar()`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#TTypes_T_Scalar_tensorflow_Tensor_scalar)- Return the Tensor data as a `TensorMap` of fixed size 1: `TensorMap<TensorFixedSize<T, 1>>`.
- [`TTypes<T>::ConstVec tensorflow::Tensor::vec() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#TTypes_T_ConstVec_tensorflow_Tensor_vec)- Const versions of all the methods above.
- [`TTypes<T>::ConstMatrix tensorflow::Tensor::matrix() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#TTypes_T_ConstMatrix_tensorflow_Tensor_matrix)
- [`TTypes< T, NDIMS >::ConstTensor tensorflow::Tensor::tensor() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#TTypes_T_NDIMS_ConstTensor_tensorflow_Tensor_tensor)
- [`TTypes<T>::ConstFlat tensorflow::Tensor::flat() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#TTypes_T_ConstFlat_tensorflow_Tensor_flat)
- [`TTypes<T>::UnalignedConstFlat tensorflow::Tensor::unaligned_flat() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#TTypes_T_UnalignedConstFlat_tensorflow_Tensor_unaligned_flat)
- [`TTypes<T>::ConstMatrix tensorflow::Tensor::flat_inner_dims() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#TTypes_T_ConstMatrix_tensorflow_Tensor_flat_inner_dims)
- [`TTypes<T>::ConstMatrix tensorflow::Tensor::flat_outer_dims() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#TTypes_T_ConstMatrix_tensorflow_Tensor_flat_outer_dims)
- [`TTypes< T, NDIMS >::ConstTensor tensorflow::Tensor::shaped(gtl::ArraySlice< int64 > new_sizes) const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#TTypes_T_NDIMS_ConstTensor_tensorflow_Tensor_shaped)
- [`TTypes< T, NDIMS >::UnalignedConstTensor tensorflow::Tensor::unaligned_shaped(gtl::ArraySlice< int64 > new_sizes) const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#TTypes_T_NDIMS_UnalignedConstTensor_tensorflow_Tensor_unaligned_shaped)
- [`TTypes< T >::ConstScalar tensorflow::Tensor::scalar() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#TTypes_T_ConstScalar_tensorflow_Tensor_scalar)
- [`string tensorflow::Tensor::SummarizeValue(int64 max_entries) const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#string_tensorflow_Tensor_SummarizeValue)- Render the first `max_entries` values in `*this` into a string.
- [`string tensorflow::Tensor::DebugString() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#string_tensorflow_Tensor_DebugString)- A human-readable summary of the tensor suitable for debugging.
- [`void tensorflow::Tensor::FillDescription(TensorDescription *description) const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#void_tensorflow_Tensor_FillDescription)
- [`StringPiece tensorflow::Tensor::tensor_data() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html#StringPiece_tensorflow_Tensor_tensor_data)- Returns a `StringPiece` mapping the current tensor's buffer.
## Member Details
`tensorflow::Tensor::Tensor()`
Default Tensor constructor. Creates a 1-dimension, 0-element float tensor.
`tensorflow::Tensor::Tensor(DataType type, const TensorShape &shape)`
Creates a Tensor of the given `type` and `shape`.
The underlying buffer is allocated using a `CPUAllocator`.
`tensorflow::Tensor::Tensor(Allocator *a, DataType type, const TensorShape &shape)`
Creates a tensor with the input `type` and `shape`, using the allocator `a` to allocate the underlying buffer.
`a` must outlive the lifetime of this Tensor .
`tensorflow::Tensor::Tensor(DataType type)`
Creates an uninitialized Tensor of the given data type.
`tensorflow::Tensor::Tensor(const Tensor &other)`
`tensorflow::Tensor::~Tensor()`
Copy constructor.
`DataType tensorflow::Tensor::dtype() const`
Returns the data type.
`const TensorShape& tensorflow::Tensor::shape() const`
Returns the shape of the tensor.
`int tensorflow::Tensor::dims() const`
Convenience accessor for the tensor shape.
For all shape accessors, see comments for relevant methods of `TensorShape` in `tensor_shape.h`.
`int64 tensorflow::Tensor::dim_size(int d) const`
Convenience accessor for the tensor shape.
`int64 tensorflow::Tensor::NumElements() const`
Convenience accessor for the tensor shape.
`bool tensorflow::Tensor::IsSameSize(const Tensor &b) const`
`bool tensorflow::Tensor::IsInitialized() const`
Has this Tensor been initialized?
`size_t tensorflow::Tensor::TotalBytes() const`
Returns the estimated memory usage of this tensor.
`Tensor& tensorflow::Tensor::operator=(const Tensor &other)`
Assign operator. This tensor shares other's underlying storage.
`bool tensorflow::Tensor::CopyFrom(const Tensor &other, const TensorShape &shape) TF_MUST_USE_RESULT`
Copy the other tensor into this tensor and reshape it.
This tensor shares other's underlying storage. Returns `true` iff `other.shape()` has the same number of elements of the given `shape`.
`Tensor tensorflow::Tensor::Slice(int64 dim0_start, int64 dim0_limit) const`
Slice this tensor along the 1st dimension.
I.e., the returned tensor satisifies returned[i, ...] == this[dim0_start + i, ...]. The returned tensor shares the underlying tensor buffer with this tensor.
NOTE: The returned tensor may not satisfies the same alignment requirement as this tensor depending on the shape. The caller must check the returned tensor's alignment before calling certain methods that have alignment requirement (e.g., `flat()`, `tensor()`).
REQUIRES: `dims()` >= 1 REQUIRES: `0 <= dim0_start <= dim0_limit <= dim_size(0)`
`bool tensorflow::Tensor::FromProto(const TensorProto &other) TF_MUST_USE_RESULT`
Parse `other` and construct the tensor.
Returns `true` iff the parsing succeeds. If the parsing fails, the state of `*this` is unchanged.
`bool tensorflow::Tensor::FromProto(Allocator *a, const TensorProto &other) TF_MUST_USE_RESULT`
`void tensorflow::Tensor::AsProtoField(TensorProto *proto) const`
Fills in `proto` with `*this` tensor's content.
`AsProtoField()` fills in the repeated field for `proto.dtype()`, while `AsProtoTensorContent()` encodes the content in `proto.tensor_content()` in a compact form.
`void tensorflow::Tensor::AsProtoTensorContent(TensorProto *proto) const`
`TTypes<T>::Vec tensorflow::Tensor::vec()`
Return the tensor data as an `Eigen::Tensor` with the type and sizes of this `Tensor`.
Use these methods when you know the data type and the number of dimensions of the Tensor and you want an `Eigen::Tensor` automatically sized to the `Tensor` sizes. The implementation check fails if either type or sizes mismatch.
Example:
```c++ typedef float T; Tensor my_mat(...built with Shape{rows: 3, cols: 5}...); auto mat = my_mat.matrix(); // 2D Eigen::Tensor, 3 x 5. auto mat = my_mat.tensor(); // 2D Eigen::Tensor, 3 x 5. auto vec = my_mat.vec(); // CHECK fails as my_mat is 2D. auto vec = my_mat.tensor(); // CHECK fails as my_mat is 2D. auto mat = my_mat.matrix();// CHECK fails as type mismatch.
```
#### `TTypes<T>::Matrix tensorflow::Tensor::matrix()` <a class="md-anchor" id="TTypes_T_Matrix_tensorflow_Tensor_matrix"></a>
#### `TTypes< T, NDIMS >::Tensor tensorflow::Tensor::tensor()` <a class="md-anchor" id="TTypes_T_NDIMS_Tensor_tensorflow_Tensor_tensor"></a>
#### `TTypes<T>::Flat tensorflow::Tensor::flat()` <a class="md-anchor" id="TTypes_T_Flat_tensorflow_Tensor_flat"></a>
Return the tensor data as an `Eigen::Tensor` of the data type and a specified shape.
These methods allow you to access the data with the dimensions and sizes of your choice. You do not need to know the number of dimensions of the Tensor to call them. However, they `CHECK` that the type matches and the dimensions requested creates an `Eigen::Tensor` with the same number of elements as the tensor.
Example:
```c++ typedef float T;
Tensor my_ten(...built with Shape{planes: 4, rows: 3, cols: 5}...);
// 1D Eigen::Tensor, size 60:
auto flat = my_ten.flat<T>();
// 2D Eigen::Tensor 12 x 5:
auto inner = my_ten.flat_inner_dims<T>();
// 2D Eigen::Tensor 4 x 15:
auto outer = my_ten.shaped<T, 2>({4, 15});
// CHECK fails, bad num elements:
auto outer = my_ten.shaped<T, 2>({4, 8});
// 3D Eigen::Tensor 6 x 5 x 2:
auto weird = my_ten.shaped<T, 3>({6, 5, 2});
// CHECK fails, type mismatch:
auto bad   = my_ten.flat<int32>();
```
`TTypes<T>::UnalignedFlat tensorflow::Tensor::unaligned_flat()`
`TTypes<T>::Matrix tensorflow::Tensor::flat_inner_dims()`
Returns the data as an Eigen::Tensor with 2 dimensions, collapsing all Tensor dimensions but the last one into the first dimension of the result.
`TTypes<T>::Matrix tensorflow::Tensor::flat_outer_dims()`
Returns the data as an Eigen::Tensor with 2 dimensions, collapsing all Tensor dimensions but the first one into the last dimension of the result.
`TTypes< T, NDIMS >::Tensor tensorflow::Tensor::shaped(gtl::ArraySlice< int64 > new_sizes)`
`TTypes< T, NDIMS >::UnalignedTensor tensorflow::Tensor::unaligned_shaped(gtl::ArraySlice< int64 > new_sizes)`
`TTypes< T >::Scalar tensorflow::Tensor::scalar()`
Return the Tensor data as a `TensorMap` of fixed size 1: `TensorMap<TensorFixedSize<T, 1>>`.
Using `scalar()` allows the compiler to perform optimizations as the size of the tensor is known at compile time.
`TTypes<T>::ConstVec tensorflow::Tensor::vec() const`
Const versions of all the methods above.
`TTypes<T>::ConstMatrix tensorflow::Tensor::matrix() const`
`TTypes< T, NDIMS >::ConstTensor tensorflow::Tensor::tensor() const`
`TTypes<T>::ConstFlat tensorflow::Tensor::flat() const`
`TTypes<T>::UnalignedConstFlat tensorflow::Tensor::unaligned_flat() const`
`TTypes<T>::ConstMatrix tensorflow::Tensor::flat_inner_dims() const`
`TTypes<T>::ConstMatrix tensorflow::Tensor::flat_outer_dims() const`
`TTypes< T, NDIMS >::ConstTensor tensorflow::Tensor::shaped(gtl::ArraySlice< int64 > new_sizes) const`
`TTypes< T, NDIMS >::UnalignedConstTensor tensorflow::Tensor::unaligned_shaped(gtl::ArraySlice< int64 > new_sizes) const`
`TTypes< T >::ConstScalar tensorflow::Tensor::scalar() const`
`string tensorflow::Tensor::SummarizeValue(int64 max_entries) const`
Render the first `max_entries` values in `*this` into a string.
`string tensorflow::Tensor::DebugString() const`
A human-readable summary of the tensor suitable for debugging.
`void tensorflow::Tensor::FillDescription(TensorDescription *description) const`
Fill in the `TensorDescription` proto with metadata about the tensor that is useful for monitoring and debugging.
`StringPiece tensorflow::Tensor::tensor_data() const`
Returns a `StringPiece` mapping the current tensor's buffer.
The returned `StringPiece` may point to memory location on devices that the CPU cannot address directly.
NOTE: The underlying tensor buffer is refcounted, so the lifetime of the contents mapped by the `StringPiece` matches the lifetime of the buffer; callers should arrange to make sure the buffer does not get destroyed while the `StringPiece` is still used.
REQUIRES: `DataTypeCanUseMemcpy( dtype() )`.
# Class `tensorflow::TensorBuffer`
## Member Summary
- [`tensorflow::TensorBuffer::~TensorBuffer() override`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorBuffer.html#tensorflow_TensorBuffer_TensorBuffer)
- [`virtual void* tensorflow::TensorBuffer::data() const =0`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorBuffer.html#virtual_void_tensorflow_TensorBuffer_data)
- [`virtual size_t tensorflow::TensorBuffer::size() const =0`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorBuffer.html#virtual_size_t_tensorflow_TensorBuffer_size)
- [`virtual TensorBuffer* tensorflow::TensorBuffer::root_buffer()=0`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorBuffer.html#virtual_TensorBuffer_tensorflow_TensorBuffer_root_buffer)
- [`virtual void tensorflow::TensorBuffer::FillAllocationDescription(AllocationDescription *proto) const =0`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorBuffer.html#virtual_void_tensorflow_TensorBuffer_FillAllocationDescription)
- [`T* tensorflow::TensorBuffer::base() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorBuffer.html#T_tensorflow_TensorBuffer_base)
## Member Details
`tensorflow::TensorBuffer::~TensorBuffer() override`
`virtual void* tensorflow::TensorBuffer::data() const =0`
`virtual size_t tensorflow::TensorBuffer::size() const =0`
`virtual TensorBuffer* tensorflow::TensorBuffer::root_buffer()=0`
`virtual void tensorflow::TensorBuffer::FillAllocationDescription(AllocationDescription *proto) const =0`
`T* tensorflow::TensorBuffer::base() const`
# Class `tensorflow::TensorShape`
Manages the dimensions of a Tensor and their sizes.
## Member Summary
- [`tensorflow::TensorShape::TensorShape(gtl::ArraySlice< int64 > dim_sizes)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShape.html#tensorflow_TensorShape_TensorShape)- Construct a `TensorShape` from the provided sizes. REQUIRES: `dim_sizes[i] >= 0`
- [`tensorflow::TensorShape::TensorShape(std::initializer_list< int64 > dim_sizes)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShape.html#tensorflow_TensorShape_TensorShape)
- [`tensorflow::TensorShape::TensorShape(const TensorShapeProto &proto)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShape.html#tensorflow_TensorShape_TensorShape)- REQUIRES: `IsValid(proto)`
- [`tensorflow::TensorShape::TensorShape()`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShape.html#tensorflow_TensorShape_TensorShape)
- [`void tensorflow::TensorShape::Clear()`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShape.html#void_tensorflow_TensorShape_Clear)- Clear a tensor shape.
- [`void tensorflow::TensorShape::AddDim(int64 size)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShape.html#void_tensorflow_TensorShape_AddDim)- Add a dimension to the end ("inner-most"). REQUIRES: `size >= 0`
- [`void tensorflow::TensorShape::AppendShape(const TensorShape &shape)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShape.html#void_tensorflow_TensorShape_AppendShape)- Appends all the dimensions from `shape`.
- [`void tensorflow::TensorShape::InsertDim(int d, int64 size)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShape.html#void_tensorflow_TensorShape_InsertDim)- Insert a dimension somewhere in the `TensorShape`. REQUIRES: `0 <= d <= dims()`REQUIRES: `size >= 0`
- [`void tensorflow::TensorShape::set_dim(int d, int64 size)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShape.html#void_tensorflow_TensorShape_set_dim)- Modifies the size of the dimension `d` to be `size` REQUIRES: `0 <= d < dims()` REQUIRES: `size >= 0`
- [`void tensorflow::TensorShape::RemoveDim(int d)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShape.html#void_tensorflow_TensorShape_RemoveDim)- Removes dimension `d` from the `TensorShape`. REQUIRES: `0 <= d < dims()`
- [`int tensorflow::TensorShape::dims() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShape.html#int_tensorflow_TensorShape_dims)- Return the number of dimensions in the tensor.
- [`int64 tensorflow::TensorShape::dim_size(int d) const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShape.html#int64_tensorflow_TensorShape_dim_size)- Returns the number of elements in dimension `d`. REQUIRES: `0 <= d < dims()`
- [`gtl::ArraySlice<int64> tensorflow::TensorShape::dim_sizes() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShape.html#gtl_ArraySlice_int64_tensorflow_TensorShape_dim_sizes)- Returns sizes of all dimensions.
- [`int64 tensorflow::TensorShape::num_elements() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShape.html#int64_tensorflow_TensorShape_num_elements)- Returns the number of elements in the tensor.
- [`bool tensorflow::TensorShape::IsSameSize(const TensorShape &b) const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShape.html#bool_tensorflow_TensorShape_IsSameSize)
- [`bool tensorflow::TensorShape::operator==(const TensorShape &b) const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShape.html#bool_tensorflow_TensorShape_operator_)
- [`void tensorflow::TensorShape::AsProto(TensorShapeProto *proto) const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShape.html#void_tensorflow_TensorShape_AsProto)- Fill `*proto` from `*this`.
- [`Eigen::DSizes< Eigen::DenseIndex, NDIMS > tensorflow::TensorShape::AsEigenDSizes() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShape.html#Eigen_DSizes_Eigen_DenseIndex_NDIMS_tensorflow_TensorShape_AsEigenDSizes)- Fill `*dsizes` from `*this`.
- [`Eigen::DSizes< Eigen::DenseIndex, NDIMS > tensorflow::TensorShape::AsEigenDSizesWithPadding() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShape.html#Eigen_DSizes_Eigen_DenseIndex_NDIMS_tensorflow_TensorShape_AsEigenDSizesWithPadding)
- [`TensorShapeIter tensorflow::TensorShape::begin() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShape.html#TensorShapeIter_tensorflow_TensorShape_begin)- For iterating through the dimensions.
- [`TensorShapeIter tensorflow::TensorShape::end() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShape.html#TensorShapeIter_tensorflow_TensorShape_end)
- [`string tensorflow::TensorShape::DebugString() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShape.html#string_tensorflow_TensorShape_DebugString)- For error messages.
- [`string tensorflow::TensorShape::ShortDebugString() const`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShape.html#string_tensorflow_TensorShape_ShortDebugString)
- [`static bool tensorflow::TensorShape::IsValid(const TensorShapeProto &proto)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShape.html#static_bool_tensorflow_TensorShape_IsValid)- Returns `true` iff `proto` is a valid tensor shape.
## Member Details
`tensorflow::TensorShape::TensorShape(gtl::ArraySlice< int64 > dim_sizes)`
Construct a `TensorShape` from the provided sizes. REQUIRES: `dim_sizes[i] >= 0`
`tensorflow::TensorShape::TensorShape(std::initializer_list< int64 > dim_sizes)`
`tensorflow::TensorShape::TensorShape(const TensorShapeProto &proto)`
REQUIRES: `IsValid(proto)`
`tensorflow::TensorShape::TensorShape()`
Create a tensor shape with no dimensions and one element, which you can then call `AddDim()` on.
`void tensorflow::TensorShape::Clear()`
Clear a tensor shape.
`void tensorflow::TensorShape::AddDim(int64 size)`
Add a dimension to the end ("inner-most"). REQUIRES: `size >= 0`
`void tensorflow::TensorShape::AppendShape(const TensorShape &shape)`
Appends all the dimensions from `shape`.
`void tensorflow::TensorShape::InsertDim(int d, int64 size)`
Insert a dimension somewhere in the `TensorShape`. REQUIRES: `0 <= d <= dims()` REQUIRES: `size >= 0`
`void tensorflow::TensorShape::set_dim(int d, int64 size)`
Modifies the size of the dimension `d` to be `size` REQUIRES: `0 <= d < dims()` REQUIRES: `size >= 0`
`void tensorflow::TensorShape::RemoveDim(int d)`
Removes dimension `d` from the `TensorShape`. REQUIRES: `0 <= d < dims()`
`int tensorflow::TensorShape::dims() const`
Return the number of dimensions in the tensor.
`int64 tensorflow::TensorShape::dim_size(int d) const`
Returns the number of elements in dimension `d`. REQUIRES: `0 <= d < dims()`
`gtl::ArraySlice<int64> tensorflow::TensorShape::dim_sizes() const`
Returns sizes of all dimensions.
`int64 tensorflow::TensorShape::num_elements() const`
Returns the number of elements in the tensor.
We use `int64` and not `size_t` to be compatible with `Eigen::Tensor` which uses `ptrdiff_t`.
`bool tensorflow::TensorShape::IsSameSize(const TensorShape &b) const`
Returns true if `*this` and `b` have the same sizes. Ignores dimension names.
`bool tensorflow::TensorShape::operator==(const TensorShape &b) const`
`void tensorflow::TensorShape::AsProto(TensorShapeProto *proto) const`
Fill `*proto` from `*this`.
`Eigen::DSizes< Eigen::DenseIndex, NDIMS > tensorflow::TensorShape::AsEigenDSizes() const`
Fill `*dsizes` from `*this`.
`Eigen::DSizes< Eigen::DenseIndex, NDIMS > tensorflow::TensorShape::AsEigenDSizesWithPadding() const`
Same as `AsEigenDSizes()` but allows for `NDIMS > dims()` in which case we pad the rest of the sizes with 1.
`TensorShapeIter tensorflow::TensorShape::begin() const`
For iterating through the dimensions.
`TensorShapeIter tensorflow::TensorShape::end() const`
`string tensorflow::TensorShape::DebugString() const`
For error messages.
`string tensorflow::TensorShape::ShortDebugString() const`
`static bool tensorflow::TensorShape::IsValid(const TensorShapeProto &proto)`
Returns `true` iff `proto` is a valid tensor shape.
# Class `tensorflow::TensorShapeIter`
## Member Summary
- [`tensorflow::TensorShapeIter::TensorShapeIter(const TensorShape *shape, int d)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShapeIter.html#tensorflow_TensorShapeIter_TensorShapeIter)
- [`bool tensorflow::TensorShapeIter::operator==(const TensorShapeIter &rhs)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShapeIter.html#bool_tensorflow_TensorShapeIter_operator_)
- [`bool tensorflow::TensorShapeIter::operator!=(const TensorShapeIter &rhs)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShapeIter.html#bool_tensorflow_TensorShapeIter_operator_)
- [`void tensorflow::TensorShapeIter::operator++()`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShapeIter.html#void_tensorflow_TensorShapeIter_operator_)
- [`TensorShapeDim tensorflow::TensorShapeIter::operator*()`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShapeIter.html#TensorShapeDim_tensorflow_TensorShapeIter_operator_)
## Member Details
`tensorflow::TensorShapeIter::TensorShapeIter(const TensorShape *shape, int d)`
`bool tensorflow::TensorShapeIter::operator==(const TensorShapeIter &rhs)`
`bool tensorflow::TensorShapeIter::operator!=(const TensorShapeIter &rhs)`
`void tensorflow::TensorShapeIter::operator++()`
`TensorShapeDim tensorflow::TensorShapeIter::operator*()`
# Class `tensorflow::TensorShapeUtils`
Static helper routines for `TensorShape`. Includes a few common predicates on a tensor shape.
## Member Summary
- [`static bool tensorflow::TensorShapeUtils::IsScalar(const TensorShape &shape)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShapeUtils.html#static_bool_tensorflow_TensorShapeUtils_IsScalar)
- [`static bool tensorflow::TensorShapeUtils::IsVector(const TensorShape &shape)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShapeUtils.html#static_bool_tensorflow_TensorShapeUtils_IsVector)
- [`static bool tensorflow::TensorShapeUtils::IsLegacyScalar(const TensorShape &shape)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShapeUtils.html#static_bool_tensorflow_TensorShapeUtils_IsLegacyScalar)
- [`static bool tensorflow::TensorShapeUtils::IsLegacyVector(const TensorShape &shape)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShapeUtils.html#static_bool_tensorflow_TensorShapeUtils_IsLegacyVector)
- [`static bool tensorflow::TensorShapeUtils::IsVectorOrHigher(const TensorShape &shape)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShapeUtils.html#static_bool_tensorflow_TensorShapeUtils_IsVectorOrHigher)
- [`static bool tensorflow::TensorShapeUtils::IsMatrix(const TensorShape &shape)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShapeUtils.html#static_bool_tensorflow_TensorShapeUtils_IsMatrix)
- [`static bool tensorflow::TensorShapeUtils::IsMatrixOrHigher(const TensorShape &shape)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShapeUtils.html#static_bool_tensorflow_TensorShapeUtils_IsMatrixOrHigher)
- [`static TensorShape tensorflow::TensorShapeUtils::MakeShape(const T *dims, int n)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShapeUtils.html#static_TensorShape_tensorflow_TensorShapeUtils_MakeShape)- Returns a `TensorShape` whose dimensions are `dims[0]`, `dims[1]`, ..., `dims[n-1]`.
- [`static string tensorflow::TensorShapeUtils::ShapeListString(const gtl::ArraySlice< TensorShape > &shapes)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShapeUtils.html#static_string_tensorflow_TensorShapeUtils_ShapeListString)
- [`static bool tensorflow::TensorShapeUtils::StartsWith(const TensorShape &shape0, const TensorShape &shape1)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShapeUtils.html#static_bool_tensorflow_TensorShapeUtils_StartsWith)
## Member Details
`static bool tensorflow::TensorShapeUtils::IsScalar(const TensorShape &shape)`
`static bool tensorflow::TensorShapeUtils::IsVector(const TensorShape &shape)`
`static bool tensorflow::TensorShapeUtils::IsLegacyScalar(const TensorShape &shape)`
`static bool tensorflow::TensorShapeUtils::IsLegacyVector(const TensorShape &shape)`
`static bool tensorflow::TensorShapeUtils::IsVectorOrHigher(const TensorShape &shape)`
`static bool tensorflow::TensorShapeUtils::IsMatrix(const TensorShape &shape)`
`static bool tensorflow::TensorShapeUtils::IsMatrixOrHigher(const TensorShape &shape)`
`static TensorShape tensorflow::TensorShapeUtils::MakeShape(const T *dims, int n)`
Returns a `TensorShape` whose dimensions are `dims[0]`, `dims[1]`, ..., `dims[n-1]`.
`static string tensorflow::TensorShapeUtils::ShapeListString(const gtl::ArraySlice< TensorShape > &shapes)`
`static bool tensorflow::TensorShapeUtils::StartsWith(const TensorShape &shape0, const TensorShape &shape1)`
# Class `tensorflow::Thread`
## Member Summary
- [`tensorflow::Thread::Thread()`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassThread.html#tensorflow_Thread_Thread)
- [`virtual tensorflow::Thread::~Thread()`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassThread.html#virtual_tensorflow_Thread_Thread)- Blocks until the thread of control stops running.
## Member Details
`tensorflow::Thread::Thread()`
`virtual tensorflow::Thread::~Thread()`
Blocks until the thread of control stops running.
# Class `tensorflow::WritableFile`
A file abstraction for sequential writing.
The implementation must provide buffering since callers may append small fragments at a time to the file.
## Member Summary
- [`tensorflow::WritableFile::WritableFile()`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassWritableFile.html#tensorflow_WritableFile_WritableFile)
- [`virtual tensorflow::WritableFile::~WritableFile()`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassWritableFile.html#virtual_tensorflow_WritableFile_WritableFile)
- [`virtual Status tensorflow::WritableFile::Append(const StringPiece &data)=0`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassWritableFile.html#virtual_Status_tensorflow_WritableFile_Append)
- [`virtual Status tensorflow::WritableFile::Close()=0`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassWritableFile.html#virtual_Status_tensorflow_WritableFile_Close)
- [`virtual Status tensorflow::WritableFile::Flush()=0`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassWritableFile.html#virtual_Status_tensorflow_WritableFile_Flush)
- [`virtual Status tensorflow::WritableFile::Sync()=0`](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassWritableFile.html#virtual_Status_tensorflow_WritableFile_Sync)
## Member Details
`tensorflow::WritableFile::WritableFile()`
`virtual tensorflow::WritableFile::~WritableFile()`
`virtual Status tensorflow::WritableFile::Append(const StringPiece &data)=0`
`virtual Status tensorflow::WritableFile::Close()=0`
`virtual Status tensorflow::WritableFile::Flush()=0`
`virtual Status tensorflow::WritableFile::Sync()=0`
# Struct `tensorflow::SessionOptions`
Configuration information for a Session .
## Member Summary
- [`Env* tensorflow::SessionOptions::env`](http://www.tensorfly.cn/tfdoc/api_docs/cc/StructSessionOptions.html#Env_tensorflow_SessionOptions_env)- The environment to use.
- [`string tensorflow::SessionOptions::target`](http://www.tensorfly.cn/tfdoc/api_docs/cc/StructSessionOptions.html#string_tensorflow_SessionOptions_target)- The TensorFlow runtime to connect to.
- [`ConfigProto tensorflow::SessionOptions::config`](http://www.tensorfly.cn/tfdoc/api_docs/cc/StructSessionOptions.html#ConfigProto_tensorflow_SessionOptions_config)- Configuration options.
- [`tensorflow::SessionOptions::SessionOptions()`](http://www.tensorfly.cn/tfdoc/api_docs/cc/StructSessionOptions.html#tensorflow_SessionOptions_SessionOptions)
## Member Details
`Env* tensorflow::SessionOptions::env`
The environment to use.
`string tensorflow::SessionOptions::target`
The TensorFlow runtime to connect to.
If 'target' is empty or unspecified, the local TensorFlow runtime implementation will be used. Otherwise, the TensorFlow engine defined by 'target' will be used to perform all computations.
"target" can be either a single entry or a comma separated list of entries. Each entry is a resolvable address of the following format: local ip:port host:port ... other system-specific formats to identify tasks and jobs ...
NOTE: at the moment 'local' maps to an in-process service-based runtime.
Upon creation, a single session affines itself to one of the remote processes, with possible load balancing choices when the "target" resolves to a list of possible processes.
If the session disconnects from the remote process during its lifetime, session calls may fail immediately.
`ConfigProto tensorflow::SessionOptions::config`
Configuration options.
`tensorflow::SessionOptions::SessionOptions()`
# Struct `tensorflow::Status::State`
## Member Summary
- [`tensorflow::error::Code tensorflow::Status::State::code`](http://www.tensorfly.cn/tfdoc/api_docs/cc/StructState.html#tensorflow_error_Code_tensorflow_Status_State_code)
- [`string tensorflow::Status::State::msg`](http://www.tensorfly.cn/tfdoc/api_docs/cc/StructState.html#string_tensorflow_Status_State_msg)
## Member Details
`tensorflow::error::Code tensorflow::Status::State::code`
`string tensorflow::Status::State::msg`
# Struct `tensorflow::TensorShapeDim`
## Member Summary
- [`int tensorflow::TensorShapeDim::size`](http://www.tensorfly.cn/tfdoc/api_docs/cc/StructTensorShapeDim.html#int_tensorflow_TensorShapeDim_size)
- [`tensorflow::TensorShapeDim::TensorShapeDim(int64 s)`](http://www.tensorfly.cn/tfdoc/api_docs/cc/StructTensorShapeDim.html#tensorflow_TensorShapeDim_TensorShapeDim)
## Member Details
`int tensorflow::TensorShapeDim::size`
`tensorflow::TensorShapeDim::TensorShapeDim(int64 s)`
# Struct `tensorflow::ThreadOptions`
Options to configure a Thread .
Note that the options are all hints, and the underlying implementation may choose to ignore it.
## Member Summary
- [`size_t tensorflow::ThreadOptions::stack_size`](http://www.tensorfly.cn/tfdoc/api_docs/cc/StructThreadOptions.html#size_t_tensorflow_ThreadOptions_stack_size)- Thread stack size to use (in bytes).
- [`size_t tensorflow::ThreadOptions::guard_size`](http://www.tensorfly.cn/tfdoc/api_docs/cc/StructThreadOptions.html#size_t_tensorflow_ThreadOptions_guard_size)- Guard area size to use near thread stacks to use (in bytes)
## Member Details
`size_t tensorflow::ThreadOptions::stack_size`
Thread stack size to use (in bytes).
`size_t tensorflow::ThreadOptions::guard_size`
Guard area size to use near thread stacks to use (in bytes)
# TensorFlow C++ Session API reference documentation
TensorFlow's public C++ API includes only the API for executing graphs, as of version 0.5. To control the execution of a graph from C++:
- Build the computation graph using the [Python API](http://www.tensorfly.cn/tfdoc/api_docs/python).
- Use [tf.train.write_graph()](http://www.tensorfly.cn/tfdoc/api_docs/python/train.html#write_graph) to write the graph to a file.
- 
Load the graph using the C++ Session API. For example:
```
// Reads a model graph definition from disk, and creates a session object you
// can use to run it.
Status LoadGraph(string graph_file_name, Session** session) {
 GraphDef graph_def;
 TF_RETURN_IF_ERROR(
     ReadBinaryProto(Env::Default(), graph_file_name, &graph_def));
 TF_RETURN_IF_ERROR(NewSession(SessionOptions(), session));
 TF_RETURN_IF_ERROR((*session)->Create(graph_def));
 return Status::OK();
}
```
- 
Run the graph with a call to `session->Run()`
## Classes
- [tensorflow::Env](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnv.html)
- [tensorflow::EnvWrapper](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnvWrapper.html)
- [tensorflow::RandomAccessFile](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassRandomAccessFile.html)
- [tensorflow::Session](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassSession.html)
- [tensorflow::Status](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassStatus.html)
- [tensorflow::Tensor](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html)
- [tensorflow::TensorBuffer](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorBuffer.html)
- [tensorflow::TensorShape](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShape.html)
- [tensorflow::TensorShapeIter](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShapeIter.html)
- [tensorflow::TensorShapeUtils](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShapeUtils.html)
- [tensorflow::Thread](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassThread.html)
- [tensorflow::WritableFile](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassWritableFile.html)
## Structs
- [tensorflow::SessionOptions](http://www.tensorfly.cn/tfdoc/api_docs/cc/StructSessionOptions.html)
- [tensorflow::Status::State](http://www.tensorfly.cn/tfdoc/api_docs/cc/StructState.html)
- [tensorflow::TensorShapeDim](http://www.tensorfly.cn/tfdoc/api_docs/cc/StructTensorShapeDim.html)
- [tensorflow::ThreadOptions](http://www.tensorfly.cn/tfdoc/api_docs/cc/StructThreadOptions.html)
