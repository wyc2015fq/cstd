# Peeking inside LuaJIT（窥探LuaJIT） - 凯奥斯的注释 - CSDN博客





2017年11月01日 13:57:58[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：820








原文链接：[https://pwparchive.wordpress.com/2012/10/16/peeking-inside-luajit/](https://pwparchive.wordpress.com/2012/10/16/peeking-inside-luajit/)

> 
[LuaJIT](http://luajit.org/) is a high-performance virtual machine for Lua. I’ve recently started playing with it and this blog post talks about what I’ve discovered so far.


[LuaJIT](http://luajit.org/)是一个高效的Lua虚拟机。最近我开始摆弄它并且发布了这篇博客来阐述一下迄今为止我的发现。

> 
LuaJIT is a [tracing JIT](https://en.wikipedia.org/wiki/Tracing_just-in-time_compilation) as opposed to a method JIT — it works not by discovering and optimizing hot methods as a whole but by discovering and optimizing hot traces or paths of execution. LuaJIT has a rather interesting [interpreter](http://nominolo.blogspot.co.uk/2012/07/implementing-fast-interpreters.html) as well, but I won’t talk about it here except in context of the JIT.


LuaJIT是一个跟踪JIT而不是方法JIT——它的工作方式并不是检测和优化整个热点方法而是检测并优化热点trace（译注：并不打算翻译成“代码踪迹”）或执行路径。此外，LuaJIT有一个挺有趣的解释器，不过我并不打算讨论它，除非在JIT的上下文中。

> 
The meat of the interpreter lies in the vm_<arch>.dasc files, which contain a specification of the interpreter (and other assembly) stubs in an assembly-like language. So, for instance, vm_x86.dasc contains a function like:


解释器的“肉体”在vm_<arch>.dasc文件中，其中包含了解释器（以及其他汇编代码）的桩代码格式，用一种类汇编语言表述。所以，举个例子，vm_x86.dasc包含了下面一个函数：

```
// Generate assembly code for a specific Lua bytecode.
static void build_ins(BuildCtx *ctx, BCOp op, int defop) {
  switch (op) {
  case BC_HHGTTG:   // hypothetical bytecode
    | mov eax, 42  // hypothetical implementation
    break;
  }
}
```

> 
vm_x86.dasc is first lowered by dynasm.lua into a C program. The lines beginning with |s are mapped to calls to dasm_put in a way that the C functions produced now emit these assembly opcodes. This new C program is then linked with buildvm.c and run at compile time to generate the required assembly stubs. For instance, calling build_ins with BC_HHGTTG (after lowering) will emit the assembly opcodes for moving 42 to eax (whatever the semantic implication might be). This assembly is emitted into lj_vm.s and linked in with the rest of the Lua JIT.


通过dynasm.lua将vm_x86.dasc首次加载（译注：lowered ？）到C程序里。以|s开头的行以C函数生成的方式被映射到dasm_put的调用中，然后发行这些汇编操作码。接着，这个新的C程序被buildvm.c链接并且在编译时运行用以生成需要的汇编桩代码。例如，使用BC_HHGTTG （在加载之后）调用build_ins会发行这些汇编操作码用以将42移动到eax（不用管这个语义含义是什么）。这个汇编代码会被发行到lj_vm.s里面，并且被链接到余下的LuaJIT里。

> 
Detecting hot traces


# 检测热点函数

> 
Now, with that bit of detail out of our way, let us look at how Lua actually picks up the traces it wishes to compile. LuaJIT maintains the hotness of relevant instructions (jumps, calls) in a hashtable but a catch — it doesn’t resolve collisions. Since hotness is a heuristic and not dense (i.e. it is unlikely that all jumps in a program will be hot) this approach works quite well in practice. The following macro, used to access a the hotness counter for a particular instruction from C, should make things clear:


现在，了解了一些题外的细节点之后，让我们看看Lua实际上是如何找到它希望去编译的trace。LuaJIT用一个散列表维护了相应指令（跳转，调用）的热度，除了捕捉指令——它并没有解决冲突。因为热度是启发式的且并不稠密（即：不太可能发生一个程序中的所有跳转都是热点的情况），所以在实践中，这个做法执行的很好。后面的这个宏，用来访问一个特定C指令的热度计数器，可以将世界变得清净：

```
// gg is the global state and pc points to the bytecode
// instruction whose hotcount we want.
#define hotcount_get(gg, pc) (gg)->hotcount[(u32ptr(pc)>>2) & (HOTCOUNT_SIZE-1)]
```

> 
The interpreter updates and checks the hotness counters when executing a jump or a call. This is done using the hotloop and hotcall macros in vm_<arch>.dasc (we’ll only be looking at the x86 architecture so <arch> is x86):


当执行跳转或调用的时候，解释器更新并检查热度计数器。这是利用vm_<arch>.dasc中的宏hotloop和hotcall来完成的（我们将只针对x86架构进行条轮，所以<arch>是x86）：

```
|.macro hotloop, reg
|  mov reg, PC
|  shr reg, 1
|  and reg, HOTCOUNT_PCMASK
|  sub word [DISPATCH+reg+GG_DISP2HOT], HOTCOUNT_LOOP
|  jb ->vm_hotloop
|.endmacro
```

> 
and later, in various places:


然后，在各个角落：

```php
case BC_FORL:
  |.if JIT
  |  hotloop RB
  |.endif
```

> 
dynasm lowers the macro invocations in a straightforward way — BC_FORL subtracts HOTCOUNT_LOOP ticks from the corresponding counter and then jumps to vm_hotloop once the counter under-flows:


dynasm以直接了当的方式加载宏调用——BC_FORL 减去来自相关计数器的 HOTCOUNT_LOOP计数，然后当计数器下溢时跳转到vm_hotloop ：

```
|->vm_hotloop:            // Hot loop counter underflow.
|.if JIT
|  mov LFUNC:RB, [BASE-8]     // Same as curr_topL(L).
|  mov RB, LFUNC:RB->pc
|  movzx RD, byte [RB+PC2PROTO(framesize)]
|  lea RD, [BASE+RD*8]
|  mov L:RB, SAVE_L
|  mov L:RB->base, BASE
|  mov L:RB->top, RD
|  mov FCARG2, PC
|  lea FCARG1, [DISPATCH+GG_DISP2J]
|  mov aword [DISPATCH+DISPATCH_J(L)], L:RBa
|  mov SAVE_PC, PC
|  call extern lj_trace_hot@8     // (jit_State *J, const BCIns *pc)
|  jmp <3
|.endif
```

> 
dynasm can be made aware of C structs and offsets of their fields using a .type directive. In the body of vm_hotloop, LFUNC:RB tells dynasm to treat the value in RB as an LFUNC. RB and LFUNC are defined earlier:


dynasm可以使用.type指示来知道C的结构及其字段的偏移。在vm_hotloop的“身体”中，LFUNC:RB告诉dynasm把RB中的值当做一个LFUNC（译注：Lua函数）。 RB和LFUNC在前文中的定义：

```
|.define RB,      ebp
// ...
|.type LFUNC,     GCfuncL
```

> 
Of course, treating RB as an LFUNC in LFUNC:RB doesn’t do anything, it is essentially documentation. In the next instruction (mov RB, LFUNC:RB->pc), however, this annotation allows us to say LFUNC:RB->pc and have dynasm automatically figure out the correct offset. Let’s step through vm_hotloop, noting that BASE points to the top of Lua’s evaluation stack, RB and RD are registers, FCARG1 and FCARG2 are the registers that are used as the first two arguments in the calling convention used when transitioning from Lua to C and that SAVE_L and SAVE_PC are stack slots.


当然，在LFUNC:RB中把RB当做LFUNC并没有做什么事，它实际上是个注解。不过，在下一个指令中(mov RB, LFUNC:RB->pc)，这个注解允许我们表示 LFUNC:RB->pc并让dynasm自动找出正确的偏移。让我们来过一遍vm_hotloop，注意BASE指向Lua的计算堆栈，RB和RD是寄存器，寄存器FCARG1和FCARG2是用来表示从Lua过渡到C的调用约定中用到的前两个参数，SAVE_L 和SAVE_PC 是堆栈槽。

> 
First, we pop the GCfuncL off the top of the stack and read pc, which points to the beginning of the bytecode for the closure:


首先，我们将GCfuncL 从栈顶出栈并且读取pc，pc指向闭包的字节码的开始位置：

```
|  mov LFUNC:RB, [BASE-8]
|  mov RB, LFUNC:RB->pc
```

> 
LuaJIT follows a common convention of storing function literals or prototypes separately from function closures. This allows the VM to save space by sharing the byte-code between two closures of the same function prototype. In [V8](http://code.google.com/p/v8/), for instance, you have SharedFunctionInfos holding the information specific to a function literal and Functions representing actual, executable, closures.


LuaJIT遵循一个公共约定，它规定了分离自函数闭包的函数字面和原型的存储方式。在拥有相同函数原型的闭包之间共享字节码，这样可以允许虚拟机节省空间。例如，在V8中，SharedFunctionInfos保存着特定函数字面信息，而Functions代表实际的、可执行的闭包。

> 
In LuaJIT, function literals are represented using GCproto. In memory, a GCproto object is followed by the bytecode for the function literal (something shared by all closures, represented by GCfuncLs). Thus, given a GCfuncL, we can extract the corresponding GCproto by subtracting sizeof(GCproto) from the pointer to the beginning of the bytecode array. PC2PROTO uses this technique to access the framesize field in the GCproto structure and uses it to compute the first free slot in the stack:


在LuaJIT中，使用GCproto来代表函数字面。内存中，一个GCproto对象后面有关于这个函数字面的字节码（某种被所有闭包共享的东西，用GCfuncLs来表示）。这样，给出一个GCfuncL，将指向字节码数组开头的指针减去sizeof(GCproto)，我们就可以提取相应的GCproto 。PC2PROTO 使用这个技术来访问GCproto 结构中的framesize 字段并用它来计算堆栈中第一个空闲的槽位：

```
|  movzx RD, byte [RB+PC2PROTO(framesize)]
|  lea RD, [BASE+RD*8]
```

> 
Then we fill up the fields in lua_State (defined in lj_obj.h):


然后我们将lua_State （在lj_obj.h中定义）中的字段填上：

```
|  mov L:RB, SAVE_L
|  mov L:RB->base, BASE
|  mov L:RB->top, RD
```

> 
set the argument registers, save few things in stack slots and call into lj_trace_hot:


设置参数寄存器，在堆栈槽中存入少量的东西并调用lj_trace_hot：

```
|  mov FCARG2, PC
|  lea FCARG1, [DISPATCH+GG_DISP2J]
|  mov aword [DISPATCH+DISPATCH_J(L)], L:RBa
|  mov SAVE_PC, PC
|  call extern lj_trace_hot@8
|  jmp <3
```

> 
which puts LuaJIT into recording mode.


它使LuaJIT进入记录模式。

> 
Recording Traces


# 记录trace

> 
A trace is the linear sequence of bytecodes encountered when executing a particular code path. Traces are recorded (the recording begins when lj_trace_hot is called) by coordinating with the [interpreter](http://nominolo.blogspot.co.uk/2012/07/implementing-fast-interpreters.html). The interpreter uses a vector of pointers to assembly stubs, indexed by the bytecode instructions they implement — in principle, interpreting a Lua program proceeds by decoding the bytecodes one by one and jumping to the corresponding assembly stubs. Traces are recorded by making each element in the dispatch vector point to lj_vm_record (dynasm adds the lj_ prefix, in vm_x86.dasc, the symbol is just vm_record). A simplified vm_record looks like this:


当执行到特定的代码路径时，此时的线性的字节码序列便是一个trace。使用解释器定位来记录（当lj_trace_hot被调用时，开始记录）trace。解释器使用一个指针数组，这些指针指向汇编桩代码，用它们实现的字节码指令来索引——原则上，使用逐个解码字节码和跳转到对应汇编桩代码的方式来解释执行一个Lua程序。将调度数组中的每个元素指向lj_vm_record（在vm_x86.dasc中，dynasm加上prefix得到的符号就是vm_record）来记录trace。一个简化的vm_record长成这样：

```
|->vm_record:
|  mov L:RB, SAVE_L
|  mov L:RB->base, BASE
|  mov FCARG2, PC
|  mov FCARG1, L:RB
|  call extern lj_dispatch_ins@8
|  mov BASE, L:RB->base
|  movzx RA, PC_RA
|  movzx OP, PC_OP
|  movzx RD, PC_RD
|  jmp aword [DISPATCH+OP*8+GG_DISP2STATIC]
|.endif
```

> 
We see that it basically boils down to a call to lj_dispatch_ins followed by a jump to whatever [DISPATCH+OP*8+GG_DISP2STATIC] points to. LuaJIT saves a backup copy of the dispatch vectors at a distance of GG_DISP2STATIC from the original dispatch vector (which now only contains pointers to lj_vm_record), and lj_vm_record uses this backup vector to jump to the real assembly stub.


我们看到，基本上归结起来就是一个lj_dispatch_ins的调用，然后跳转得到[DISPATCH+OP*8+GG_DISP2STATIC] 指向的地方。LuaJIT在原先的调度数组（它现在只包含指向lj_vm_record的指针）的GG_DISP2STATIC距离之后保存了一个调度数组的备份副本（译注：副本指针=原先的调度数组的指针+GG_DISP2STATIC），lj_vm_record使用这个备份数组来跳转到真正的汇编桩代码。

> 
Let’s get off the armchair now and look at some actual code and understand some subtleties.


我们不要再说这些虚的，直接撸实际的代码，然后理解其中玄妙之处。

> 
Once a loop or a call is measured to be hot, the interpreter calls lj_trace_hot which is the entry point into recording, compiling and installing a trace. lj_trace_hot sets state of the central jit_State object to LJ_TRACE_START and hands off control to lj_trace_ins.


当一个循环或者调用被测定为热点，解释器调用lj_trace_hot，它是记录、编译和安装trace的入口。lj_trace_hot 将中央jit_State对象的状态设置为LJ_TRACE_START 然后放开对lj_trace_ins的控制。

> 
The tracing subsystem of LuaJIT has five states: START, RECORD, END, ASM and ERR. lj_trace_ins is a finite automaton which moves between these states based on the bytecode instructions read off the execution trace. The overall scheme is simple:


LuaJIT 的跟踪子系统有5个状态：START（译注：开始）, RECORD（译注：记录）, END（译注：结束）, ASM（译注：汇编） 和ERR（译注：出错）。lj_trace_ins 是一个有限自动机，它根据从执行trace中读取的字节码指令在这些状态之间切换。简单的整体图：

```
START   -------------------> RECORD ---> ERR
          set up dispatch      |             
              vector           |            [IDLE]
                               v             /
                              END -----> ASM
```

> 
Of course, this doesn’t happen all at once — the state is remembered and transitions are made (or not) as calls to lj_dispatch_ins uncover more and more of the currently executing trace. The bytecode stream the tracer sees is translated into an [intermediate SSA representation](http://wiki.luajit.org/create/SSA%20IR) which is then optimized and compiled into assembly.


当然，这些不会一次都发生——记住状态，并且调用lj_dispatch_ins来制作（或不）中间过渡，一步步分析当前执行的trace。跟踪器得到的字节码流被翻译成一个中间的[SSA](http://wiki.luajit.org/create/SSA%20IR) 表达方式，然后被优化和编译到汇编代码中。

> 
Tracing gives us only one linear path through the code, disregarding other, equally valid paths that may have been taken. A tracing JIT usually deals with this by installing guards which check assumptions and bail out from the trace (to the interpreter, in LuaJIT’s case) on violated assumptions. For instance, consider this simple Lua program:


跟踪只会给我们一个代码的线性执行路径，不管别的同样有效的可能已经被采用的路径。通常，一个跟踪JIT会用安装守卫的方法来这个问题，他们检查假设并将失败的假设从trace中摆脱（LuaJIT的情况中，摆脱给解释器）。例如，下面这个简单的Lua程序：

```
total = 0

function test()
  if total < 500 then
      total = total + 1
   end
end

for i=1,501 do
   test()
end
```

> 
This compiles to the following [bytecode](http://wiki.luajit.org/create/Bytecode) (stripping out some parts non-essential to our understanding):


它会被编译成下面的字节码（无视那些我们非必需理解的）：

```php
;; function test()
0001    GGET     0   0
0002    KSHORT   1 500
0003    ISGE     0   1
0004    JMP      0 => 0008
0005    GGET     0   0
0006    ADDVN    0   0   0
0007    GSET     0   0
0008 => RET0     0   1

;; body (instructions 0001 to 0007 removed)
0008    FORI     0 => 0012
0009 => GGET     4   2      ; "test"
0010    CALL     4   1   1
0011    FORL     0 => 0009
0012 => RET0     0   1
```

> 
(You can get luajit to emit bytecode in this form using -b -l and have it dump all the trace information using -jdump.)


（要发行这种格式的字节码，你可以对luajit 使用-b -l并且使用-jdump让它输出所有trace信息。）

> 
Nothing unexpected here — you have a for loop (delimited by FORI and FORL) running 501 times calling test in each iteration. LuaJIT extracts the following (again, trimmed down) trace (in SSA form):


毫无意外的——你得到了一个跑了501次的for循环（被FORI和FORL分隔），每次迭代中调用了test。LuaJIT提取下面的（再次，修剪过的）trace（以SSA格式）：

```
;; Instructions 0001 to 0013 removed
0014 >  p32 HREFK  0013  "total" @11
0015 >  num HLOAD  0014
0016 >  num LT     0015  +500
0017  + num ADD    0015  +1
0018    num HSTORE 0014  0017
0019  + int ADD    0001  +1
0020 >  int LE     0019  +501
0021 ------ LOOP ------------
0022 >  num LT     0017  +500
0023  + num ADD    0017  +1
0024    num HSTORE 0014  0023
0025  + int ADD    0019  +1
0026 >  int LE     0025  +501
0027    int PHI    0019  0025
0028    num PHI    0017  0023
```

> 
The instructions following LOOP are unsurprising: a normal loop with the usual SSA phi nodes. Note that (as is usual in tracing JITs) a version of test has been inlined completely into a tight loop. Instruction 0016 is rather interesting; the > means that a particular instruction is a guard and that the trace bails out to the interpreter if the condition doesn’t hold. In this case, we bail out if 0015 (which is the value of total, as you can make out from instructions 0014 and 0015) is greater or equal to 500. This is interesting because the tracing compiler doesn’t try to be a smartass and infer to not do anything when total is >= 500, which also is correct behavior. All it knows is that when total is < 500, the correct behaviour is to add 1 to total, because that is what it has observed in the trace. Notably, if the total >= 500 path gets taken often enough, it would be marked hot and be compiled into a side-trace (you should try reproducing this).


LOOP后面的指令没啥新鲜的：一个用常规的SSA phi节点实现的常规的循环。注意（虽然在跟踪JIT中稀松平常）test一个版本被完全的级联到一个紧凑的循环中。0016指令还挺有意思的，>符号表示一个特定的指令是一个守卫并且这个trace会在不满足条件的时候被摆脱到解释器中。在这个例子中，当0015（total的值，你可以从0014指令和0015指令中看出来）大于或等于500的时候会摆脱这个trace。有意思的是跟踪编译器并不尝试自作聪明并推断出当total大于等于500的时候什么都不做，虽然那也是一种正确的行为。它所知道的所有事情就是当total小于500时，正确的行为是对total加1，因为这就是它从trace中观察得到的。值得注意的是，如果total大于等于500的路径被执行足够频繁，它将被标记成热点并且编译到一个边界trace中去（你可以试着重现它）。

> 
lj_record_ins in lj_record.c records each bytecode instruction into SSA before it is executed. One architectural subtlety is knowing which guard to emit — given a condition IF A < B THEN {0} ELSE {1} END do you emit a guard for A < B or for NOT (A < B)? We get that information only after the condition is evaluated, if A < B is true during the trace recording phase, then we need a guard asserting (A < B) and vice versa. Instead of implementing a partial interpreter in the tracer, LuaJIT does this by postponing the insertion of the guard instructions till before lj_record_ins is called with the next bytecode. It knows the result of evaluating the condition by then.


lj_record.c中的lj_record_ins将每一个字节码指令在它执行之前记录到SSA中。一个架构型的玄妙之处是它知道该发行哪一个守卫——给出一个条件`IF A < B THEN {0} ELSE {1} END`，是该发行`A<B`的守卫还是`NOT(A<B)`的？我们只能在条件被计算之后才能获得信息，在trace的记录阶段如果`A<B`是true，我们则需要一个守卫来断言`(A<B)`，反之亦然。LuaJIT并不用在跟踪器中实现部分解释器的方式来解决这个问题，而是在lj_record_ins被调用执行下一条字节码之前推迟插入守卫指令。那时候，它知道了该条件计算的结果。

> 
Another subtlety involves tracing across already-compiled traces. LuaJIT keep track of compiled traces by replacing the normal loop or call bytecodes with special marker bytecodes. These special bytecodes signal the existence and provide the location of a compiled trace beginning at that point. However, when tracing, we’d like to be able to see the function bytecodes. This is done by temporarily patching the special JIT marker bytecode (BC_JFUNCF or BC_JLOOP, for instance) with the original bytecode, tracing through the original bytecode and patching back the JIT marker bytecode once the tracing is done. To see this happening, have a look at rec_func_jit (in lj_record.c) and trace_pendpatch in lj_trace.c.


另外一个玄妙之处涉及到在已编译的trace中进行跟踪。LuaJIT将普通的循环或调用字节码替换成特殊标记的字节码，用来保存对已编译的trace的跟踪。这些特殊的字节码示意trace的存在并且提供在这个位置上已编译trace的开始位置。不过，当在跟踪过程中，我们希望函数的字节码可见。可以通过以下方式实现：临时用原始的字节码替换特殊的JIT标记字节码（例如BC_JFUNCF或BC_JLOOP），当跟踪结束的时候在原始字节码中进行跟踪并重新还原JIT标记字节码。想知道这些怎么发生的，去看看rec_func_jit（在lj_record.c里）和lj_trace.c里的trace_pendpatch。

> 
Snapshots


# 快照

（译注：区别于Lua的内存快照）

> 
Any VM that involves transitioning between two (or more!) modes of execution faces the problem of converting between unoptimized and optimized stack layouts. This is true of LuaJIT too, with a slightly different meaning of stack. We’d like observational equivalence between a interpreted trace and the same trace compiled. We’d like the compiled trace to have the same side effects and map the operand stack the same way as the interpreted trace. However, this equivalence need not hold inside the compiled trace, we only need to make sure that this holds in the trace exits (guarded or not).


任何涉及到两个（或更多！）执行模式之间过渡的虚拟机都会面临一个问题，在未优化和已优化的堆栈布局之间的转换。对LuaJIT来说也不例外，不过它的堆栈的含义略有不同。我们希望在一个被解释的trace和相同的被编译的trace之间有监督性的等价关系。我们希望被编译的trace和被解释的trace拥有相同的边界效益，以及用相同的方式来映射操作数堆栈。这种等价关系需要在被编译的trace里不作控制，然而，我们只能确认的是这些控制在trace中是存在的（被守卫或不）。

> 
LuaJIT solves this by maintaining a mapping from stack location to SSA IR instructions. Such mappings are called snapshots in the LuaJIT codebase. Using a snapshot, it can then reconstruct the operand stack just like how it would have been had the trace been interpreted (snap_restoreval and ln_snap_restore in lj_snap.c).


为了解决这个问题，LuaJIT维护了一个从堆栈位置到SSA IR指令的映射。在LuaJIT的代码库里这种映射被称为快照。使用一个快照时，它可以重建操作数堆栈，就像这个trace被解释执行时的样子（lj_snap.c中的snap_restoreval 和ln_snap_restore）。

> 
This is slow, but [quoting](http://lua-users.org/lists/lua-l/2009-11/msg00089.html) Mike Pall: “State restoration using this data-driven approach is slow of course. But repeatedly taken side exits quickly trigger the generation of side traces. The snapshot is used to initialize the IR of the side trace with the necessary state using pseudo-loads. These can be optimized together with the remainder of the side trace. The pseudo-loads are unified with the machine state of the parent trace by the backend to enable zero-cost linking to side traces.”


这很慢，不过用Mike Pall的话说：“使用这个数据驱动的手法来进行状态恢复必须很慢。不过，反复执行的边界条件的出口会很快触发边界trace的生成。快照和一些利用假负荷的必要状态被使用来初始化边界trace的IR。这些都可以和边界trace的剩余部分一起被优化。后台使用父trace的机器状态来统一这些假负荷，用以启用零消耗的边界trace的链接。”



