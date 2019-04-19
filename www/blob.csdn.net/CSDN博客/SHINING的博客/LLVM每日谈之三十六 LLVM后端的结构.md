# LLVM每日谈之三十六  LLVM后端的结构 - SHINING的博客 - CSDN博客
2018年07月24日 10:40:56[snsn1984](https://me.csdn.net/snsn1984)阅读数：891
所属专栏：[LLVM每日谈](https://blog.csdn.net/column/details/llvm-study.html)
最近在重读《Gettiing Started with LLVM Core Libraries》，后端方面感觉有新的认识。在这个过程中，发现这本书中关于后端结构的几个图非常的好，能说明很多问题，就将这几个图摘录下来，根据自己的理解，简单做个介绍。希望能对初学者有用。
![](https://img-blog.csdn.net/2018072409144092?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Nuc24xOTg0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                                         图1   LLVM后端的整个流程图（《Getting》P134）
 图1是整个后端的流程图。从LLVM IR开始一直到生成Assembly或者是Object code，涵盖了LLVM后端的整个过程。这里需要说明的是灰色框和白色框的区别。白色框全部是pass，灰色框是具体操作环节。灰色框内的具体环节，其实也是由一系列的pass组成的，它们是生成代码所必须的。而白色框中的pass，则是为了提高效率，可以理解为不是必须的。
Pass这个概念之前在LLVM IR部分的介绍的时候,已经谈过很多次了，基本上大家也都理解了。但是Pass在后端同样十分重要。而且图中所包含的pass，可以泛泛的理解为后端的pass。后端的pass可以分为两种，一种还是针对于LLVM IR的，比如上图中从LLVM IR出来遇到的第一个Passes白色框，这就是还是针对于LLVM IR(IR instructions)的。另外一种，则是针对MachineInstr instructions的pass（寄存器分配的环节就包含了此类pass）。这二者所针对的指令不同，所继承的pass类也不同，调用方式也略有不同，但是实际的运行方式，差别不是太大。
![](https://img-blog.csdn.net/20180724092344221?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Nuc24xOTg0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                            图2   Instruction selection之前的所有环节（《Getting》P150）
图2是将从LLVM IR开始,直到Instruction selection环节的过程中的所有步骤，都详细的列了出来。这里要明白，此处所说的Instruction selection环节是狭义的Instruction selection环节。图1中Instruction selection环节是广义的，狭义的Instruction selection环节只是广义的Instruction selection环节中最后的一部分。而此图也可以理解为是整个广义的Instruction selection环节的内部过程。最后一环则为狭义的Instruction selection环节。
此图比较核心的部分，就是不断的和Target lowering不断的交互。这里涉及到了DAG中目标相关node和目标无关node。很多资料介绍后端的时候，在没生成Machine code instruction的时候，都叫平台无关或者目标无关，其实是不严谨的。从SelectionDAG nodes开始，已经开始有目标依赖的指令加入到DAG node。这也是Target lowering的主要作用，生成相关的目标依赖的指令。
![](https://img-blog.csdn.net/20180724093834490?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Nuc24xOTg0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                                 图3   Register allocation环节的内部流程（《Getting》P162）
图3是Register allocation环节的内部流程，当然也包含了一个狭义的Register allocation环节。这里将包含了虚拟寄存器的MachineInstr最终转变为包含了物理寄存器的MachineInstr。
![](https://img-blog.csdn.net/20180724100405645?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Nuc24xOTg0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                                       图4   Code emission的过程（《Getting》P170）
图4表明了Code emission的整个过程。这个过程将MachineInst转化为MCInst，然后再根据需要转化为assembly或者是binary instruction(Object Code)。在这个过程之中，一个重点就是要明白MachineInst(MI)和MCInst的区别。MachineInst是Machine instructions，MCInst是machine code instructions。前者要比后者包含的信息更多一些，后者不光可以从后端生成，还可以通过binary code反汇编出来。
参考文献：《Gettiing Started with LLVM Core Libraries》，文中所采用的四个图都已经标明所出自的页面。
