# Kotlin : Retrofit + RxAndroid + Realm - =朝晖= - 博客园
# [Kotlin : Retrofit + RxAndroid + Realm](https://www.cnblogs.com/dhcn/p/7124931.html)
https://jqs7.com/kotlin-retrofit-rxandroid-realm/
> 
原作者：Ahmed Rizwan
原文链接：[Kotlin : Retrofit + RxAndroid + Realm](https://medium.com/@ahmedrizwan/kotlin-retrofit-rxandroid-realm-39d7be5dc9dc)
译文作者：[Jqs7](https://jqs7.com/)
审阅：[@SusuwANjr](https://telegram.me/SusuwANjr)
在这篇文章里面我会使用一些库（就我平时用那些），比如使用 Kotlin 和 Rx 来写 Retrofit 和 Realm 。
如果你是个 Retrofit 新手……建议你阅读一下[这篇文章](https://medium.com/@ahmedrizwan/rxandroid-and-retrofit-2-0-66dc52725fff#.51syz2rr5)——保证~~亮瞎你狗眼~~让你眼前一亮！如果你不知道 Rx & Kotlin —— 看[这里](https://medium.com/@ahmedrizwan/rxandroid-and-kotlin-part-1-f0382dc26ed8#.i0smhvwlm)!
那我们开始吧：Kotlin + Rx + Retrofit + Realm 如果你像我一样，那一定很讨厌一大片的冗余的代码 咳咳 [Java](http://lib.csdn.net/base/java)6 。自我开始从事[安卓](http://lib.csdn.net/base/android)开发以来，经常遇到一些“什么鬼？！”的问题。
在语言上我觉得 Kotlin 大法就是好！像 Retrofit ，Realm 以及 RxAndroid 这些库真心省了不少代码。
蜀黍给你讲个可怕的故事：有次我负责一个项目，里面有 12 个 POJO 类，还有 12 个[数据库](http://lib.csdn.net/base/mysql)表类（ORM），还有 12 个数据库模型映射类。如果你数学学得好的话，你就会发现……特喵的有 36 个类！还不只是这样，如果要改一个类，那就意味着 3 个类全都得改。
![没错这就是我…](https://o4qmz7rto.qnssl.com/image/c/18/1534856faa81bf03a15d60769b45b.jpeg)
我当时就这表情…
如果我们不用 ORM 的话，我们还得去写模型到数据库的映射类，真特么得吓尿我！
所以解决办法是啥？对我来说就是 Realm + Retrofit 啦 (＾o＾)ﾉ 我还用了 RxAndroid ，因为有了 Rx 生活更美好，当然还有 Kotlin ，因为……
> 
Kotlin 大法好！Kotlin 千秋万代，一统江湖！
把这些东西结合起来，那 36 个类所做的事情只要 12 个类就能完成了 （本来就应该这样嘛 (ゝ∀･)）。不光这样，代码也会因为 Kotlin 而变得更加简洁，表达性更为良好！
#### 小例子：从 Github API 获取数据
一个常见的应用场景：从 API 获取数据，把数据存储到数据库并显示出来。
完成品就像下图这样，只是一个很简单的例子……希望可以覆盖到基本的姿势点！
![](https://o4qmz7rto.qnssl.com/image/3/2f/eeb3589643170a83d979cb9c7a49f.png)
下面就讲讲该怎么把这个 app 给撸出来……
#### 创建项目并启用 Kotlin
创建完项目我们做的第一件事就是启用 Kotlin ——首先要在 [Android](http://lib.csdn.net/base/android) Studio 里面装好 Kotlin 插件。
我创建了一个只有一个 MainActivity 的空项目——直接打开 build.gradle 文件，然后打开 action 列表（快捷键 ctrl+shift+a）
![](https://o4qmz7rto.qnssl.com/image/f/3d/fe6f57c89d44777f0ddacd72249a2.png)
![](https://o4qmz7rto.qnssl.com/image/e/0d/98233457f4bfb1ae0445f6d8e9419.png)
![](https://o4qmz7rto.qnssl.com/image/3/78/0291aba3e44ff8c69836f9ea8ae75.png)
吼的！我们现在可以用 Kotlin 来写代码了！
![](https://o4qmz7rto.qnssl.com/image/6/de/d36e79735ea7f58c9ce235cfdbba1.png)
现在的 MainActivity，标准格式，木有惊喜
我们可以像下面这样把 java 代码转成 Kotlin
![](https://o4qmz7rto.qnssl.com/image/5/4d/f5b746bb4da9bf84f7d6533716636.png)
现在 MainActivity 变成了下面这样……
![](https://o4qmz7rto.qnssl.com/image/9/a0/1b17e9ce4d623f6cb13f18c1520cf.png)
Holy mother of Kotlin!
#### 添加 Rx — Retrofit — Realm 依赖
```
//Realm
    compile 'io.realm:realm-android:0.87.4'
    //RxAndroid
    compile 'io.reactivex:rxandroid:1.1.0'
    compile 'io.reactivex:rxjava:1.1.0'
    //RxBindings
    compile 'com.jakewharton.rxbinding:rxbinding:0.3.0'
    //Retrofit
    compile 'com.squareup.retrofit:retrofit:2.0.0-beta2'
    //Retrofit Adapter and Converter
    compile 'com.squareup.retrofit:adapter-rxjava:2.0.0-beta2'
    compile 'com.squareup.retrofit:converter-gson:2.0.0-beta2'
    //Glide
    compile 'com.github.bumptech.glide:glide:3.7.0'
    //Databinding and Realm Annotations
    kapt 'com.android.databinding:compiler:1.0-rc4'
    kapt "io.realm:realm-annotations:0.87.4"
    kapt "io.realm:realm-annotations-processor:0.87.4"
```
我还使用了 Databinding ！因为用 Databinding 的人比较帅 (ゝ∀･)
你大概想问： kapt 是什么鬼？好吧，它是Kotlin内置的注解处理器。
![](https://o4qmz7rto.qnssl.com/image/c/93/81cea35499464953eead7c53f508e.png)
把这个添加到 build.gradle
在这个项目里面我还得把 kapt 的 generateStubs 给打开，因为要用它来生成代码。
万事俱备！开始写代码啦！
#### Model
我们使用的 API 地址是：
`https://api.github.com/users/ahmedrizwan`
服务端返回的内容如下：
```
{
      "login": "ahmedrizwan",
      "id": 4357275,
      "avatar_url": "https://avatars.githubusercontent.com/u/4357275?v=3",
      "gravatar_id": "",
      "url": "https://api.github.com/users/ahmedrizwan",
      "html_url": "https://github.com/ahmedrizwan",
      "followers_url": "https://api.github.com/users/ahmedrizwan/followers",
      "following_url": "https://api.github.com/users/ahmedrizwan/following{/other_user}",
      "gists_url": "https://api.github.com/users/ahmedrizwan/gists{/gist_id}",
      "starred_url": "https://api.github.com/users/ahmedrizwan/starred{/owner}{/repo}",
      "subscriptions_url": "https://api.github.com/users/ahmedrizwan/subscriptions",
      "organizations_url": "https://api.github.com/users/ahmedrizwan/orgs",
      "repos_url": "https://api.github.com/users/ahmedrizwan/repos",
      "events_url": "https://api.github.com/users/ahmedrizwan/events{/privacy}",
      "received_events_url": "https://api.github.com/users/ahmedrizwan/received_events",
      "type": "User",
      "site_admin": false,
      "name": "ahmed",
      "company": null,
      "blog": null,
      "location": "Rawalpindi, Pakistan",
      "email": null,
      "hireable": null,
      "bio": null,
      "public_repos": 9,
      "public_gists": 0,
      "followers": 5,
      "following": 9,
      "created_at": "2013-05-06T18:32:59Z",
      "updated_at": "2015-08-29T18:17:58Z"
    }
```
在这个例子里面，只要把 *id, name, avatar*url 和 public*repos* 从返回信息里面提取出来就够了。所以 model 类就像下面这样（同时也是个 realm 类）：
```
@RealmClass
    open class Github : RealmObject() {
        @PrimaryKey
        @SerializedName("id")
        @Expose
        open var id: Int = 0
        @SerializedName("avatar_url")
        @Expose
        open var avatarUrl: String? = null
        @SerializedName("name")
        @Expose
        open var name: String? = null
        @SerializedName("public_repos")
        @Expose
        open var publicRepos: Int? = null
    }
```
> 
小提示：如果你需要JSON返回的所以属性的话，我建议你使用[这个网站](http://www.jsonschema2pojo.org/)来生成一个 POJO 类，再像上面说的那样把 [Java](http://lib.csdn.net/base/java) 代码转成 Kotlin 代码。
我们就用 *RealmClass* 注解说起吧，在 Kotlin 中需要为 Realm model 类加上这个注解，Realm 才能生成 Realm 代理类，*PrimaryKey* 也是一个 Realm 注解，表示主键属性（废话），剩下的就是 Gson 注解了……
open关键字正好跟 Java 里面的 final 相反。默认情况下，Kotlin 的类就是 final 的，如果你如果要一个类可以被继承，就必须把它声明为 open。这对于属性也是一样的，比如上面的 model 类里面，*name* 这个属性，它拥有一个 getter 和 setter，为了使它们可以被重写（因为 Realm 需要），我们就得在属性的前面加上 open 关键字。
#### Retrofit 接口
因为我们的端点地址是：
`https://api.github.com/users/[some_user]`
所以接口写出来就是下面这样：
```
interface GithubService {  
  @GET("users/{username}")
  fun getGithubUser(@Path("username") username: String): Observable<Github>
}
```
上面我们直接返回了一个 Observable 的 Github，因为 Retrofit 集成了 Rx，简直就是碉堡了！
#### Retrofit Builder
```
val gson = GsonBuilder().setExclusionStrategies(object : ExclusionStrategy {  
      override fun shouldSkipField(f: FieldAttributes): Boolean {
        return f.declaringClass == RealmObject::class.java
      }
      override fun shouldSkipClass(clazz: Class<*>): Boolean {
        return false
      }
    }).create()
    val retrofit: Retrofit = Retrofit.Builder()
        .addCallAdapterFactory(RxJavaCallAdapterFactory.create())
        .addConverterFactory(GsonConverterFactory.create(gson))
        .baseUrl("https://api.github.com/")
        .build()
    val githubService: GithubService = retrofit.create(
        GithubService::class.java)
```
因为用了 Realm ，所以我们得重新声明一个 Gson 实例，添加排除策略（exclusion strategy）来跳过Realm生成的属性，不然 Gson 对于这个 model 就没什么卯月了。
在下面就是一个加入 RxJavaCallAdapter 工厂类启用 Rx 集成的 Retrofit 实例，我们还加了一个使用了上面创建的 Gson 实例的 Gson 解析器。
#### Rx 魔法
```
githubService.getGithubUser("ahmedrizwan")
        .subscribeOn(Schedulers.newThread())
        .observeOn(AndroidSchedulers.mainThread())
        .subscribe(
            { user ->
              realm.beginTransaction()
              realm.copyToRealmOrUpdate(user)
              realm.commitTransaction()
              updateViews(binding, user)
            },
            { error ->
              Log.e("Error", error.message)
            }
        )
```
现在我们所做的就是：获取 observable 对象，订阅之，并获取 Github 对象。只有一获取到 Github 对象，就可以很简单的把用户保存到 Realm 数据库里面了。
#### 缓存
缓存也是可以有的 (=ﾟωﾟ)=，我们可以一开始就可以获取 Realm 数据库里面的数据（如果存在的话），就像下面这样：
```
val realm = Realm.getDefaultInstance()
    //get user if it's already saved
    val savedUser: Github? = RealmQuery.createQuery(realm,
        Github::class.java).findFirst()
    updateViews(binding, savedUser)
```
到这里就已经差不多完成了，接下来就差运行…![](https://o4qmz7rto.qnssl.com/image/3/2f/eeb3589643170a83d979cb9c7a49f.png)
鼓个掌
你可以在[这里](https://github.com/ahmedrizwan/RxRealmRetroKotlin/tree/master)找到这个例子的所有代码，希望这篇文章对你有所帮助。
Happy coding!

