
# 利用posix_fadvise清理系统中的文件缓存 - 高科的专栏 - CSDN博客

2012年08月24日 20:49:56[高科](https://me.csdn.net/pbymw8iwm)阅读数：5120


[利用posix_fadvise清理系统中的文件缓存](http://leoncom.org/?p=650719)
leoncom[c/c++](http://leoncom.org/?cat=4),[unix](http://leoncom.org/?cat=5)2011-08-03
当我们需要对某段读写文件并进行处理的程序进行性能测试时，文件会被系统cache住从而影响I/O的效率，必须清理cache中的对应文件的才能正确的进行性能测试。通常清理内存可以采用下面的这条命令，但这条命令只有root才能使用，另外一方面这个会清理所有的cache，也许会影响其他程序的性能。
echo 3>/proc/sys/vm/drop_caches
linux下有一个posix_fadvise函数可以用来对cache中的文件进行清理，有关posix_fadvise的详细说明查看man手册。
[view plain](http://leoncom.org/?p=650719#)[copy to clipboard](http://leoncom.org/?p=650719#)[print](http://leoncom.org/?p=650719#)[?](http://leoncom.org/?p=650719#)
intposix_fadvise(intfd, off_t offset, off_t len,intadvice);
”Programs  can  use  posix_fadvise  to  announce an intention to access file data in a specific pattern in the future, thus allowing the kernel to perform appropriate optimisations”
fd是文件的描述符，用于清理对应文件cache的advice值选取POSIX_FADV_DONTNEED，利用此函数编写下面程序进行文件的清理。
[view plain](http://leoncom.org/?p=650719#)[copy to clipboard](http://leoncom.org/?p=650719#)[print](http://leoncom.org/?p=650719#)[?](http://leoncom.org/?p=650719#)
intclear_file_cache(constchar*filename)
{
structstat st;
if(stat(filename , &st) < 0) {
fprintf(stderr ,"stat localfile failed, path:%s\n",filename);
return-1;
}
intfd = open(filename, O_RDONLY);
if( fd < 0 ) {
fprintf(stderr ,"open localfile failed, path:%s\n",filename);
return-1;
}
//clear cache by posix_fadvise
if( posix_fadvise(fd,0,st.st_size,POSIX_FADV_DONTNEED) != 0) {
printf("Cache FADV_DONTNEED failed, %s\n",strerror(errno));
}
else{
printf("Cache FADV_DONTNEED done\n");
}
return0;
}
此外，[linux-ftools](http://code.google.com/p/linux-ftools/)这个工具也可以帮助清理并查看文件的内存状态，主页上也有详细的使用说明。编译后我们利用fincore这个工具来查看文件在内存中的状态，有关fincore的实现可以在linux下man mincore，mincore是根据缓存buffer指针来其指向的缓冲区判断在cache中的状态，fincore就是在mincore的基础上直接操作文件，就是通过对文件mmap获得指针，再调用mincore来判断。
首先我们通过cp命令拷贝了一个相对有点容量的文件，然后利用fincore查看文件在内存中的cache情况，可以看到大概cache了99.55%。
![image](http://leoncom.org/wp-content/uploads/2011/08/image_thumb.png)
接着执行上面那段代码的运行程序，之后再执行命令查看该文件的缓存状态
![image](http://leoncom.org/wp-content/uploads/2011/08/image_thumb1.png)
可以看到，该文件在内存中已经没有被cache了。实际的清理效果也可以通过一些占用I/O的读文件程序来测试，一般程序第二遍运行时候由于文件已经被cache，实际程序运行的速度会比较快，通过上面的posix_fadivse清理之后，又会恢复和第一遍差不多的时间。


