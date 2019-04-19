# cygwin不支持__malloc_hook - Koma Hub - CSDN博客
2019年01月29日 21:13:03[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：77
GLIBC-2.28中malloc.h里面有这样一段：
```cpp
/* Hooks for debugging and user-defined versions. */
extern void (*__MALLOC_HOOK_VOLATILE __free_hook) (void *__ptr,
                                                   const void *)
__MALLOC_DEPRECATED;
extern void *(*__MALLOC_HOOK_VOLATILE __malloc_hook)(size_t __size,
                                                     const void *)
__MALLOC_DEPRECATED;
extern void *(*__MALLOC_HOOK_VOLATILE __realloc_hook)(void *__ptr,
                                                      size_t __size,
                                                      const void *)
__MALLOC_DEPRECATED;
extern void *(*__MALLOC_HOOK_VOLATILE __memalign_hook)(size_t __alignment,
                                                       size_t __size,
                                                       const void *)
__MALLOC_DEPRECATED;
extern void (*__MALLOC_HOOK_VOLATILE __after_morecore_hook) (void);
/* Activate a standard set of debugging hooks. */
extern void __malloc_check_init (void) __THROW __MALLOC_DEPRECATED;
```
但是在cygwin的malloc.h中却找不到类似于hook的任何语句。
