
# Unity读取Android SDcard文件 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[Unity读取Android SDcard文件](https://www.cnblogs.com/eniac1946/p/7519798.html)
|Posted on|2017-09-14 11:37|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7519798)|[收藏](#)
|一、添加权限
|权限添加 ：Player settings -- Other settings -- write permission的设置 Sdcard。这个是在Unity编辑器里打包的情况。
|如果导出到studio 里面的话，可自行修改Manifest文件。
![](http://img.blog.csdn.net/20170524162532939)
|二、两种方式
|IO方式 加载sdcard上的图片资源
|加载的  /storage/emulated/0/ProjectName/image.jpg,
|image = this.GetComponentInChildren<Image>();  
  
      Debug.Log("IO加载用时: image = this.GetComponent<Image> ==========  " + image);  
  
      Debug.Log("IO加载:  Application.dataPath "  + Application.dataPath );  
      // Application.dataPath /data/app/com.putao.ptx.core-1/base.apk  
  
      Debug.Log("IO加载:  Application.persistentDataPath " + Application.persistentDataPath);  
      // Application.persistentDataPath /storage/emulated/0/Android/data/com.putao.ptx.core/files   
  
  
      Debug.Log("IO加载:GameObject.Find" + GameObject.Find("Canvas/Avator").GetComponent<Image>());  
  
       // /data/user/0/com.putao.paichallenge/cache/20170524_130527.jpg  
  
  
      // path =======  "/storage/emulated/0/ProjectName/image.jpg"  
      FileStream fileStream = new FileStream(path, FileMode.Open, FileAccess.Read);  
      fileStream.Seek(0, SeekOrigin.Begin);  
      //创建文件长度缓冲区  
      byte[] bytes = new byte[fileStream.Length];  
      //读取文件  
      fileStream.Read(bytes, 0, (int)fileStream.Length);  
      //释放文件读取流  
      fileStream.Close();  
      fileStream.Dispose();  
      fileStream = null;  
  
      //创建Texture  
      int width = 300;  
      int height = 372;  
      Texture2D texture = new Texture2D(width, height);  
      texture.LoadImage(bytes);  
  
  
      //创建Sprite       
      Sprite sprite = Sprite.Create(texture, new Rect(0, 0, texture.width,texture.height), new Vector2(0.5f, 0.5f));  
  
      image.sprite = sprite;
|log上可以看出来  android上 getExternalFilesDir(|""|) 和 Unity里面的Application.persistentDataPath是一致的
|/storage/emulated/0/Android/data/com.putao.ptx.core/files
|WWW方式加载本地图片
|url=  "file://"+ "/storage/emulated/0/ProjectName/image.jpg"
|根本不是加"jar:file://..."
|public void LoadByWWW(String path)  
    {  
        StartCoroutine(doLoadByWWW(path));  
  
    }  
  
    IEnumerator doLoadByWWW(String path)  
    {  
        string url = "file://" + "/storage/emulated/0/PaiChallenge/image.jpg";  
        Debug.Log("doLoadByWWW == url ==================   " + url);  
  
        WWW w = new WWW(url);  
  
        yield return w;  
  
        if (w.isDone)  
        {  
            Sprite sprite = Sprite.Create(w.texture, new Rect(0, 0, w.texture.width, w.texture.height), new Vector2(0.5f, 0.5f));  
  
            GameObject.Find("Canvas/Avator").GetComponent<Image>().sprite = sprite;  
  
        }








|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
