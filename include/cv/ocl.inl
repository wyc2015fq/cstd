

#define HAVE_OPENCL1


#pragma warning( disable : 4996 )
#pragma comment(lib,"OpenCL.lib")
#define CL_USE_DEPRECATED_OPENCL_1_1_APIS
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/cl.h>//包含CL的头文件


struct ProgramEntry
{
    const char* name;
    const char* programStr;
    const char* programHash;
};

enum OclVendorID
{
    UNKNOWN_VENDOR=0,
    VENDOR_AMD=1,
    VENDOR_INTEL=2,
    VENDOR_NVIDIA=3
};

enum OclVectorStrategy
{
    // all matrices have its own vector width
    OCL_VECTOR_OWN = 0,
    // all matrices have maximal vector width among all matrices
    // (useful for cases when matrices have different data types)
    OCL_VECTOR_MAX = 1,

    // default strategy
    OCL_VECTOR_DEFAULT = OCL_VECTOR_OWN
};


//! @addtogroup core_opencl
//! @{
typedef struct ocl_device_t {
	cl_uint num_platform;
	cl_platform_id platform_id[4];
	cl_device_id device;
	cl_context context;
	cl_command_queue cmdQueue;


  char name_[64];
  char version_[64];
  int doubleFPConfig_;
  bool hostUnifiedMemory_;
  int maxComputeUnits_;
  size_t maxWorkGroupSize_;
  int type_;
  int deviceVersionMajor_;
  int deviceVersionMinor_;
  char driverVersion_[64];
  char vendorName_[64];
  char deviceVersion_[64];
  OclVendorID vendorID_;
} ocl_device_t;

template<typename _TpCL>
_TpCL ocl_getProp(cl_device_id handle, cl_device_info prop, _TpCL temp)
{
    size_t sz = 0;
    return clGetDeviceInfo(handle, prop, sizeof(temp), &temp, &sz) == CL_SUCCESS &&
        sz == sizeof(temp) ? (temp) : _TpCL();
}

bool getBoolProp(cl_device_id handle, cl_device_info prop)
{
    cl_bool temp = CL_FALSE;
    size_t sz = 0;

    return clGetDeviceInfo(handle, prop, sizeof(temp), &temp, &sz) == CL_SUCCESS &&
        sz == sizeof(temp) ? temp != 0 : false;
}

char* ocl_getStrProp(cl_device_id handle, cl_device_info prop, char* buf, int buflen)
{
    size_t sz=0;
    char* ret = clGetDeviceInfo(handle, prop, buflen-1, buf, &sz) == CL_SUCCESS &&
        sz < buflen ? (buf) : NULL;
    printf("%s\n", ret);
    return ret;
}

// deviceVersion has format
//   OpenCL<space><major_version.minor_version><space><vendor-specific information>
// by specification
//   http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clGetDeviceInfo.html
//   http://www.khronos.org/registry/cl/sdk/1.2/docs/man/xhtml/clGetDeviceInfo.html
static void parseDeviceVersion(const char* deviceVersion, int &major, int &minor)
{
    major = minor = 0;
    if (10 >= strlen(deviceVersion))
        return;
    const char *pstr = deviceVersion;
    if (0 != strncmp(pstr, "OpenCL ", 7))
        return;
    const char* ppos = strchr(deviceVersion+7, '.');
    if (NULL == ppos)
        return;
    const char* temp = deviceVersion+7;
    major = atoi(temp);
    temp = ppos + 1;
    minor = atoi(temp);
}

ocl_device_t* ocl_device_get() {
  static ocl_device_t g_s[1] = {0};
  static int inited=0;
  if (!inited) {
    cl_int err;
    //step 1:初始化OpenCL
    err = clGetPlatformIDs(0, NULL, &g_s->num_platform);
    g_s->num_platform = MIN(g_s->num_platform, 4);
    err = clGetPlatformIDs(g_s->num_platform, g_s->platform_id, NULL);
    
    if (err != CL_SUCCESS) {
      printf("clGetPlatformIDs error\n");
      return 0;
    }
    
    //博主计算机三个plantform，platform_id[2]为CPU，根据情况来改  
    //clGetDeviceIDs(platform_id[1], CL_DEVICE_TYPE_CPU, 1, &device, NULL);
    clGetDeviceIDs(g_s->platform_id[0], CL_DEVICE_TYPE_GPU, 1, &g_s->device, NULL);
    
    ocl_getStrProp(g_s->device, CL_DEVICE_NAME, g_s->name_, countof(g_s->name_));
    ocl_getStrProp(g_s->device, CL_DEVICE_VERSION, g_s->version_, countof(g_s->version_));
    g_s->doubleFPConfig_ = 0;
#ifdef CL_DEVICE_DOUBLE_FP_CONFIG
    g_s->doubleFPConfig_ = (int)ocl_getProp(g_s->device, CL_DEVICE_DOUBLE_FP_CONFIG, cl_device_fp_config());
#endif // CL_DEVICE_DOUBLE_FP_CONFIG
    g_s->hostUnifiedMemory_ = getBoolProp(g_s->device, CL_DEVICE_HOST_UNIFIED_MEMORY);
    g_s->maxComputeUnits_ = (int)ocl_getProp(g_s->device, CL_DEVICE_MAX_COMPUTE_UNITS, cl_uint());
    g_s->maxWorkGroupSize_ = ocl_getProp(g_s->device, CL_DEVICE_MAX_WORK_GROUP_SIZE, size_t());
    g_s->tid_ = (int)ocl_getProp(g_s->device, CL_DEVICE_TYPE, cl_device_type());
    ocl_getStrProp(g_s->device, CL_DRIVER_VERSION, g_s->driverVersion_, countof(g_s->driverVersion_));
    
    ocl_getStrProp(g_s->device, CL_DEVICE_VERSION, g_s->deviceVersion_, countof(g_s->deviceVersion_));
    parseDeviceVersion(g_s->deviceVersion_, g_s->deviceVersionMajor_, g_s->deviceVersionMinor_);
    
    ocl_getStrProp(g_s->device, CL_DEVICE_VENDOR, g_s->vendorName_, countof(g_s->vendorName_));
    if (0==strcmp(g_s->vendorName_, "Advanced Micro Devices, Inc.") || 0==strcmp(g_s->vendorName_, "AMD"))
      g_s->vendorID_ = VENDOR_AMD;
    else if (0==strcmp(g_s->vendorName_, "Intel(R) Corporation") || 0==strcmp(g_s->vendorName_, "Intel") || strstr(g_s->name_, "Iris") != 0)
      g_s->vendorID_ = VENDOR_INTEL;
    else if (0==strcmp(g_s->vendorName_, "NVIDIA Corporation"))
      g_s->vendorID_ = VENDOR_NVIDIA;
    else
      g_s->vendorID_ = UNKNOWN_VENDOR;

    //step 2:创建上下文  
    g_s->context = clCreateContext(NULL, 1, &g_s->device, NULL, NULL, NULL);
    //step 3:创建命令队列  
    g_s->cmdQueue = clCreateCommandQueue(g_s->context, g_s->device, 0, NULL);
    
    inited = 1;
    void ocl_free(void);
    atexit(ocl_free);
  }
  return g_s;
}

void ocl_free(void) {
  ocl_device_t* s = ocl_device_get();
	clReleaseCommandQueue(s->cmdQueue);
	clReleaseContext(s->context);
  return ;
}

struct ProgramSource {
  cl_program program;
  cl_kernel kernel;
  ProgramSource(const char* programStr) {
    program = NULL;
    kernel = NULL;
    set(programStr);}
  ~ProgramSource() {
	  if (program) clReleaseProgram(program);
	  if (kernel) clReleaseKernel(kernel);
  }
  int run(const char * kernel_name, int dims, size_t _globalsize[], size_t _localsize[], int narg, buf_t* args) {
    ocl_device_t* dev = ocl_device_get();
    if (!kernel)
      kernel = clCreateKernel(program, kernel_name, NULL);
    int i;
    for (i=0; i<narg; ++i) {
      clSetKernelArg(kernel, i, args[i].len, args[i].data);
    }
    clEnqueueNDRangeKernel(dev->cmdQueue, kernel, dims, NULL, _globalsize, _localsize, 0, NULL, NULL);
    return 0;
  }
  int set(const char * progstr) {
    ocl_device_t* dev = ocl_device_get();
    size_t srclen = strlen(progstr);
    cl_int retval = 0;
    program = clCreateProgramWithSource(dev->context, 1, (const char**)&progstr, &srclen, &retval);
    if (program && retval == CL_SUCCESS) {
      const char* buildflags = NULL;
      if (dev->vendorID_ == VENDOR_AMD)
        buildflags = " -D AMD_DEVICE";
      else if (dev->vendorID_ == VENDOR_INTEL)
        buildflags = " -D INTEL_DEVICE";
      
      retval = clBuildProgram(program, 1, &dev->device, buildflags, NULL, NULL);
      if( retval != CL_SUCCESS ) {
        size_t retsz = 0;
        cl_int buildInfo_retval = clGetProgramBuildInfo(program, dev->device,
          CL_PROGRAM_BUILD_LOG, 0, 0, &retsz);
        if (buildInfo_retval == CL_SUCCESS && retsz > 1)
        {
          char* bufbuf = (char*)MALLOC(char, retsz + 16);
          char* buf = bufbuf;
          buildInfo_retval = clGetProgramBuildInfo(program, dev->device,
            CL_PROGRAM_BUILD_LOG, retsz+1, buf, &retsz);
          if (buildInfo_retval == CL_SUCCESS)
          {
            // TODO It is useful to see kernel name & program file name also
            const char* errmsg = (buf);
            printf("OpenCL program build log: %s\n%s\n", buildflags, errmsg);
            fflush(stdout);
          }
          FREE(bufbuf);
        }
        if (retval != CL_SUCCESS && program)
        {
          clReleaseProgram(program);
          program = NULL;
        }
      }
    }
    return 0;
  }
};

//#include "ocl/opencl_kernels_core.inl"



#if 0

bool haveOpenCL()
{
#ifdef HAVE_OPENCL
    static bool g_isOpenCLInitialized = false;
    static bool g_isOpenCLAvailable = false;

    if (!g_isOpenCLInitialized)
    {
        cl_uint n = 0;
        g_isOpenCLAvailable = ::clGetPlatformIDs(0, NULL, &n) == CL_SUCCESS;
        g_isOpenCLInitialized = true;
    }
    return g_isOpenCLAvailable;
#else
    return false;
#endif
}

bool useOpenCL()
{
    CoreTLSData* data = getCoreTlsData().get();
    if( data->useOpenCL < 0 )
    {
        data->useOpenCL = (int)haveOpenCL() && Device::getDefault()->tt.data && Device::getDefault().available();
    }
    return data->useOpenCL > 0;
}

bool haveAmdBlas();
bool haveAmdFft();
void setUseOpenCL(bool flag);
void finish();

CC_EXPORTS bool haveSVM();

class Device
{
public:
    Device();
    explicit Device(void* d);
    Device(const Device& d);
    Device& operator = (const Device& d);
    ~Device();

    void set(void* d);

    enum
    {
        TYPE_DEFAULT     = (1 << 0),
        TYPE_CPU         = (1 << 1),
        TYPE_GPU         = (1 << 2),
        TYPE_ACCELERATOR = (1 << 3),
        TYPE_DGPU        = TYPE_GPU + (1 << 16),
        TYPE_IGPU        = TYPE_GPU + (1 << 17),
        TYPE_ALL         = 0xFFFFFFFF
    };

    String name() const;
    String extensions() const;
    String version() const;
    String vendorName() const;
    String OpenCL_C_Version() const;
    String OpenCLVersion() const;
    int deviceVersionMajor() const;
    int deviceVersionMinor() const;
    String driverVersion() const;
    void* ptr() const;

    int type() const;

    int addressBits() const;
    bool available() const;
    bool compilerAvailable() const;
    bool linkerAvailable() const;

    enum
    {
        FP_DENORM=(1 << 0),
        FP_INF_NAN=(1 << 1),
        FP_ROUND_TO_NEAREST=(1 << 2),
        FP_ROUND_TO_ZERO=(1 << 3),
        FP_ROUND_TO_INF=(1 << 4),
        FP_FMA=(1 << 5),
        FP_SOFT_FLOAT=(1 << 6),
        FP_CORRECTLY_ROUNDED_DIVIDE_SQRT=(1 << 7)
    };
    int doubleFPConfig() const;
    int singleFPConfig() const;
    int halfFPConfig() const;

    bool endianLittle() const;
    bool errorCorrectionSupport() const;

    enum
    {
        EXEC_KERNEL=(1 << 0),
        EXEC_NATIVE_KERNEL=(1 << 1)
    };
    int executionCapabilities() const;

    size_t globalMemCacheSize() const;

    enum
    {
        NO_CACHE=0,
        READ_ONLY_CACHE=1,
        READ_WRITE_CACHE=2
    };
    int globalMemCacheType() const;
    int globalMemCacheLineSize() const;
    size_t globalMemSize() const;

    size_t localMemSize() const;
    enum
    {
        NO_LOCAL_MEM=0,
        LOCAL_IS_LOCAL=1,
        LOCAL_IS_GLOBAL=2
    };
    int localMemType() const;
    bool hostUnifiedMemory() const;

    bool imageSupport() const;

    bool imageFromBufferSupport() const;
    uint imagePitchAlignment() const;
    uint imageBaseAddressAlignment() const;

    size_t image2DMaxWidth() const;
    size_t image2DMaxHeight() const;

    size_t image3DMaxWidth() const;
    size_t image3DMaxHeight() const;
    size_t image3DMaxDepth() const;

    size_t imageMaxBufferSize() const;
    size_t imageMaxArraySize() const;

    int vendorID() const;
    // FIXIT
    // dev.isAMD() doesn't work for OpenCL CPU devices from AMD OpenCL platform.
    // This method should use platform name instead of vendor name.
    // After fix restore code in arithm.cpp: ocl_compare()
    inline bool isAMD() const { return vendorID() == VENDOR_AMD; }
    inline bool isIntel() const { return vendorID() == VENDOR_INTEL; }
    inline bool isNVidia() const { return vendorID() == VENDOR_NVIDIA; }

    int maxClockFrequency() const;
    int maxComputeUnits() const;
    int maxConstantArgs() const;
    size_t maxConstantBufferSize() const;

    size_t maxMemAllocSize() const;
    size_t maxParameterSize() const;

    int maxReadImageArgs() const;
    int maxWriteImageArgs() const;
    int maxSamplers() const;

    size_t maxWorkGroupSize() const;
    int maxWorkItemDims() const;
    void maxWorkItemSizes(size_t*) const;

    int memBaseAddrAlign() const;

    int nativeVectorWidthChar() const;
    int nativeVectorWidthShort() const;
    int nativeVectorWidthInt() const;
    int nativeVectorWidthLong() const;
    int nativeVectorWidthFloat() const;
    int nativeVectorWidthDouble() const;
    int nativeVectorWidthHalf() const;

    int preferredVectorWidthChar() const;
    int preferredVectorWidthShort() const;
    int preferredVectorWidthInt() const;
    int preferredVectorWidthLong() const;
    int preferredVectorWidthFloat() const;
    int preferredVectorWidthDouble() const;
    int preferredVectorWidthHalf() const;

    size_t printfBufferSize() const;
    size_t profilingTimerResolution() const;

    static const Device& getDefault();

protected:
    struct Impl;
    Impl* p;
};


class CC_EXPORTS Context
{
public:
    Context();
    explicit Context(int dtype);
    ~Context();
    Context(const Context& c);
    Context& operator = (const Context& c);

    bool create();
    bool create(int dtype);
    size_t ndevices() const;
    const Device& device(size_t idx) const;
    Program getProg(const ProgramSource& prog,
                    const String& buildopt, String& errmsg);

    static Context& getDefault(bool initialize = true);
    void* ptr() const;

    friend void initializeContextFromHandle(Context& ctx, void* platform, void* context, void* device);

    bool useSVM() const;
    void setUseSVM(bool enabled);

    struct Impl;
    Impl* p;
};

class CC_EXPORTS Platform
{
public:
    Platform();
    ~Platform();
    Platform(const Platform& p);
    Platform& operator = (const Platform& p);

    void* ptr() const;
    static Platform& getDefault();

    friend void initializeContextFromHandle(Context& ctx, void* platform, void* context, void* device);
protected:
    struct Impl;
    Impl* p;
};

/*
//! @brief Attaches OpenCL context to OpenCV
//
//! @note Note:
//    OpenCV will check if available OpenCL platform has platformName name,
//    then assign context to OpenCV and call clRetainContext function.
//    The deviceID device will be used as target device and new command queue
//    will be created.
//
// Params:
//! @param platformName - name of OpenCL platform to attach,
//!                       this string is used to check if platform is available
//!                       to OpenCV at runtime
//! @param platfromID   - ID of platform attached context was created for
//! @param context      - OpenCL context to be attached to OpenCV
//! @param deviceID     - ID of device, must be created from attached context
*/
CC_EXPORTS void attachContext(const String& platformName, void* platformID, void* context, void* deviceID);

/*
//! @brief Convert OpenCL buffer to UMat
//
//! @note Note:
//   OpenCL buffer (cl_mem_buffer) should contain 2D image data, compatible with OpenCV.
//   Memory content is not copied from clBuffer to UMat. Instead, buffer handle assigned
//   to UMat and clRetainMemObject is called.
//
// Params:
//! @param  cl_mem_buffer - source clBuffer handle
//! @param  step          - num of bytes in single row
//! @param  rows          - number of rows
//! @param  cols          - number of cols
//! @param  type          - OpenCV type of image
//! @param  dst           - destination UMat
*/
CC_EXPORTS void convertFromBuffer(void* cl_mem_buffer, size_t step, int rows, int cols, int type, UMat& dst);

/*
//! @brief Convert OpenCL image2d_t to UMat
//
//! @note Note:
//   OpenCL image2d_t (cl_mem_image), should be compatible with OpenCV
//   UMat formats.
//   Memory content is copied from image to UMat with
//   clEnqueueCopyImageToBuffer function.
//
// Params:
//! @param  cl_mem_image - source image2d_t handle
//! @param  dst          - destination UMat
*/
CC_EXPORTS void convertFromImage(void* cl_mem_image, UMat& dst);

// TODO Move to internal header
void initializeContextFromHandle(Context& ctx, void* platform, void* context, void* device);

class Queue
{
public:
    Queue();
    explicit Queue(const Context& c, const Device& d=Device());
    ~Queue();
    Queue(const Queue& q);
    Queue& operator = (const Queue& q);

    bool create(const Context& c=Context(), const Device& d=Device());
    void finish();
    void* ptr() const;
    static Queue& getDefault();

protected:
    struct Impl;
    Impl* p;
};

class KernelArg
{
public:
    enum { LOCAL=1, READ_ONLY=2, WRITE_ONLY=4, READ_WRITE=6, CONSTANT=8, PTR_ONLY = 16, NO_SIZE=256 };
    KernelArg(int _flags, UMat* _m, int wscale=1, int iwscale=1, const void* _obj=0, size_t _sz=0);
    KernelArg();

    static KernelArg Local() { return KernelArg(LOCAL, 0); }
    static KernelArg PtrWriteOnly(const UMat& m)
    { return KernelArg(PTR_ONLY+WRITE_ONLY, (UMat*)&m); }
    static KernelArg PtrReadOnly(const UMat& m)
    { return KernelArg(PTR_ONLY+READ_ONLY, (UMat*)&m); }
    static KernelArg PtrReadWrite(const UMat& m)
    { return KernelArg(PTR_ONLY+READ_WRITE, (UMat*)&m); }
    static KernelArg ReadWrite(const UMat& m, int wscale=1, int iwscale=1)
    { return KernelArg(READ_WRITE, (UMat*)&m, wscale, iwscale); }
    static KernelArg ReadWriteNoSize(const UMat& m, int wscale=1, int iwscale=1)
    { return KernelArg(READ_WRITE+NO_SIZE, (UMat*)&m, wscale, iwscale); }
    static KernelArg ReadOnly(const UMat& m, int wscale=1, int iwscale=1)
    { return KernelArg(READ_ONLY, (UMat*)&m, wscale, iwscale); }
    static KernelArg WriteOnly(const UMat& m, int wscale=1, int iwscale=1)
    { return KernelArg(WRITE_ONLY, (UMat*)&m, wscale, iwscale); }
    static KernelArg ReadOnlyNoSize(const UMat& m, int wscale=1, int iwscale=1)
    { return KernelArg(READ_ONLY+NO_SIZE, (UMat*)&m, wscale, iwscale); }
    static KernelArg WriteOnlyNoSize(const UMat& m, int wscale=1, int iwscale=1)
    { return KernelArg(WRITE_ONLY+NO_SIZE, (UMat*)&m, wscale, iwscale); }
    static KernelArg Constant(const CvMat& m);
    template<typename _Tp> static KernelArg Constant(const _Tp* arr, size_t n)
    { return KernelArg(CONSTANT, 0, 1, 1, (void*)arr, n); }

    int flags;
    UMat* m;
    const void* obj;
    size_t sz;
    int wscale, iwscale;
};

class Kernel
{
public:
    Kernel();
    Kernel(const char* kname, const Program& prog);
    Kernel(const char* kname, const ProgramSource& prog,
           const String& buildopts = String(), String* errmsg=0);
    ~Kernel();
    Kernel(const Kernel& k);
    Kernel& operator = (const Kernel& k);

    bool empty() const;
    bool create(const char* kname, const Program& prog);
    bool create(const char* kname, const ProgramSource& prog,
                const String& buildopts, String* errmsg=0);

    int set(int i, const void* value, size_t sz);
    int set(int i, const Image2D& image2D);
    int set(int i, const UMat& m);
    int set(int i, const KernelArg& arg);
    template<typename _Tp> int set(int i, const _Tp& value)
    { return set(i, &value, sizeof(value)); }

    template<typename _Tp0>
    Kernel& args(const _Tp0& a0)
    {
        set(0, a0); return *this;
    }

    template<typename _Tp0, typename _Tp1>
    Kernel& args(const _Tp0& a0, const _Tp1& a1)
    {
        int i = set(0, a0); set(i, a1); return *this;
    }

    template<typename _Tp0, typename _Tp1, typename _Tp2>
    Kernel& args(const _Tp0& a0, const _Tp1& a1, const _Tp2& a2)
    {
        int i = set(0, a0); i = set(i, a1); set(i, a2); return *this;
    }

    template<typename _Tp0, typename _Tp1, typename _Tp2, typename _Tp3>
    Kernel& args(const _Tp0& a0, const _Tp1& a1, const _Tp2& a2, const _Tp3& a3)
    {
        int i = set(0, a0); i = set(i, a1); i = set(i, a2); i = set(i, a3); return *this;
    }

    template<typename _Tp0, typename _Tp1, typename _Tp2, typename _Tp3, typename _Tp4>
    Kernel& args(const _Tp0& a0, const _Tp1& a1, const _Tp2& a2,
                 const _Tp3& a3, const _Tp4& a4)
    {
        int i = set(0, a0); i = set(i, a1); i = set(i, a2);
        i = set(i, a3); set(i, a4); return *this;
    }

    template<typename _Tp0, typename _Tp1, typename _Tp2,
             typename _Tp3, typename _Tp4, typename _Tp5>
    Kernel& args(const _Tp0& a0, const _Tp1& a1, const _Tp2& a2,
                 const _Tp3& a3, const _Tp4& a4, const _Tp5& a5)
    {
        int i = set(0, a0); i = set(i, a1); i = set(i, a2);
        i = set(i, a3); i = set(i, a4); set(i, a5); return *this;
    }

    template<typename _Tp0, typename _Tp1, typename _Tp2, typename _Tp3,
             typename _Tp4, typename _Tp5, typename _Tp6>
    Kernel& args(const _Tp0& a0, const _Tp1& a1, const _Tp2& a2, const _Tp3& a3,
                 const _Tp4& a4, const _Tp5& a5, const _Tp6& a6)
    {
        int i = set(0, a0); i = set(i, a1); i = set(i, a2); i = set(i, a3);
        i = set(i, a4); i = set(i, a5); set(i, a6); return *this;
    }

    template<typename _Tp0, typename _Tp1, typename _Tp2, typename _Tp3,
             typename _Tp4, typename _Tp5, typename _Tp6, typename _Tp7>
    Kernel& args(const _Tp0& a0, const _Tp1& a1, const _Tp2& a2, const _Tp3& a3,
                 const _Tp4& a4, const _Tp5& a5, const _Tp6& a6, const _Tp7& a7)
    {
        int i = set(0, a0); i = set(i, a1); i = set(i, a2); i = set(i, a3);
        i = set(i, a4); i = set(i, a5); i = set(i, a6); set(i, a7); return *this;
    }

    template<typename _Tp0, typename _Tp1, typename _Tp2, typename _Tp3, typename _Tp4,
             typename _Tp5, typename _Tp6, typename _Tp7, typename _Tp8>
    Kernel& args(const _Tp0& a0, const _Tp1& a1, const _Tp2& a2, const _Tp3& a3,
                 const _Tp4& a4, const _Tp5& a5, const _Tp6& a6, const _Tp7& a7,
                 const _Tp8& a8)
    {
        int i = set(0, a0); i = set(i, a1); i = set(i, a2); i = set(i, a3); i = set(i, a4);
        i = set(i, a5); i = set(i, a6); i = set(i, a7); set(i, a8); return *this;
    }

    template<typename _Tp0, typename _Tp1, typename _Tp2, typename _Tp3, typename _Tp4,
             typename _Tp5, typename _Tp6, typename _Tp7, typename _Tp8, typename _Tp9>
    Kernel& args(const _Tp0& a0, const _Tp1& a1, const _Tp2& a2, const _Tp3& a3,
                 const _Tp4& a4, const _Tp5& a5, const _Tp6& a6, const _Tp7& a7,
                 const _Tp8& a8, const _Tp9& a9)
    {
        int i = set(0, a0); i = set(i, a1); i = set(i, a2); i = set(i, a3); i = set(i, a4); i = set(i, a5);
        i = set(i, a6); i = set(i, a7); i = set(i, a8); set(i, a9); return *this;
    }

    template<typename _Tp0, typename _Tp1, typename _Tp2, typename _Tp3,
             typename _Tp4, typename _Tp5, typename _Tp6, typename _Tp7,
             typename _Tp8, typename _Tp9, typename _Tp10>
    Kernel& args(const _Tp0& a0, const _Tp1& a1, const _Tp2& a2, const _Tp3& a3,
                 const _Tp4& a4, const _Tp5& a5, const _Tp6& a6, const _Tp7& a7,
                 const _Tp8& a8, const _Tp9& a9, const _Tp10& a10)
    {
        int i = set(0, a0); i = set(i, a1); i = set(i, a2); i = set(i, a3); i = set(i, a4); i = set(i, a5);
        i = set(i, a6); i = set(i, a7); i = set(i, a8); i = set(i, a9); set(i, a10); return *this;
    }

    template<typename _Tp0, typename _Tp1, typename _Tp2, typename _Tp3,
             typename _Tp4, typename _Tp5, typename _Tp6, typename _Tp7,
             typename _Tp8, typename _Tp9, typename _Tp10, typename _Tp11>
    Kernel& args(const _Tp0& a0, const _Tp1& a1, const _Tp2& a2, const _Tp3& a3,
                 const _Tp4& a4, const _Tp5& a5, const _Tp6& a6, const _Tp7& a7,
                 const _Tp8& a8, const _Tp9& a9, const _Tp10& a10, const _Tp11& a11)
    {
        int i = set(0, a0); i = set(i, a1); i = set(i, a2); i = set(i, a3); i = set(i, a4); i = set(i, a5);
        i = set(i, a6); i = set(i, a7); i = set(i, a8); i = set(i, a9); i = set(i, a10); set(i, a11); return *this;
    }

    template<typename _Tp0, typename _Tp1, typename _Tp2, typename _Tp3,
             typename _Tp4, typename _Tp5, typename _Tp6, typename _Tp7,
             typename _Tp8, typename _Tp9, typename _Tp10, typename _Tp11, typename _Tp12>
    Kernel& args(const _Tp0& a0, const _Tp1& a1, const _Tp2& a2, const _Tp3& a3,
                 const _Tp4& a4, const _Tp5& a5, const _Tp6& a6, const _Tp7& a7,
                 const _Tp8& a8, const _Tp9& a9, const _Tp10& a10, const _Tp11& a11,
                 const _Tp12& a12)
    {
        int i = set(0, a0); i = set(i, a1); i = set(i, a2); i = set(i, a3); i = set(i, a4); i = set(i, a5);
        i = set(i, a6); i = set(i, a7); i = set(i, a8); i = set(i, a9); i = set(i, a10); i = set(i, a11);
        set(i, a12); return *this;
    }

    template<typename _Tp0, typename _Tp1, typename _Tp2, typename _Tp3,
             typename _Tp4, typename _Tp5, typename _Tp6, typename _Tp7,
             typename _Tp8, typename _Tp9, typename _Tp10, typename _Tp11, typename _Tp12,
             typename _Tp13>
    Kernel& args(const _Tp0& a0, const _Tp1& a1, const _Tp2& a2, const _Tp3& a3,
                 const _Tp4& a4, const _Tp5& a5, const _Tp6& a6, const _Tp7& a7,
                 const _Tp8& a8, const _Tp9& a9, const _Tp10& a10, const _Tp11& a11,
                 const _Tp12& a12, const _Tp13& a13)
    {
        int i = set(0, a0); i = set(i, a1); i = set(i, a2); i = set(i, a3); i = set(i, a4); i = set(i, a5);
        i = set(i, a6); i = set(i, a7); i = set(i, a8); i = set(i, a9); i = set(i, a10); i = set(i, a11);
        i = set(i, a12); set(i, a13); return *this;
    }

    template<typename _Tp0, typename _Tp1, typename _Tp2, typename _Tp3,
             typename _Tp4, typename _Tp5, typename _Tp6, typename _Tp7,
             typename _Tp8, typename _Tp9, typename _Tp10, typename _Tp11, typename _Tp12,
             typename _Tp13, typename _Tp14>
    Kernel& args(const _Tp0& a0, const _Tp1& a1, const _Tp2& a2, const _Tp3& a3,
                 const _Tp4& a4, const _Tp5& a5, const _Tp6& a6, const _Tp7& a7,
                 const _Tp8& a8, const _Tp9& a9, const _Tp10& a10, const _Tp11& a11,
                 const _Tp12& a12, const _Tp13& a13, const _Tp14& a14)
    {
        int i = set(0, a0); i = set(i, a1); i = set(i, a2); i = set(i, a3); i = set(i, a4); i = set(i, a5);
        i = set(i, a6); i = set(i, a7); i = set(i, a8); i = set(i, a9); i = set(i, a10); i = set(i, a11);
        i = set(i, a12); i = set(i, a13); set(i, a14); return *this;
    }

    template<typename _Tp0, typename _Tp1, typename _Tp2, typename _Tp3,
             typename _Tp4, typename _Tp5, typename _Tp6, typename _Tp7,
             typename _Tp8, typename _Tp9, typename _Tp10, typename _Tp11, typename _Tp12,
             typename _Tp13, typename _Tp14, typename _Tp15>
    Kernel& args(const _Tp0& a0, const _Tp1& a1, const _Tp2& a2, const _Tp3& a3,
                 const _Tp4& a4, const _Tp5& a5, const _Tp6& a6, const _Tp7& a7,
                 const _Tp8& a8, const _Tp9& a9, const _Tp10& a10, const _Tp11& a11,
                 const _Tp12& a12, const _Tp13& a13, const _Tp14& a14, const _Tp15& a15)
    {
        int i = set(0, a0); i = set(i, a1); i = set(i, a2); i = set(i, a3); i = set(i, a4); i = set(i, a5);
        i = set(i, a6); i = set(i, a7); i = set(i, a8); i = set(i, a9); i = set(i, a10); i = set(i, a11);
        i = set(i, a12); i = set(i, a13); i = set(i, a14); set(i, a15); return *this;
    }
    /*
    Run the OpenCL kernel.
    @param dims the work problem dimensions. It is the length of globalsize and localsize. It can be either 1, 2 or 3.
    @param globalsize work items for each dimension.
    It is not the final globalsize passed to OpenCL.
    Each dimension will be adjusted to the nearest integer divisible by the corresponding value in localsize.
    If localsize is NULL, it will still be adjusted depending on dims.
    The adjusted values are greater than or equal to the original values.
    @param localsize work-group size for each dimension.
    @param sync specify whether to wait for OpenCL computation to finish before return.
    @param q command queue
    */
    bool run(int dims, size_t globalsize[],
             size_t localsize[], bool sync, const Queue& q=Queue());
    bool runTask(bool sync, const Queue& q=Queue());

    size_t workGroupSize() const;
    size_t preferedWorkGroupSizeMultiple() const;
    bool compileWorkGroupSize(size_t wsz[]) const;
    size_t localMemSize() const;

    void* ptr() const;
    struct Impl;

protected:
    Impl* p;
};

class CC_EXPORTS Program
{
public:
    Program();
    Program(const ProgramSource& src,
            const String& buildflags, String& errmsg);
    explicit Program(const String& buf);
    Program(const Program& prog);

    Program& operator = (const Program& prog);
    ~Program();

    bool create(const ProgramSource& src,
                const String& buildflags, String& errmsg);
    bool read(const String& buf, const String& buildflags);
    bool write(String& buf) const;

    const ProgramSource& source() const;
    void* ptr() const;

    String getPrefix() const;
    static String getPrefix(const String& buildflags);

protected:
    struct Impl;
    Impl* p;
};


class CC_EXPORTS ProgramSource
{
public:
    typedef uint64 hash_t;

    ProgramSource();
    explicit ProgramSource(const String& prog);
    explicit ProgramSource(const char* prog);
    ~ProgramSource();
    ProgramSource(const ProgramSource& prog);
    ProgramSource& operator = (const ProgramSource& prog);

    const String& source() const;
    hash_t hash() const;

protected:
    struct Impl;
    Impl* p;
};

class CC_EXPORTS PlatformInfo
{
public:
    PlatformInfo();
    explicit PlatformInfo(void* id);
    ~PlatformInfo();

    PlatformInfo(const PlatformInfo& i);
    PlatformInfo& operator =(const PlatformInfo& i);

    String name() const;
    String vendor() const;
    String version() const;
    int deviceNumber() const;
    void getDevice(Device& device, int d) const;

protected:
    struct Impl;
    Impl* p;
};

CC_EXPORTS const char* convertTypeStr(int sdepth, int ddepth, int cn, char* buf);
CC_EXPORTS const char* typeToStr(int t);
CC_EXPORTS const char* memopTypeToStr(int t);
CC_EXPORTS const char* vecopTypeToStr(int t);
CC_EXPORTS String kernelToStr(const CvMat* _kernel, int ddepth = -1, const char * name = NULL);
CC_EXPORTS void getPlatfomsInfo(std::vector<PlatformInfo>& platform_info);


CC_EXPORTS int predictOptimalVectorWidth(const CvMat* src1, const CvMat* src2 = noArray(), const CvMat* src3 = noArray(),
                                         const CvMat* src4 = noArray(), const CvMat* src5 = noArray(), const CvMat* src6 = noArray(),
                                         const CvMat* src7 = noArray(), const CvMat* src8 = noArray(), const CvMat* src9 = noArray(),
                                         OclVectorStrategy strat = OCL_VECTOR_DEFAULT);

CC_EXPORTS int checkOptimalVectorWidth(const int *vectorWidths,
                                       const CvMat* src1, const CvMat* src2 = noArray(), const CvMat* src3 = noArray(),
                                       const CvMat* src4 = noArray(), const CvMat* src5 = noArray(), const CvMat* src6 = noArray(),
                                       const CvMat* src7 = noArray(), const CvMat* src8 = noArray(), const CvMat* src9 = noArray(),
                                       OclVectorStrategy strat = OCL_VECTOR_DEFAULT);

// with OCL_VECTOR_MAX strategy
CC_EXPORTS int predictOptimalVectorWidthMax(const CvMat* src1, const CvMat* src2 = noArray(), const CvMat* src3 = noArray(),
                                            const CvMat* src4 = noArray(), const CvMat* src5 = noArray(), const CvMat* src6 = noArray(),
                                            const CvMat* src7 = noArray(), const CvMat* src8 = noArray(), const CvMat* src9 = noArray());

CC_EXPORTS void buildOptionsAddMatrixDescription(String& buildOptions, const String& name, const CvMat* _m);

class CC_EXPORTS Image2D
{
public:
    Image2D();

    // src:     The UMat from which to get image properties and data
    // norm:    Flag to enable the use of normalized channel data types
    // alias:   Flag indicating that the image should alias the src UMat.
    //          If true, changes to the image or src will be reflected in
    //          both objects.
    explicit Image2D(const UMat &src, bool norm = false, bool alias = false);
    Image2D(const Image2D & i);
    ~Image2D();

    Image2D & operator = (const Image2D & i);

    // Indicates if creating an aliased image should succeed.  Depends on the
    // underlying platform and the dimensions of the UMat.
    static bool canCreateAlias(const UMat &u);

    // Indicates if the image format is supported.
    static bool isFormatSupported(int depth, int cn, bool norm);

    void* ptr() const;
protected:
    struct Impl;
    Impl* p;
};


CC_EXPORTS MatAllocator* getOpenCLAllocator();


#ifdef __OPENCC_BUILD
namespace internal {

CC_EXPORTS bool isOpenCLForced();
#define OCL_FORCE_CHECK(condition) (ocl::internal::isOpenCLForced() || (condition))

CC_EXPORTS bool isPerformanceCheckBypassed();
#define OCL_PERFORMANCE_CHECK(condition) (ocl::internal::isPerformanceCheckBypassed() || (condition))

CC_EXPORTS bool isCLBuffer(UMat& u);

} // namespace internal
#endif

//! @}

#endif
