
# MongoDB—驱动实践 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月30日 21:52:33[seven-soft](https://me.csdn.net/softn)阅读数：771


作为系列的最后一篇，得要说说C\#驱动对mongodb的操作，目前驱动有两种：官方驱动和samus驱动，不过我个人还是喜欢后者，
因为提供了丰富的linq操作，相当方便。
官方驱动：[https://github.com/mongodb/mongo-csharp-driver/downloads](https://github.com/mongodb/mongo-csharp-driver/downloads)。下载后，还提供了一个酷似msdn的帮助文档。
samus驱动：[https://github.com/samus/mongodb-csharp/downloads](https://github.com/samus/mongodb-csharp/downloads)。
下面就具体看看samus驱动，[https://github.com/samus/mongodb-csharp/blob/master/examples/Simple/Main.cs](https://github.com/samus/mongodb-csharp/blob/master/examples/Simple/Main.cs)上面提供了
一个简单的demo，大体上看看我们就知道怎么玩了。
一： 实践
1：我们建立一个Person实体，MongoAlias特性表示取别名，这里的ID值将会覆盖掉数据库自动生成的_id。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
1\#region数据实体
2///<summary>
3///数据实体
4///</summary>
5publicclassPerson
6{
7[MongoAlias("_id")]
8publicstringID {get;set; }
9
10publicstringName {get;set; }
11
12publicintAge {get;set; }
13
14publicDateTime CreateTime {get;set; }
15}
16\#endregion![复制代码](http://common.cnblogs.com/images/copycode.gif)

2：初始化一些变量
![复制代码](http://common.cnblogs.com/images/copycode.gif)
1stringconnectionString =string.Empty;
2
3stringdatabaseName =string.Empty;
4
5stringcollectionName =string.Empty;
6
7staticMongodbHelper<T> mongodb;
8
9\#region初始化操作
10///<summary>
11///初始化操作
12///</summary>
13publicMongodbHelper()
14{
15connectionString ="Server=127.0.0.1:2222";
16databaseName ="shopex";
17collectionName ="person";
18}
19\#endregion![复制代码](http://common.cnblogs.com/images/copycode.gif)

3：为了方便T的继承类使用linq功能，我们还需要映射一下。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
1\#region实现linq查询的映射配置
2///<summary>
3///实现linq查询的映射配置
4///</summary>
5publicMongoConfiguration configuration
6{
7get
8{
9varconfig =newMongoConfigurationBuilder();
10
11config.Mapping(mapping =>
12{
13mapping.DefaultProfile(profile =>
14{
15profile.SubClassesAre(t => t.IsSubclassOf(typeof(T)));
16});
17mapping.Map<T>();
18mapping.Map<T>();
19});
20
21config.ConnectionString(connectionString);
22
23returnconfig.BuildConfiguration();
24}
25}
26\#endregion![复制代码](http://common.cnblogs.com/images/copycode.gif)

4：下面是一些基本的CURD的代码，跟写EF代码很类似，写起来好舒服。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
1\#region插入操作
2///<summary>
3///插入操作
4///</summary>
5///<param name="person"></param>
6///<returns></returns>
7publicvoidInsert(T t)
8{
9using(Mongo mongo =newMongo(configuration))
10{
11try
12{
13mongo.Connect();
14
15vardb = mongo.GetDatabase(databaseName);
16
17varcollection = db.GetCollection<T>(collectionName);
18
19collection.Insert(t,true);
20
21mongo.Disconnect();
22
23}
24catch(Exception)
25{
26mongo.Disconnect();
27throw;
28}
29}
30}
31\#endregion
32
33\#region更新操作
34///<summary>
35///更新操作
36///</summary>
37///<param name="person"></param>
38///<returns></returns>
39publicvoidUpdate(T t, Expression<Func<T,bool>> func)
40{
41using(Mongo mongo =newMongo(configuration))
42{
43try
44{
45mongo.Connect();
46
47vardb = mongo.GetDatabase(databaseName);
48
49varcollection = db.GetCollection<T>(collectionName);
50
51collection.Update<T>(t, func,true);
52
53mongo.Disconnect();
54
55}
56catch(Exception)
57{
58mongo.Disconnect();
59throw;
60}
61}
62}
63\#endregion
64
65\#region获取集合
66///<summary>
67///获取集合
68///</summary>
69///<param name="person"></param>
70///<returns></returns>
71publicList<T> List(intpageIndex,intpageSize, Expression<Func<T,bool>> func,outintpageCount)
72{
73pageCount =0;
74
75using(Mongo mongo =newMongo(configuration))
76{
77try
78{
79mongo.Connect();
80
81vardb = mongo.GetDatabase(databaseName);
82
83varcollection = db.GetCollection<T>(collectionName);
84
85pageCount = Convert.ToInt32(collection.Count());
86
87varpersonList = collection.Linq().Where(func).Skip(pageSize * (pageIndex -1))
88.Take(pageSize).Select(i => i).ToList();
89
90mongo.Disconnect();
91
92returnpersonList;
93
94}
95catch(Exception)
96{
97mongo.Disconnect();
98throw;
99}
100}
101}
102\#endregion
103
104\#region读取单条记录
105///<summary>
106///读取单条记录
107///</summary>
108///<param name="person"></param>
109///<returns></returns>
110publicT Single(Expression<Func<T,bool>> func)
111{
112using(Mongo mongo =newMongo(configuration))
113{
114try
115{
116mongo.Connect();
117
118vardb = mongo.GetDatabase(databaseName);
119
120varcollection = db.GetCollection<T>(collectionName);
121
122varsingle = collection.Linq().FirstOrDefault(func);
123
124mongo.Disconnect();
125
126returnsingle;
127
128}
129catch(Exception)
130{
131mongo.Disconnect();
132throw;
133}
134}
135}
136\#endregion
137
138\#region删除操作
139///<summary>
140///删除操作
141///</summary>
142///<param name="person"></param>
143///<returns></returns>
144publicvoidDelete(Expression<Func<T,bool>> func)
145{
146using(Mongo mongo =newMongo(configuration))
147{
148try
149{
150mongo.Connect();
151
152vardb = mongo.GetDatabase(databaseName);
153
154varcollection = db.GetCollection<T>(collectionName);
155
156//这个地方要注意，一定要加上T参数，否则会当作object类型处理
157//导致删除失败
158collection.Remove<T>(func);
159
160mongo.Disconnect();
161
162}
163catch(Exception)
164{
165mongo.Disconnect();
166throw;
167}
168}
169}
170\#endregion![复制代码](http://common.cnblogs.com/images/copycode.gif)


5.   好，我们开一下2222端口，由于前前篇我已经把这个mongodb做成了服务，现在就直接连过去了，并做一下对Name的索引。
![](http://pic002.cnblogs.com/images/2012/214741/2012030900455844.png)
6. 一切准备妥当，我们做下基本的操作，比如这里我添加一千条数据，注意我开启的是安全模式，如果插入不成功，将会抛出异常。
<1> Add:
![复制代码](http://common.cnblogs.com/images/copycode.gif)
1staticvoidMain(string[] args)
2{
3MongodbHelper<Person> helper =newMongodbHelper<Person>();
4
5//插入1000条数据
6for(inti =0; i <1000; i++)
7{
8helper.Insert(newPerson()
9{
10ID = Guid.NewGuid().ToString(),
11Name ="jack"+ i,
12Age = i,
13CreateTime = DateTime.Now
14});
15}
16
17Console.WriteLine("插入成功");
18
19Console.Read();
20}![复制代码](http://common.cnblogs.com/images/copycode.gif)
![](http://pic002.cnblogs.com/images/2012/214741/2012030900193928.png)
乍一看显示的数据以为有问题，为什么没有出现jack0或者jack999，不过find的一下后心情舒坦了。

<2> update:   这里就把jack941的名字改掉“mary”
![复制代码](http://common.cnblogs.com/images/copycode.gif)
1staticvoidMain(string[] args)
2{
3MongodbHelper<Person> helper =newMongodbHelper<Person>();
4
5//修改jack941改成mary
6varsingle = helper.Single(i => i.Name =="jack941");
7single.Name ="mary";
8helper.Update(single, i => i.ID == single.ID);
9
10Console.WriteLine("修改成功");
11Console.Read();
12}![复制代码](http://common.cnblogs.com/images/copycode.gif)
![](http://pic002.cnblogs.com/images/2012/214741/2012030900241194.png)
<3>Delete：  删除mary这条记录
![复制代码](http://common.cnblogs.com/images/copycode.gif)
1staticvoidMain(string[] args)
2{
3MongodbHelper<Person> helper =newMongodbHelper<Person>();
4
5//删除mary这个记录
6helper.Delete(i => i.Name =="mary");
7
8Console.WriteLine("删除成功");
9Console.Read();
10}![复制代码](http://common.cnblogs.com/images/copycode.gif)
![](http://pic002.cnblogs.com/images/2012/214741/2012030900272887.png)
<4> list操作： 这里我获取一下名字里面带9的人数列表
![复制代码](http://common.cnblogs.com/images/copycode.gif)
1staticvoidMain(string[] args)
2{
3MongodbHelper<Person> helper =newMongodbHelper<Person>();
4
5intpagecount;
6
7//获取名字里面带9的人数
8varlist = helper.List(1,20, i => i.Name.Contains("9"),outpagecount);
9
10Console.Read();
11}![复制代码](http://common.cnblogs.com/images/copycode.gif)
![](http://pic002.cnblogs.com/images/2012/214741/2012030900311418.png)
总的运行代码
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)[
](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)![复制代码](http://common.cnblogs.com/images/copycode.gif)
1usingSystem;
2usingSystem.Collections.Generic;
3usingSystem.Linq;
4usingSystem.Text;
5usingSystem.Configuration;
6usingSystem.Linq.Expressions;
7
8usingMongoDB.Configuration;
9usingMongoDB.Linq;
10usingMongoDB.Attributes;
11
12
13namespaceMongoDB.Test
14{
15publicclassMongodbHelper<T>whereT :class
16{
17stringconnectionString =string.Empty;
18
19stringdatabaseName =string.Empty;
20
21stringcollectionName =string.Empty;
22
23staticMongodbHelper<T> mongodb;
24
25\#region初始化操作
26///<summary>
27///初始化操作
28///</summary>
29publicMongodbHelper()
30{
31connectionString ="Server=127.0.0.1:2222";
32databaseName ="shopex";
33collectionName ="person";
34}
35\#endregion
36
37\#region实现linq查询的映射配置
38///<summary>
39///实现linq查询的映射配置
40///</summary>
41publicMongoConfiguration configuration
42{
43get
44{
45varconfig =newMongoConfigurationBuilder();
46
47config.Mapping(mapping =>
48{
49mapping.DefaultProfile(profile =>
50{
51profile.SubClassesAre(t => t.IsSubclassOf(typeof(T)));
52});
53mapping.Map<T>();
54mapping.Map<T>();
55});
56
57config.ConnectionString(connectionString);
58
59returnconfig.BuildConfiguration();
60}
61}
62\#endregion
63
64\#region插入操作
65///<summary>
66///插入操作
67///</summary>
68///<param name="person"></param>
69///<returns></returns>
70publicvoidInsert(T t)
71{
72using(Mongo mongo =newMongo(configuration))
73{
74try
75{
76mongo.Connect();
77
78vardb = mongo.GetDatabase(databaseName);
79
80varcollection = db.GetCollection<T>(collectionName);
81
82collection.Insert(t,true);
83
84mongo.Disconnect();
85
86}
87catch(Exception)
88{
89mongo.Disconnect();
90throw;
91}
92}
93}
94\#endregion
95
96\#region更新操作
97///<summary>
98///更新操作
99///</summary>
100///<param name="person"></param>
101///<returns></returns>
102publicvoidUpdate(T t, Expression<Func<T,bool>> func)
103{
104using(Mongo mongo =newMongo(configuration))
105{
106try
107{
108mongo.Connect();
109
110vardb = mongo.GetDatabase(databaseName);
111
112varcollection = db.GetCollection<T>(collectionName);
113
114collection.Update<T>(t, func,true);
115
116mongo.Disconnect();
117
118}
119catch(Exception)
120{
121mongo.Disconnect();
122throw;
123}
124}
125}
126\#endregion
127
128\#region获取集合
129///<summary>
130///获取集合
131///</summary>
132///<param name="person"></param>
133///<returns></returns>
134publicList<T> List(intpageIndex,intpageSize, Expression<Func<T,bool>> func,outintpageCount)
135{
136pageCount =0;
137
138using(Mongo mongo =newMongo(configuration))
139{
140try
141{
142mongo.Connect();
143
144vardb = mongo.GetDatabase(databaseName);
145
146varcollection = db.GetCollection<T>(collectionName);
147
148pageCount = Convert.ToInt32(collection.Count());
149
150varpersonList = collection.Linq().Where(func).Skip(pageSize * (pageIndex -1))
151.Take(pageSize).Select(i => i).ToList();
152
153mongo.Disconnect();
154
155returnpersonList;
156
157}
158catch(Exception)
159{
160mongo.Disconnect();
161throw;
162}
163}
164}
165\#endregion
166
167\#region读取单条记录
168///<summary>
169///读取单条记录
170///</summary>
171///<param name="person"></param>
172///<returns></returns>
173publicT Single(Expression<Func<T,bool>> func)
174{
175using(Mongo mongo =newMongo(configuration))
176{
177try
178{
179mongo.Connect();
180
181vardb = mongo.GetDatabase(databaseName);
182
183varcollection = db.GetCollection<T>(collectionName);
184
185varsingle = collection.Linq().FirstOrDefault(func);
186
187mongo.Disconnect();
188
189returnsingle;
190
191}
192catch(Exception)
193{
194mongo.Disconnect();
195throw;
196}
197}
198}
199\#endregion
200
201\#region删除操作
202///<summary>
203///删除操作
204///</summary>
205///<param name="person"></param>
206///<returns></returns>
207publicvoidDelete(Expression<Func<T,bool>> func)
208{
209using(Mongo mongo =newMongo(configuration))
210{
211try
212{
213mongo.Connect();
214
215vardb = mongo.GetDatabase(databaseName);
216
217varcollection = db.GetCollection<T>(collectionName);
218
219//这个地方要注意，一定要加上T参数，否则会当作object类型处理
220//导致删除失败
221collection.Remove<T>(func);
222
223mongo.Disconnect();
224
225}
226catch(Exception)
227{
228mongo.Disconnect();
229throw;
230}
231}
232}
233\#endregion
234}
235
236\#region数据实体
237///<summary>
238///数据实体
239///</summary>
240publicclassPerson
241{
242[MongoAlias("_id")]
243publicstringID {get;set; }
244
245publicstringName {get;set; }
246
247publicintAge {get;set; }
248
249publicDateTime CreateTime {get;set; }
250}
251\#endregion
252}![复制代码](http://common.cnblogs.com/images/copycode.gif)



