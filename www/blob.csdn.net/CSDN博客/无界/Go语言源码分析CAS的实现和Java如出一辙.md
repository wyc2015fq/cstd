
# Go语言源码分析CAS的实现和Java如出一辙 - 无界 - CSDN博客

2019年03月27日 20:41:02[21aspnet](https://me.csdn.net/21aspnet)阅读数：149个人分类：[汇编语言																](https://blog.csdn.net/21aspnet/article/category/861842)



看了Go的源码CAS这块实现和java还是类似的。
关于Java的分析参考：[Java使用字节码和汇编语言同步分析volatile，synchronized的底层实现](https://blog.csdn.net/21aspnet/article/details/88571740)
都是使用汇编指令：**LOCK+CMPXCHGL**
原因很简单：单核肯定不能发挥Go的高并发性能，Go如果要支持多核，必然遇到并发编程数据可见性的问题，底层必然加锁。
无锁并不等于没有锁，只能说无重量级的锁而已。
Go语言源码：
![](https://img-blog.csdnimg.cn/20190327202403702.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
Go的CAS是调用CompareAndSwapInt32，
golang中的互斥锁定义在`src/sync/mutex.go`：
```python
// Lock locks m.
// If the lock is already in use, the calling goroutine
// blocks until the mutex is available.
func (m *Mutex) Lock() {
	// Fast path: grab unlocked mutex.
	if atomic.CompareAndSwapInt32(&m.state, 0, mutexLocked) {
		if race.Enabled {
			race.Acquire(unsafe.Pointer(m))
		}
		return
	}
```
**无锁操作CAS**: Compare And Swap 比较并交换。
源码在/src/runtime/internal/atomic/asm_amd64.s
可以看到实际上底层还是通过lock来实现，关于lock可以参考intel处理器指令。
![](https://img-blog.csdnimg.cn/20190315153411947.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
```python
// Copyright 2015 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
// Note: some of these functions are semantically inlined
// by the compiler (in src/cmd/compile/internal/gc/ssa.go).
#include "textflag.h"
// bool Cas(int32 *val, int32 old, int32 new)
// Atomically:
//	if(*val == old){
//		*val = new;
//		return 1;
//	} else
//		return 0;
TEXT runtime∕internal∕atomic·Cas(SB),NOSPLIT,$0-17
	MOVQ	ptr+0(FP), BX
	MOVL	old+8(FP), AX
	MOVL	new+12(FP), CX
	LOCK
	CMPXCHGL	CX, 0(BX)
	SETEQ	ret+16(FP)
	RET
// bool	runtime∕internal∕atomic·Cas64(uint64 *val, uint64 old, uint64 new)
// Atomically:
//	if(*val == *old){
//		*val = new;
//		return 1;
//	} else {
//		return 0;
//	}
TEXT runtime∕internal∕atomic·Cas64(SB), NOSPLIT, $0-25
	MOVQ	ptr+0(FP), BX
	MOVQ	old+8(FP), AX
	MOVQ	new+16(FP), CX
	LOCK
	CMPXCHGQ	CX, 0(BX)
	SETEQ	ret+24(FP)
	RET
TEXT runtime∕internal∕atomic·Casuintptr(SB), NOSPLIT, $0-25
	JMP	runtime∕internal∕atomic·Cas64(SB)
TEXT runtime∕internal∕atomic·CasRel(SB), NOSPLIT, $0-17
	JMP	runtime∕internal∕atomic·Cas(SB)
TEXT runtime∕internal∕atomic·Loaduintptr(SB), NOSPLIT, $0-16
	JMP	runtime∕internal∕atomic·Load64(SB)
TEXT runtime∕internal∕atomic·Loaduint(SB), NOSPLIT, $0-16
	JMP	runtime∕internal∕atomic·Load64(SB)
TEXT runtime∕internal∕atomic·Storeuintptr(SB), NOSPLIT, $0-16
	JMP	runtime∕internal∕atomic·Store64(SB)
TEXT runtime∕internal∕atomic·Loadint64(SB), NOSPLIT, $0-16
	JMP	runtime∕internal∕atomic·Load64(SB)
TEXT runtime∕internal∕atomic·Xaddint64(SB), NOSPLIT, $0-24
	JMP	runtime∕internal∕atomic·Xadd64(SB)
// bool Casp1(void **val, void *old, void *new)
// Atomically:
//	if(*val == old){
//		*val = new;
//		return 1;
//	} else
//		return 0;
TEXT runtime∕internal∕atomic·Casp1(SB), NOSPLIT, $0-25
	MOVQ	ptr+0(FP), BX
	MOVQ	old+8(FP), AX
	MOVQ	new+16(FP), CX
	LOCK
	CMPXCHGQ	CX, 0(BX)
	SETEQ	ret+24(FP)
	RET
// uint32 Xadd(uint32 volatile *val, int32 delta)
// Atomically:
//	*val += delta;
//	return *val;
TEXT runtime∕internal∕atomic·Xadd(SB), NOSPLIT, $0-20
	MOVQ	ptr+0(FP), BX
	MOVL	delta+8(FP), AX
	MOVL	AX, CX
	LOCK
	XADDL	AX, 0(BX)
	ADDL	CX, AX
	MOVL	AX, ret+16(FP)
	RET
TEXT runtime∕internal∕atomic·Xadd64(SB), NOSPLIT, $0-24
	MOVQ	ptr+0(FP), BX
	MOVQ	delta+8(FP), AX
	MOVQ	AX, CX
	LOCK
	XADDQ	AX, 0(BX)
	ADDQ	CX, AX
	MOVQ	AX, ret+16(FP)
	RET
TEXT runtime∕internal∕atomic·Xadduintptr(SB), NOSPLIT, $0-24
	JMP	runtime∕internal∕atomic·Xadd64(SB)
TEXT runtime∕internal∕atomic·Xchg(SB), NOSPLIT, $0-20
	MOVQ	ptr+0(FP), BX
	MOVL	new+8(FP), AX
	XCHGL	AX, 0(BX)
	MOVL	AX, ret+16(FP)
	RET
TEXT runtime∕internal∕atomic·Xchg64(SB), NOSPLIT, $0-24
	MOVQ	ptr+0(FP), BX
	MOVQ	new+8(FP), AX
	XCHGQ	AX, 0(BX)
	MOVQ	AX, ret+16(FP)
	RET
TEXT runtime∕internal∕atomic·Xchguintptr(SB), NOSPLIT, $0-24
	JMP	runtime∕internal∕atomic·Xchg64(SB)
TEXT runtime∕internal∕atomic·StorepNoWB(SB), NOSPLIT, $0-16
	MOVQ	ptr+0(FP), BX
	MOVQ	val+8(FP), AX
	XCHGQ	AX, 0(BX)
	RET
TEXT runtime∕internal∕atomic·Store(SB), NOSPLIT, $0-12
	MOVQ	ptr+0(FP), BX
	MOVL	val+8(FP), AX
	XCHGL	AX, 0(BX)
	RET
TEXT runtime∕internal∕atomic·StoreRel(SB), NOSPLIT, $0-12
	JMP	runtime∕internal∕atomic·Store(SB)
TEXT runtime∕internal∕atomic·Store64(SB), NOSPLIT, $0-16
	MOVQ	ptr+0(FP), BX
	MOVQ	val+8(FP), AX
	XCHGQ	AX, 0(BX)
	RET
// void	runtime∕internal∕atomic·Or8(byte volatile*, byte);
TEXT runtime∕internal∕atomic·Or8(SB), NOSPLIT, $0-9
	MOVQ	ptr+0(FP), AX
	MOVB	val+8(FP), BX
	LOCK
	ORB	BX, (AX)
	RET
// void	runtime∕internal∕atomic·And8(byte volatile*, byte);
TEXT runtime∕internal∕atomic·And8(SB), NOSPLIT, $0-9
	MOVQ	ptr+0(FP), AX
	MOVB	val+8(FP), BX
	LOCK
	ANDB	BX, (AX)
	RET
```
这里还有其他处理器的汇编代码：
![](https://img-blog.csdnimg.cn/2019032720404565.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
其实这篇文章写得也还可以：[Golang 源代码解析(一) 锁机制的研究](https://blog.csdn.net/qq_35554975/article/details/86476105)

