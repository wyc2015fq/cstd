# linux下加载动态链接库的方法 - h13 - 博客园
linux下加载动态链接库的方法：以下以触摸屏为例：介绍是如何加载一下共享库的。
首先让我们看看以下几个函数：
dlopen（）
　　功能：打开一个动态链接库 
　　包含头文件： 
　　#include <dlfcn.h> 
　　函数定义： 
　　void * dlopen( const char * pathname, int mode ); 
　　函数描述： 
　　在dlopen的（）函数以指定模式打开指定的动态连接库文件，并返回一个句柄给调用进程。使用dlclose（）来卸载打开的库。
dlclose(handle);
    功能：关闭一个动态链接库
dlsym()
    函数原型: void* dlsym(void* handle,const char* symbol) 
　　该函数在<dlfcn.h>文件中。 
　　handle是由dlopen打开动态链接库后返回的指针，symbol就是要求获取的函数的名称，函数返回值是void*,指向函数的地址，供调用使用
getenv()
　　功 能: 从环境中取字符串,获取环境变量的值
 　　头文件: stdlib.h
 　　用 法:char *getenv(char *envvar);
 　　函数说明:getenv()用来取得参数enwar环境变量的内容。参数enwar为环境变量的名称，如果该变量存在则会返回指向该内容的指针。环境变量的格式为enwar＝value
 　　返回值: 执行成功则返回指向该内容的指针，找不到符合的环境变量名称则返回NULL 
alloca()
 　　包含在头文件malloc.h中. 　　在某些系统中会宏定义成_alloca使用.
```
int load_module(struct tsdev *ts, const char *module, const char *params, int raw)
{
 struct tslib_module_info * (*init)(struct tsdev *, const char *);
 struct tslib_module_info *info;
 char fn[1024];
 void *handle;
 int ret;
 char *plugin_directory=NULL;
 if( (plugin_directory = getenv("TSLIB_PLUGINDIR")) != NULL ) {
  //fn = alloca(sizeof(plugin_directory) + strlen(module) + 4);
  strcpy(fn,plugin_directory);
 } else {
  //fn = alloca(sizeof(PLUGIN_DIR) + strlen(module) + 4);
  strcpy(fn, PLUGIN_DIR);
 }
/*构造共享库路径*/
 strcat(fn, "/");
 strcat(fn, module);
 strcat(fn, ".so");
#ifdef DEBUG
 printf ("Loading module %s\n", fn);
#endif
 handle = dlopen(fn, RTLD_NOW);
 if (!handle)
  return -1;
 init = dlsym(handle, "mod_init");
 if (!init) {
  dlclose(handle);
  return -1;
 }
 info = init(ts, params);
 if (!info) {
  dlclose(handle);
  return -1;
 }
 info->handle = handle;
 if (raw) {
  ret = __ts_attach_raw(ts, info);
 } else {
  ret = __ts_attach(ts, info);
 }
 if (ret) {
  info->ops->fini(info);
  dlclose(handle);
 }
 return ret;
}
```
