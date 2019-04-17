# svn 设置文件可执行权限 - DoubleLi - 博客园






本地文件在commit到仓库之前若没有chmod +x 权限的话，那在svn仓库里的文件将会保持当前无可执行属性状态。

即使在本地chmod +x filename 之后，再提交到仓库也是没有用的。check out到其他目录的时候，文件仍然没有可执行权限。

需要使用svn 设置文件属性，如下

linux：

svn propset svn:executable on *

svn commit -m "" Hiviewxxx

windows：

右键 - > propeties ->new  ->executable.

commit上去即可









