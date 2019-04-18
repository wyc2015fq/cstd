# 小坑(用Element-ui) - weixin_33985507的博客 - CSDN博客
2018年05月16日 18:08:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
### 今天碰见了一个小问题，拿来分享一下：
- 前几天写了一个大文件上传的组件，文件传输时涉及到了文件是否在队列，文件名字是否相同，相同的情况下需要改名或者是直接覆盖或者是放弃上传这三种情况，检查名字时候也需要检查MD5是否相同，如果相同的情况下需要哪几种操作，检查完后还需要检查安全性，还需.........
- 我不想再说了真心麻烦，但考虑到vue可用的文件上传的组件很有限，同时功能很单一，并且传输大文件时，后台一般为了减轻服务器的压力，对每次上传的大小做了限制，所以我们要想通过一个请求将1G的文件传上去，想都别想！连100M都不给你机会，所以在前端的处理过程中我们需要将大文件进行分片连续发送多次请求。鉴这种极其无奈的情况下引入了webUploader，这个组件对于各种上传功能的实现和添加各种验证同时开放了在原型上添加方法和钩子的API，确实是挺不错的！但，但最可恶的是这是一个jq插件，万般无奈又把jQuery引进来。虽然jq引进来了，但只在封装这个组件的时候用也还好，不到100k，对于整个项目来说还可以接受。
- ***发现自己扯远了,有点文不对题了***，对于大文件用webUploader封好的组件，但今天我发现项目里还有一个简单的图片上传的功能，感觉没必要去引封装的组件，自己本身已经将element的上传upload组件引进来了，对于这种小文件的上传感觉用到就够了，但在写业务时，发现后台那边给的上传接口中必传的参数是文件的md5值，本身自带的upload并没有计算MD5的API,所以需要在文件上传之前before-upload方法上进行formdata.md5=md5,这时候坑就出现了，当我按照正常的逻辑写完代码后台返回的数据
![4804580-eba9c1a1643b41f7.jpg](https://upload-images.jianshu.io/upload_images/4804580-eba9c1a1643b41f7.jpg)
上传错误
![4804580-da84055a6e39d542.jpg](https://upload-images.jianshu.io/upload_images/4804580-da84055a6e39d542.jpg)
传的参数为啥没值
![4804580-012fcf1074c68d89.jpg](https://upload-images.jianshu.io/upload_images/4804580-012fcf1074c68d89.jpg)
vue-devtools中显示的data
我懵了，神马情况？看看代码
```
// 图片上传之前
      logoBeforeUpload(file) {
        const self = this
        // 计算md5并传给upload组件
        this.fileMd5Calculate(file, (md5) => {
          console.log(md5);
          debugger
          self.$set(self.postData, 'md5', md5)
        })
      }
```
感觉逻辑也没有写错呀，况且el-upload组件的props中md5确实已经有值了，这是肿么情况，打个断点吧，
![4804580-3ddedc40d91cd5bf.jpg](https://upload-images.jianshu.io/upload_images/4804580-3ddedc40d91cd5bf.jpg)
感觉找到错误了
我去，在beforeUpload中为啥我的逻辑还没走完，它就先给我触发上传了？？仔细看了一下代码，fileMd5Calculate计算文件md5是个耗时工程，且我赋给md5又是个异步，而element封装的upload的方法才不管你是不是异步，我该运行就运行，擦擦擦。。。
找到问题了，就把代码改一下
```
// 图片上传之前
      logoBeforeUpload(file) {
        const self = this
        // 当文件上传之前必须返回false或promise才能阻止文件上传，当我们对文件进行配置后，我们需要继续            
           运行所以通过promise的resolve
        return new Promise((resolve, reject) => {
          // 计算md5并传给upload组件
          self.fileMd5Calculate(file, (md5) => {
          console.log(md5);
          self.$set(self.postData, 'md5', md5)
          // 继续执行上传
          resolve()
        })
        })
      }
```
用个promise吧（也给自己大大的备注），当我们md5赋值的过程进行完成后我们再触发真正的上传操作，
这时候成功了
![4804580-b099289583b07309.jpg](https://upload-images.jianshu.io/upload_images/4804580-b099289583b07309.jpg)
成功了
