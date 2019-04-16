# 在 ASP.NET Core 中执行租户服务 - ggabcda的博客 - CSDN博客





2017年08月28日 11:01:28[ggabcda](https://me.csdn.net/ggabcda)阅读数：131








# 在 ASP.NET Core 中执行租户服务

**不定时更新翻译系列，此系列更新毫无时间规律，文笔菜翻译菜求各位看官老爷们轻喷，如觉得我翻译有问题请挪步原博客地址**

**本博文翻译自：**
**http://gunnarpeipman.com/2017/08/tenant-providers/**

在我之前关于 Entity Framework core 2.0 全局查询过滤器的文章中，我提出了一个想法，当构建模型时，如何自动地将查询过滤器应用到所有的领域实体中，也就是说领域实体总是来自同一租户。这篇文章更深入地介绍了在 ASP.NET Core 应用程序中检测当前租户的可能解决方案，并建议一些租户提供者将为实际应用程序中提供多租户的支持作为出发点。

> 
**注意!** 请阅读我之前在Entity Framework core 2.0 全局查询过滤器中的文章，这篇文章将继续下去，并期待读者熟悉我为多租户提供的解决方案。另外，将多租户规则应用到所有领域实体的方法是从我以前的全局查询过滤器中获取的，而不是在这里复制的。


### 如何检测当前租户?

情况是这样的。数据上下文是在请求传入和构建模型全局查询过滤器时构建的。其中一个过滤器是关于当前租户的。在代码中还需要租户ID，但模型还没有准备好。同一时间，租户ID只能在数据库中使用。我们该怎么办?

一些想法:
- 在数据上下文中使用数据库连接，并对租户表进行直接查询 
- 为租户的信息和操作使用单独的数据上下文 
- 保持租户信息在云存储上可用 
- 使用域名的哈希值作为租户ID 

> 
**注意!** 在本文中，我希望在web应用程序中通过host的header检测租户。


我在这篇文章中使用的租户表如下图所示。

![ef-core-tenants-table](http://gunnarpeipman.com/wp-content/uploads/2017/08/ef-core-tenants-table.png)

> 
**注意!** 依赖于解决方案的租户ID也可以是其他的，而不是像上图所示的int类型。


### 使用数据上下文连接数据库

这可能是最轻量级的解决方案了，因为不需要添加额外的类，也不再需要租户提供程序。而且使用IHttpContextAccessor很容易获得当前host的header。

```
public class PlaylistContext : DbContext
{
    private int _tenantId;
    private string _tenantHost;
 
    public DbSet<Playlist> Playlists { get; set; }
    public DbSet<Song> Songs { get; set; }
 
    public PlaylistContext(DbContextOptions<PlaylistContext> options,
                           IHttpContextAccessor accessor)
        : base(options)
    {
        _tenantHost = accessor.HttpContext.Request.Host.Value;
    }
 
    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        var connection = Database.GetDbConnection();
        using (var command = connection.CreateCommand())
        {
            connection.Open();
 
            command.CommandText = "select ID from Tenants where Host=@Host";
            command.CommandType = CommandType.Text;
 
            var param = command.CreateParameter();
            param.ParameterName = "@Host";
            param.Value = _tenantHost;
 
            command.Parameters.Add(param);
            _tenantId = (int)command.ExecuteScalar();
            connection.Close();
        }
 
        foreach (var type in GetEntityTypes())
        {
            var method = SetGlobalQueryMethod.MakeGenericMethod(type);
            method.Invoke(this, new object[] { modelBuilder });
        }
 
        base.OnModelCreating(modelBuilder);
    }
 
    // Other methods follow
}
```

上面的代码是基于数据上下文所持有的数据库连接创建命令，并运行sql命令，以通过host的header来获取租户ID。

这个解决方案的代码量是比较少的，但是它会用主机名检测内部细节的方法来污染数据上下文。

### 为租户使用单独的数据上下文

第二种方法是使用单独的web应用程序访问特定的租户上下文。可以编写租户提供程序(请参阅我的Entity Framework core 2.0 全局查询过滤器)，并将其注入到主数据上下文

让我们从文章开头提到的租户表开始。

```
public class Tenant
{
    public int Id { get; set; }
    public string Name { get; set; }
    public string Host { get; set; }
}
```

现在，让我们构建租户数据上下文。这个上下文不依赖于其他有依赖关系的自定义接口和类。它只使用租户模型。请注意，租户集是私有的，其他类只能通过host的header查询租户ID。

```
public class TenantsContext : DbContext
{
    private DbSet<Tenant> Tenants { get; set; }
 
    public TenantsContext(DbContextOptions<TenantsContext> options)
        : base(options)
    {
    }
 
    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        modelBuilder.Entity<Tenant>().HasKey(e => e.Id);
    }
 
    public int GetTenantId(string host)
    {
        var tenant = Tenants.FirstOrDefault(t => t.Host == host);
        if(tenant == null)
        {
            return 0;
        }
 
        return tenant.Id;
    }
}
```

现在是时候回到ITenantProvider并编写使用租户数据上下文的实现了。这个提供程序包含检测host的header和获取租户ID的所有逻辑，在实际应用中它将更加复杂，但是在这里我将使用简单的版本。

```
public class WebTenantProvider : ITenantProvider
{
    private int _tenantId;
 
    public WebTenantProvider(IHttpContextAccessor accessor,
                                TenantsContext context)
    {
        var host = accessor.HttpContext.Request.Host.Value;
 
        _tenantId = context.GetTenantId(host);
    }
 
    public int GetTenantId()
    {
        return _tenantId;
    }
}
```

现在，需要检查租户并找到它的ID，因为已经到了重新编写主数据上下文的时候了，所以它使用新的租户提供程序。

```
public class PlaylistContext : DbContext
{
    private int _tenantId;
 
    public DbSet<Playlist> Playlists { get; set; }
    public DbSet<Song> Songs { get; set; }
 
    public PlaylistContext(DbContextOptions<PlaylistContext> options,
                           ITenantProvider tenantProvider)
        : base(options)
    {
        _tenantId = tenantProvider.GetTenantId();
    }
 
    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        foreach (var type in GetEntityTypes())
        {
            var method = SetGlobalQueryMethod.MakeGenericMethod(type);
            method.Invoke(this, new object[] { modelBuilder });
        }
 
        base.OnModelCreating(modelBuilder);
    }
 
    // Other methods follow
}
```

在web应用程序的启动类中，必须在ConfigureServices()方法中 为框架级定义的所有依赖项进行依赖注入。

```
public void ConfigureServices(IServiceCollection services)
{
    services.AddMvc();
 
    var connection = Configuration["ConnectionString"];
    services.AddEntityFrameworkSqlServer();
    services.AddDbContext<PlaylistContext>(options => options.UseSqlServer(connection));
    services.AddDbContext<TenantsContext>(options => options.UseSqlServer(connection));
    services.AddScoped<ITenantProvider, WebTenantProvider>();
    services.AddSingleton<IHttpContextAccessor, HttpContextAccessor>();
}
```

这个解决方案更优雅，因为它将与租户相关的功能从主数据上下文中移出。ITenantProvider是主数据上下文唯一必须知道的东西，现在它也可以在其他不一定是web应用程序的项目中使用。

### 将租户信息存储在云存储中

我现在说的是，租户并不是一直都在使用，而不是租户提供程序查询数据库，在需要的时候可以缓存租户信息，并在需要时更新它。考虑到云的场景，最好让租户信息在web应用程序的多个实例中都可以访问。我的选择是云存储。

让我们从json格式的简单的租户文件开始，让我们期望它是一些内部应用程序或后台任务的职责，以使这个文件保持最新。这是我使用的样本文件。

```
[
  {
    "Id": 2,
    "Name": "Local host",
    "Host": "localhost:30172"
  },
  {
    "Id": 3,
    "Name": "Customer X",
    "Host": "localhost:3331"
  },
  {
    "Id": 4,
    "Name": "Customer Y",
    "Host": "localhost:33111"
  }
]
```

要读取云存储应用程序中的文件，需要了解存储帐户连接字符串、容器名称和云名称。Blob是租户文件。我再次使用ITenantProvider接口，并为Azure 云存储创建了一个新的实现。我把它叫做BlobStorageTenantProvider。它很简单，不需要考虑很多实际的方面，比如刷新租户信息和处理锁。

```
public class BlobStorageTenantProvider : ITenantProvider
{
    private static IList<Tenant> _tenants;
 
    private int _tenantId = 0;
 
    public BlobStorageTenantProvider(IHttpContextAccessor accessor, IConfiguration conf)
    {
        if(_tenants == null)
        {
            LoadTenants(conf["StorageConnectionString"], conf["TenantsContainerName"], conf["TenantsBlobName"]);
        }
 
        var host = accessor.HttpContext.Request.Host.Value;
        var tenant = _tenants.FirstOrDefault(t => t.Host.ToLower() == host.ToLower());
        if(tenant != null)
        {
            _tenantId = tenant.Id;
        }
    }
 
    private void LoadTenants(string connStr, string containerName, string blobName)
    {
        var storageAccount = CloudStorageAccount.Parse(connStr);
        var blobClient = storageAccount.CreateCloudBlobClient();
        var container = blobClient.GetContainerReference(containerName);
        var blob = container.GetBlobReference(blobName);
 
        blob.FetchAttributesAsync().GetAwaiter().GetResult();
 
        var fileBytes = new byte[blob.Properties.Length];
 
        using (var stream = blob.OpenReadAsync().GetAwaiter().GetResult())
        using (var textReader = new StreamReader(stream))
        using (var reader = new JsonTextReader(textReader))
        {
            _tenants = JsonSerializer.Create().Deserialize<List<Tenant>>(reader);
        }
    }
 
    public int GetTenantId()
    {
        return _tenantId;
    }
}
```

提供者的代码可能不是很好，但是它比以前的代码好，因为不需要额外的数据库调用，而且租户id是由内存服务的。

### 用host的header的哈希值作为租户ID

第三种方法是最简单的方法，但这意味着租户ID与host的 header相同，或者从它派生而来。我不喜欢这种做法，因为如果客户想要更改host的 header，那么更改将分布在整个数据库中。客户可能希望从服务自动提供的自定义主机名开始，然后使用他们自己的子域名。

这里是作为主机名的租户ID的代码。

```
public class PlaylistContext : DbContext
{
    private string _tenantId;
 
    public DbSet<Playlist> Playlists { get; set; }
    public DbSet<Song> Songs { get; set; }
 
    public PlaylistContext(DbContextOptions<PlaylistContext> options,
                            IHttpContextAccessor accessor)
        : base(options)
    {
        _tenantId = accessor.HttpContext.Request.Host.Value;
    }
 
    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        foreach (var type in GetEntityTypes())
        {
            var method = SetGlobalQueryMethod.MakeGenericMethod(type);
            method.Invoke(this, new object[] { modelBuilder });
        }
 
        base.OnModelCreating(modelBuilder);
    }
 
    // Other methods follow
}
```

可以使用MD5代替主机的名称，但它不会改变主机的问题。

### 总结

这篇文章是关于在Entity Framework Core 2.0中真正的去利用全局查询过滤器。虽然这里所展示的代码是简单的而不我们实际运用场景所需要的，但在构建真正的解决方案之前，它们仍然是很好的例子。我尽量让解决方案尽可能的接近完美的架构原则。我认为读者他们自己的多租户应用程序可以在这里提供的解决方案中获得帮助。

**欢迎转载，转载请注明翻译原文出处(本文章)，原文出处(原博客地址)，然后谢谢观看**

**如果觉得我的翻译对您有帮助，请点击推荐支持：）**



