
# oracle的基本概念 - 李昆鹏的博客 - CSDN博客


2018年05月21日 15:26:07[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：54


----------------------------------------------oracle的基本概念------------------------------------------------
**Mysql的概念**
![](https://img-blog.csdn.net/20180521152340963?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**Oracle的基本概念**
![](https://img-blog.csdn.net/20180521152352729?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
|Oracle数据库：是一个文件系统，是物理概念。
|实例：在Oracle的数据库中可有多个实例，通常我们只用一个实例
|用户：一个实例下有多个用户。
|表空间：一个实例下有多个表空间，表空间是逻辑概念，一个表空间对应着一个或者多个物理存储文件（.dbf,.ora）
|用户和表空间的关系：一个用户有一个默认的表空间，一个表空间可以为多个用户作为默认表空间，用户和用户之间的数据是隔离，数据不会混。
|Oracle数据库源：
|driver:    oracle.jdbc.OracleDriver
|url:         jdbc:oracle:thin:@localhost:1521:orcl
|username:   zhangsan
|password:   zhangsan
|oracle 中用户的概念相当于mysql中的database的概念
|
Ø**数据库：**
Oracle数据库是数据的物理存储。这就包括（数据文件ORA或者DBF、控制文件、联机日志、参数文件）。其实Oracle数据库的概念和其它数据库不一样，这里的数据库是一个操作系统只有一个库。可以看作是Oracle就只有一个大数据库。
Ø**实例：**
一个Oracle实例（Oracle Instance）有一系列的后台进程（Backguound Processes)和内存结构（MemoryStructures)组成。一个数据库可以有n个实例。
Ø**用户：**
用户是在实例下建立的。不同实例可以建相同名字的用户。
Ø**表空间：**
表空间是Oracle对物理数据库上相关数据文件（ORA或者DBF文件）的逻辑映射。一个数据库在逻辑上被划分成一到若干个表空间，每个表空间包含了在逻辑上相关联的一组结构。每个数据库至少有一个表空间(称之为system表空间)。

每个表空间由同一磁盘上的一个或多个文件组成，这些文件叫数据文件(datafile)。一个数据文件只能属于一个表空间。
![](https://img-blog.csdn.net/20180521152522764?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Ø**数据文件（dbf、ora）:**
数据文件是数据库的物理存储单位。数据库的数据是存储在表空间中的，真正是在某一个或者多个数据文件中。而一个表空间可以由一个或多个数据文件组成，一个数据文件只能属于一个表空间。一旦数据文件被加入到某个表空间后，就不能删除这个文件，如果要删除某个数据文件，只能删除其所属于的表空间才行。
注：表的数据，是有用户放入某一个表空间的，而这个表空间会随机把这些表数据放到一个或者多个数据文件中。
由于oracle的数据库不是普通的概念，oracle是有用户和表空间对数据进行管理和存放的。但是表不是有表空间去查询的，而是由用户去查的。因为不同用户可以在同一个表空间建立同一个名字的表！这里区分就是用户了！
![](https://img-blog.csdn.net/2018052115253710?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

