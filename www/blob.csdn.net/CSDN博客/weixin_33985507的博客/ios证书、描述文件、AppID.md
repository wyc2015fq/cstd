# ios证书、描述文件、AppID - weixin_33985507的博客 - CSDN博客
2017年01月09日 17:59:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：25
# AppID
应用程序的包名（Bundle Identifier）。应用的唯一标识，在你的开发者中心创建，不能重复。
### 创建AppID
![1243802-13217f71f1872fde.png](https://upload-images.jianshu.io/upload_images/1243802-13217f71f1872fde.png)
创建appId
![1243802-a4077bb51536b343.png](https://upload-images.jianshu.io/upload_images/1243802-a4077bb51536b343.png)
描述信息
![1243802-6b492d4fb796b970.png](https://upload-images.jianshu.io/upload_images/1243802-6b492d4fb796b970.png)
输入你的bundleId创建即可
# 证书（Certificate）
mac生成csr文件，上传csr文件到开发者中心生成证书，下载生成的证书并导入mac使用。
但并不是所有的mac都可以下载并导入开发者中心的证书并使用。
只有创建该证书的mac才可以直接使用，其它mac如果需要导入证书，需要先从创建证书csr文件的mac上导出p12文件，再导入使用。
![1243802-025a557958fb1107.png](https://upload-images.jianshu.io/upload_images/1243802-025a557958fb1107.png)
流程图
### 证书的创建
打开钥匙串->证书助理->从证书颁发机构请求证书
输入用户电子邮件地址，选择存储到磁盘，然后继续即可生成csr文件。
![1243802-40c78772be7c2d2c.png](https://upload-images.jianshu.io/upload_images/1243802-40c78772be7c2d2c.png)
创建csr文件
选择证书类型，然后继续下一步。
![1243802-333695f9361a96c1.png](https://upload-images.jianshu.io/upload_images/1243802-333695f9361a96c1.png)
选择证书类型
选择刚才创建的csr文件并生成证书，这样证书的创建就完成了。
找到你生成的证书，下载，然后双击即可导入到mac的钥匙串中使用。
![1243802-7ed89ec62eb8f39f.png](https://upload-images.jianshu.io/upload_images/1243802-7ed89ec62eb8f39f.png)
选择csr文件
# mac导出p12文件
打开钥匙串，选中你要导出的证书，右键导出，注意此时不要展开证书。
然后输入密码即可完成导出。生成的p12文件即可在其它mac上使用。
![1243802-168a42c39c77e74e.png](https://upload-images.jianshu.io/upload_images/1243802-168a42c39c77e74e.png)
选择证书
# 描述文件（profile）
是xcode在最后生成应用程序的时候打包所用的文件。
它是由AppId、证书、测试硬件设备uuid一起创建生成的文件。
![1243802-b20b173e28071ab4.png](https://upload-images.jianshu.io/upload_images/1243802-b20b173e28071ab4.png)
关系图
### 创建描述文件
![1243802-af768d6ee4a2d9da.png](https://upload-images.jianshu.io/upload_images/1243802-af768d6ee4a2d9da.png)
点击创建
![1243802-67da6b05c5a51d7a.png](https://upload-images.jianshu.io/upload_images/1243802-67da6b05c5a51d7a.png)
选择类型
![1243802-08947757bba6701d.png](https://upload-images.jianshu.io/upload_images/1243802-08947757bba6701d.png)
选择你的appId
![1243802-a1ea5826d0f483d5.png](https://upload-images.jianshu.io/upload_images/1243802-a1ea5826d0f483d5.png)
选择你的证书
![1243802-fe55747b46a06e2f.png](https://upload-images.jianshu.io/upload_images/1243802-fe55747b46a06e2f.png)
选择测试设备
![1243802-f65511192f5b4f41.png](https://upload-images.jianshu.io/upload_images/1243802-f65511192f5b4f41.png)
输入文件名字，生成描述文件
