# C#下JSON字符串的反序列化 - 左直拳的马桶_日用桶 - CSDN博客
2015年01月05日 22:45:47[左直拳](https://me.csdn.net/leftfist)阅读数：19601
C#下JSON字符串的反序列化，一般都是用newtonsoft.json，比较方便。.net当然也有提供相应功能，但觉得比较复杂。
所谓反序列化，就是将一个包含JSON内容的字符串，转换回指定对象（不一定是转换回JSON对象）。
方法是：
using Newtonsoft.Json;
。。。
JsonConvert.DeserializeObject<。。。>(strJson)
示例1：
```
public class ViewTag
{
    public int ViewId { get; set; }
    public string Name { get; set; }
    public bool IsValid { get; set; }
    public int Seq { get; set; }
    public byte ChangeType { get; set; }
}
string strJson = "[{'ViewId':14,'Name':'view 1','IsValid':true,'Seq':'1','ChangeType':0},{'ViewId':15,'Name':'view 2','IsValid':true,'Seq':'2','ChangeType':0}]";
List<ViewTag> list = JsonConvert.DeserializeObject<List<ViewTag>>(strJson);
```
上述例子中，json字符串内，每个json元素的数据结构都一样。但假如不一样，怎么办？
示例2：
```
string strJson = @"[{
	'id':1,
	'text':'All Object',
	'iconCls':'', 
	'children':[{
		'id':11,
		'text':'Key attributes',
		'children':[{
			'id':111,
			'text':'Item1'
		},{
			'id':112,
			'text':'Item2'
		},{
			'id':113,
			'text':'Item3'
		}]
	},{
		'id':12,
		'text':'Service table 1',
		'state':'closed',
		'children':[{
			'id':121,
			'text':'Service 1'
		},{
			'id':122,
			'text':'Service 2'
		},{
			'id':123,
			'text':'Service 3'
		}]
	},{
		'id':13,
		'text':'Service table 2',
		'state':'closed',
		'children':[{
			'id':131,
			'text':'Service 1'
		},{
			'id':132,
			'text':'Service 2'
		},{
			'id':133,
			'text':'Service 3'
		}]
	},{
		'id':14,
		'text':'Service table 3'
	}]
}]";
            return JsonConvert.DeserializeObject<List<object>>(strJson);
```
不管三七二十一，将泛型对象定为 object 即可。
