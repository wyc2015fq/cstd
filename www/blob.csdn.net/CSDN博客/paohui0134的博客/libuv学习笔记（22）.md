
# libuv学习笔记（22） - paohui0134的博客 - CSDN博客


2016年07月02日 13:51:50[paohui0134](https://me.csdn.net/paohui0134)阅读数：385


# libuv学习笔记（22）
## 动态库相关数据结构与函数
### 数据结构
```python
typedef
```
```python
struct
```
```python
{
  HMODULE handle;
```
```python
char
```
```python
* errmsg;
} uv_lib_t;
```
### 相关函数
#### 打开动态库
```python
int
```
```python
uv_dlopen(
```
```python
const
```
```python
char
```
```python
* filename, uv_lib_t* lib) {
  WCHAR filename_w[
```
```python
32768
```
```python
];
  lib->handle =
```
```python
NULL
```
```python
;
  lib->errmsg =
```
```python
NULL
```
```python
;
```
```python
//utf8转unicod
```
```python
if
```
```python
(!MultiByteToWideChar(CP_UTF8,
```
```python
0
```
```python
,
                           filename,
                           -
```
```python
1
```
```python
,
                           filename_w,
                           ARRAY_SIZE(filename_w))) {
```
```python
return
```
```python
uv__dlerror(lib, GetLastError());
  }
```
```python
//调用API动态加载dll
```
```python
lib->handle = LoadLibraryExW(filename_w,
```
```python
NULL
```
```python
, LOAD_WITH_ALTERED_SEARCH_PATH);
```
```python
if
```
```python
(lib->handle ==
```
```python
NULL
```
```python
) {
```
```python
return
```
```python
uv__dlerror(lib, GetLastError());
  }
```
```python
return
```
```python
0
```
```python
;
}
```
#### 关闭一个动态库
```python
void
```
```python
uv_dlclose(uv_lib_t
```
```python
*
```
```python
lib) {
```
```python
if
```
```python
(lib
```
```python
->
```
```python
errmsg) {
    LocalFree((
```
```python
void
```
```python
*
```
```python
)lib
```
```python
->
```
```python
errmsg);
```
```python
//释放错误信息内存
```
```python
lib
```
```python
->
```
```python
errmsg
```
```python
=
```
```python
NULL
```
```python
;
  }
```
```python
if
```
```python
(lib
```
```python
->
```
```python
handle
```
```python
) {
```
```python
/* Ignore errors. No good way to signal them without leaking memory. */
```
```python
FreeLibrary(lib
```
```python
->
```
```python
handle
```
```python
);
    lib
```
```python
->
```
```python
handle
```
```python
=
```
```python
NULL
```
```python
;
  }
}
```
#### 获取动态库中函数的地址
```python
int
```
```python
uv_dlsym(uv_lib_t* lib,
```
```python
const
```
```python
char
```
```python
* name,
```
```python
void
```
```python
** ptr) {
  *ptr = (
```
```python
void
```
```python
*) GetProcAddress(lib->handle, name);
```
```python
return
```
```python
uv__dlerror(lib, *ptr ?
```
```python
0
```
```python
: GetLastError());
}
```
#### 获取动态库相关错误
```python
const
```
```python
char
```
```python
* uv_dlerror(
```
```python
const
```
```python
uv_lib_t* lib) {
```
```python
return
```
```python
lib->errmsg ? lib->errmsg :
```
```python
"no error"
```
```python
;
}
```
uv_lib_t只是简单的对dll的动态加载、卸载和获取函数地址做了封装，与loop并没有什么关系，可以单独使用。

