# linux设备驱动-原子操作 - V__KING__的专栏 - CSDN博客





2018年07月20日 08:59:41[v__king__](https://me.csdn.net/V__KING__)阅读数：140








## 1. 原子操作

整型原子操作和位原子操作，比如一个设备只能被一个进程打开，比如serial串口设备

### 1.1 整形原子操作：

1．设置原子变量的值

```
void atomic_set(atomic_t *v, int i); //设置原子变量的值为i
atomic_t v = ATOMIC_INIT(0); //定义原子变量v 并初始化为0
```

2．获取原子变量的值 

atomic_read(atomic_t *v); //返回原子变量的值 

3．原子变量加/减
```
void atomic_add(int i, atomic_t *v); //原子变量增加i
void atomic_sub(int i, atomic_t *v); //原子变量减少i
```

4．原子变量自增/自减

```
void atomic_inc(atomic_t *v); //原子变量增加1
void atomic_dec(atomic_t *v); //原子变量减少1
```

5．操作并测试

```
int atomic_inc_and_test(atomic_t *v);
int atomic_dec_and_test(atomic_t *v);
int atomic_sub_and_test(int i, atomic_t *v);
```

上述操作对原子变量执行自增、自减和减操作后（注意没有加）测试其是否为0，为0 则返回true， 

否则返回false。 

6．操作并返回
```
int atomic_add_return(int i, atomic_t *v);
int atomic_sub_return(int i, atomic_t *v);
int atomic_inc_return(atomic_t *v);
int atomic_dec_return(atomic_t *v);
```

上述操作对原子变量进行加/减和自增/自减操作，并返回新的值。

```
//使用原子变量使设备只能被一个进程打开
static atomic_t xxx_available = ATOMIC_INIT(1); /*定义原子变量*/

static int xxx_open(struct inode *inode, struct file *filp) {
    ...
    if (!atomic_dec_and_test(&xxx_available))
    {
        atomic_inc(&xxx_available);
        return - EBUSY; /*已经打开*/
    }
    ...
    return 0; /* 成功 */
}

static int xxx_release(struct inode *inode, struct file *filp) {
    atomic_inc(&xxx_available); /* 释放设备 */
    return 0;
}
```

### 1.1 位原子操作：

1．设置位
`void set_bit(n, void *addr);`
上述操作设置addr 地址的第n 位，所谓设置位即将位写为1。 

2．清除位
`void clear_bit(n, void *addr);`
上述操作清除addr 地址的第n 位，所谓清除位即将位写为0。 

3．改变位
`void change_bit(n, void *addr);`
上述操作对addr 地址的第n 位进行反置。 

4．测试位
`test_bit(n, void *addr);`
上述操作返回addr 地址的第n 位。 

5．测试并操作位 

“Set a bit and return its old value” 

 nr    – Bit to set 

 addr – Address to count from
注意：nr是需要设置的“哪一位”，而不是将addr的值设置成nr。

例如”test_and_set_bit(0, &var)”，”0” 不是要设置的值，而是表示var中第0位需要被设置为”1”。此函数返回相应比特位上一次被设置的值。 

test_and_clear_bit()用法正好相反， which assigns “0” to the specified bit.

```
int test_and_set_bit(n, void *addr);
int test_and_clear_bit(n, void *addr);
int test_and_change_bit(n, void *addr);
```

上面test_and_xxx_bit（n, void *addr）操作等同于执行test_bit （n, void *addr） 后再执行xxx_bit（n,void *addr）。






