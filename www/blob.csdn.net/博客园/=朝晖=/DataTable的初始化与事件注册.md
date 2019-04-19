# DataTable的初始化与事件注册 - =朝晖= - 博客园
# [DataTable的初始化与事件注册](https://www.cnblogs.com/dhcn/p/9866133.html)
     TypeScript代码：
```
let tabledata = data as any[];
        if (this.dataTable) {
            this.dataTable.clear();
            this.dataTable.destroy();
          }
        /*
        *     ip = models.IPAddressField()
        access_time = models.DateTimeField()
        method = models.CharField()
        url = models.CharField()
        protocol = models.CharField()
        status_code = models.SmallIntegerField()
        size = models.BigIntegerField()
        referer = models.CharField()
        client = models.CharField()
        detail = models.CharField()*/
        let search:boolean|object;
        if (this.searchParams.keyword && this.searchParams.keyword.length>0){
          search = {"search": this.searchParams.keyword};
        }else {
          search = {};
        }
        this.dataTable = $('#eventstable').DataTable({
          'data'        : tabledata,
          'paging'      : true,
          'lengthChange': true,
          'searching'   : true,
          'ordering'    : true,
          'info'        : false,
          'autoWidth'   : false,
          'columns': [
            { title: "access_time", 'data': "access_time" },
            { title: "detail", 'data': "detail" }
          ]
        });
```
 事件注册代码，ES6的：
```
let table = $('#dataTable').DataTable( {
                data: data,
                'paging'      : true,
                'lengthChange': true,
                'searching'   : true,
                'ordering'    : true,
                'info'        : false,
                'autoWidth'   : true,
                //使用对象数组，一定要配置columns，告诉 DataTables 每列对应的属性
                //data 这里是固定不变的，name，position，salary，office 为你数据里对应的属性
                columns: [
                    { data: 'id',title:"ID" },
                    { data: 'name',title:"名称" },
                    { data: 'id',title:"操作" ,"render":function (data, type, row, meta ) {
                        return'<button type="button" data-id="'+data+'" class="btn btn-primary btn-sm">执行</button>';
                    }}
                ]
            } );
            $("button.btn-sm").on("click",function (event) {
                console.log($(event.target).attr("data-id"))
                return false
            })
            table.on("draw",function () {
                console.log("draw------>")
                $("button.btn-sm").on("click",function (event) {
                    console.log($(event.target).attr("data-id"))
                    return false
                })
            })
```
解释一下，为什么这么写，初始化完需要注册一次，每次页面page或者排序draw完需要再注册一次，比较奇怪的事第一次init以后的draw不会引发draw事件，所以不得不为第一次初始化完成绑定了一次。

