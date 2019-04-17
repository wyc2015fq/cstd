# UCRT: VC 2015 Universal CRT, by Microsoft - DoubleLi - 博客园






[https://blogs.msdn.microsoft.com/vcblog/2015/03/03/introducing-the-universal-crt/](https://blogs.msdn.microsoft.com/vcblog/2015/03/03/introducing-the-universal-crt/)

App local UCRT DLLs:`C:\Program Files (x86)\Windows Kits\10\Redist\ucrt\DLLs\x86`

经Liigo本人测试，VC2015非静态链接C运行库(/MD)编译的EXE/DLL，在XP SP3系统内，除了依赖以上目录内的(几乎)所有dll以外，还额外依赖`vcruntime140.dll`。前面提到的这些dll，共计42个，统统放到EXE/DLL所在目录，可确保所有依赖项都不欠缺。这42个dll文件分别是(行首是字节数)：

```
18,624        api-ms-win-core-console-l1-1-0.dll
17,600        api-ms-win-core-datetime-l1-1-0.dll
17,600        api-ms-win-core-debug-l1-1-0.dll
18,104        api-ms-win-core-errorhandling-l1-1-0.dll
21,696        api-ms-win-core-file-l1-1-0.dll
18,112        api-ms-win-core-file-l1-2-0.dll
18,112        api-ms-win-core-file-l2-1-0.dll
18,112        api-ms-win-core-handle-l1-1-0.dll
18,112        api-ms-win-core-heap-l1-1-0.dll
18,104        api-ms-win-core-interlocked-l1-1-0.dll
18,624        api-ms-win-core-libraryloader-l1-1-0.dll
20,672        api-ms-win-core-localization-l1-2-0.dll
18,624        api-ms-win-core-memory-l1-1-0.dll
18,112        api-ms-win-core-namedpipe-l1-1-0.dll
19,136        api-ms-win-core-processenvironment-l1-1-0.dll
20,160        api-ms-win-core-processthreads-l1-1-0.dll
18,624        api-ms-win-core-processthreads-l1-1-1.dll
17,600        api-ms-win-core-profile-l1-1-0.dll
17,600        api-ms-win-core-rtlsupport-l1-1-0.dll
18,112        api-ms-win-core-string-l1-1-0.dll
20,160        api-ms-win-core-synch-l1-1-0.dll
18,624        api-ms-win-core-synch-l1-2-0.dll
19,136        api-ms-win-core-sysinfo-l1-1-0.dll
18,112        api-ms-win-core-timezone-l1-1-0.dll
18,112        api-ms-win-core-util-l1-1-0.dll
19,136        api-ms-win-crt-conio-l1-1-0.dll
22,208        api-ms-win-crt-convert-l1-1-0.dll
18,624        api-ms-win-crt-environment-l1-1-0.dll
20,160        api-ms-win-crt-filesystem-l1-1-0.dll
18,624        api-ms-win-crt-heap-l1-1-0.dll
18,624        api-ms-win-crt-locale-l1-1-0.dll
28,864        api-ms-win-crt-math-l1-1-0.dll
25,792        api-ms-win-crt-multibyte-l1-1-0.dll
72,896        api-ms-win-crt-private-l1-1-0.dll
19,136        api-ms-win-crt-process-l1-1-0.dll
22,720        api-ms-win-crt-runtime-l1-1-0.dll
24,256        api-ms-win-crt-stdio-l1-1-0.dll
24,256        api-ms-win-crt-string-l1-1-0.dll
20,672        api-ms-win-crt-time-l1-1-0.dll
18,624        api-ms-win-crt-utility-l1-1-0.dll
917,184       ucrtbase.dll
85,328        vcruntime140.dll
```

Windows Kits 10可通过VC2015安装包安装，`vcruntime140.dll`文件可从安装过`vc_redist.x86.exe` for vc2015的系统System32目录中提取。见附件vc2015-rtl-x86.7z。64位的另外一套，但文件名称是一致的。

把这么一大堆dll放到应用程序目录也挺尴尬的（放入单独的\bin子目录可有所缓解）。微软推荐的方法是额外安装`vc_redist.exe`（却也是麻烦了一步）。很多开发者还是希望把**单个dll**运行库（如msvcrt.dll）放到应用目录的，方便部署。这个死微软佬，到了VC2015，非要把运行库拆分出几十个dll（其实合起来才几百KB [注1]），一开始甚至是拒绝提供App Local DLLs的，被抗议太多了才被迫妥协（详见上文链接）。观望VC2016（即VC15）的策略。

注1: 非压缩版1.75MB，zip压缩后911KB，7zip压缩后435KB。

VC 2015静态链接C运行库(/MT)编译的EXE/DLL，运行时不依赖任何C运行库。微软似乎并不太愿意推荐这种编译方式。









