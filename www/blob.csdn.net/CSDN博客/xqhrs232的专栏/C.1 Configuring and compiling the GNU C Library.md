# C.1 Configuring and compiling the GNU C Library - xqhrs232的专栏 - CSDN博客
2017年02月21日 17:25:36[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：381
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://www.gnu.org/software/libc/manual/html_node/Configuring-and-compiling.html](http://www.gnu.org/software/libc/manual/html_node/Configuring-and-compiling.html)
The GNU C Library cannot be compiled in the source directory. You must build it in a separate build directory. For example, if you have unpacked the GNU C Library sources in /src/gnu/glibc-version,
 create a directory /src/gnu/glibc-build to put the object files in. This allows removing the whole build directory in case an error occurs, which is the safest way to get a fresh start and should always be done.
From your object directory, run the shell script configure located at the top level of the source tree. In the scenario above, you’d type
$ ../glibc-version/configure args…
Please note that even though you’re building in a separate build directory, the compilation may need to create or modify files and directories in the source directory.
`configure` takes many options, but the only one that is usually mandatory is ‘--prefix’. This option tells `configure` where you want the GNU C Library installed. This defaults to /usr/local,
 but the normal setting to install as the standard system library is ‘--prefix=/usr’ for GNU/Linux systems and ‘--prefix=’ (an empty prefix) for GNU/Hurd systems.
It may also be useful to set the CC and CFLAGS variables in the environment when running `configure`. CC selects the C compiler that will be used, and CFLAGS sets
 optimization options for the compiler.
The following list describes all of the available options for `configure`:
- ‘--prefix=directory’
Install machine-independent data files in subdirectories of directory. The default is to install in /usr/local.
- ‘--exec-prefix=directory’
Install the library and other machine-dependent files in subdirectories of directory. The default is to the ‘--prefix’ directory if that option is specified, or /usr/local otherwise.
- ‘--with-headers=directory’
Look for kernel header files in directory, not /usr/include. The GNU C Library needs information from the kernel’s header files describing the interface to the kernel. The GNU C Library will normally look in /usr/include for
 them, but if you specify this option, it will look in DIRECTORY instead.
This option is primarily of use on a system where the headers in /usr/include come from an older version of the GNU C Library. Conflicts can occasionally happen in this case. You can also use this option if you want to compile the GNU C Library
 with a newer set of kernel headers than the ones found in /usr/include.
- ‘--enable-add-ons[=list]’
Specify add-on packages to include in the build. If this option is specified with no list, it enables all the add-on packages it finds in the main source directory; this is the default behavior. You may specify an explicit list of add-ons to use in list,
 separated by spaces or commas (if you use spaces, remember to quote them from the shell). Each add-on in list can be an absolute directory name or can be a directory name relative to the main source directory, or relative to the build directory
 (that is, the current working directory). For example, ‘--enable-add-ons=nptl,../glibc-libidn-version’.
- ‘--enable-kernel=version’
This option is currently only useful on GNU/Linux systems. The version parameter should have the form X.Y.Z and describes the smallest version of the Linux kernel the generated library is expected to support. The higher the version number
 is, the less compatibility code is added, and the faster the code gets.
- ‘--with-binutils=directory’
Use the binutils (assembler and linker) in directory, not the ones the C compiler would default to. You can use this option if the default binutils on your system cannot deal with all the constructs in the GNU C Library. In that case, `configure` will
 detect the problem and suppress these constructs, so that the library will still be usable, but functionality may be lost—for example, you can’t build a shared libc with old binutils.
- ‘--without-fp’
Use this option if your computer lacks hardware floating-point support and your operating system does not emulate an FPU.
- ‘--disable-shared’
Don’t build shared libraries even if it is possible. Not all systems support shared libraries; you need ELF support and (currently) the GNU linker.
- ‘--disable-profile’
Don’t build libraries with profiling information. You may want to use this option if you don’t plan to do profiling.
- ‘--enable-static-nss’
Compile static versions of the NSS (Name Service Switch) libraries. This is not recommended because it defeats the purpose of NSS; a program linked statically with the NSS libraries cannot be dynamically reconfigured to use a different name database.
- ‘--enable-hardcoded-path-in-tests’
By default, dynamic tests are linked to run with the installed C library. This option hardcodes the newly built C library path in dynamic tests so that they can be invoked directly.
- ‘--disable-timezone-tools’
By default, timezone related utilities (`zic`, `zdump`, and `tzselect`) are installed with the GNU C Library. If you are building these independently (e.g. by using the ‘tzcode’ package), then this option will
 allow disabling the install of these.
Note that you need to make sure the external tools are kept in sync with the versions that the GNU C Library expects as the data formats may change over time. Consult the timezone subdirectory for more details.
- ‘--enable-lock-elision=yes’
Enable lock elision for pthread mutexes by default.
- ‘--enable-stack-protector’
- ‘--enable-stack-protector=strong’
- ‘--enable-stack-protector=all’
Compile the C library and all other parts of the glibc package (including the threading and math libraries, NSS modules, and transliteration modules) using the GCC -fstack-protector, -fstack-protector-strong or -fstack-protector-all options
 to detect stack overruns. Only the dynamic linker and a small number of routines called directly from assembler are excluded from this protection.
- ‘--enable-pt_chown’
The file pt_chown is a helper binary for `grantpt` (see [Pseudo-Terminals](http://www.gnu.org/software/libc/manual/html_node/Allocation.html#Allocation)) that is installed setuid root to fix up pseudo-terminal
 ownership. It is not built by default because systems using the Linux kernel are commonly built with the `devpts` filesystem enabled and mounted at /dev/pts, which manages pseudo-terminal ownership automatically. By using ‘--enable-pt_chown’,
 you may build pt_chown and install it setuid and owned by `root`. The use of pt_chown introduces additional security risks to the system and you should enable it only if you understand and accept those risks.
- ‘--disable-werror’
By default, the GNU C Library is built with -Werror. If you wish to build without this option (for example, if building with a newer version of GCC than this version of the GNU C Library was tested with, so new warnings cause the build with -Werror to
 fail), you can configure with --disable-werror.
- ‘--disable-mathvec’
By default for x86_64, the GNU C Library is built with the vector math library. Use this option to disable the vector math library.
- ‘--enable-tunables’
Tunables support allows additional library parameters to be customized at runtime. This is an experimental feature and affects startup time and is thus disabled by default. This option can take the following values:
- `no`
This is the default if the option is not passed to configure. This disables tunables.
- `yes`
This is the default if the option is passed to configure. This enables tunables and selects the default frontend (currently ‘valstring’).
- `valstring`
This enables tunables and selects the ‘valstring’ frontend for tunables. This frontend allows users to specify tunables as a colon-separated list in a single environment variable `GLIBC_TUNABLES`.
- ‘--build=build-system’
- ‘--host=host-system’
These options are for cross-compiling. If you specify both options and build-system is different from host-system, `configure` will prepare to cross-compile the GNU C Library from build-system to be used on host-system.
 You’ll probably need the ‘--with-headers’ option too, and you may have to override configure’s selection of the compiler and/or binutils.
If you only specify ‘--host’, `configure` will prepare for a native compile but use what you specify instead of guessing what your system is. This is most useful to change the CPU submodel. For example, if `configure` guesses
 your machine as `i686-pc-linux-gnu` but you want to compile a library for 586es, give ‘--host=i586-pc-linux-gnu’ or just ‘--host=i586-linux’ and add the appropriate compiler flags (‘-mcpu=i586’ will do the
 trick) to CFLAGS.
If you specify just ‘--build’, `configure` will get confused.
- ‘--with-pkgversion=version’
Specify a description, possibly including a build number or build date, of the binaries being built, to be included in --version output from programs installed with the GNU C Library. For example, --with-pkgversion='FooBar GNU/Linux glibc
 build 123'. The default value is ‘GNU libc’.
- ‘--with-bugurl=url’
Specify the URL that users should visit if they wish to report a bug, to be included in --help output from programs installed with the GNU C Library. The default value refers to the main bug-reporting information for the GNU C Library.
To build the library and related programs, type `make`. This will produce a lot of output, some of which may look like errors from `make` but aren’t. Look for error messages from `make` containing
 ‘***’. Those indicate that something is seriously wrong.
The compilation process can take a long time, depending on the configuration and the speed of your machine. Some complex modules may take a very long time to compile, as much as several minutes on slower machines. Do
 not panic if the compiler appears to hang.
If you want to run a parallel make, simply pass the ‘-j’ option with an appropriate numeric parameter to `make`. You need a recent GNU `make` version, though.
To build and run test programs which exercise some of the library facilities, type `make check`. If it does not complete successfully, do not use the built library, and report a bug after verifying that the
 problem is not already known. See [Reporting Bugs](http://www.gnu.org/software/libc/manual/html_node/Reporting-Bugs.html#Reporting-Bugs), for instructions on reporting bugs. Note that some of the tests assume they are not being run
 by `root`. We recommend you compile and test the GNU C Library as an unprivileged user.
Before reporting bugs make sure there is no problem with your system. The tests (and later installation) use some pre-existing files of the system such as /etc/passwd, /etc/nsswitch.conf and
 others. These files must all contain correct and sensible content.
Normally, `make check` will run all the tests before reporting all problems found and exiting with error status if any problems occurred. You can specify ‘stop-on-test-failure=y’ when running 
```
make
 check
```
 to make the test run stop and exit with an error status immediately when a failure occurs.
The GNU C Library pretty printers come with their own set of scripts for testing, which run together with the rest of the testsuite through `make check`. These scripts require the following tools to run successfully:
- Python 2.7.6/3.4.3 or later
Python is required for running the printers’ test scripts.
- PExpect 4.0
The printer tests drive GDB through test programs and compare its output to the printers’. PExpect is used to capture the output of GDB, and should be compatible with the Python version in your system.
- GDB 7.8 or later with support for Python 2.7.6/3.4.3 or later
GDB itself needs to be configured with Python support in order to use the pretty printers. Notice that your system having Python available doesn’t imply that GDB supports it, nor that your system’s Python and GDB’s have the same version.
If these tools are absent, the printer tests will report themselves as `UNSUPPORTED`. Notice that some of the printer tests require the GNU C Library to be compiled with debugging symbols.
To format the GNU C Library Reference Manual for printing, type `make dvi`. You need a working TeX installation to do this. The distribution builds the on-line formatted version of the manual,
 as Info files, as part of the build process. You can build them manually with `make info`.
The library has a number of special-purpose configuration parameters which you can find in Makeconfig. These can be overwritten with the file configparms. To change them, create a configparms in
 your build directory and add values as appropriate for your system. The file is included and parsed by `make` and has to follow the conventions for makefiles.
It is easy to configure the GNU C Library for cross-compilation by setting a few variables in configparms. Set `CC` to the cross-compiler for the target you configured the library for; it is
 important to use this same `CC` value when running `configure`, like this: ‘CC=target-gcc configure target’. Set `BUILD_CC` to the compiler to use for programs run on the build system as part
 of compiling the library. You may need to set `AR` to cross-compiling versions of `ar` if the native tools are not configured to work with object files for the target you configured for. When cross-compiling the GNU C Library, it may
 be tested using ‘make check test-wrapper="srcdir/scripts/cross-test-ssh.sh hostname"’, where srcdir is the absolute directory name for the main source directory and hostname is the host name of a system
 that can run the newly built binaries of the GNU C Library. The source and build directories must be visible at the same locations on both the build system and hostname.
In general, when testing the GNU C Library, ‘test-wrapper’ may be set to the name and arguments of any program to run newly built binaries. This program must preserve the arguments to the binary being run,
 its working directory and the standard input, output and error file descriptors. If ‘test-wrapper env’ will not work to run a program with environment variables set, then ‘test-wrapper-env’ must be set to a program that
 runs a newly built program with environment variable assignments in effect, those assignments being specified as ‘var=value’ before the name of the program to be run. If multiple assignments to the same variable are specified,
 the last assignment specified must take precedence. Similarly, if ‘test-wrapper env -i’ will not work to run a program with an environment completely empty of variables except those directly assigned, then ‘test-wrapper-env-only’
 must be set; its use has the same syntax as ‘test-wrapper-env’, the only difference in its semantics being starting with an empty set of environment variables rather than the ambient set.
