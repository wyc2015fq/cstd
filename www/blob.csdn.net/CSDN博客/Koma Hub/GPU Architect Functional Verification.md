# GPU Architect Functional Verification - Koma Hub - CSDN博客
2018年06月05日 10:17:19[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：86
个人分类：[CUDA](https://blog.csdn.net/Rong_Toa/article/category/7156216)
**GPU Architect "Functional Verification"要做什么**：
1.研究下一代GPU架构和最先进的新计算特性。
2.使用随机方法在全芯片级验证并验证新的GPU架构和复杂的计算特性。
3.与GPU架构沟通并计算功能设计师制定随机测试计划。
4.设计和开发随机方法和基础设施来生成随机测试。
5.在各种平台上运行和调试随机测试，例如功能模拟器，仿真器和芯片。
6.与ASIC团队合作，确定并根源于计算功能的RTL错误。
7.设计创新的方法论，流程和工具，以提高GPU架构验证和验证的效率。
8.与设计人员讨论新的计算特性，并提供关于未来计算特性的设计和实现的反馈。
**能力**：
1.经过验证的计算机科学知识，专注于计算机体系结构。
2.深入理解GPU概念和流水线，重点关注计算处理和并行计算。
3.深入理解C++语言和至少5年的C/C++开发和调试经验。
4.BSEE（电机工程学士学位Bachelor of Science in Electrical Engineering），BSCSE或同等要求。 MS或PhD是一个优点。
**描述**：
1.GPU full chip 随机验证的工作属于GPU架构部门。
2.不同于ASIC DV 验证，这个职位负责从GPU架构的角度来验证GPU功能的正确性，Test的主要运行环境是Cmodel（C++实现的GPU功能模拟器）。
3.不同于IP，unit单元验证，这个职位是从整个GPU的角度来验证功能的正确性，所以会学习和覆盖整个GPU pipeline。
4.不同于只是将GPU作为一个模块的SOC验证。且这个职位不涉及video, display的验证。
5.这个职位是功能验证，所以不会验证性能的优劣。
6.这个职位目前是计算特性相关的职位。
7.这里的测试用例是由随机测试用例生成器批量自动生成，而非人工逐条编写的测试用例。
**主要工作**
1.学习Nvidia最新的GPU架构和计算特性。了解整个GPU架构，各个模块的功能，各模块之间如何协作，以及sw，fake driver, GPU之间如何配合完成计算功能。
2.跟GPU架构设计者沟通讨论，制定随机验证计划， 覆盖率目标和检测方法。
3.用随机的方法，从GPU full chip level 来验证新的GPU架构和复杂的计算特性的功能。
4.设计和开发随机测试用例生成器来生成随机测试用例。
5.保证随机测试用例的正确性。
6.设计和实现不损害随机测试用例的自由度和覆盖率。
7.运行随机测试用例并调试在cmodel, RTL（寄存器转换级电路（Register Transfer Level））, emulator（仿真器）上出现的问题。
8.随机验证通常会生成和运行大量不同的随机测试用例，因此要不断创新以提高生成和debug的效率
9.工作中，在Cmodel上跑的测试用例的数量更大， 而在RTL和Emulation的Debug更加复杂和困难
10.工作语言主要是C++，部分infrastructure“基础设施”也会是脚本。
**从人群中脱颖而出的方法**：
1.深刻理解CUDA编程模型及其最新功能，如UVM（通用验证方法学Universal Verification Methodology, UVM）。
2.验证和/或验证GPU计算单元和功能的经验。
3.对GPU和计算架构验证，验证和设计有强烈的热情。
