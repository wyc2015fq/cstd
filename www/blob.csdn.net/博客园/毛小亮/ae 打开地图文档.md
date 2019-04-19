# ae 打开地图文档 - 毛小亮 - 博客园
# [ae 打开地图文档](https://www.cnblogs.com/xianerwonder/p/4325410.html)
```csharp;gutter
if (openMxdDialog.ShowDialog() == DialogResult.OK)
            {
                pathMXD = openMxdDialog.FileName;
                
                if (pathMXD != null)
                {
                    if (axMapC.CheckMxFile(pathMXD))
                    {
                        threadProgress = new Thread
                            (new ParameterizedThreadStart(showProgress));
                        threadProgress.Start();
                        axMapC.LoadMxFile(pathMXD);
                    }
                }
            }
```
