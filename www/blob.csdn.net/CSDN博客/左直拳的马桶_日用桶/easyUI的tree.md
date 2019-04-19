# easyUI的tree - 左直拳的马桶_日用桶 - CSDN博客
2015年09月02日 19:34:54[左直拳](https://me.csdn.net/leftfist)阅读数：2153
前端使用easyUI，放了一个tree，搞死了。
easyUI的tree，后端传过来的数据，是json格式；然后easyUI向后端提交、请求时，会自动将节点的id附在url后面。
主要有两个注意的地方：
1、json中的key及value，不能使用单引号，只能使用双引号；
2、json数据要以数组的形式传送，就是说，最外层要有”[]”
以   ASP.NET MVC页面使用示例：
view：
```
<ul id="treeQh" class="easyui-tree" data-options="
    url: '@Url.Action("GetData", "Node")'
    , animate: true
    , onClick: f_node_left.onClick
    "></ul>
```
controller：
```
[HttpPost]
        public ActionResult GetData(string id = "")
        {//id从哪里来？easyUI会自动附在 url: '@Url.Action("GetData", "Node")'后面
            string result = !String.IsNullOrEmpty(id) 
                ? GetSubNodes(id)
                : GetRootNode();
            return Content(result, "application/json");
        }
```
业务逻辑代码：
```
public string GetNodeJsonStr(string id)
        {
            JObject jObj;
            NodeConfigInfo node = this.nodeConfigInfoRepository.GetById(id);
            if (this.nodeConfigInfoRepository.GetMany(m => m.QH_PARENTID.Equals(node.QH_ID)).Any())
            {//有子节点
                jObj = new JObject
                {
                    {"id",node.QH_ID}
                    ,{"text",node.QH_NAME}
                    ,{"state","closed"}
                };
            }
            else
            {
                jObj = new JObject
                {
                    {"id",node.QH_ID}
                    ,{"text",node.QH_NAME}
                };
            }
            return getJsonStr(jObj);
        }
        string getJsonStr(object obj)
        {
            string jsonstr = JsonConvert.SerializeObject(obj);
            if(!jsonstr.StartsWith("["))
            {
                jsonstr = "[" + jsonstr + "]";
            }
            return jsonstr;
        }
```
