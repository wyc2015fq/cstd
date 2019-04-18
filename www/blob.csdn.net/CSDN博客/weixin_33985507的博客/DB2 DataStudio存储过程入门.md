# DB2 DataStudio存储过程入门 - weixin_33985507的博客 - CSDN博客
2017年03月21日 14:20:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：53
#### 创建存储过程
- 
文件——》新建——》数据开发项目（输入项目名称）——》选择连接概要文件
![3832654-23e8871ad5227d25.png](https://upload-images.jianshu.io/upload_images/3832654-23e8871ad5227d25.png)
- 
在 Data Project Explorer界面找到新建的数据开发项目，并新建——》存储过程
![3832654-f2b9971f9a775fc9.png](https://upload-images.jianshu.io/upload_images/3832654-f2b9971f9a775fc9.png)
建好后 看到如下界面，图中DDL部分是选择的存储过程模板
![3832654-8af1a99c98da2f0b.png](https://upload-images.jianshu.io/upload_images/3832654-8af1a99c98da2f0b.png)
#### 实例
**场景：**
输入两个参数A和B，并求和
**DDL**
只需要按照模板，修改参数部分和主体部分即可
```
CREATE OR REPLACE PROCEDURE PROCEDURE1 (IN A INT,IN B INT, OUT SUM INT)
P1: BEGIN
SET SUM = A+B;
END P1
```
#### 部署和运行
DDL的空白处，鼠标右键——》部署    或者 单击 部署 按钮
![3832654-d475e16ab6265a85.png](https://upload-images.jianshu.io/upload_images/3832654-d475e16ab6265a85.png)
存储过程——》运行  或者 单击  运行  按钮  如图
![3832654-02707e330b644b2c.png](https://upload-images.jianshu.io/upload_images/3832654-02707e330b644b2c.png)
#### 结果
![3832654-1010e5a96d92b03a.png](https://upload-images.jianshu.io/upload_images/3832654-1010e5a96d92b03a.png)
