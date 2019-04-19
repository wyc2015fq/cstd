# 封装Nvelocity的渲染方法 - 毛小亮 - 博客园
# [封装Nvelocity的渲染方法](https://www.cnblogs.com/xianerwonder/p/4296327.html)
```
public class CommonHelper
    {
        /// <summary>
        /// 用data数据填充templatename模板，渲染返回html返回
        /// </summary>
        /// <param name="templatename"></param>
        /// <param name="data"></param>
        /// <returns></returns>
        public static string RenderHtml(string templatename,object data)
        {
            
            VelocityEngine vltEngine = new VelocityEngine();
            vltEngine.SetProperty(RuntimeConstants.RESOURCE_LOADER, "file");
            vltEngine.SetProperty(RuntimeConstants.FILE_RESOURCE_LOADER_PATH, System.Web.Hosting.HostingEnvironment.MapPath("~/templates"));//模板所在路径
            vltEngine.Init();
            var p = new { Title = "特大喜讯", Author = "毛健", PostDate = "2015-02-06", Msg = "今夜你要尿炕~" };
            VelocityContext vltContext = new VelocityContext();
            vltContext.Put("Data", data);//只放一个data，如果有多个参数，以data属性的形式传入。
            Template vltTemplate = vltEngine.GetTemplate(templatename);//拿到模板文件
            System.IO.StringWriter vltWriter = new System.IO.StringWriter();
            vltTemplate.Merge(vltContext, vltWriter);
            string html = vltWriter.GetStringBuilder().ToString();
            return html;
        }
    }
```

