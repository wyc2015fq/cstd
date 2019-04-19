# Mac OSX下Sublime Text配置使用ctags实现代码跳转 - 三少GG - CSDN博客
2017年05月07日 16:34:42[三少GG](https://me.csdn.net/scut1135)阅读数：2789
[https://www.smslit.top/2015/11/14/macSTctags-Develop/](https://www.smslit.top/2015/11/14/macSTctags-Develop/)
### 配置ST的CTags插件
需要配置一下CTags插件，更改一下默认配置，主要修改其中ctags目录，因为配置中没有写入ctags的目录。
- 
打开 菜单栏->Sublime Text->Preferences->Package Settings->CTags->Settings-Default/Settings-User这两个文件：
![7](https://www.smslit.top/assets/post/Develop/macSTctags/7.png)
- 
将Settings-Default中的内容全选复制到Settings-User文件中，并将其中**Command**的值设置为：**/usr/local/bin/ctags**:
![8](https://www.smslit.top/assets/post/Develop/macSTctags/8.png)
### [](https://www.smslit.top/2015/11/14/macSTctags-Develop/#%E6%B5%8B%E8%AF%95)测试
随便找到一个含有c代码的工程目录，用ST打开，我打开的是我的一个BLE开发的工程**DJB001_150509_1700V1.14**。
- 
右击ST中左边栏的文件夹，出现**CTags：Rebuild Tags**，点击会生成tags文件（这个文件就是函数、变量、宏定义等的 索引文件）：
![9](https://www.smslit.top/assets/post/Develop/macSTctags/9.png)
- 
打开工程文件的DJB001_150509_1700V1.4->Projects->ble->CC2541F128->Source->OSAL*SimpleBLEPeripheral.c文件，*尝试跳转，按下**shift+ctrl**，鼠标左键点击**SimpleBLEPeripheral_Init(taskID++)** ：
![10](https://www.smslit.top/assets/post/Develop/macSTctags/10.png)
- 
就会调转到**SimpleBLEPeripheral_Init**d的定义：
![11](https://www.smslit.top/assets/post/Develop/macSTctags/11.png)
- 
然后**Shift+Ctrl+鼠标右键单击**，便会调回刚才的位置。
可以在菜单栏**Sublime Text->Preferences->Package Settings->CTags**下找到快捷键和鼠标操作的设定，根据自己喜欢在用户设置文件里更改就可以。
