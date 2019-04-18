# ②Linux创建、移动、删除文件夹 - weixin_33985507的博客 - CSDN博客
2019年01月14日 16:26:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
本人是linux菜鸡，这里记录的是平时用的比较多的功能和命令，比较复杂的option并没有记录下来。
### 1. 创建文件夹：mkdir
【用法】mkdir path_to_filename
【例子】mkdir /Users/shinianyike/desktop/new_file
这时在/Users/shinianyike/desktop/目录下就创建了new_file这个文件夹/目录.
【记忆】mkdir = make directory
### 2. 移动文件或重命名：mv
【用法】mv OLD_path_to_file NEW_path_to_file
【例子】**移动文件** mv ./new_file/test.txt ./123/test.txt
这时test.txt就从new_file这个文件夹中移动到了123这个文件夹当中。如果在123这个文件夹中已经存在了test.txt这个文件，那么将会被覆盖掉。./ 表示当前目录。
需要注意的是前后的test.txt文件名是一致的，若不一致的话就会将文件重命名，看下面例子。
【例子】**重命名** mv ./new_file/test.txt ./new_file/renamefile.txt
路径是不变的，只是文件名从test.txt改成了renamefile.txt，这时候文件就被重命名了。
【记忆】mv = move
### 3. 删除：rm
【用法】rm filename
【例子】删除文件 rm ./new_file/test.txt
这时test.txt这个文件被删除了。若要**删除整个文件夹的内容**，则要加上-r：
【例子】删除文件夹 rm -r ./new_file
这时候连同new_file这个文件夹一起，这个路径下所有的文件也被删除。
【记忆】rm = remove
