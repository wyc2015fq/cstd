
# libuv学习笔记（6） - paohui0134的博客 - CSDN博客


2016年06月12日 13:50:17[paohui0134](https://me.csdn.net/paohui0134)阅读数：840


# libuv学习笔记（6）
## uv_prepare_t、uv_idel_t、uv_check_t的数据结构与相关API
三种监视器（watcher），每次循环迭代都会调用它的回调函数
### 数据结构
```python
typedef
```
```python
struct
```
```python
uv_prepare_s uv_prepare_t;
```
```python
typedef
```
```python
struct
```
```python
uv_check_s uv_check_t;
```
```python
typedef
```
```python
struct
```
```python
uv_idle_s uv_idle_t;
```
```python
struct
```
```python
uv_prepare_s 
{
  UV_HANDLE_FIELDS
```
```python
//uv_handle_t的数据，此处不再展开，请参考之前的内容
```
```python
//UV_PREPARE_PRIVATE_FIELDS展开如下：
```
```python
uv_prepare_t* prepare_prev;
```
```python
//指向上一个
```
```python
uv_prepare_t* prepare_next;
```
```python
//指向下一个
```
```python
uv_prepare_cb prepare_cb;
```
```python
//回调函数
```
```python
};
```
三种watcher的结构基本一致，只是其中的指针类型以及回调函数类型不同
### 相关API （以uv_prepare_t的相关函数为例，另外两种的处理函数流程基本一致）
#### 1.初始化函数，导出函数，在uv.h中声明，loop-watcher.c中通过宏定义
基本的handle初始化流程，没有什么特殊的处理。
```python
int
```
```python
uv_prepare_init(uv_loop_t* loop, uv_prepare_t* handle) 
{
```
```python
//uv__handle_init(loop, (uv_handle_t*) handle, UV_PREPARE);展开：
```
```python
do
```
```python
{                                                                      
      (h)->loop = (loop);                                                    
      (h)->type = (UV_PREPARE);                                                     
      (h)->flags = UV__HANDLE_REF;
```
```python
/* 设为引用状态. */
```
```python
//将handle添加到loop的handle队列
```
```python
QUEUE_INSERT_TAIL(&(loop)->handle_queue, &(handle)->handle_queue);
```
```python
//uv__handle_platform_init(h);  展开
```
```python
((handle)->u.fd = -
```
```python
1
```
```python
);                                           
    }
```
```python
while
```
```python
(
```
```python
0
```
```python
)
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
#### 2.开始（激活），导出函数，在uv.h中声明，loop-watcher.c中通过宏定义
```python
int
```
```python
uv_prepare_start(uv_prepare_t* handle, uv_prepare_cb cb) 
{          
    uv_loop_t* loop = handle->loop;                                         
    uv_prepare_t* old_head;                                                 
    assert(handle->type == UV_PREPARE);
```
```python
//如果已经是激活状态，直接返回
```
```python
if
```
```python
(uv__is_active(handle))
```
```python
return
```
```python
0
```
```python
;
```
```python
//没有设置回调函数，返回错误
```
```python
if
```
```python
(cb == NULL)
```
```python
return
```
```python
UV_EINVAL;
```
```python
//将本handle插入到loop的prepare_handles列表的表头
```
```python
old_head = loop->prepare_handles;                                                                                                                
    handle->prepare_next = old_head;                                           
    handle->prepare_prev = NULL;
```
```python
if
```
```python
(old_head) 
    {                                                           
      old_head->prepare_prev = handle;                                         
    }                                                                         
    loop->prepare_handles = handle;                                            
    handle->prepare_cb = cb;
```
```python
//uv__handle_start(handle);    展开如下
```
```python
do
```
```python
{                                                                       
      assert(((handle)->flags & UV__HANDLE_CLOSING) ==
```
```python
0
```
```python
);
```
```python
if
```
```python
(((handle)->flags & UV__HANDLE_ACTIVE) !=
```
```python
0
```
```python
)
```
```python
break
```
```python
;                         
      (handle)->flags |= UV__HANDLE_ACTIVE;
```
```python
//设为active状态
```
```python
//如果同时处于引用状态，loop的活动handle计数加一
```
```python
if
```
```python
(((handle)->flags & UV__HANDLE_REF) !=
```
```python
0
```
```python
) uv__active_handle_add(handle);        
    }
```
```python
while
```
```python
(
```
```python
0
```
```python
);
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
#### 3.停止handle，导出函数，在uv.h中声明，loop-watcher.c中通过宏定义
```python
int
```
```python
uv_prepare_stop(uv_prepare_t* handle) 
 {                              
    uv_loop_t* loop = handle->loop;                                                                                                        
    assert(handle->type == UV_PREPARE);
```
```python
if
```
```python
(!uv__is_active(handle))
```
```python
return
```
```python
0
```
```python
;
```
```python
/* 如果本handle为loop  prepare_handle列表的第一个元素，更新loop的prepare_handles指针 */
```
```python
if
```
```python
(loop->prepare_handles == handle) {                                     
      loop->prepare_handles = handle->prepare_next;                             
    }
```
```python
/* 如果loop的下一个prepare_handle是本handle，更新 */
```
```python
if
```
```python
(loop->next_prepare_handle == handle) {                               
      loop->next_prepare_handle = handle->prepare_next;                       
    }
```
```python
//从列表中删除本handle
```
```python
if
```
```python
(handle->prepare_prev) {                                                
      handle->prepare_prev->prepare_next = handle->prepare_next;                 
    }
```
```python
if
```
```python
(handle->prepare_next) {                                                
      handle->prepare_next->prepare_prev = handle->prepare_prev;                 
    }
```
```python
//uv__handle_stop(handle); 展开如下：
```
```python
do
```
```python
{                                                                        
      assert(((h)->flags & UV__HANDLE_CLOSING) ==
```
```python
0
```
```python
);
```
```python
if
```
```python
(((h)->flags & UV__HANDLE_ACTIVE) ==
```
```python
0
```
```python
)
```
```python
break
```
```python
;                        
      (h)->flags &= ~UV__HANDLE_ACTIVE;
```
```python
//去掉active标志
```
```python
//如果同时在引用状态，loop的活动handle计数减一
```
```python
if
```
```python
(((h)->flags & UV__HANDLE_REF) !=
```
```python
0
```
```python
) uv__active_handle_rm(h);          
    }
```
```python
while
```
```python
(
```
```python
0
```
```python
);
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
#### 关闭操作（通过uv_close）
在uv_close中处理如下
```python
...
```
```python
case UV_PREPARE:
      uv_prepare_stop((uv_prepare_t*)handle);
      uv__handle_closing(handle);
      uv_want_endgame(loop, handle);
```
```python
return
```
```python
;
```
```python
...
```
通过uv_want_endgame将handle插入到loop->endgame_handles的列表头，接着在uv_run中会调用uv_process_endgames处理所有的需要关闭的handle
```python
...
```
```python
case UV_PREPARE:
      case UV_CHECK:
      case UV_IDLE:
        uv_loop_watcher_endgame(loop, handle);
```
```python
break
```
```python
;
```
```python
...
```
通过代码可知，三种监视器的关闭都是调用的uv_loop_watcher_endgame：
```python
void
```
```python
uv_loop_watcher_endgame(uv_loop_t
```
```python
*
```
```python
loop
```
```python
, uv_handle_t
```
```python
*
```
```python
handle
```
```python
) 
{
```
```python
if
```
```python
(
```
```python
handle
```
```python
->
```
```python
flags
```
```python
&
```
```python
UV__HANDLE_CLOSING) 
  {
    assert(
```
```python
!
```
```python
(
```
```python
handle
```
```python
->
```
```python
flags
```
```python
&
```
```python
UV_HANDLE_CLOSED));
```
```python
handle
```
```python
->
```
```python
flags
```
```python
|=
```
```python
UV_HANDLE_CLOSED;
```
```python
//状态变为UV__HANDLE_CLOSED
```
```python
//uv__handle_close(handle);展开如下：
```
```python
do
```
```python
{                                                                  
      QUEUE_REMOVE(
```
```python
&
```
```python
(
```
```python
handle
```
```python
)
```
```python
->
```
```python
handle_queue);
```
```python
//从队列中移除
```
```python
uv__active_handle_rm((uv_handle_t
```
```python
*
```
```python
) (
```
```python
handle
```
```python
));
```
```python
//handle计数递减
```
```python
(
```
```python
handle
```
```python
)
```
```python
->
```
```python
flags
```
```python
|=
```
```python
UV_HANDLE_CLOSED;
```
```python
//调用回调函数
```
```python
if
```
```python
((
```
```python
handle
```
```python
)
```
```python
->
```
```python
close_cb)                                             
        (
```
```python
handle
```
```python
)
```
```python
->
```
```python
close_cb((uv_handle_t
```
```python
*
```
```python
) (
```
```python
handle
```
```python
));                      
    }
```
```python
while
```
```python
(
```
```python
0
```
```python
)
  }
}
```
#### 4.循环迭代中的调用（uv_run）
```python
void
```
```python
uv_prepare_invoke(uv_loop_t*
```
```python
loop
```
```python
) {                                  
    uv_prepare_t* handle;
```
```python
(
```
```python
loop
```
```python
)
```
```python
->
```
```python
next_prepare_handle
```
```python
=
```
```python
(
```
```python
loop
```
```python
)
```
```python
->
```
```python
prepare_handles;
```
```python
//
```
```python
循环调用每一个uv_prepare_t的回调函数
```
```python
while
```
```python
((
```
```python
loop
```
```python
)->next_prepare_handle != NULL)
```
```python
{
```
```python
handle
```
```python
=
```
```python
(
```
```python
loop
```
```python
)
```
```python
->
```
```python
next_prepare_handle;
```
```python
(
```
```python
loop
```
```python
)
```
```python
->
```
```python
next_prepare_handle = handle->prepare_next;                     
      handle->prepare_cb(handle);                                              
    }                                                                         
  }
```
三种watcher的结构以及处理流程基本都一致，相关函数都是通过loop-watcher.c中的UV_LOOP_WATCHER_DEFINE宏定义的。三种watcher的区别在于：
1.在循环迭代中的调用顺序不同：
idle（空转）在处理完上个迭代获取的请求之后调用
prepare紧接着idle之后调用
check在轮询之后调用
2.idle会影响loop i/o轮询的超时设置，当有idle时，超时时间为0。

