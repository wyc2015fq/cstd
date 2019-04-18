# RxPermission 用法 - weixin_33985507的博客 - CSDN博客
2018年07月26日 18:36:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
#### （1）添加依赖
```
compile 'com.tbruyelle.rxpermissions2:rxpermissions:0.9.5@aar' 
compile 'io.reactivex.rxjava2:rxjava:2.0.1' 
compile 'io.reactivex.rxjava2:rxandroid:2.0.1'
```
用法：
```
RxPermissions rxPermissions = new RxPermissions(activity);
    rxPermissions.requestEach(permissions)
            .subscribe(new Consumer<Permission>() {
                @Override
                public void accept(Permission permission) throws Exception {
                    if (permission.granted) {
                        // 用户已经同意该权限
                        //result.agree(permission);
                    } else if (permission.shouldShowRequestPermissionRationale) {
                        // 用户拒绝了该权限，没有选中『不再询问』（Never ask again）,那么下次再次启动时，还会提示请求权限的对话框
                        //result.refuse(permission);
                    } else {
                        // 用户拒绝了该权限，并且选中『不再询问』，提醒用户手动打开权限
                        //result.noMoreQuestions(permission);
                    }
                }
            });
```
requestEach后面的形参是可变长度参数列表， 可以写一个权限， 也可以写多个权限并用逗号隔开，也可以写String数组。
