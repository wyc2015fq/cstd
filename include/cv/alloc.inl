
static void*
icvDefaultAlloc(size_t size, void*)
{
    char *ptr, *ptr0 = (char*)malloc(
      (size_t)(size + CC_MALLOC_ALIGN*((size >= 4096) + 1) + sizeof(char*)));

    if(!ptr0)
        return 0;

    // align the pointer
    ptr = (char*)cvAlignPtr(ptr0 + sizeof(char*) + 1, CC_MALLOC_ALIGN);
    *(char**)(ptr - sizeof(char*)) = ptr0;

    return ptr;
}


// default <free>
static int
icvDefaultFree(void* ptr, void*)
{
    // Pointer must be aligned by CC_MALLOC_ALIGN
    if(((size_t)ptr & (CC_MALLOC_ALIGN-1)) != 0)
        return CC_BADARG_ERR;
    free(*((char**)ptr - 1));

    return CC_OK;
}


// pointers to allocation functions, initially set to default
static CvAllocFunc p_cvAlloc = icvDefaultAlloc;
static CvFreeFunc p_cvFree = icvDefaultFree;
static void* p_cvAllocUserData = 0;

CC_IMPL void cvSetMemoryManager(CvAllocFunc alloc_func, CvFreeFunc free_func, void* userdata)
{
    CC_FUNCNAME("cvSetMemoryManager");

    __BEGIN__;
    
    if((alloc_func == 0) ^ (free_func == 0))
        CC_ERROR(CC_StsNullPtr, "Either both pointers should be NULL or none of them");

    p_cvAlloc = alloc_func ? alloc_func : icvDefaultAlloc;
    p_cvFree = free_func ? free_func : icvDefaultFree;
    p_cvAllocUserData = userdata;

    __END__;
}


CC_IMPL  void*  cAlloc(size_t size)
{
    void* ptr = 0;
    
    CC_FUNCNAME("cAlloc");

    __BEGIN__;

    if((size_t)size > CC_MAX_ALLOC_SIZE)
        CC_ERROR(CC_StsOutOfRange,
                  "Negative or too large argument of cAlloc function");

    ptr = p_cvAlloc(size, p_cvAllocUserData);
    if(!ptr)
        CC_ERROR(CC_StsNoMem, "Out of memory");

    __END__;

    return ptr;
}


CC_IMPL  void  cvFree_(void* ptr)
{
    CC_FUNCNAME("cvFree_");

    __BEGIN__;

    if(ptr)
    {
        CVStatus status = p_cvFree(ptr, p_cvAllocUserData);
        if(status < 0)
            CC_ERROR(status, "Deallocation error");
    }

    __END__;
}


