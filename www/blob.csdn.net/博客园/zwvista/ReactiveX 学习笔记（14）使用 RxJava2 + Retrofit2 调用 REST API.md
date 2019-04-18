# ReactiveX 学习笔记（14）使用 RxJava2 + Retrofit2 调用 REST API - zwvista - 博客园

## [ReactiveX 学习笔记（14）使用 RxJava2 + Retrofit2 调用 REST API](https://www.cnblogs.com/zwvista/p/9497908.html)

### JSON : Placeholder

[JSON : Placeholder (https://jsonplaceholder.typicode.com/)](https://jsonplaceholder.typicode.com/) 是一个用于测试的 REST API 网站。

以下使用 RxJava2 + Retrofit2 调用该网站的 REST API，获取字符串以及 JSON 数据。
- GET /posts/1
- GET /posts
- POST /posts
- PUT /posts/1
- DELETE /posts/1

所有 GET API 都返回JSON数据，格式（JSON-Schema）如下：

```
{
  "type":"object",
  "properties": {
    "userId": {"type" : "integer"},
    "id": {"type" : "integer"},
    "title": {"type" : "string"},
    "body": {"type" : "string"}
  }
}
```

### 创建工程

打开 Intellij IDEA，File / New / Project...

在 New Project 向导的第1页，选 Gradle，Project SDK 选 1.8，Additional Libraries and Frameworks 选 Java + Kotlin(Java)。

在向导的第2页填上 ArtifactId

在向导的第3页选中 use auto-import

在向导的第4页点击 Finish 按钮创建工程

build.gradle 内容如下

```
plugins {
    id 'java'
    id 'org.jetbrains.kotlin.jvm' version '1.2.60'
}

version '1.0-SNAPSHOT'

sourceCompatibility = 1.8

repositories {
    mavenCentral()
}

dependencies {
    compile "org.jetbrains.kotlin:kotlin-stdlib-jdk8"
    testCompile group: 'junit', name: 'junit', version: '4.12'
}

compileKotlin {
    kotlinOptions.jvmTarget = "1.8"
}
compileTestKotlin {
    kotlinOptions.jvmTarget = "1.8"
}
```

将 dependencies 这部分的内容改为：

```
def retrofit_version = '2.4.0'
dependencies {
    implementation "org.jetbrains.kotlin:kotlin-stdlib-jdk8"
    testImplementation group: 'junit', name: 'junit', version: '4.12'
    implementation 'io.reactivex.rxjava2:rxjava:2.1.16'
    implementation 'io.reactivex.rxjava2:rxkotlin:2.2.0'
    implementation "com.squareup.retrofit2:retrofit:$retrofit_version"
    implementation "com.squareup.retrofit2:adapter-rxjava2:$retrofit_version"
    implementation "com.squareup.retrofit2:converter-gson:$retrofit_version"
    implementation "com.squareup.retrofit2:converter-scalars:$retrofit_version"
}
```

这一段一共引用了 RxJava, RxKotlin, Retrofit 3个库。

其中 Retrofit 这个库中还包含了 RxJava 的适配器，以及 Gson 和字符串的转换器。

### Post 对象

[jsonschema2pojo](http://www.jsonschema2pojo.org/) 可以将 JSON 数据或格式自动转换为 Java 的 POJO 类。

```
data class Post(val userId: Int, val id: Int, val title: String, val body: String) {
    override fun toString() =
        "Post {userId = $userId, id = $id, title = \"$title\", body = \"${body.replace("\n", "\\n")}\"}"
}
```

Post 对象负责 Kotlin 对象与 JSON 数据之间的相互转换。

由于两者字段名相同，这里不需要使用注解。

```
// 如果需要加上注解的话
data class Post(@SerializedName("userId") @Expose val userId: Int,
                @SerializedName("id") @Expose val id: Int,
                @SerializedName("title") @Expose val title: String,
                @SerializedName("body") @Expose val body: String) {
// ...
}
```

### Retrofit 接口

```
interface RestPost {
    @GET
    fun getPostAsString(@Url url: String): Observable<String>
    @GET("posts/{id}")
    fun getPostAsJson(@Path("id") id: Int): Observable<Post>
    @GET("posts")
    fun getPosts(): Observable<List<Post>>
    @FormUrlEncoded
    @POST("posts")
    fun createPost(@Field("userId") userId: Int,
                   @Field("title") title: String,
                   @Field("body") body: String): Observable<Post>
    @FormUrlEncoded
    @PUT("posts/{id}")
    fun updatePost(@Field("userId") userId: Int,
                   @Path("id") id: Int,
                   @Field("title") title: String,
                   @Field("body") body: String): Observable<Post>
    @DELETE("posts/{id}")
    fun deletePost(@Path("id") id: Int): Observable<String>
}
```

Retrofit 库使用专用接口调用 REST API。
- 接口中的每一个方法都对应于一种 API 调用。
- 注解 @GET @POST @PUT @DELETE 表示 API 调用时所使用的 HTTP 方法。
- 注解 @GET 中带的值表示 API 调用时所包含的相对路径，其中可包含路径变量。

"posts/{id}" 中的 {id} 为路径变量。
- 注解 @Url 表示参数为路径。
- 注解 @Path("id") 表示参数为路径变量。
- 注解 @Field 表示参数为 HTTP 请求体中的键值对。
- 使用注解 @Field 的方法必须加上注解 @FormUrlEncoded。

### Retrofit 对象

```
val retrofitJson: Retrofit = Retrofit.Builder()
    .baseUrl("https://jsonplaceholder.typicode.com/")
    .addCallAdapterFactory(RxJava2CallAdapterFactory.create())
    .addConverterFactory(GsonConverterFactory.create())
    .build()
val retrofitString: Retrofit = Retrofit.Builder()
    .baseUrl("https://jsonplaceholder.typicode.com/")
    .addCallAdapterFactory(RxJava2CallAdapterFactory.create())
    .addConverterFactory(ScalarsConverterFactory.create())
    .build()
```
- retrofitJson 对象用于处理 REST API 所返回的 JSON 数据。
- retrofitString 对象用于处理 REST API 所返回的字符串数据。

### 调用 REST API

```
fun getPostAsString(): Observable<String> =
    retrofitString.create(RestPost::class.java)
        .getPostAsString("posts/1")

fun getPostAsJson(): Observable<Post> =
    retrofitJson.create(RestPost::class.java)
        .getPostAsJson(1)

fun getPosts(n: Long): Observable<Post> =
    retrofitJson.create(RestPost::class.java)
        .getPosts().flatMapIterable { x -> x }.take(n)

fun createPost(): Observable<Post> =
    retrofitJson.create(RestPost::class.java)
        .createPost(101, "test title", "test body")

fun updatePost(): Observable<Post> =
    retrofitJson.create(RestPost::class.java)
        .updatePost(101, 1, "test title", "test body")

fun deletePost(): Observable<String> =
    retrofitString.create(RestPost::class.java)
        .deletePost(1)
```
- getPostAsString 函数取出第1个Post，返回字符串
- getPostAsJson 函数取出第1个Post，返回Post对象
- getPosts 函数取出前n个Post，返回n个Post对象
- createPost 函数创建1个Post，返回所创建的Post对象
- updatePost 函数更新第1个Post，返回所更新的Post对象
- deletePost 函数删除第1个Post，返回字符串

### main 函数

```
fun main(args: Array<String>) {
    getPostAsString().subscribe(::println)
    getPostAsJson().subscribe(::println)
    getPosts(2).subscribe(::println)
    createPost().subscribe(::println)
    updatePost().subscribe(::println)
    deletePost().subscribe(::println)
}
```

### 输出结果

```
{
  "userId": 1,
  "id": 1,
  "title": "sunt aut facere repellat provident occaecati excepturi optio reprehenderit",
  "body": "quia et suscipit\nsuscipit recusandae consequuntur expedita et cum\nreprehenderit molestiae ut ut quas totam\nnostrum rerum est autem sunt rem eveniet architecto"
}
Post {userId = 1, id = 1, title = "sunt aut facere repellat provident occaecati excepturi optio reprehenderit", body = "quia et suscipit\nsuscipit recusandae consequuntur expedita et cum\nreprehenderit molestiae ut ut quas totam\nnostrum rerum est autem sunt rem eveniet architecto"}
Post {userId = 1, id = 1, title = "sunt aut facere repellat provident occaecati excepturi optio reprehenderit", body = "quia et suscipit\nsuscipit recusandae consequuntur expedita et cum\nreprehenderit molestiae ut ut quas totam\nnostrum rerum est autem sunt rem eveniet architecto"}
Post {userId = 1, id = 2, title = "qui est esse", body = "est rerum tempore vitae\nsequi sint nihil reprehenderit dolor beatae ea dolores neque\nfugiat blanditiis voluptate porro vel nihil molestiae ut reiciendis\nqui aperiam non debitis possimus qui neque nisi nulla"}
Post {userId = 101, id = 101, title = "test title", body = "test body"}
Post {userId = 101, id = 1, title = "test title", body = "test body"}
{}
```


