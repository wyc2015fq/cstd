# micro-fusion & macro-fusion - TaigaComplex求职中 - 博客园







# [micro-fusion & macro-fusion](https://www.cnblogs.com/TaigaCon/p/7702920.html)





# micro-fusion

随着技术的发展，CPU内部指令处理单元（execution unit）以及端口（port）增多，在Pentium 4的时候，发出到Execution Unit的μops的throughput可以高达6（6 μops/clock cycle），这时候，流水线中的瓶颈会出现在register renaming（RAT）以及retirement（RRF），这两部分的throughput为3。为了突破这部分的瓶颈，Intel从Pentium M处理器开始引入了micro-fusion技术。

有很多对内存进行操作的指令都会被分成两个或以上的μops，如 add eax, [mem] 在解码时就会分成 mov tmp, [mem]; add eax, tmp。这类型的指令在前端只需要fetch与decode一条指令，相比原来的两条指令占用更少资源（带宽、解码资源、功耗），不过由于在解码后分成多个μops，占用资源（μop entries）增多，但是throughput相对较小，使得RAT以及RRF阶段显得更为拥堵。

micro-fusion为这种问题提供了很好的解决方案：把同一条指令的几个μops混合成一个复杂的μop，使得其在RAT以及RRF阶段只占用一项；而在EU阶段，该复杂μop会被多次发送到EU中进行处理，表现得像是有多个已被分解的μops一样。micro-fusion的指令可以被任意decoder进行解码

可以被micro-fused的指令：
- 所有的store指令，写回内存的store指令分为两个步骤：store-address、store-data。
- 所有读内存与运算的混合指令（load+op），如：

- ADDPS XMM9, OWORD PTR [RSP+40]
- FADD DOUBLE PTR [RDI+RSI*8]
- XOR RAX, QWORD PTR [RBP+32]


- 所有读内存与跳转的混合指令（load+jmp），如：

- JMP [RDI+200]
- RET


- CMP与TEST对比内存操作数并与立即数的指令（cmp mem-imm）。

上面的描述有例外的情况，就是不能采用RIP寄存器进行内存寻址：
- CMP [RIP+400], 27
- MOV [RIP+3000], 142
- JMP [RIP+5000000]

采用了RIP寄存器进行内存寻址的指令是不能被micro-fused的，并且这些指令只能由decoder0进行解码。



# macro-fusion

Intel在Core处理器时开始引入macro-fusion技术。与micro-fusion一样，macro-fusion能使得指令占用更少的资源与功耗。不同的是macro-fusion是把两条指令组合成一个复杂的μop，这种μops在pipeline的各个阶段中也是只占用一项。

在Core微处理器中，第一条指令必须为CMP或者TEST，作用于REG-REG，REG-IMM或者REG-MEM，第二条指令必须为条件跳转指令，并且指令恰好位于第一条指令之后。比较然后进行条件跳转这种指令组合在日常的代码中非常普遍，因此如果代码设计得当，将能更容易触发macro-fusion，达到减少带宽占用以及功耗的效果。

指令流在predecode后，可以分离出一条一条的指令，然后流经instruction queue。在instruction queue与decoder之间会对可以合并的指令对进行macro-fusion。macro-fused后的指令可以被任意decoder进行解码



进行macro-fusion的指令对需要满足以下条件：

**1.** 对与CMP与TEST来说，只有是下面的模式才能进行合并：

REG-REG，如：CMP EAX, ECX; JZ label
REG-IMM， 如：CMP EAX, 0x80; JZ label
REG-MEM， 如：CMP EAX, [ECX]; JZ label
MEM-REG， 如：CMP [EAX], ECX; JZ label

如果是MEM-IMM模式的话，则不能合并。

**2.** 不同的第一条指令，对第二条指令有不同的匹配要求，并不是所有的条件跳转指令都能与第一条指令进行配对合并。

Core微处理器中可以进行macro-fusion的指令对：
JL/JNGE/JGE/JNL/JLE/JNG/JG/JNLE ✔ ✘ 
可见TEST可以跟所有的条件跳转指令合并，但是CMP只能跟检查了carry flag或者zero flag的条件跳转指令合并，也就是说Core微处理器中，对于有符号的比较跳转无法进行合并，但是无符号的比较跳转则可以进行合并。

![Core-macro-fusion-signed](https://images2017.cnblogs.com/blog/421096/201710/421096-20171021014001677-1993670497.png)![Core-macro-fusion-unsigned](https://images2017.cnblogs.com/blog/421096/201710/421096-20171021014002740-2004457821.png)

如上图所示，有符号的条件跳转指令为JGE，而无符号的条件跳转指令为JAE。在Core微处理器中CMP与JAE可以进行macro-fusion，得到形如CMPJAE的μop。

不过在Nehalem微处理器之后就可以支持有符号跳转指令的macro-fusion了。

Sandy Bridge微处理器进一步扩充了指令可以进行macro-fusion的指令对：
JL/JNGE/JGE/JNL/JLE/JNG/JG/JNLE ✔ ✔ ✔ ✔ ✔ ✔ ✔ 


**Reference:**

[Intel® 64 and IA-32 Architectures Optimization Reference Manual](https://www.intel.com/content/www/us/en/architecture-and-technology/64-ia-32-architectures-optimization-manual.html)

[Macro-Operation Fusion (MOP Fusion)](https://en.wikichip.org/wiki/macro-operation_fusion)

[A JOURNEY IN MODERN COMPUTER ARCHITECTURES](http://abinstein.blogspot.tw/2007/05/decoding-x86-from-p6-to-core-2-and.html)

[Intel 64 and IA-32 Architectures Software Developer's Manual](https://www.intel.com/content/www/us/en/architecture-and-technology/64-ia-32-architectures-software-developer-manual-325462.html)












