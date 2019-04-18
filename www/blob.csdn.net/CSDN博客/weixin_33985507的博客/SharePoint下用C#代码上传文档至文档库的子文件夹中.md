# SharePoint下用C#代码上传文档至文档库的子文件夹中 - weixin_33985507的博客 - CSDN博客
2013年06月11日 18:21:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
> 
SharePoint的文档库非常方便，可以管理日常的文档，更强大的是可以创建文件夹来分类，通常在UI界面上是非常简单了，点击文件夹，Create a Folder即可。但是，怎样手动去创建文件夹呢，怎样在文件夹中创建子文件夹。
### 项目分析
- 举个栗子，我需要上传文档到文档库，希望把该文件上传到以年、月、日、上传人命名的子文件夹中，如：2013年-->5月-->11日-->陈小春，如下图所示这样
![](https://images0.cnblogs.com/blog/299214/201306/11181403-d1dd903ba6ae46119471f7c598a88043.png)
- 点击2013年，进入当前月子文件夹
![](https://images0.cnblogs.com/blog/299214/201306/11181445-47e3c583b30144a3821d8032cbe2f961.png)
- 点击当前月，进入以上传人命名的子文件夹
![](https://images0.cnblogs.com/blog/299214/201306/11181511-f78bdacde9d546b1acf803fc231d276e.png)
- 点击上传人（陈小春），即可看到我们上传的文档
![](https://images0.cnblogs.com/blog/299214/201306/11181600-c7d660e1a0054ca6b0d08c9befbb106a.png)
### 代码实现
- 首先我们完成对参数的设置，初始化参数
```
//获取年度参数
            string folderYearParameter = DateTime.Now.Year.ToString() + "年";
            //获取月度参数
            string folderMonthParameter = DateTime.Now.Month.ToString() + "月";
            //获取当前登录用户名：
            string currentLoginName = SPContext.Current.Web.CurrentUser.LoginName;
            currentLoginName = currentLoginName.Substring(currentLoginName.LastIndexOf("\\") + 1);
            string currentUserDisplayName;
            //获得Display name
            //当Administrator上传有问题 ?因为 当administrator登陆时 DisplayName=Wllgwh\administrator,invalid Path
            SPUser spUser = SPContext.Current.Web.AllUsers[SPContext.Current.Web.CurrentUser.LoginName];
            currentUserDisplayName = spUser.Name;
            if (currentUserDisplayName.Contains("\\"))
            {
                currentUserDisplayName = currentUserDisplayName.Substring(currentUserDisplayName.LastIndexOf("\\")+1);   
            }
```
- 提权
创建文件夹之前，判断是否存在该文件夹，若不存在，则创建之。之后创建Byte数组，将上传的文档附加到SubFolder中。
```
SPSecurity.RunWithElevatedPrivileges(delegate()
            {
                var homeUrl = SPContext.Current.Site.Url;
                SPSite site = new SPSite(homeUrl + "/jxkp");
                SPWeb web = site.OpenWeb();
                //获取List
                SPList list = web.Lists.TryGetList("绩效考评");
                SPFolder folderYear =
                list.ParentWeb.GetFolder(list.RootFolder.ServerRelativeUrl + "/" + folderYearParameter);
                if (!folderYear.Exists)
                {
                    SPListItem folder = list.Items.Add(list.RootFolder.ServerRelativeUrl, SPFileSystemObjectType.Folder, folderYearParameter);
                    web.AllowUnsafeUpdates = true;
                    folder.Update();
                    web.AllowUnsafeUpdates = false;
                }
                //判断folderMonth是否存在，不存在则创建
                SPFolder folderMon = list.ParentWeb.GetFolder(list.RootFolder.ServerRelativeUrl + "/" + folderYearParameter + "/" + folderMonthParameter);
                if (!folderMon.Exists)
                {
                    //创建子文件夹
                    SPListItem subFolder = list.AddItem(list.RootFolder.ServerRelativeUrl, SPFileSystemObjectType.Folder, folderYearParameter + "/" + folderMonthParameter);
                    web.AllowUnsafeUpdates = true;
                    subFolder.Update();
                    web.AllowUnsafeUpdates = false;
                }
                //判断CurrentlyUser是否存在，不存在则创建,这么长的一串，之后重构
                SPFolder folderCurrUser = list.ParentWeb.GetFolder(list.RootFolder.ServerRelativeUrl + "/" + folderYearParameter + "/" + folderMonthParameter + "/" + currentUserDisplayName);
                if (!folderCurrUser.Exists)
                {
                    SPListItem userFolder = list.AddItem(list.RootFolder.ServerRelativeUrl, SPFileSystemObjectType.Folder, folderYearParameter + "/" + folderMonthParameter + "/" + currentUserDisplayName);
                    web.AllowUnsafeUpdates = true;
                    userFolder.Update();
                    web.AllowUnsafeUpdates = false;
                }                          
                if (fileUploadJxkp.PostedFile != null)
                {
                    if (fileUploadJxkp.PostedFile.ContentLength > 0)
                    {
                        System.IO.Stream stream = fileUploadJxkp.PostedFile.InputStream;
                        byte[] bytFile = new byte[Convert.ToInt32(fileUploadJxkp.PostedFile.ContentLength)];
                        stream.Read(bytFile, 0, Convert.ToInt32(fileUploadJxkp.PostedFile.ContentLength));
                        stream.Close();
                        SPFolder subSPFolder =
                       list.ParentWeb.GetFolder(list.RootFolder.ServerRelativeUrl + "/" + folderYearParameter + "/" +
                                                 folderMonthParameter + "/" + currentUserDisplayName);
                        web.AllowUnsafeUpdates = true;
                        subSPFolder.Files.Add(System.IO.Path.GetFileName(fileUploadJxkp.PostedFile.FileName),
                                              bytFile, true);
                        web.AllowUnsafeUpdates = false;
                       // labMsg.Text = "绩效考评已上传成功";
                        string successScript = string.Format("$.jnotify('{0}');", "上传成功");
                        ScriptManager.RegisterStartupScript(this.Page, this.GetType(), "smManager", successScript, true);
                    }
                }
            });
```
### 小结
> 
**SharePoint的文档库归根到底还是List,学习SharePoint是条漫长的路，吾将上下而求索，有距，无惧。**
