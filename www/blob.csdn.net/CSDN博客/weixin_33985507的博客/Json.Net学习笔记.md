# Json.Net学习笔记 - weixin_33985507的博客 - CSDN博客
2013年09月03日 21:43:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
摘自:  [http://www.verydemo.com/demo_c360_i45119.html](http://www.verydemo.com/demo_c360_i45119.html)
分类：[编程语言](http://www.verydemo.com/one_c52.html)/[ASP.NET](http://www.verydemo.com/two_c360.html)/[文章](http://www.verydemo.com/two_c360.html)
**导读：**string googleSearchText = @"{ ""responseData"": { ""results"": [ { ""GsearchResultClass"": ""GwebSearch"", ""unescapedUrl"": ""http://en.wikipedia.org/wiki/Paris_Hilton"", ""url"": ""http://en.wikipedia.org/wiki/Paris_Hilton"", ""visibleUrl"": ""en.wikipedia.org"", ""cacheUrl"": ……
Json.Net**学习****笔记**(十四) JSON的部分序列化
通常当用到大的Json文档的时候，你可能只对其中的一小个片段信息感兴趣。这种情况下你想把Json.Net转换为.Net 对象就会让人很困扰，因为你必须为Json的整个结果定义一个.Net的类。
使用Json.Net很容易避开这个问题。在把它们传递到Json.Net序列化器之前，你可以使用Linq to Json 提取Json中你想要序列化的一些片段。
 string googleSearchText = @"{
                  ""responseData"": {
                    ""results"": [
                      {
                        ""GsearchResultClass"": ""GwebSearch"",
                        ""unescapedUrl"": ""http://en.wikipedia.org/wiki/Paris_Hilton"",
                        ""url"": ""http://en.wikipedia.org/wiki/Paris_Hilton"",
                        ""visibleUrl"": ""en.wikipedia.org"",
                        ""cacheUrl"": ""http://www.google.com/search?q=cache:TwrPfhd22hYJ:en.wikipedia.org"",
                        ""title"": ""<b>Paris Hilton</b> - Wikipedia, the free encyclopedia"",
                        ""titleNoFormatting"": ""Paris Hilton - Wikipedia, the free encyclopedia"",
                        ""content"": ""[1] In 2006, she released her debut album...""
                      },
                      {
                        ""GsearchResultClass"": ""GwebSearch"",
                        ""unescapedUrl"": ""http://www.imdb.com/name/nm0385296/"",
                        ""url"": ""http://www.imdb.com/name/nm0385296/"",
                        ""visibleUrl"": ""www.imdb.com"",
                        ""cacheUrl"": ""http://www.google.com/search?q=cache:1i34KkqnsooJ:www.imdb.com"",
                        ""title"": ""<b>Paris Hilton</b>"",
                        ""titleNoFormatting"": ""Paris Hilton"",
                        ""content"": ""Self: Zoolander. Socialite <b>Paris Hilton</b>...""
                      }
                    ],
                    ""cursor"": {
                      ""pages"": [
                        {
                          ""start"": ""0"",
                          ""label"": 1
                        },
                        {
                          ""start"": ""4"",
                          ""label"": 2
                        },
                        {
                          ""start"": ""8"",
                          ""label"": 3
                        },
                        {
                          ""start"": ""12"",
                          ""label"": 4
                        }
                      ],
                      ""estimatedResultCount"": ""59600000"",
                      ""currentPageIndex"": 0,
                      ""moreResultsUrl"": ""http://www.google.com/search?oe=utf8&ie=utf8...""
                    }
                  },
                  ""responseDetails"": null,
                  ""responseStatus"": 200
                }";
            JObject googleSearch = JObject.Parse(googleSearchText);
            // get JSON result objects into a list
            IList<JToken> results = googleSearch["responseData"]["results"].Children().ToList();
            // serialize JSON results into .NET objects
            IList<SearchResult> searchResults = new List<SearchResult>();
            foreach (JToken result in results)
            {
                SearchResult searchResult = JsonConvert.DeserializeObject<SearchResult>(result.ToString());
                searchResults.Add(searchResult);
            }
            // Title = <b>Paris Hilton</b> - Wikipedia, the free encyclopedia
            // Content = [1] In 2006, she released her debut album...
            // Url = http://en.wikipedia.org/wiki/Paris_Hilton
            // Title = <b>Paris Hilton</b>
            // Content = Self: Zoolander. Socialite <b>Paris Hilton</b>...
            // Url = http://www.imdb.com/name/nm0385296/
posted @ 2011-06-11 15:14 资源收集 阅读(44) 评论(0) 编辑
Json.Net**学习****笔记**(十三) 缩小序列化Json的大小
当序列化.Net对象为Json对象时经常会遇到的一个问题是：最终的Json包含大量多余的属性和属性值。这个问题在返回Json到客户端时显得特别重要。Json越大意味着需要更大的带宽，使网速变得更慢。
为了解决多余的Json这个问题，Json.Net有一系列内置的选项可以进行调整。
->JsonIgnoreAttribute and DataMemberAttribute
默认情况下，在Json创建的时候Json.Net会包含所有类级别的public属性和字段。添加JsonIgnoreAttribute到属性上，告诉序列化器序列化时跳过它。
 public class Car
    {
        // included in JSON
        public string Model { get; set; }
        public DateTime Year { get; set; }
        public List<string> Features { get; set; }
        // ignored
        [JsonIgnore]
        public DateTime LastModified { get; set; }
    }
如果类有很多属性，你只想序列化它的一小部分，添加JsonIgore到所有其他的属性上会比较冗余，也比较容易出错。有一种用来处理这种情况的方法，添加DataContractAttribute到类上，添加DataMemberAttribute到需要被序列化的属性上。与使用JsonIgnoreAttribute的opt-out序列化相比，opt-in序列化仅仅你标记的属性被需列化。
using System.Runtime.Serialization;
  [DataContract]
    public class Computer
    {
        // included in JSON
        [DataMember]
        public string Name { get; set; }
        [DataMember]
        public decimal SalePrice { get; set; }
        // ignored
        public string Manufacture { get; set; }
        public int StockCount { get; set; }
        public decimal WholeSalePrice { get; set; }
        public DateTime NextShipmentDate { get; set; }
    }
->Formatting
Json序列化时，用可选参数Formatting.Indented生成良好的显示格式，可读性更好。另一方面，Formatting.None会跳过不必要的空格和换行符，让Json的结果更小。生成的显示格式更加紧凑，也许效率更高。
->NullValueHandling
   在序列化器中NullVlaueHandling是可选的。它控制序列化器如何处理值为null的属性。通过设置NullValueHandling.Ignore值，序列化器会跳过值为null的属性。
  public class Movie
    {
        public string Name { get; set; }
        public string Description { get; set; }
        public string Classification { get; set; }
        public string Studio { get; set; }
        public DateTime? ReleaseDate { get; set; }
        public List<string> ReleaseCountries { get; set; }
    }
测试1：
           Movie movie = new Movie();
            movie.Name = "Bad Boys III";
            movie.Description = "It's no Bad Boys";
            string included = JsonConvert.SerializeObject(movie,
              Formatting.Indented,
              new JsonSerializerSettings { });
            Console.WriteLine(included);
输出结果：
{
  "Name": "Bad Boys III",
  "Description": "It's no Bad Boys",
  "Classification": null,
  "Studio": null,
  "ReleaseDate": null,
  "ReleaseCountries": null
}
测试2:
           string ignored = JsonConvert.SerializeObject(movie,
              Formatting.Indented,
              new JsonSerializerSettings { NullValueHandling = NullValueHandling.Ignore });
            Console.WriteLine(ignored);
输出结果：
{
  "Name": "Bad Boys III",
  "Description": "It's no Bad Boys"
}
NullValueHandling也可以在单独的属性上用JsonPropertyAttribute进行定制。使用JsonPropertyAttribute时设置的NullValueHandling值会在Json序列化器中为该属性进行重写。
->DefaultValuehandling
   在序列化器中DefaultValuehandling是可选的。它控制序列化器如何处理带有默认值的属性。通过设置DefaultValuehandling.Ignore值，序列化器会跳过带有默认值的属性。对于引用类型为null，对于值类型，如int和DateTime，序列化器将跳过默认未初使化值的值类型。
  Json.Net也允许你通过DefaultValueAttribute自定义默认值，例如：如果一个字符串类型的属性Department在它的默认状态下总是返回一个空字符，但是在你的Json对象中你不想要那个空字符串，你可以在Department上使用空字符串参数标记DefaultValueAttribute，这意味着Department在序列化时不再写入Json中，除非它有非空值。
  public class Invoice
    {
        public string Company { get; set; }
        public decimal Amount { get; set; }
        // false is default value of bool
        public bool Paid { get; set; }
        // null is default value of nullable
        public DateTime? PaidDate { get; set; }
        // customize default values
        [DefaultValue(30)]
        public int FollowUpDays { get; set; }
        [DefaultValue("")]
        public string FollowUpEmailAddress { get; set; }
    }
测试1：
           Invoice invoice = new Invoice
            {
                Company = "Acme Ltd.",
                Amount = 50.0m,
                Paid = false,
                FollowUpDays = 30,
                FollowUpEmailAddress = string.Empty,
                PaidDate = null
            };
            string includedDefaultValue = JsonConvert.SerializeObject(invoice,
              Formatting.Indented,
              new JsonSerializerSettings { });
            Console.WriteLine(includedDefaultValue);
输出结果：
{
  "Company": "Acme Ltd.",
  "Amount": 50.0,
  "Paid": false,
  "PaidDate": null,
  "FollowUpDays": 30,
  "FollowUpEmailAddress": ""
}
测试2:
            string ignoredDefaultValue = JsonConvert.SerializeObject(invoice,
              Formatting.Indented,
              new JsonSerializerSettings { DefaultValueHandling = DefaultValueHandling.Ignore });
            Console.WriteLine(ignoredDefaultValue);
输出结果:
{
  "Company": "Acme Ltd.",
  "Amount": 50.0,
  "Paid": false//??
}
DefaultValueHandling也可以在单独的属性上用JsonPropertyAttribute进行定制。使用JsonPropertyAttribute时设置的DefaultValueHandling值会在Json序列化器中为该属性进行重写。
->IContractResolver
  为了获得更多的灵活性，IContractResolver接口提供了定制.Net对象序列化为Json的每一个方面，包括在运行时改变序列化的行为。
 public class DynamicContractResolver : DefaultContractResolver
    {
        private readonly char _startingWithChar;
        public DynamicContractResolver(char startingWithChar)
        {
            _startingWithChar = startingWithChar;
        }
        protected override IList<JsonProperty> CreateProperties(JsonObjectContract contract)
        {
            IList<JsonProperty> properties = base.CreateProperties(contract);
            // only serializer properties that start with the specified character
            properties =
              properties.Where(p => p.PropertyName.StartsWith(_startingWithChar.ToString())).ToList();
            return properties;
        }
    }
    public class Book
    {
        public string BookName { get; set; }
        public decimal BookPrice { get; set; }
        public string AuthorName { get; set; }
        public int AuthorAge { get; set; }
        public string AuthorCountry { get; set; }
    }
测试:
           Book book = new Book
            {
                BookName = "The Gathering Storm",
                BookPrice = 16.19m,
                AuthorName = "Brandon Sanderson",
                AuthorAge = 34,
                AuthorCountry = "United States of America"
            };
            string startingWithA = JsonConvert.SerializeObject(book, Formatting.Indented,
              new JsonSerializerSettings { ContractResolver = new DynamicContractResolver('A') });
            Console.WriteLine(startingWithA);
            // {
            //   "AuthorName": "Brandon Sanderson",
            //   "AuthorAge": 34,
            //   "AuthorCountry": "United States of America"
            // }
            string startingWithB = JsonConvert.SerializeObject(book, Formatting.Indented,
              new JsonSerializerSettings { ContractResolver = new DynamicContractResolver('B') });
            Console.WriteLine(startingWithA);
            // {
            //   "BookName": "The Gathering Storm",
            //   "BookPrice": 16.19
            // }
posted @ 2011-06-11 15:13 资源收集 阅读(81) 评论(0) 编辑
Json.Net**学习****笔记**(十二) 协议解析
IContractResolver接口提供了一种方法去定制序列化器如何去序列化和反序列化.Net对象为Json对象
实现IContractResolver接口，然后为Json序列化器分配一个实例对象，这样你就可以控制对象是否被序列化为Json对象或者Json数组、对象的成员应该被序列化成什么、如何被序列化、及他们被谁调用等。
->DefaultContractResolver
   DefaultContractResolver是序列化器默认的解析器，它以虚函数的形式提供了很多扩展功能，可以被覆写(overriden).
->CamelCasePropertyNamesContractResolver
   CamelCasePropertyNamesContractResolver继承自DefaultContractResolver，并且简单的覆写了Json属性名为camel命名规则的形式(首字母小写)
  public class Product
    {
        public string Name { get; set; }
        public decimal Price { get; set; }
        public DateTime ExpiryDate { get; set; }
        public string[] Sizes { get; set; }
    }
测试：
 Product product = new Product
            {
                ExpiryDate = new DateTime(2010, 12, 20, 18, 1, 0, DateTimeKind.Utc),
                Name = "Widget",
                Price = 9.99m,
                Sizes = new[] { "Small", "Medium", "Large" }
            };
            string json = JsonConvert.SerializeObject(
                            product,
                            Formatting.Indented,
                            new JsonSerializerSettings { ContractResolver = new CamelCasePropertyNamesContractResolver() }
                             );
            Console.WriteLine(json);
输出结果:
{
  "name": "Widget",
  "price": 9.99,
  "expiryDate": "\/Date(1292868060000)\/",
  "sizes": [
    "Small",
    "Medium",
    "Large"
  ]
posted @ 2011-06-11 15:12 资源收集 阅读(53) 评论(0) 编辑
Json.Net**学习****笔记**(十一) CustomCreationConverter
CustomCreationConverter是一个在序列化过程中提供自定方式去创建一个对象的Json转换器，一旦对象被创建，它将被序列化器填充值。
 public interface IPerson
    {
        string FirstName { get; set; }
        string LastName { get; set; }
        DateTime BirthDate { get; set; }
    }
    public class Employee : IPerson
    {
        public string FirstName { get; set; }
        public string LastName { get; set; }
        public DateTime BirthDate { get; set; }
        public string Department { get; set; }
        public string JobTitle { get; set; }
    }
    public class PersonConverter : CustomCreationConverter<IPerson>
    {
        public override IPerson Create(Type objectType)
        {
            return new Employee();
        }
    }
这是一个非常简单的例子。更复杂的场景可能包含一个对象工厂或者服务定位器(service locator)用来在运行时解析这个对象。
测试:   
      string json = @"[
              {
                ""FirstName"": ""Maurice"",
                ""LastName"": ""Moss"",
                ""BirthDate"": ""\/Date(252291661000)\/"",
                ""Department"": ""IT"",
                ""JobTitle"": ""Support""
              },
              {
                ""FirstName"": ""Jen"",
                ""LastName"": ""Barber"",
                ""BirthDate"": ""\/Date(258771661000)\/"",
                ""Department"": ""IT"",
                ""JobTitle"": ""Manager""
              }
            ]";
            List<IPerson> people = JsonConvert.DeserializeObject<List<IPerson>>(json, new PersonConverter());
            IPerson person = people[0];
            Console.WriteLine(person.GetType());// CustomCreationConverterTest.Employee          
            Console.WriteLine(person.FirstName);// Maurice
            Employee employee = (Employee)person;
            Console.WriteLine(employee.JobTitle);// Support
posted @ 2011-06-11 15:11 资源收集 阅读(28) 评论(0) 编辑
Json.Net**学习****笔记**(九) 异常处理
Json.Net支持在序列化和反序列化的过程中进行异常处理。异常处理让您捕获一个异常，您可以选择是否处理它，继续序列化或者让异常抛给上一层，在你的应用程序中被抛出。
异常处理通过两个方法来定义：the Error event on JsonSerializer 和 OnErrorAttribute
>Error Event
  error event是一个建立在JsonSerializer 上的异常处理.当序列化或者反序列化JSON时，任何有异常抛出的情况error event都会被触发.就像建立在JsonSerializer上的所有设置一样，它也可以在JsonSerializerSettings 上进行设置从而传递给JsonConvert的序列化方法.
示例：
 List<string> errors = new List<string>();
            List<DateTime> c = JsonConvert.DeserializeObject<List<DateTime>>(@"[
                  ""2010-12-19T00:00:00Z"",
                  ""I am not a date and will error!"",
                  [
                    1
                  ],
                  ""2011-01-01T00:00:00Z"",
                  null,
                  ""2010-12-25T00:00:00Z""
                  ]", new JsonSerializerSettings()
                        {
                            Error = delegate(object sender, Newtonsoft.Json.Serialization.ErrorEventArgs e)
                            {
                                errors.Add(e.ErrorContext.Error.Message);
                                e.ErrorContext.Handled = true;
                            },
                            Converters = { new IsoDateTimeConverter() }
                        });
            foreach (DateTime t in c)
            {
                Console.WriteLine(t.ToString());
            }
            //2010-12-19 00:00:00
            //2011-01-01 00:00:00
            //2010-12-25 00:00:00
            foreach (string err in errors)
            {
                Console.WriteLine(err);
            }
            //The string was not recognized as a valid DateTime. There is a unknown word starting at index 0.
            //Unexpected token parsing date. Expected String, got StartArray.
            //Cannot convert null value to System.DateTime.
  在这个例子中我们把一个Json数组反序列化为一个DateTime的集合，在JsonSerializerSettings中有一个hander被赋值成了error event ,它用来记录error message，并标记这个error为已处理(handled).
反序列化JSON的结果是三个被成功反序列化的日期和三个error messages：一个是不正确的格式,"I am not a date and will error!",一个是嵌套了JSON数组，最后一个是null值，因为定义的list不允许有可空类型的DateTime.这个事件处理已经记录了这些信息，Json.Net在序列化时继续进行(没有因为异常而停止)，因为这些错误已经被标记为已处理。
   值得注意的是，在Json.Net进行异常处理时，没有处理的异常将会被抛到上一层，并在它的每个parent触发事件，例如：在序列化若干对象的集合时，一个未处理的异常将被触发两次，首先在对象上，然后在集合上。这样就会让您在处理异常的时候，选择在它发生的地方，或者是它的一个parent上。
 JsonSerializer serializer = new JsonSerializer();
            serializer.Error += delegate(object sender, Newtonsoft.Json.Serialization.ErrorEventArgs e)
            {
                // only log an error once
                if (e.CurrentObject == e.ErrorContext.OriginalObject)
                    errors.Add(e.ErrorContext.Error.Message);
            };
如果您不是立即处理一个异常，仅仅是想针对它完成一次操作，您可以验证一下ErrorEventArg's CurrentObject是否等于OriginalObject.OriginalObject是抛出异常的对象，CurrentObject是事件被触发的对象.他们只会在第一次(事件被OriginalObject触发时)相等.
>OnErrorAttribute
 OnErrorAttribute的工作方式非常像其他Json.Net支持的.NET serialization attributes ,简单地把它标记在带有正确参数(一个StreamingContext和一个ErrorContext)的方法上就可以使用了，与方法的名字没有关系。
示例：
  public class PersonError
    {
        private List<string> _roles;
        public string Name { get; set; }
        public int Age { get; set; }
        public List<string> Roles
        {
            get
            {
                if (_roles == null)
                    throw new Exception("Roles not loaded!");
                return _roles;
            }
            set { _roles = value; }
        }
        public string Title { get; set; }
        [OnError]
        internal void OnError(StreamingContext context, ErrorContext errorContext)
        {
            errorContext.Handled = true;
        }
    }
在这个例子中，当_roles没有被设置值时访问Roles属性将会抛出一个异常.在序列化Roles属性时，异常处理的方法将设置error为handled,从而允许Json.Net继续序列化这个类。
测试：
 PersonError person = new PersonError
            {
                Name = "George Michael Bluth",
                Age = 16,
                Roles = null,
                Title = "Mister Manager"
            };
            string json = JsonConvert.SerializeObject(person, Formatting.Indented);
            Console.WriteLine(json);
输出：
            {
              "Name": "George Michael Bluth",
              "Age": 16,
              "Title": "Mister Manager"
            }
posted @ 2011-06-11 15:10 资源收集 阅读(57) 评论(0) 编辑
Json.Net**学习****笔记**(十) 保持对象引用
默认情况下，Json.Net将通过对象的值来序列化它遇到的所有对象。如果工个list包含两个Person引用，这两个引用都指向同一个对象，Json序列化器将输出每一个引用的所有名称和值。
定义类：
  public class Person
    {
        public DateTime BirthDate { get; set; }
        public DateTime LastModified { get; set; }
        public string Name { get; set; }
    }
测试：
         Person p = new Person()
            {
                BirthDate = new DateTime(1985, 11, 27, 0, 0, 0, DateTimeKind.Utc),
                LastModified = new DateTime(2010, 12, 20, 0, 0, 0, DateTimeKind.Utc),
                Name = "James"
            };
            List<Person> people = new List<Person>();
            people.Add(p);
            people.Add(p);
            string json = JsonConvert.SerializeObject(people, Formatting.Indented);
            Console.WriteLine(json);
输出结果：
[
  {
    "BirthDate": "\/Date(501897600000)\/",
    "LastModified": "\/Date(1292803200000)\/",
    "Name": "James"
  },
  {
    "BirthDate": "\/Date(501897600000)\/",
    "LastModified": "\/Date(1292803200000)\/",
    "Name": "James"
  }
]
在大多数情况下这是期望的结果，但是在某些场景下，将list中的第二项作为第一项的一个引用来输出会是一个更好的解决方案。如果上面的Json现在被反序列化，返回的list会包含两个完全分离的对象，它们具有相同的值。通过值来输出引用也会在对象上导致循环引用的发生。
>PreserveReferencesHandling
   string json2 = JsonConvert.SerializeObject(people, Formatting.Indented,
                new JsonSerializerSettings() { PreserveReferencesHandling = PreserveReferencesHandling.Objects });
            Console.WriteLine(json2);
输出结果：
          [
              {
                "$id": "1",
                "BirthDate": "\/Date(501897600000)\/",
                "LastModified": "\/Date(1292803200000)\/",
                "Name": "James"
              },
              {
                "$ref": "1"
              }
            ] 
          List<Person> deserializedPeople = JsonConvert.DeserializeObject<List<Person>>(json2,
                new JsonSerializerSettings { PreserveReferencesHandling = PreserveReferencesHandling.Objects });
            Console.WriteLine(deserializedPeople.Count);// 2       
            Person p1 = deserializedPeople[0];
            Person p2 = deserializedPeople[1];
            Console.WriteLine(p1.Name);// James
            Console.WriteLine(p2.Name);// James
            bool equal = Object.ReferenceEquals(p1, p2);// true
在list中的第一个Person被序列化时增加了一个额外的对象Id,现在第二个Person对象仅仅是第一个的引用。
现在使用PreserveReferencesHandling后，在序列化时只创建了一个Person对象，list中包含它的两个引用，原来我们叫作反射(mirroring) 。
>IsReference on JsonObjectAttribute, JsonArrayAttribute and JsonPropertyAttribute
  在对象序列化器上设置PreserveReferencesHandling，将会改变所有对象被序列化和反序列化的方式。为了更加细致地控制对象和成员被序列化为一个引用，可以在JsonObjectAttribute, JsonArrayAttribute 和 JsonPropertyAttribute上使用IsReference 属性.
在JsonObjectAttribute, JsonArrayAttribute 上设置IsReference 为true，意味着Json序列化器总是会序列这个类型为一个引用。在JsonPropertyAttribute上设置IsReference为true将只序列化这个属性为一个引用。
 [JsonObject(IsReference = true)]
    public class EmployeeReference
    {
        public string Name { get; set; }
        public EmployeeReference Manager { get; set; }
    }
测试：
           List<EmployeeReference> empList = new List<EmployeeReference>();
            empList.Add(empRef);
            empList.Add(empRef);
            string empRefJson = JsonConvert.SerializeObject(empList, Formatting.Indented);
            Console.WriteLine(empRefJson);
输出结果:
[
  {
    "$id": "1",
    "Name": "IsReference",
    "Manager": null
  },
  {
    "$ref": "1"
  }
]
>IReferenceResolver
要想定制引用的生成方式，可以继承自IReferenceResolver接口来使用Json序列化器。
posted @ 2011-06-11 15:10 资源收集 阅读(49) 评论(0) 编辑
Json.Net**学习****笔记**(七) 序列化集合
Json序列化器为序列化及反序列化集合对象提供了良好的支持.
->Serializing
   为了序列化一个集合---一个泛型的list,array,dictionary,或者自定义集合---简单地调用序列化器，使用您想要进行序列化的集合对象作为参数，Json.Net会序列化集合以及所有它包含的值。
示例：
public class Product
    {
        public string Name { get; set; }
        public decimal Price { get; set; }
        [JsonConverter(typeof(IsoDateTimeConverter))]
        public DateTime ExpiryDate { get; set; }
    }
测试：
            Product p1 = new Product()
            {
                Name = "Product 1",
                Price = 99.95m,
                ExpiryDate = new DateTime(2010, 12, 19, 0, 0, 0, DateTimeKind.Utc)
            };
            Product p2 = new Product
            {
                Name = "Product 2",
                Price = 12.50m,
                ExpiryDate = new DateTime(2011, 1, 1, 0, 0, 0, DateTimeKind.Utc)
            };
            List<Product> products = new List<Product>();
            products.Add(p1);
            products.Add(p2);
            string json = JsonConvert.SerializeObject(products, Formatting.Indented);
            Console.WriteLine(json);
输出结果：
[
  {
    "Name": "Product 1",
    "Price": 99.95,
    "ExpiryDate": "2010-12-19T00:00:00Z"
  },
  {
    "Name": "Product 2",
    "Price": 12.50,
    "ExpiryDate": "2011-01-01T00:00:00Z"
  }
]
->Deserializing
  为了反序列化Json到一个.Net集合中，只要指定一个您想要反序列化的集合类型就可以了，Json.Net支持多种类型的集合.
示例:
    string json2 = @"[
                              {
                                ""Name"": ""Product 1"",
                                ""ExpiryDate"": ""2010-12-19T00:00:00Z"",
                                ""Price"": 99.95,
                                ""Sizes"": null
                              },
                              {
                                ""Name"": ""Product 2"",
                                ""ExpiryDate"": ""2011-01-01T00:00:00Z"",
                                ""Price"": 12.50,
                                ""Sizes"": null
                              }
                            ]";
            List<Product> productList = JsonConvert.DeserializeObject<List<Product>>(json2);
            Console.WriteLine(productList.Count);//2
            Product product1 = productList[0];
            Console.WriteLine(product1.Name);//Product 1
->Deserializing Dictionaries
  使用Json.Net你也可以反序列化一个Json对象到一个.Net的泛型Dictionary中.Json对象的属性名和属性值将会被添加到Dictionary中.
示例:
            string jsonDictionary = @"{""key1"":""value1"",""key2"":""value2""}";
            Dictionary<string, string> dic = JsonConvert.DeserializeObject<Dictionary<string, string>>(jsonDictionary);
            Console.WriteLine(dic.Count);//2
            Console.WriteLine(dic["key1"]);//value1
posted @ 2011-06-11 15:09 资源收集 阅读(73) 评论(0) 编辑
Json.Net**学习****笔记**(八) 序列化的回调方法
Json.Net支持序列化回调方法，回调方法通过Json序列化器(JsonSerializer)可以用来操作一个对象，在它被序列化和反序列化之前或者之后.
OnSerializing 
OnSerialized 
OnDeserializing 
OnDeserialized
为了告诉序列化器在对象的序列化生命周期中哪个方法应该被调用，需要用适当的attribute(OnSerializingAttribute, OnSerializedAttribute, OnDeserializingAttribute, OnDeserializedAttribute)来标记方法.
例如对象序列化的回调方法:
using Newtonsoft.Json;
using System.Runtime.Serialization;
 public class SerializationEventTestObject
    {
        // This member is serialized and deserialized with no change.
        public int Member1 { get; set; }
        // The value of this field is set and reset during and 
        // after serialization.
        public string Member2 { get; set; }
        // This field is not serialized. The OnDeserializedAttribute 
        // is used to set the member value after serialization.
        [JsonIgnore]
        public string Member3 { get; set; }
        // This field is set to null, but populated after deserialization.
        public string Member4 { get; set; }
        public SerializationEventTestObject()
        {
            Member1 = 11;
            Member2 = "Hello World!";
            Member3 = "This is a nonserialized value";
            Member4 = null;
        }
        [OnSerializing]
        internal void OnSerializingMethod(StreamingContext context)
        {
            Member2 = "This value went into the data file during serialization.";
        }
        [OnSerialized]
        internal void OnSerializedMethod(StreamingContext context)
        {
            Member2 = "This value was reset after serialization.";
        }
        [OnDeserializing]
        internal void OnDeserializingMethod(StreamingContext context)
        {
            Member3 = "This value was set during deserialization";
        }
        [OnDeserialized]
        internal void OnDeserializedMethod(StreamingContext context)
        {
            Member4 = "This value was set after deserialization.";
        }
    }
示例对象被序列化和反序列化:
 SerializationEventTestObject obj = new SerializationEventTestObject();
            Console.WriteLine(obj.Member1);// 11
            Console.WriteLine(obj.Member2);// Hello World!
            Console.WriteLine(obj.Member3);// This is a nonserialized value
            Console.WriteLine(obj.Member4);// null
            string json = JsonConvert.SerializeObject(obj, Formatting.Indented);
            Console.WriteLine(json);
            //{
            //  "Member1": 11,
            //  "Member2": "This value went into the data file during serialization.",
            //  "Member4": null
            //}
            Console.WriteLine(obj.Member1);//11
            Console.WriteLine(obj.Member2);// This value was reset after serialization.
            Console.WriteLine(obj.Member3);// This is a nonserialized value
            Console.WriteLine(obj.Member4);// null
            obj = JsonConvert.DeserializeObject<SerializationEventTestObject>(json);
            Console.WriteLine(obj.Member1);//11
            Console.WriteLine(obj.Member2);// This value went into the data file during serialization.
            Console.WriteLine(obj.Member3);// This value was set during deserialization
            Console.WriteLine(obj.Member4);// This value was set after deserialization.
posted @ 2011-06-11 15:09 资源收集 阅读(39) 评论(0) 编辑
Json.Net**学习****笔记**(五) 自定义Json序列化特性
Attributes 可以用来控制Json.Net如何序列化和反序列化.Net对象。
>JsonObjectAttribute--标记在类上，用于控制该类如何被序列化为一个Json对象(JSON Object)
>JsonArrayAttribute--标记在集合上，用于控制该集合如何被序列化为一个Json集合(JSON Array)
>JsonPropertyAttribute--标记在字段和属性上，用于控制它如何被序列化为一个Json对象中的属性
>JsonConverterAttribute--标记在类或者字段和属性上，用于序列化时指定Json转换器(JsonConverter)
 和使用内置的Json.Net attributes一样，当确定Json如何被序列化和反序列化时，Json.Net也查找DataContract和DataMember attributes。如果都存在，Json.Net 序列化特性将优先使用。
示例：
   [JsonObject(MemberSerialization.OptOut)]
    public class Person
    {
        // "John Smith"
        [JsonProperty]
        public string Name { get; set; }
        // "2000-12-15T22:11:03"
        [JsonProperty]
        [JsonConverter(typeof(IsoDateTimeConverter))]
        public DateTime BirthDate { get; set; }
        // new Date(976918263055)
        [JsonProperty]
        [JsonConverter(typeof(JavaScriptDateTimeConverter))]
        public DateTime LastModified { get; set; }
        // not serialized
        public string Department { get; set; }
    }
->JsonObjectAttribute
  这里的MemberSerialization标记指定成员序列化为opt-in(每一个成员都必须标记JsonProperty或者DataMember attribute才被序列化)或者opt-out(默认情况下，除了被标记为JsonIgnoreAttribute的所有成员都将被序列化,这是Json.Net 的默认行为)
  Json.Net把实现了IEnumerable接口的.Net类序列化为一个Json集合，用IEnumerable 的值来填充该集合。标记JsonPropertyAttribute来重写此行为，并强制序列化器序列化该类的字段和属性。
->JsonPropertyAttribute
  JsonPropertyAttribute有很多用途
    1.默认情况下，Json 属性与.Net属性有相同的名字，这个attribute允许自定义属性名
    2.当member serialization 被设置成opt-in时，指定属性应该被序列化
    3.在序列化和反序列化时包含非public属性
->JsonIgnoreAttribute
  从序列化中排除指定的字段或者属性
->JsonConverterAttribute
  JsonConverterAttribute指定Json序列化器(JsonSerializer)用来转换一个对象
  这个特性可以用来标记类或者成员，当它用来标记类的时候，由attribute指定的JsonConverter将为类提供默认的序列化方法.当它用来标记字段或者属性的时候，指定的JsonConverter将一直用于序列化该值。
  使用JsonConverter的优先级顺序为：先成员，后类，最后为传给JsonSerializer的任何转换器.
示例:
    public class MemberConverterClass
    {
        public DateTime DefaultConverter { get; set; }
        [JsonConverter(typeof(IsoDateTimeConverter))]
        public DateTime MemberConverter { get; set; }
    }//注：这是一个JsonConverterAttribute 应用于属性的例子
测试:
 DateTime date = Convert.ToDateTime("1970-01-01T00:00:00Z").ToUniversalTime();
            MemberConverterClass c = new MemberConverterClass
            {
                DefaultConverter = date,
                MemberConverter = date
            };
            string json = JsonConvert.SerializeObject(c, Formatting.Indented);
            Console.WriteLine(json);
输出结果:
{
  "DefaultConverter": "\/Date(0)\/",
  "MemberConverter": "1970-01-01T00:00:00Z"
}
posted @ 2011-06-11 15:08 资源收集 阅读(133) 评论(0) 编辑
Json.Net**学习****笔记**(六) Json.Net中关于时间的序列化
DateTime类型在Json中表示是很难的。
问题来源于JSON spec (Json规范)自身，没有精确的语法。这个规范包括objects, arrays, strings, integers 和 floats,但是对于date是什么样子的没有定义准确的标准。
关于时间，Json.Net使用的默认格式与微软:"\/Date(1198908717056)\/"相同，您可以从此外获得更多信息.
DateTime JsonConverters
由于在Json中没有关于时间的准确标准，当与其他系统进行互操作时就会有大量不同的时间格式。幸运的是，Json.Net为了处理读写自定义时间提供了一个解决方案:JsonConverters. JsonConverter是用来重写一个类型如何被序列化的。
示例： 
public class LogEntry
    {
        public string Details { get; set; }
        public DateTime LogDate { get; set; }
    }
测试1：
       LogEntry logEntry = new LogEntry()
            {
                LogDate = new DateTime(2010, 12, 19, 0, 0, 0, DateTimeKind.Utc),
                Details = "Application started."
            };
            string defaultJson = JsonConvert.SerializeObject(logEntry, Formatting.Indented);
            Console.WriteLine(defaultJson);
输出结果：
{
  "Details": "Application started.",
  "LogDate": "\/Date(1292716800000)\/"
}
测试2:
    string javascriptJson = JsonConvert.SerializeObject(logEntry, Formatting.Indented, new JavaScriptDateTimeConverter());
     Console.WriteLine(javascriptJson);
输出结果：
{
  "Details": "Application started.",
  "LogDate": new Date(
    1292716800000
  )
}
测试3:
            string isoJson = JsonConvert.SerializeObject(logEntry, Formatting.Indented, new IsoDateTimeConverter());
            Console.WriteLine(isoJson);
输出结果：
{
  "Details": "Application started.",
  "LogDate": "2010-12-19T00:00:00Z"
}
 简单地，通过JsonConverter您可以使用Json.Net的序列化器
>JavaScriptDateTimeConverter
  JavaScriptDateTimeConverter类是来自于Json.Net的两个DataTime Json转换器之一,这个转换器序列化一个DateTime类型为一个JavaScipt日期对象(https://developer.mozilla.org/en/Core_JavaScript_1.5_Reference/Global_Objects/Date)
  new Date(1292716800000)
从技术上讲，根据规范这是无效的数据格式，但是所有的浏览器、和一些Json的框架包括Json.Net都支持它。
>IsoDateTimeConverter
  IsoDateTimeConverter序列一个DateTime类型为一个ISO 8601格式的字符串.
  "2010-12-19T00:00:00Z"
IsoDateTimeConverter 类有一个属性DateTimeFormat,用来更进一步的自定义字符串格式.
最后要注意的一件事是，所有被Json.Net返回的日期值都是UTC Time
posted @ 2011-06-11 15:08 资源收集 阅读(109) 评论(0) 编辑
Json.Net**学习****笔记**(三) Json与Xml之间的转换
Json.Net 支持 Json 与Xml之间的相互转换.
转换规则：
 1.元素保持不变
 2.属性添加前缀@
 3.单个子文本结点直接作为元素结点的值，否则通过#text访问
 4.XML声明和处理指令以?为前缀
 5.字符数据(Character data)、注释、whitespace 和significate whitespace结点分别通过#cdata-section,#comment,#whitespace and #significate-whitespace 访问。
 6.同一级多个相同名字的结点作为一组，放到同一个集合中
 7.空元素为null
using Newtonsoft.Json;
测试：
string xml = <?xml version=""1.0"" standalone=""no""?>
                            <root>
                            <person id=""1"">
                            <name>Alan</name>
                            <url>http://www.google.com</url>
                            </person>
                            <person id=""2"">
                            <name>Louis</name>
                            <url>http://www.yahoo.com</url>
                            </person>
                            Hello World
                            </root>";
            XmlDocument doc = new XmlDocument();
            doc.LoadXml(xml);
           //XML to JSON
            string jsonText = JsonConvert.SerializeXmlNode(doc, Newtonsoft.Json.Formatting.Indented);
            Console.WriteLine(jsonText);
输出结果：
{
  "?xml": {
    "@version": "1.0",
    "@standalone": "no"
  },
  "root": {
    "person": [
      {
        "@id": "1",
        "name": "Alan",
        "url": "http://www.google.com"
      },
      {
        "@id": "2",
        "name": "Louis",
        "url": "http://www.yahoo.com"
      }
    ],
    "#text": "\r\n                            Hello World\r\n                            "
  }
}
            //JSON to XML
            XmlDocument docJson = (XmlDocument)JsonConvert.DeserializeXmlNode(jsonText);
            Console.WriteLine(docJson.InnerXml);
posted @ 2011-06-11 15:04 资源收集 阅读(68) 评论(0) 编辑
Json.Net**学习****笔记**(四) Json对象的读写
为了手动读写Json对象，Json.Net提供了JsonReader 和 JsonWriter这两个抽象类及其相应的派生类:
1.JsonTextReader 和 JsonTextWriter
用来读写Json对象的文本，JsonTextWriter 有大量设置去控制Json对象的格式。
测试：
            //写操作
            StringBuilder sb = new StringBuilder();
            StringWriter sw = new StringWriter(sb);
            using (JsonWriter jsonWriter = new JsonTextWriter(sw))
            {               
                jsonWriter.Formatting = Formatting.Indented;
                jsonWriter.WriteStartObject();
                jsonWriter.WritePropertyName("CPU");
                jsonWriter.WriteValue("Intel");
                jsonWriter.WritePropertyName("PSU");
                jsonWriter.WriteValue("500W");
                jsonWriter.WritePropertyName("Drives");
                jsonWriter.WriteStartArray();
                jsonWriter.WriteValue("DVD read/writer");
                jsonWriter.WriteComment("(broken)");
                jsonWriter.WriteValue("500 gigabyte hard drive");
                jsonWriter.WriteValue("200 gigabype hard drive");
                jsonWriter.WriteEndArray();
                jsonWriter.WriteEndObject();
            }
            Console.WriteLine(sb.ToString());
输出结果：
{
  "CPU": "Intel",
  "PSU": "500W",
  "Drives": [
    "DVD read/writer"
    /*(broken)*/,
    "500 gigabyte hard drive",
    "200 gigabype hard drive"
  ]
}
      //读操作
     TextReader txtReader = new StringReader(sb.ToString());
            using (JsonReader jsonReader = new JsonTextReader(txtReader))
            {
                string readResult = "{\n";
                while (jsonReader.Read())
                {
                    if (jsonReader.TokenType == JsonToken.PropertyName)
                    {
                        readResult += (string)jsonReader.Value + ":";
                    }
                    else if (jsonReader.TokenType == JsonToken.String)
                    {
                        readResult += (string)jsonReader.Value + "\n";
                    }
                    else if (jsonReader.TokenType == JsonToken.StartArray)
                    {
                        readResult += "[\n";
                        while (jsonReader.Read())
                        {
                            if (jsonReader.TokenType == JsonToken.EndArray)
                            {
                                readResult = readResult.Remove(readResult.Length - 2, 1);
                                readResult += "]\n";
                                break;
                            }
                            if (jsonReader.TokenType == JsonToken.String)
                                readResult += (string)jsonReader.Value + ",\n";
                            else if (jsonReader.TokenType == JsonToken.Comment)
                                readResult += "/*" + (string)jsonReader.Value + "*/,\n";
                        }
                    }
                }
                readResult += "}\n";
                Console.WriteLine(readResult);
             }
2.JTokenReader and JTokenWriter
JTokenReader and JTokenWriter是读写Linq to Json对象的，他们存在于Newtonsoft.Json.Linq名字空间中
         JObject o = new JObject(
                        new JProperty("Name", "ZhangSan"),
                        new JProperty("BirthDay", new DateTime(1985, 11, 27)));
            JsonSerializer serializer = new JsonSerializer();
            Person person = (Person)serializer.Deserialize(new JTokenReader(o), typeof(Person));
            Console.WriteLine(person.Name);//输出ZhangSan
posted @ 2011-06-11 15:04 资源收集 阅读(167) 评论(0) 编辑
Json.Net**学习****笔记**(二) Linq to Json
using Newtonsoft.Json.Linq;
定义类：
    public class Product
    {
        public string Name { get; set; }
        public DateTime Expiry { get; set; }
        public decimal Price { get; set; }
        public string[] Sizes { get; set; }
    }
测试:
            Product product = new Product
            {
                Name = "Apple",
                Expiry = new DateTime(2010, 12, 18),
                Price = 3.99M,
                Sizes = new string[] { "Small", "Medium", "Large" }
            };
            string serializedJson = JsonConvert.SerializeObject(product);
            JObject o = JObject.Parse(serializedJson);
            string name = (string)o["Name"];
            //Apple
            JArray sizes = (JArray)o["Sizes"];
            string smallest = (string)sizes[0];
            Response.Write(name + "," + smallest + "<br/>");//输出Small
            //SelectToken
            smallest = (string)o.SelectToken("Sizes[0]");
            Response.Write(smallest + "<br/>");//输出Small
            //SelectToken with Linq
            var sizeLen5 = o["Sizes"].Select(i => (string)i).Where(i => i.Length == 5).ToList<string>();
           foreach (var size in sizeLen5)
            {
                Response.Write((string)size+ " <br/>");
            };//输出Small和Large
注:JArray表示一个Json集合，JObject表示一个Json对象。
posted @ 2011-06-11 15:03 资源收集 阅读(117) 评论(0) 编辑
Json.Net**学习****笔记**(一)
使用Newtonsoft.Json这是一个开源的Json.Net库。
下载地址：http://json.codeplex.com/releases/view/50552。当前版本为 Release 8
从下载到的源代码中获取Newtonsoft.Json.Net20.dll,添加到自己的工程中。
using Newtonsoft.Json;
定义类:
 public class Message
    {
        public string Address { get; set; }
        [JsonProperty(TypeNameHandling = TypeNameHandling.All)]
        public object Body { get; set; }
    }
    public class SearchDetails
    {
        public string Query { get; set; }
        public string Language { get; set; }
    }
测试:
           Message message = new Message
            {
                Address = "http://google.com",
                Body = new SearchDetails { Query = "Json.Net", Language = "en-us" }
            };
            string jsonMsg = JsonConvert.SerializeObject(message, Formatting.Indented);//Indented表示以缩进形式显示结果
            System.Diagnostics.Debug.Write(jsonMsg);
            Message deserialized = JsonConvert.DeserializeObject<Message>(jsonMsg);
            SearchDetails searchDetails = (SearchDetails)deserialized.Body;
            Response.Write(searchDetails.Query + "," + searchDetails.Language + "<br/>");
Debug输出结果格式：
{
  "Address": "http://google.com",
  "Body": {
    "$type": "TestJsonSerialization.SearchDetails, TestJsonSerialization",
    "Query": "Json.Net",
    "Language": "en-us"
  }
}
注：1.JsonProperty标记字段或属性，用来控制它作为一个Json对象的属性序列化。
       2.TypeNameHandling 用来为Json序列化指定类型名。它有几个枚举值：
|Member|Description|
|----|----|
|None|Do not include the .NET type name when serializing types.|
|Objects|Include the .NET type name when serializing into a JSON object structure.|
|Arrays|Include the .NET type name when serializing into a JSON array structure.|
|Auto|Include the .NET type name when the type of the object being serialized is not the same as its declared type.|
|All|Always include the .NET type name when serializing.|
|**上一篇 **[在ASP.NET MVC 中获取当前URL、controller、action(转载)](http://www.verydemo.com/demo_c360_i45118.html)|**下一篇 **[ASP.NET-关于Container dataitem 与 eval方法介绍（转帖）](http://www.verydemo.com/demo_c360_i45120.html)|
**总结：**{ "Address": "http://google.com", "Body": { "$type": "TestJsonSerialization.SearchDetails, TestJsonSerialization", "Query": "Json.Net", "Language": "en-us" }}
