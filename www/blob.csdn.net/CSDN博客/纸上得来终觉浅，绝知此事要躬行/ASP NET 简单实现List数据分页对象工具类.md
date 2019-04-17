# ASP.NET  简单实现List数据分页对象工具类 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年10月26日 15:07:42[boonya](https://me.csdn.net/boonya)阅读数：146








对于一个List分页，这个就很简单了只需要组装数据结构，然后根据分页参数改变数据显示就OK了。

### Pager工具

```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace RTVSWeb.Utils
{
    /// <summary>
    /// List数据分页对象
    /// </summary>
    public class Pager<T>
    {
        public int CurrentPage { set; get; }

        public int PageSize { set; get; }

        public int TotalPage { set; get; }

        public int TotalData { set; get; }

        private List<T> Results = new List<T>();

        public List<T> Data
        {
            set { value = Results; }
            get { return Results; }
        }
        /// <summary>
        /// 分页原始数据
        /// </summary>
        /// <param name="page"></param>
        /// <param name="pageSize"></param>
        /// <param name="deviceInfos"></param>
        public void Paging(int page, int pageSize, List<T> deviceInfos)
        {
            this.PageSize = pageSize;
            this.CurrentPage = page;
            this.TotalData = deviceInfos.Count;
            if (deviceInfos.Count % PageSize == 0)
            {
                TotalPage = deviceInfos.Count / PageSize;
            }
            else
            {
                TotalPage = deviceInfos.Count / PageSize + 1;
            }

            if (CurrentPage > TotalPage)
            {
                CurrentPage = TotalPage;
            }

            if (deviceInfos.Count > 0)
            {
                int left = deviceInfos.Count - (CurrentPage - 1) * PageSize;
                Results = deviceInfos.GetRange((CurrentPage - 1) * PageSize, left > PageSize ? PageSize : left);
            }

        }

    }
}
```

### Controller查询使用

```
// 上传的设备分页列表: /<controller>/
        public Pager<DeviceInfo> DevicesToPage(int page,int pageSize)
        {
            Pager<DeviceInfo> pager = new Pager<DeviceInfo>();
            pager.Paging(page,pageSize, Program.task.ServerInfoGet().ListDeviceInfo);
            return pager;
        }
```

### View显示cshtml

```
@{
    Layout = null;
}
<center><input type="button" class="btn-primary" style="width:48px;height:24px;" value="刷新" onclick="reload()"></center>
<!--这个地方展示数据-->
<table id="data-table" class="table"  style="width:100%;">
    <tr>
        <th >设备IP</th>
        <th >设备SIM</th>
        <th >设备逻辑通道</th>
        <th >转发客户端列表</th>
    </tr>

</table>
<!--这个地方展示分页-->
<div class="m-style M-box3"></div>

<link rel="stylesheet" type="text/css" href="/js/pagination/common/highlight.min.css" media="screen">
<link rel="stylesheet" type="text/css" href="/js/pagination/common/common.css" media="screen">
<link rel="stylesheet" type="text/css" href="/js/pagination/pagination.css" media="screen">
<script src="/js/pagination/common/highlight.min.js"></script>
<script src="/js/pagination/jquery.pagination.js"></script>

<script type="text/javascript">
    $(function () {
        query(1,10);
    });

    var url = "/Server/DevicesToPage";

    var queryStr = {};

    function getJson(url, data, func) {
        $.getJSON(url, data, func);
    }

    //进行查询
    function query(page, pageSize) {
        queryStr.page = page;
        queryStr.pageSize = pageSize;
        getJson(url+"?random=" + Math.random(), queryStr, loadlist);
    }

    function showResult(json)
    {
        $(".loaded-data").remove();
        for (var i = 0; i < json.data.length; i++) {
            var fdlist = "[";
            var list = json.data[i].listClientInfo;
            var count = 0;
            for (var j = 0; j < list.length; j++) {
                var index = 0;
                // JSON转字符串
                var jStr = "{ ";
                for (var item in list[j]) {
                    if (index == 0) {
                        jStr += "'" + item + "':'" + list[j][item] + "'";
                    }
                    else {
                        jStr += ",'" + item + "':'" + list[j][item] + "'";
                    }
                    index++;
                }
                jStr += " }";
                if (count == 0) {
                    fdlist += jStr;
                } else {
                    fdlist += "," + jStr;
                }
                count++;
            }
            fdlist += "]";

            $("#data-table").append(
                "<tr class=\"loaded-data\"><th>" +
                json.data[i].remoteInfo + "</th><th>" +
                json.data[i].sim + "</th><th>" +
                json.data[i].channel + "</th><th>"
                + fdlist + "</th></tr > ")
        }
    }
    //返回结果处理:注意JSON字段的大小写问题
    function loadlist(json) {     
        showResult(json);
        //初始化分页控件
        $(".M-box3").pagination({
            pageCount: json.totalPage,
            totalData: json.totalData,
            current: json.currentPage,
            showData: json.pageSize,
            jump: true,
            coping: true,
            homePage: '首页',
            endPage: '末页',
            prevContent: '上页',
            nextContent: '下页',
            callback: function (api)
            {
                // 当前第几页
                queryStr.page = api.getCurrent();

                $.getJSON(url + "?random=" + Math.random(), queryStr, function (json) {
                    showResult(json);
                });
            }
        });
    }

    function reload()
    {
        query(1, 10);
    }

</script>
```





