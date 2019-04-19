# goto - Ms.Fox的专栏 - CSDN博客
2015年01月15日 17:21:58[lulu-lu](https://me.csdn.net/smbluesky)阅读数：521
个人分类：[C++：注意事项](https://blog.csdn.net/smbluesky/article/category/2641013)
goto 的反对声音很多，但是也不能否认goto的各种优点
第一种情况：跳出多层嵌套的循环。
  由于break语句只影响包围它的最内层循环，如欲从深层循环跳出只有一种方法，那就是使用goto语句。
while（condition1）
{
> 
while（condition2）
{
> 
> 
while（condition3）
> 
{
> 
> 
> 
if（some disaster）
> 
> 
> 
> 
goto quit；
> 
> 
}
> 
}
}
quit：
……；
第二种情况： 将所有的异常处理以及资源释放过程都放到END_PROC处理中。可以更好的避免资源泄露和异常处理的错误问题，也方便定位。
……
while（）
{
> 
if（！FIndFile（fileList（fileInde），fileToPurge））
{
> 
> 
errorState=FileStatus_FileFindeError；
> 
goto END_PROC；
> 
}
if（！OpenFile（fileToPurge））
{
> 
> 
errorState=FileStatus_FileOpenError；
goto END_PROC；
}
}
END_PROC：
> 
deletePurgeFileList（fileList，numFilesToPurge）；
