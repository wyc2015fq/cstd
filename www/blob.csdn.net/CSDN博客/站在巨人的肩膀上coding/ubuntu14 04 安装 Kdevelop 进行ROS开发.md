# ubuntu14.04 安装 Kdevelop 进行ROS开发 - 站在巨人的肩膀上coding - CSDN博客





2018年04月25日 09:18:50[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：354








**1. 安装gcc**

sudo apt-get build-dep gcc
sudo apt-get install build-essential


** 2. 安装Kdevelop**

sudo apt-get install Kdevelop


** 3. 安装cmake等**

sudo apt-get install automake autoconf g++ libtool cmake 


** 4. 配置(参考ros-wiki)**[http://wiki.ros.org/IDEs](http://wiki.ros.org/IDEs)

cd ~/Desktop
touch kDevelop.desktop
chmod +x kDevelop.desktop


sudo gedit kDevelop.desktop


 添加如下文本：


```
[Desktop Entry]
Type=Application
Terminal=false
Exec=bash -i -c "kdevelop"
Name=kDevelop
Icon=kdevelop
```


 保存退出，桌面就出现了kDevelop的图标啦

**5. 创建catkin软件包**

**5.1 导入catkin顶层工作空间**

      首先清楚掉catkin_ws文件夹下的build文件夹（下次编译的时候它会再次出现）

      打开kDevelop，使用shell 或者桌面图标均可

      点击“工程”->"打开工程" 找到catkin_ws/src 下的 CMakelists.txt,选中，点击“Next”, 将工程名称修改为自己的（默认为src）

      选择“构建系统”为 "CMake Project Manager"

      "Finish”

      出现“配置构建目录”窗口

      将“构建目录”改为 /home/user/catkin_ws/build/    （起初删除的那个）

      选择合适的构建类型 “Debug/Release”

      额外参数

-DCATKIN_DEVEL_PREFIX=../../devel -DCMAKE_INSTALL_PREFIX=../../install


      注意：对于嵌套的子目录结构，必须添加适当数量的 ../ 

**6. 运行调试自己的可执行程序**

    点击“代码右侧向下箭头”->"调试启动  F9"    进行  “启动配置”

    点击“Add New...”   选择要调试的可执行文件  e.g."~/catkin_ws/devel/lib/package/node_name"

    "OK"

    点击 “Execute” or "Debug" 即可

**7. 注意事项**

    如果上述过程中出现了问题，可以将 "catkin_ws/src./"  下的  “*.kdve4” 文件删除，然后重复上述过程即可。

The "Build Type" selected during project import (e.g. "Debug", "Release", "[RelWithDebInfo](http://wiki.ros.org/RelWithDebInfo)") can be changed by right-clicking on the project -> "Open Configuration...".
 Select CMake in the left menu and change the CMake variable "CMAKE_BUILD_TYPE" appropriately.


**Note**, if a package inside the catkin workspace specifies its own "Build Type", e.g. by adding
set(CMAKE_BUILD_TYPE Release) to the underlying package "CMakeLists.txt", it will be used for that package instead of the global one defined in the top-level CMake project.


You may set up a project filter since kDevelop displays a lot of files and (ros) binaries in the source tree that are not really interesting for the developer and finding individual files could be really confusing.
Right click on the project and select "Open Configuration...".
Choose "Project Filter" in the left menu. 
The cleanest solution is to first exclude all files and then include only desired file types:

- 
Click Add. Type in the pattern: "*" (just the star character) and select as "Target" only
**Files**. Choose "Action" **Exclude**. 

- Now add all files you want include into the project tree:

Click "Add" and type in your desired file pattern. Choose "Action"
**Include**. Suggested file patterns are: ".h" ".cpp" ".hpp ".c" ".ipp" ".c" ".txt" ".launch" ".xml" ".md" ".cmake" ".cfg" ".py" ".yaml" ".urdf" ".sdf" ".xacro"








