# SVN，HG，GIT命令对照 - 三少GG - CSDN博客
2013年01月04日 17:58:08[三少GG](https://me.csdn.net/scut1135)阅读数：1019
SVN，HG，GIT是三种常见的版本控制系统，本文简单列出一些相似的操作命令。
当然，三种系统各有特点，严格的对应关系是没有的。
## 1，第一次下载，包括源码和版本库：svn checkout http://path/to/repo repo_namehg clone http://path/to/repo  repo_namegit glone http://path/to/repo repo_name或者git glone git://path/to/repo repo_name2，下载服务器上最新的更新：svn updatehg pull && hg update -Cgit pull3，检出某个修订版本svn checkout -r <rev>hg update -C -r <rev>git reset --hard -r <rev>4，新增被跟踪文件svn add /path/to/filehg add /path/to/filegit add /path/to/file4，移除被跟踪文件svn rm /path/to/filehg remove /path/to/filegit rm /path/to/file5，生成补丁svn diff  >patch_filehg diff >patch_filegit diff >patch_file6，提交更改svn commithg commitgit commit6，查看当前状态svn infohg statusgit status7，查看修订记录svn loghg loggit log8，启动服务器svnserve -dhg serve -p 8002 &git daemon --base-path=/path/to/repo --export-all &
