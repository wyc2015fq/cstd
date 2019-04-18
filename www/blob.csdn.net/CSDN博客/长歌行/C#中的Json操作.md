# C#中的Json操作 - 长歌行 - CSDN博客





2017年08月12日 11:37:26[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：408








随着JSON越来越多的应用到IT领域，C#中再也不是XML一家独占的情况了。熟练的解析和构建JSON也是必备技能。目前我正在参与的ArcGIS Earth产品，其工作区的保存格式以及软件中的传输格式都是使用JSON。就在此分享一些浅显的经验吧。

工欲善其事必先利其器，对于C#一般使用Newtonsoft.Json就行了，如果使用python则用json，但对于C++，由于许可和效率的问题，选择会很多，对于开源软件，我比较推荐Jsoncpp。

## 基础使用

对于一般的类或者对象，使用`JsonConvert.SerializeObject(yourObject)`或者 
`JsonConvert.DeserializeObject<T>(jsonString)`就行。示例如下：

```
using Newtonsoft.Json.Linq;
using Newtonsoft.Json;
using Newtonsoft.Json.Converters;
namespace AboutJson
{
    public enum Country
    {
        CN,
        EN,
        US
    }
    public class Family 
    {
        public Country Country { set; get; }
        public string Dad { set; get; }
        public string Mum { set; get; }
        public List<string> Children { set; get; }
    }

    class Program
    {
        static private string TestSerializeObject(Family f)
        {
            return JsonConvert.SerializeObject(f);
        }

        static private Family TestDeserializeObject(string familyJson)
        {
            return JsonConvert.DeserializeObject<Family>(familyJson);
        }
        static void Main(string[] args)
        {
            Family f = new Family {
                Country = Country.US,
                Dad = "Mr Brown",
                Mum = "Mrs Brown",
                Children = new List<string>{"Lily", "Lucy"} };

            // The result will be
            // { "Country":2,"Dad":"Mr Brown","Mum":"Mrs Brown","Children":["Lily","Lucy"]}
            // But sometimes we need country like this : "Country" : "US"
            string familyJson = TestSerializeObject(f);

            Family f1 = TestDeserializeObject(familyJson);
        }
    }
}
```

但是有时候我们有一些特殊的需求，例如，某些字段在转换为Json的时候不输出，枚举需要转换为字符串而不是数字，某些字段的键值需要自定义和变量名不相同。。。

这时候，在不修改原始对象的情况下，最直接的办法是自己手动来构建和解析Json字符串。

其中涉及到的内容为，JObject（JToken，JArray）和基础数据类型的转换。示例如下：

```
static private string ConstructJsonManually(Family f)
        {
            JObject familyObj = new JObject();
            JArray childrenObj = new JArray();
            familyObj["Dad"] = f.Dad;
            familyObj[nameof(Family.Mum)] = f.Mum;
            familyObj[nameof(Country)] = f.Country.ToString();
            foreach(var child in f.Children)
            {
                childrenObj.Add(child);
            }
            familyObj[nameof(f.Children)] = childrenObj;
            return familyObj.ToString(Formatting.None);
        }

        static private Family ParseJsonManually(string familyJson)
        {
            Family f = new Family();
            Country country = Country.CN;
            JObject familyObj = JObject.Parse(familyJson);
            if(familyObj[nameof(Country)] != null)
            {
                Enum.TryParse(familyObj[nameof(Country)].ToString(), true, out country);
                f.Country = country;
            }

            if(familyObj[nameof(Family.Dad)] != null)
            {
                f.Dad = familyObj[nameof(Family.Dad)].ToString();
            }

            if(familyObj[nameof(Country)] != null)
            {
                f.Mum = familyObj[nameof(Family.Mum)].ToString();
            }

            if(familyObj[nameof(Family.Children)] != null)
            {
                JArray childrenObj = (JArray)familyObj[nameof(Family.Children)];
                f.Children = new List<string>();
                foreach(var child in childrenObj)
                {
                    f.Children.Add(child.ToString());
                }
            }
            return f;
        }
```

调用结果是：

```
// The result will be 
            // { "Dad":"Mr Brown","Mum":"Mrs Brown","Country":"US","Children":["Lily","Lucy"]
            string familyJsonManual = ConstructJsonManually(f);

            Family f2 = ParseJsonManually(familyJsonManual);
```

## 进阶用法

如果觉得手动构建解析工作量大，后期不好维护，并且有权限修改原始的类，则可以使用一些进阶的用法了。如`JsonSerializerSettings`和`JsonProperty`，但是这儿，我对此不做过多介绍，对于高阶使用，查阅第一手的官方文档更加重要。

我在这儿举三个常用情况的例子吧。输出的时候修改键值，输出字符串形式的枚举，选择性的输出某些字段。

```
[JsonObject(MemberSerialization.OptOut)]
    public class FamilyEx
    {
        [JsonConverter(typeof(StringEnumConverter))]
        public Country Country { set; get; }
        [JsonProperty(PropertyName = "Father")]
        public string Dad { set; get; }
        [JsonIgnore]
        public string Mum { set; get; }
        public List<string> Children { set; get; }
    }

    // The result will be 
    // { "Country":"US","Father":"Mr Brown","Children":["Lily","Lucy"]}
    string familyJsonEx = JsonConvert.SerializeObject(fEx);
```

其中使用`[JsonObject(MemberSerialization.OptOut)]`和`[JsonIgnore]`来选择性输出。 

使用`[JsonConverter(typeof(StringEnumConverter))]`来以字符串输出枚举值。 

使用`[JsonProperty(PropertyName = "Father")]`将键值由`Dad`修改为`Father`
## 结语

更个性化定制的内容，建议查阅第一手官方文档。消除隐患。C#中处理Json还是很方便的，但是由于Json格式的多样性和用户输入的不确定性，请使用try catch来捕获和判断异常，以便log和给出人性化的用户提示。

完整代码，见[GitHub](https://github.com/bentan2013/master-csharp/blob/master/AboutJson/AboutJson/Program.cs)




