# Undefined Behavior Causes CPU Meltdown in IBM PC's! - Koma Hub - CSDN博客
2019年01月31日 20:30:56[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：51
个人分类：[C/C++](https://blog.csdn.net/Rong_Toa/article/category/7156199)
The suggestion of undefined software behavior causing CPU meltdown isn't as farfetched asit first appears.
The original IBM PC monitor operated at a horizontal scan rate provided by the video controller chip. The flyback transformer (the gadget that produces the high voltage needed to accelerate the electrons to light up the phosphors on the monitor) relied on this being a reasonable frequency. However, it was possible, in software, to set the video chip scan rate to zero, thus feeding a constant voltage into the primary side of the transformer. It then acted as a resistor, and dissipated its power as heat rather than transforming it up onto the screen. This burned the monitor out in seconds. Voilà: undefined software behavior causes system meltdown!
**Bad Code:**
**undefined**— The behavior for something incorrect, on which the standard does not impose any requirements. Anything is allowed to happen, from nothing, to a warning message to program termination, to CPU meltdown, to launching nuclear missiles (assuming you have the correct hardware option installed).
