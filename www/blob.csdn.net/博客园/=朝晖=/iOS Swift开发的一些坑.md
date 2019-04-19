# iOS Swift开发的一些坑 - =朝晖= - 博客园
# [iOS Swift开发的一些坑](https://www.cnblogs.com/dhcn/p/7131095.html)
      0、人难招，特别是对于没钱的小团队，大多数的[iOS](http://lib.csdn.net/base/ios)开发者没有[Swift](http://lib.csdn.net/base/swift)经验，有经验的也并不是很深入
      0.1、语言unwrap坑，虽然有自动修正提示，但感觉代码过程不流畅。 
      1、Realm的缺憾： 最近filter用得多，发现realm的一个真正的毛病，其查询语法关键字和结构和常规标准SQL完全不一致，这真是shit，必须认真看filter部分的文档并多次尝试才能找到真正正确的查询语句结构
     2、UIButton的远程图像获取的坑,尝试了很多种方法，最后发现得给他预设一个图像，远程图像加载才能成功，真是一个好坑啊,[Android](http://lib.csdn.net/base/android)里面好像也见过类似问题。
     3、Alamosfire的文件上传，参考：https://stackoverflow.com/questions/26121827/uploading-file-with-parameters-using-alamofire
- // import Alamofire  
- func uploadWithAlamofire() {  
-   let image = UIImage(named: "bodrum")!  
- 
-   // define parameters  
-   let parameters = [  
-     "hometown": "yalikavak",  
-     "living": "istanbul"  
-   ]  
- 
-   Alamofire.upload(multipartFormData: { multipartFormData in  
-     if let imageData = UIImageJPEGRepresentation(image, 1) {  
-       multipartFormData.append(imageData, withName: "file", fileName: "file.png", mimeType: "image/png")  
-     }  
- 
-     for (key, value) in parameters {  
-       multipartFormData.append((value?.data(using: .utf8))!, withName: key)  
-     }}, to: "upload_url", method: .post, headers: ["Authorization": "auth_token"],  
-         encodingCompletion: { encodingResult in  
-           switch encodingResult {  
-           case .success(let upload, _, _):  
-             upload.response { [weak self] response in  
-               guard let strongSelf = self else {  
-                 return  
-               }  
-               debugPrint(response)  
-             }  
-           case .failure(let encodingError):  
-             print("error:\(encodingError)")  
-           }  
-   })  
- }  
    4、NavigationViewController的TabBarViewController的内嵌ViewController设置其NavigationButtionItem，请注意，**这种情况是因为TabBarVC是NavigationVC的栈顶VC，所以操作它的NavagationItem**
- let b = UIBarButtonItem( image:#imageLiteral(resourceName: "post_message"), style:.plain, target:self, action:#selector(CommunityViewController.postClick(_:)))  
- 
- 
- elf.tabBarController?.navigationItem.rightBarButtonItem = b  

