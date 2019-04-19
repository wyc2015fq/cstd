# The Linux Kernel Archives Download - Koma Hub - CSDN博客
2019年01月12日 08:14:44[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：51
[https://www.kernel.org/](https://www.kernel.org/)
There are several main categories into which kernel releases may fall:
Prepatch
Prepatch or "RC" kernels are mainline kernel pre-releases that are mostly aimed at other kernel developers and Linux enthusiasts. They must be compiled from source and usually contain new features that must be tested before they can be put into a stable release. Prepatch kernels are maintained and released by Linus Torvalds.
Mainline
Mainline tree is maintained by Linus Torvalds. It's the tree where all new features are introduced and where all the exciting new development happens. New mainline kernels are released every 2-3 months.
Stable
After each mainline kernel is released, it is considered "stable." Any bug fixes for a stable kernel are backported from the mainline tree and applied by a designated stable kernel maintainer. There are usually only a few bugfix kernel releases until next mainline kernel becomes available -- unless it is designated a "longterm maintenance kernel." Stable kernel updates are released on as-needed basis, usually once a week.
Longterm
There are usually several "longterm maintenance" kernel releases provided for the purposes of backporting bugfixes for older kernel trees. Only important bugfixes are applied to such kernels and they don't usually see very frequent releases, especially for older trees.
|Version|Maintainer|Released|Projected EOL|
|----|----|----|----|
|4.19|Greg Kroah-Hartman|2018-10-22|Dec, 2020|
|4.14|Greg Kroah-Hartman|2017-11-12|Jan, 2020|
|4.9|Greg Kroah-Hartman|2016-12-11|Jan, 2023|
|4.4|Greg Kroah-Hartman|2016-01-10|Feb, 2022|
|3.16|Ben Hutchings|2014-08-03|Apr, 2020|
## Distribution kernels
Many Linux distributions provide their own "longterm maintenance" kernels that may or may not be based on those maintained by kernel developers. These kernel releases are not hosted at kernel.org and kernel developers can provide no support for them.
It is easy to tell if you are running a distribution kernel. Unless you downloaded, compiled and installed your own version of kernel from kernel.org, you are running a distribution kernel. To find out the version of your kernel, run uname -r:
# uname -r
3.7.5-201.fc18.x86_64
If you see anything at all after the dash, you are running a distribution kernel. Please use the support channels offered by your distribution vendor to obtain kernel support.
