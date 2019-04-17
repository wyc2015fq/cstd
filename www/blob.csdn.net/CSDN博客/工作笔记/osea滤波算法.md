# osea滤波算法 - 工作笔记 - CSDN博客





2012年01月20日 11:35:46[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5410








3.1 The Filters

滤波器

We filter the signal to generate a windowed (time limited) estimate of the energy in the QRS frequency band. We do this by:

1. Low pass filtering,

2. High pass filtering,

3. Taking the derivative,

4. Taking the absolute value of the signal, and

5. Averaging the absolute value over an 80 ms window.

我们过滤信号生成一个窗口的（时间限制）QRS频带中的能量估计值，通过低通，高通，求导，求绝对值，且计算一个80ms移动窗口的平均绝对值



The final filter output produces what might be called a lump every time a QRS complex occurs.


每次出现一个QRS波，滤波器输出都会产生一个块状信号。

T-waves generally produce smaller lumps than QRS complexes.

T波通常产生比QRS波更小的块状信号。

The high pass, low pass, and derivative combine to produce a bandpass filter with a pass band from 5 to 11 Hz, roughly the bandwidth that contains most of the energy in the QRS complex.


高通，低通，求导级联生成一个5~11Hz通频带的带通滤波器，这个带宽大致包含QRS波中大多数能量

In this implementation we have used the absolute value, reducing the gain sensitivity and slightly improving the performance of the algorithm.

平方过程，会让QRS波获得较敏感的增益。因此，我们在这个，用求绝对值来减少敏感增益，少许提高算法性能。

The averaging window was chosen to be roughly the width of a typical QRS complex.

选取的移动窗口大致是一个典型QRS波宽度



 In the original algorithm this window was 150 ms wide to allow for the wide QRS complexes produced by Premature Ventricular Contractions (PVCs). Since then, it has been shown that a narrower window produces better results [9].

在原来的算法中，这个窗口宽度是150ms，为了允许PVC产生的QRS宽度。从那时，有人指出，窄的移动窗口，会产生更好的结果。



