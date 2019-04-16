# BREW应用间通信之共享文件 - 我相信...... - CSDN博客





2011年03月17日 13:51:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：924








上面讲到，IFIFO实际上就是一个文件，而且共享文件，即共享驻留数据也是BREW应用间通信最简单的一种方式。BREW中有三种驻留数据的方式：系统存储，文件和数据库。

系统存储是指通过ISHELL_SetPrefs()和ISHELL_GetPrefs()来实现驻留数据的方法。ISHELL_SetPrefs()和ISHELL_GetPrefs()提供了一种方法来保存包含或读取应用或类级别首地址的数据结构，相当于VC里面的串行化，使用起来非常简便，数据由整个AEE层共享。

通常在手机上还存在shared这样一个共享目录，在shared目录下的所有文件可以被任何应用所访问（应用至少拥有FILE权限）。所以，约定好一个文件名，两个或多个应用就可以通过共同访问该文件进行通信了。Const文件可以允许OEM 减少RAM 的负荷，一般不能对Const文件做删改，IFILE_Map() 可以用来将Const文件映射到内存，因此Const文件很少用来做通信的载体。IFILEMGR_GetInfo()和 IFILEMGR_GetInfoEx() 支持的文件大小不能超过4GB（BREW3.x之前的版本），文件名的长度也是有限制的，通过ISHELL_GetDeviceInfo(pIShell, pi)， pi->wMaxPath 可以得到文件名包括路径的最大长度.

IDATABASE是一个数据库接口，多个应用可以共享同一个数据库。Idatabase可以创建和访问由 IDBMgr创建并打开的数据库中的记录，调用 IDBMGR_OpenDatabase() 打开所需的数据库可以获取 Idatabase 接口的。另外，还可以使用 IDBRecord中的函数来访问各个数据库记录的字段。应用程序的权限级别必须是“文件”或“全部”，才能调用此接口中可修改数据库内容的函数。

IDATABASE_CreateRecord() 函数创建新的记录并将其添加到数据库中（IDBRECORD_Remove()用于删除数据库中的记录）。每个记录可包含一个或多个字段，每个字段由 AEEDBField 结构定义，创建建记录时，先要填写 AEEDBField 结构数组，来指定记录中每个字段的名称、类型、内容和长度。然后，调用 IDATABASE_CreateRecord()并提供此数组的指针和要包含的 AEEDBField 结构数作为输入参数。 IDATABASE_CreateRecord() 将返回指向 IDBRecord 接口实例的指针，可用于访问和更新记录的字段。创建或更新记录时，会为每个记录指定字段数和字段类型；并不要求给定数据库中的全部记录都具有相同的结构。创建数据库并添加记录后，即可使用其他 Idatabase 函数从数据库中检索记录。



本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/wireless_com/archive/2010/09/29/5914161.aspx](http://blog.csdn.net/wireless_com/archive/2010/09/29/5914161.aspx)



