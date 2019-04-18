# SVN（Cornerstone）设置及使用说明 - weixin_33985507的博客 - CSDN博客
2018年08月24日 15:09:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
步骤一、首先添加现有的SVN 仓库（使用的素材，存储在仓库内，链接后可以下载 或 同步到本地）
1.点击 Add Repositary
![6922949-1ab94b4df3a09396](https://upload-images.jianshu.io/upload_images/6922949-1ab94b4df3a09396)
2.添加SVN地址、设置名称、添加账号、密码，然后点击“Add”
有cloud service 、file Repository、HTTP Server、SVN Server；我们用的是HTTP Server，输入地址，账户信息即可；
![6922949-a45cb86b751d10cf](https://upload-images.jianshu.io/upload_images/6922949-a45cb86b751d10cf)
3.添加成功后，出现SVN仓库内已有的内容（右侧的文件）
![6922949-8d57040cf2d029c9](https://upload-images.jianshu.io/upload_images/6922949-8d57040cf2d029c9)
步骤二、本地建立同步文件夹，和SVN仓库同步内容
1.在Cornerstone中，选中刚才链接的SVN仓库，右键菜单，选择“check out working coopy…”，签出仓库到本地
![6922949-a9d0aedb905075a2](https://upload-images.jianshu.io/upload_images/6922949-a9d0aedb905075a2)
3.设置要同步到本地的位置，然后点击“check out”
![6922949-9caffa6c1c8a974c](https://upload-images.jianshu.io/upload_images/6922949-9caffa6c1c8a974c)
4.本地仓库开始同步资料（即下载，内网服务器相对比较快），同步成功后和云盘内容会保持同步。
4.1同步中
![6922949-22027a639fdb3bf8](https://upload-images.jianshu.io/upload_images/6922949-22027a639fdb3bf8)
4.1同步成功后，Cornerstone中展示出本地的仓库文件夹。此时本地文件已经和SVN仓库保持一致了。
![6922949-97dc835e48d947fa](https://upload-images.jianshu.io/upload_images/6922949-97dc835e48d947fa)
步骤三：SVN仓库（服务器）同步本地文件
1.本地新增文件后
2.切换到Cornerstone，Cornerstone内显示新增文件内容
![6922949-d408350c0cfd2dc4](https://upload-images.jianshu.io/upload_images/6922949-d408350c0cfd2dc4)
图中标识了常用的Changed,Modified,Conflicted,Missing,Unversioned; 基本上可以理解有标识符提示的文件都是未同步的内容
![6922949-dcc5487c8875aa2d](https://upload-images.jianshu.io/upload_images/6922949-dcc5487c8875aa2d)
3.选中要同步的内容，右键菜单，点击“COMMIT CHANGER”，同步更新内容
![6922949-6059861e680ffc8d](https://upload-images.jianshu.io/upload_images/6922949-6059861e680ffc8d)
![6922949-f864261cbb07f311](https://upload-images.jianshu.io/upload_images/6922949-f864261cbb07f311)
![6922949-9d1176be0b1b1001](https://upload-images.jianshu.io/upload_images/6922949-9d1176be0b1b1001)
![6922949-d25e15268849d7a3](https://upload-images.jianshu.io/upload_images/6922949-d25e15268849d7a3)
按照以上操作，可同步新增、删除、修改的内容。
步骤四、本地同步SVN仓库内容（例如：SVN仓库内删除了新建的文件夹；同步后，本地仓库才能和SVN仓库一致)
1.右键菜单点击 “Update to last Revition”
![6922949-ef93b92adcbff530](https://upload-images.jianshu.io/upload_images/6922949-ef93b92adcbff530)
2.开始同步SVN仓库的数据（变更内容）
![6922949-ee4c4d74243c3064](https://upload-images.jianshu.io/upload_images/6922949-ee4c4d74243c3064)
3.文件同步成功
![6922949-e4cd3003d3c0bd83](https://upload-images.jianshu.io/upload_images/6922949-e4cd3003d3c0bd83)
