# The Linux kernel user-space API guide - Koma Hub - CSDN博客
2019年03月09日 15:11:23[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：27
个人分类：[Linux kernel																[Linux](https://blog.csdn.net/Rong_Toa/article/category/7156254)](https://blog.csdn.net/Rong_Toa/article/category/7348890)
[https://www.kernel.org/doc/html/latest/userspace-api/index.html](https://www.kernel.org/doc/html/latest/userspace-api/index.html)
# The Linux kernel user-space API guide
While much of the kernel’s user-space API is documented elsewhere (particularly in the [man-pages](https://www.kernel.org/doc/man-pages/) project), some user-space information can also be found in the kernel tree itself. This manual is intended to be the place where this information is gathered.
Table of contents
- [No New Privileges Flag](https://www.kernel.org/doc/html/latest/userspace-api/no_new_privs.html)
- [Seccomp BPF (SECure COMPuting with filters)](https://www.kernel.org/doc/html/latest/userspace-api/seccomp_filter.html)- [Introduction](https://www.kernel.org/doc/html/latest/userspace-api/seccomp_filter.html#introduction)
- [What it isn’t](https://www.kernel.org/doc/html/latest/userspace-api/seccomp_filter.html#what-it-isn-t)
- [Usage](https://www.kernel.org/doc/html/latest/userspace-api/seccomp_filter.html#usage)
- [Return values](https://www.kernel.org/doc/html/latest/userspace-api/seccomp_filter.html#return-values)
- [Pitfalls](https://www.kernel.org/doc/html/latest/userspace-api/seccomp_filter.html#pitfalls)
- [Example](https://www.kernel.org/doc/html/latest/userspace-api/seccomp_filter.html#example)
- [Userspace Notification](https://www.kernel.org/doc/html/latest/userspace-api/seccomp_filter.html#userspace-notification)
- [Sysctls](https://www.kernel.org/doc/html/latest/userspace-api/seccomp_filter.html#sysctls)
- [Adding architecture support](https://www.kernel.org/doc/html/latest/userspace-api/seccomp_filter.html#adding-architecture-support)
- [Caveats](https://www.kernel.org/doc/html/latest/userspace-api/seccomp_filter.html#caveats)
- [unshare system call](https://www.kernel.org/doc/html/latest/userspace-api/unshare.html)- [Change Log](https://www.kernel.org/doc/html/latest/userspace-api/unshare.html#change-log)
- [Contents](https://www.kernel.org/doc/html/latest/userspace-api/unshare.html#contents)
- [1) Overview](https://www.kernel.org/doc/html/latest/userspace-api/unshare.html#overview)
- [2) Benefits](https://www.kernel.org/doc/html/latest/userspace-api/unshare.html#benefits)
- [3) Cost](https://www.kernel.org/doc/html/latest/userspace-api/unshare.html#cost)
- [4) Requirements](https://www.kernel.org/doc/html/latest/userspace-api/unshare.html#requirements)
- [5) Functional Specification](https://www.kernel.org/doc/html/latest/userspace-api/unshare.html#functional-specification)
- [6) High Level Design](https://www.kernel.org/doc/html/latest/userspace-api/unshare.html#high-level-design)
- [7) Low Level Design](https://www.kernel.org/doc/html/latest/userspace-api/unshare.html#low-level-design)
- [8) Test Specification](https://www.kernel.org/doc/html/latest/userspace-api/unshare.html#test-specification)
- [9) Future Work](https://www.kernel.org/doc/html/latest/userspace-api/unshare.html#future-work)
- [Speculation Control](https://www.kernel.org/doc/html/latest/userspace-api/spec_ctrl.html)- [PR_GET_SPECULATION_CTRL](https://www.kernel.org/doc/html/latest/userspace-api/spec_ctrl.html#pr-get-speculation-ctrl)
- [PR_SET_SPECULATION_CTRL](https://www.kernel.org/doc/html/latest/userspace-api/spec_ctrl.html#pr-set-speculation-ctrl)
- [Common error codes](https://www.kernel.org/doc/html/latest/userspace-api/spec_ctrl.html#common-error-codes)
- [PR_SET_SPECULATION_CTRL error codes](https://www.kernel.org/doc/html/latest/userspace-api/spec_ctrl.html#pr-set-speculation-ctrl-error-codes)
- [Speculation misfeature controls](https://www.kernel.org/doc/html/latest/userspace-api/spec_ctrl.html#speculation-misfeature-controls)
[Next](https://www.kernel.org/doc/html/latest/userspace-api/no_new_privs.html)[Previous](https://www.kernel.org/doc/html/latest/admin-guide/perf-security.html)
© Copyright The kernel development community.
Built with [Sphinx](http://sphinx-doc.org/) using a [theme](https://github.com/snide/sphinx_rtd_theme) provided by [Read the Docs](https://readthedocs.org/).
