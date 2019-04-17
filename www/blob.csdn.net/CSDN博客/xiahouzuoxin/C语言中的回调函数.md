# C语言中的回调函数 - xiahouzuoxin - CSDN博客





2013年08月21日 22:06:12[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：8876











C语言中通过函数指针实现回调函数（Callback Function）




**====== 首先使用typedef定义回调函数类型 ====== **



```cpp
typedef void (*event_cb_t)(const struct event *evt, void *userdata);
```
上面的语句表示event_cb_t类型函数范围值类型为void类型。




**====== 定义并实现一个函数用于注册回调函数 ======**



```cpp
int event_cb_register(event_cb_t cb, void *userdata);
```

下面是注册回调函数my_event_cb的一个模板



```cpp
static void my_event_cb(const struct event *evt, void *data)
{
    /* do stuff and things with the event */
}

...
   event_cb_register(my_event_cb, &my_custom_data);
...
```


在事件调度器（event dispatcher）中，常常将回调函数放在结构体中，



```cpp
struct event_cb {
    event_cb_t cb;
    void *data;
};
```
此时通过访问结构体成员变量调用回调函数



```cpp
struct event_cb *callback;

...

/* Get the event_cb that you want to execute */

callback->cb(event, callback->data);
```


**====== 一个回调函数的例子 ======**


```cpp
#include <stdio.h>

struct event_cb;

typedef void (*event_cb_t)(const struct event_cb *evt, void *user_data);

struct event_cb
{
    event_cb_t cb;
    void *data;
};

static struct event_cb saved = { 0, 0 };

void event_cb_register(event_cb_t cb, void *user_data)
{
    saved.cb = cb;
    saved.data = user_data;
}

static void my_event_cb(const struct event_cb *evt, void *data)
{
    printf("in %s\n", __func__);
    printf("data1: %s\n", (const char *)data);
    printf("data2: %s\n", (const char *)evt->data);
}

int main(void)
{
    char my_custom_data[40] = "Hello!";
    event_cb_register(my_event_cb, my_custom_data);

    saved.cb(&saved, saved.data);

    return 0;
}
```


**Refrences：**
[1] [http://stackoverflow.com/questions/142789/what-is-a-callback-in-c-and-how-are-they-implemented](http://stackoverflow.com/questions/142789/what-is-a-callback-in-c-and-how-are-they-implemented)





