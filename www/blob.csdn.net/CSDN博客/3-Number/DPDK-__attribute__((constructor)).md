
# DPDK-__attribute__((constructor)) - 3-Number - CSDN博客


2017年11月20日 10:36:19[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：298


# 0x01 缘由
在DPDK库开发过程中，遇到的问题。做个简短小结。
1.有一些结构是在main函数开始前就运行了；
2.静态库没连接，有些初始化工作无法完成；
# 0x02 介绍
gcc为函数提供了几种类型的属性，其中包含：构造函数(constructors)和析构函数(destructors)。
# 0x03 例子
```python
#include<stdio.h>
__attribute__((constructor)) void before_main() {
   printf("Before main\n");
}
__attribute__((destructor)) void after_main() {
   printf("After main\n");
}
int main(int argc, char **argv) {
   printf("In main\n");
   return 0;
}
```

# 0x04 DPDK用法
**驱动数据结构：**
\#define RTE_INIT(func) \
static void __attribute__((constructor, used)) func(void)
**内存池相关：**
\#define MEMPOOL_REGISTER_OPS(ops)                    \
void mp_hdlr_init_\#\#ops(void);                    \
void __attribute__((constructor, used)) mp_hdlr_init_\#\#ops(void)\
{                                \
rte_mempool_register_ops(&ops);            \
}

