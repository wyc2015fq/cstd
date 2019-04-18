# ReactiveX 学习笔记（15）使用 Rx.NET + Json.NET 调用 REST API - zwvista - 博客园

## [ReactiveX 学习笔记（15）使用 Rx.NET + Json.NET 调用 REST API](https://www.cnblogs.com/zwvista/p/9500975.html)

### JSON : Placeholder

[JSON : Placeholder (https://jsonplaceholder.typicode.com/)](https://jsonplaceholder.typicode.com/) 是一个用于测试的 REST API 网站。

以下使用 Task API/Rx.NET + Json.NET 调用该网站的 REST API，获取字符串以及 JSON 数据。
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

### Post 对象

```
public class Post
{
    [JsonProperty("userId")]
    public int UserId { get; set; }
    [JsonProperty("id")]
    public int Id { get; set; }
    [JsonProperty("title")]
    public string Title { get; set; }
    [JsonProperty("body")]
    public string Body { get; set; }

    public override string ToString() =>
        $"Post {{userId = {UserId}, id = {Id}, title = \"{Title}\", body = \"{Body.Replace("\n", "\\n")}\"}}";
}
```

Post 对象负责 .NET 对象和 JSON 数据的相互转换。

Post 类使用了 Json.NET 所提供的 attributes。

### Task API

```
public class PostDataStoreByTask
{
    protected HttpClient client = new HttpClient
    {
        BaseAddress = new Uri("https://jsonplaceholder.typicode.com/")
    };

    public async Task<string> GetPostAsString(int id) =>
    !CrossConnectivity.Current.IsConnected ? null : await client.GetStringAsync($"posts/{id}");

    public async Task<Post> GetPostAsJson(int id)
    {
        if (!CrossConnectivity.Current.IsConnected) return null;

        var json = await client.GetStringAsync($"posts/{id}");
        var item = await Task.Run(() => JsonConvert.DeserializeObject<Post>(json));

        return item;
    }

    public async Task<IEnumerable<Post>> GetPosts(int n)
    {
        IEnumerable<Post> items = new List<Post>();
        if (CrossConnectivity.Current.IsConnected)
        {
            var json = await client.GetStringAsync("posts");
            items = await Task.Run(() => JsonConvert.DeserializeObject<IEnumerable<Post>>(json));
        }
        return items.Take(n);
    }

    private StringContent GetStringContent(Post item) =>
    new StringContent(JsonConvert.SerializeObject(item), Encoding.UTF8, "application/json");

    public async Task<Post> CreatePost(Post item)
    {
        if (item == null || !CrossConnectivity.Current.IsConnected) return null;

        var response = await client.PostAsync("posts", GetStringContent(item));
        if (!response.IsSuccessStatusCode) return null;

        var json = await response.Content.ReadAsStringAsync();
        var item2 = await Task.Run(() => JsonConvert.DeserializeObject<Post>(json));

        return item2;
    }

    public async Task<Post> UpdatePost(Post item)
    {
        if (item == null || !CrossConnectivity.Current.IsConnected) return null;

        var response = await client.PutAsync($"posts/{item.Id}", GetStringContent(item));
        if (!response.IsSuccessStatusCode) return null;

        var json = await response.Content.ReadAsStringAsync();
        var item2 = await Task.Run(() => JsonConvert.DeserializeObject<Post>(json));

        return item2;
    }

    public async Task<string> DeletePost(int id)
    {
        if (!CrossConnectivity.Current.IsConnected) return null;

        var response = await client.DeleteAsync($"posts/{id}");
        if (!response.IsSuccessStatusCode) return null;

        var json = await response.Content.ReadAsStringAsync();
        return json;
    }
}
```
- getPostAsString 方法取出第1个Post，返回字符串
- getPostAsJson 方法取出第1个Post，返回Post对象
- getPosts 方法取出前n个Post，返回n个Post对象
- createPost 方法创建1个Post，返回所创建的Post对象
- updatePost 方法更新第1个Post，返回所更新的Post对象
- deletePost 方法删除第1个Post，返回字符串

### Rx.NET

```
public class PostDataStoreByRx
{
    protected HttpClient client = new HttpClient
    {
        BaseAddress = new Uri("https://jsonplaceholder.typicode.com/")
    };

    public IObservable<string> GetPostAsString(int id) =>
    !CrossConnectivity.Current.IsConnected ? Observable.Empty<string>() :
        client.GetStringAsync($"posts/{id}").ToObservable();

    public IObservable<Post> GetPostAsJson(int id) =>
    !CrossConnectivity.Current.IsConnected? Observable.Empty<Post>() :
        client.GetStringAsync($"posts/{id}").ToObservable()
        .Select(json => JsonConvert.DeserializeObject<Post>(json));

    public IObservable<Post> GetPosts(int n) =>
    !CrossConnectivity.Current.IsConnected ? Observable.Empty<Post>() :
        client.GetStringAsync("posts").ToObservable()
        .SelectMany(json => JsonConvert.DeserializeObject<IEnumerable<Post>>(json))
        .Take(n);

    private StringContent GetStringContent(Post item) =>
    new StringContent(JsonConvert.SerializeObject(item), Encoding.UTF8, "application/json");

    public IObservable<Post> CreatePost(Post item) =>
    item == null || !CrossConnectivity.Current.IsConnected ? Observable.Empty<Post>() :
        client.PostAsync("posts", GetStringContent(item)).ToObservable()
        .SelectMany(response => !response.IsSuccessStatusCode ? Observable.Empty<Post>() :
            response.Content.ReadAsStringAsync().ToObservable()
            .Select(json => JsonConvert.DeserializeObject<Post>(json)));

    public IObservable<Post> UpdatePost(Post item) =>
    item == null || !CrossConnectivity.Current.IsConnected? Observable.Empty<Post>() :
        client.PutAsync($"posts/{item.Id}", GetStringContent(item)).ToObservable()
        .SelectMany(response => !response.IsSuccessStatusCode? Observable.Empty<Post>() :
            response.Content.ReadAsStringAsync().ToObservable()
            .Select(json => JsonConvert.DeserializeObject<Post>(json)));

    public IObservable<string> DeletePost(int id) =>
    !CrossConnectivity.Current.IsConnected ? Observable.Empty<string>() :
        client.DeleteAsync($"posts/{id}").ToObservable()
        .SelectMany(response => !response.IsSuccessStatusCode ? Observable.Empty<string>() :
            response.Content.ReadAsStringAsync().ToObservable());
}
```

相比较 Task API，Rx.NET 代码简练但是比较难理解。

### Main 方法

```
public static void Main(string[] args)
{
    {
        var dataStore = new PostDataStoreByTask();
        Console.WriteLine(dataStore.GetPostAsString(1).Result);
        Console.WriteLine(dataStore.GetPostAsJson(1).Result);
        dataStore.GetPosts(2).Result.ToList().ForEach(Console.WriteLine);
        Console.WriteLine(dataStore.CreatePost(new Post
        {
            UserId = 101,
            Id = 0,
            Title = "test title",
            Body = "test body"
        }).Result);
        Console.WriteLine(dataStore.UpdatePost(new Post
        {
            UserId = 101,
            Id = 1,
            Title = "test title",
            Body = "test body"
        }).Result);
        Console.WriteLine(dataStore.DeletePost(1).Result);
    }
    {
        var dataStore = new PostDataStoreByRx();
        dataStore.GetPostAsString(1).Subscribe(Console.WriteLine);
        dataStore.GetPostAsJson(1).Subscribe(Console.WriteLine);
        dataStore.GetPosts(2).Do(Console.WriteLine).Subscribe();
        dataStore.CreatePost(new Post
        {
            UserId = 101,
            Id = 0,
            Title = "test title",
            Body = "test body"
        }).Subscribe(Console.WriteLine);
        dataStore.UpdatePost(new Post
        {
            UserId = 101,
            Id = 1,
            Title = "test title",
            Body = "test body"
        }).Subscribe(Console.WriteLine);
        dataStore.DeletePost(1).Subscribe(Console.WriteLine);
        Console.ReadKey();
    }
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
{
  "userId": 1,
  "id": 1,
  "title": "sunt aut facere repellat provident occaecati excepturi optio reprehenderit",
  "body": "quia et suscipit\nsuscipit recusandae consequuntur expedita et cum\nreprehenderit molestiae ut ut quas totam\nnostrum rerum est autem sunt rem eveniet architecto"
}
Post {userId = 1, id = 1, title = "sunt aut facere repellat provident occaecati excepturi optio reprehenderit", body = "quia et suscipit\nsuscipit recusandae consequuntur expedita et cum\nreprehenderit molestiae ut ut quas totam\nnostrum rerum est autem sunt rem eveniet architecto"}
Post {userId = 1, id = 1, title = "sunt aut facere repellat provident occaecati excepturi optio reprehenderit", body = "quia et suscipit\nsuscipit recusandae consequuntur expedita et cum\nreprehenderit molestiae ut ut quas totam\nnostrum rerum est autem sunt rem eveniet architecto"}
Post {userId = 1, id = 2, title = "qui est esse", body = "est rerum tempore vitae\nsequi sint nihil reprehenderit dolor beatae ea dolores neque\nfugiat blanditiis voluptate porro vel nihil molestiae ut reiciendis\nqui aperiam non debitis possimus qui neque nisi nulla"}
Post {userId = 101, id = 1, title = "test title", body = "test body"}
Post {userId = 101, id = 101, title = "test title", body = "test body"}
{}
```


