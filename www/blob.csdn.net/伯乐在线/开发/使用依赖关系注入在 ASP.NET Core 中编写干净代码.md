# 使用依赖关系注入在 ASP.NET Core 中编写干净代码 - 文章 - 伯乐在线
原文出处： [Steve Smith](https://msdn.microsoft.com/zh-cn/magazine/mt703433)
ASP.NET Core 1.0 是 ASP.NET 的完全重新编写，这个新框架的主要目标之一就是更多的模块化设计。即，应用应该能够仅利用其所需的框架部分，方法是框架在它们请求时提供依赖关系。此外，使用 ASP.NET Core 构建应用的开发人员应该能够利用这一相同功能保持其应用松散耦合和模块化。借助 ASP.NET MVC，ASP.NET 团队极大地提高了框架的支持以便编写松散耦合代码，但仍非常容易落入紧密耦合的陷阱，尤其是在控制器类中。
## 紧密耦合
紧密耦合适用于演示软件。如果你看一下说明如何构建 ASP.NET MVC（版本 3 到 5）站点的典型示例应用程序，你很可能会找到如下所示代码（从 NerdDinner MVC 4 示例的 DinnersController 类）：

C#
```
private NerdDinnerContext db = new NerdDinnerContext();
private const int PageSize = 25;
public ActionResult Index(int? page)
{
  int pageIndex = page ?? 1;
  var dinners = db.Dinners
  .Where(d => d.EventDate >= DateTime.Now).OrderBy(d => d.EventDate);
  return View(dinners.ToPagedList(pageIndex, PageSize));
}
```
这类代码难以进行单元测试，因为 NerdDinnerContext 作为类的构造的一部分而创建，并需要一个要连接的数据库。毫无疑问，这种演示应用程序通常不包括任何单元测试。但是，你的应用程序可能会从一些单元测试受益，即使你不是测试驱动开发，但最好是编写代码以便进行测试。
另外，此代码违反了切勿重复 (DRY) 原则，因为每个执行任何数据访问的控制器类都在其中具有相同的代码以创建 Entity Framework (EF) 数据库上下文。这使未来更改的成本更高且更容易出错，尤其是随着时间的推移应用程序不断增长。
在查看代码以评估其耦合度时，请记住这句话“新关键字就是粘附”。 也就是说，在看到“新”关键字实例化类的任何地方，应意识到你正在将你的实现粘贴到该特定实现代码。依赖关系注入原则 ([bit.ly/DI-Principle](http://bit.ly/DI-Principle)) 指出： “抽象不应依赖于详细信息，详细信息应依赖于抽象。” 在本示例中，控制器如何将数据整合在一起以传入视图的详细信息依赖于如何获取此数据（即 EF）的详细信息。
除了新关键字外，“墨守成规”是造成紧密耦合的另一个原因，使得应用程序更加难以进行测试和维护。在上述示例中，执行计算机的系统时钟上存在一个依赖关系，其形式为对 DateTime.Now 的调用。此耦合度可能导致难以创建一组用于某些单元测试的测试 Dinners，因为其 EventDate 属性需要相对于当前时钟的设置进行设置。有多种方法可以将耦合度从此方法中删除，其中最简单的方法就是让返回 Dinners 的任何新抽象来处理这一问题，因此，这不再是此方法的一部分。
此外，我赋予此值一个参数，因此方法可能会在提供的 DateTime 参数后返回所有 Dinners，而不是始终使用 DateTime.Now。最后，我创建当前时间的抽象，并通过该抽象引用当前时间。如果应用程序经常引用 DateTime.Now，这将是一个不错的方法。（另外还应该注意，由于这些 Dinners 可能会出现在不同时区中，所以在实际应用中 DateTimeOffset 类型可能是一个更好的选择）。
## 诚实
这类代码在可维护性方面的另一个问题在于它对它的协作者并不诚实。你应避免编写可在无效状态中实例化的类，因为这些类经常会成为错误的来源。因此，类为了执行其任务所需的一切都应通过其构造函数提供。如显式依赖关系原则 ([bit.ly/ED-Principle](http://bit.ly/ED-Principle)) 所述：“方法和类应显式要求正常工作所需的任何协作对象。”
DinnersController 类只有一个默认构造函数，这意味着它应该不需要任何协作者就能正常工作。但是如果你测试这个类会发生什么情况？ 如果你从引用 MVC 项目的新控制台应用程序运行此类，这个代码会执行哪些操作？

C#
```
var controller = new DinnersController();
var result = controller.Index(1);
```
在这种情况下，代码执行的第一个操作就是尝试实例化 EF 上下文。代码引发 InvalidOperationException： “应用程序配置文件中找不到名为‘NerdDinnerContext’的连接字符串。” 我被骗了！ 该类需要比其构造函数所声明的更多内容才能正常工作。 如果类需要一种访问 Dinner 实例集合的方法，则应通过其构造函数进行请求（或者，作为其方法上的参数）。
## 依赖关系注入
依赖关系注入 (DI) 引用将某个类或方法的依赖关系作为参数传递的技术，而不是通过新的或静态调用对这些关系进行硬编码。这是 .NET 开发中一种越来越常见的技术，因为该技术向使用此技术的应用程序提供分离。ASP.NET 的早期版本没有利用 DI，尽管 ASP.NET MVC 和 Web API 在支持 DI 的问题上取得了一些进展，但都没有生成对产品的完全支持，包括用于管理依赖关系及其对象生命周期的容器。借助 ASP.NET Core 1.0，DI 不仅得到现成支持，还被产品本身广泛使用。
ASP.NET Core 不仅支持 DI，它还包括一个 DI 容器—又称为控制反转 (IoC) 或服务容器。每个 ASP.NET Core 应用使用 Startup 类的 ConfigureServices 方法中的此容器配置其依赖关系。此容器提供所需的基本支持，但如果需要，可将其替换为自定义实现。而且，EF Core 还提供对 DI 的内置支持，因此，在 ASP.NET Core 应用程序中配置 DI 就像调用扩展方法一样简单。我为本文创建了 NerdDinner 衍生，称为 GeekDinner。配置 EF Core，如此处所示：

C#
```
public void ConfigureServices(IServiceCollection services)
{
  services.AddEntityFramework()
  .AddSqlServer()
  .AddDbContext<GeekDinnerDbContext>(options =>
  options.UseSqlServer(ConnectionString));
  services.AddMvc();
}
```
配置好后，即可非常轻松地使用 DI 从诸如 DinnersController 的控制器类请求 GeekDinnerDbContext 的实例。

C#
```
public class DinnersController : Controller
{
  private readonly GeekDinnerDbContext _dbContext;
  public DinnersController(GeekDinnerDbContext dbContext)
  {
  _dbContext = dbContext;
  }
  public IActionResult Index()
  {
  return View(_dbContext.Dinners.ToList());
  }
}
```
请注意，没有新关键字的单个实例，控制器所需要的依赖关系全部通过其构造函数传入，并且 ASP.NET DI 容器会代我负责处理此进程。在专注于编写应用程序时，我无需担心通过其构造函数完成我的类请求的依赖关系所涉及的探测。
当然，如果我愿意，我可以自定义此行为，甚至可以用其他实现完全替换默认容器。因为我的控制器类现在遵循显式依赖关系原则，我知道要想使控制器类正常工作，我需要为其提供一个 GeekDinnerDbContext 实例。通过对 DbContext 进行一些设置，我可以单独实例化控制器，如此控制台应用程序所示：

C#
```
var optionsBuilder = new DbContextOptionsBuilder();
optionsBuilder.UseSqlServer(Startup.ConnectionString);
var dbContext = new GeekDinnerDbContext(optionsBuilder.Options);
var controller = new DinnersController(dbContext);
var result = (ViewResult) controller.Index();
```
构造 EF Core DbContext 所涉及的操作要比构造 EF6 DbContext 稍微多一些，后者只需一个连接字符串。这是因为，就像 ASP.NET Core 一样，EF Core 已设计得更加模块化。通常情况下，你无需直接处理 DbContextOptionsBuilder，因为当你通过扩展方法（如 AddEntityFramework 和 AddSqlServer）配置 EF 时会在后台使用它。
## 但能否对它进行测试？
手动测试你的应用程序非常重要—你希望能够运行应用程序，查看它是否真正运行并产生预期的输出。但是，每次进行更改都必须进行测试很浪费时间。相比紧密耦合应用，松散耦合应用程序的最大好处之一是它们更适合进行单元测试。更妙的是，相比其前身，ASP.NET Core 和 EF Core 都更易于进行测试。
首先，我将通过传入已配置为使用内存存储的 DbContext 来直接针对控制器编写一个简单测试。我将使用 DbContextOptions 参数来配置 GeekDinnerDbContext，它通过其构造函数公开为我的测试的设置代码的一部分：

C#
```
var optionsBuilder = new DbContextOptionsBuilder<GeekDinnerDbContext>();
optionsBuilder.UseInMemoryDatabase();
_dbContext = new GeekDinnerDbContext(optionsBuilder.Options);
// Add sample data
_dbContext.Dinners.Add(new Dinner() { Title = "Title 1" });
_dbContext.Dinners.Add(new Dinner() { Title = "Title 2" });
_dbContext.Dinners.Add(new Dinner() { Title = "Title 3" });
_dbContext.SaveChanges();
```
在我的测试类中进行上述配置后，即可轻松编写一个测试，显示正确的数据已返回到 ViewResult 的模型中：

C#
```
[Fact]
public void ReturnsDinnersInViewModel()
{
  var controller = new OriginalDinnersController(_dbContext);
  var result = controller.Index();
  var viewResult = Assert.IsType<ViewResult>(result);
  var viewModel = Assert.IsType<IEnumerable<Dinner>>(
  viewResult.ViewData.Model).ToList();
  Assert.Equal(1, viewModel.Count(d => d.Title == "Title 1"));
  Assert.Equal(3, viewModel.Count);
}
```
当然，这里还没有大量的逻辑以供测试，因此，本测试不会真的进行那么多测试。批评家们会辩驳这不是非常有价值的测试，我同意他们的观点。但是，这是具备更多逻辑时进行操作的起点，因为很快就会有更多逻辑。但首先，尽管 EF Core 可以通过其内存选项支持单元测试，我仍会避免直接耦合到我的控制器中的 EF。没有理由通过数据访问基础结构问题来耦合 UI 问题—实际上，这违反了另一条原则，即关注点分离原则。
## 不要依赖于你不使用的内容
接口分隔原则 ([bit.ly/LS-Principle](http://bit.ly/LS-Principle)) 指出类应仅依赖于它们实际使用的功能。对于启用 DI 的新 DinnersController 而言，它仍依赖于整个 DbContext。可以使用仅提供必需功能的抽象，而不将控制器实现整合到 EF 中。
此操作方法真正需要什么才能正常工作？ 当然不是整个 DbContext。它甚至无需访问上下文的完整 Dinners 属性。它需要的只是能够显示合适页面的 Dinner 实例。表示此内容的最简单 .NET 抽象为 IEnumerable<Dinner>。因此，我将定义一个接口，该接口仅返回 IEnumerable<Dinner>，并满足 Index 方法的（大多数）要求。

C#
```
public interface IDinnerRepository
{
   IEnumerable<Dinner> List();
}
```
我将此称之为存储库，因为它符合该模式： 它抽象出类似集合的接口后的数据访问。如果出于某些原因，你不喜欢存储库模式或名称，你可以将其称之为 IGetDinners 或 IDinnerService 或者任何你喜欢的名称（我的技术审阅者建议将其称为 ICanHasDinner）。无论你如何为此类型命名，它都能起到相同的作用。
一切就绪后，我现在就可以调整 DinnersController 以接受将 IDinnerRepository 作为构造函数参数，而不是 GeekDinnerDbContext，并调用 List 方法，而不直接访问 Dinners DbSet：

C#
```
private readonly IDinnerRepository _dinnerRepository;
public DinnersController(IDinnerRepository dinnerRepository)
{
  _dinnerRepository = dinnerRepository;
}
public IActionResult Index()
{
  return View(_dinnerRepository.List());
}
```
此时，你可以生成并运行你的 Web 应用程序，但如果你导航到 /Dinners 则会遇到异常： Invalid­OperationException： 在尝试激活 GeekDinner.Controllers.DinnersController 时，无法解析类型“Geek­Dinner.Core.Interfaces.IdinnerRepository”的服务。
我尚未实现此接口，并且在我进行实现时，我还需要配置在 DI 满足 IDinnerRepository 请求时要使用的实现。实现接口并不复杂：

C#
```
public class DinnerRepository : IDinnerRepository
{
 private readonly GeekDinnerDbContext _dbContext;
  public DinnerRepository(GeekDinnerDbContext dbContext)
  {
  _dbContext = dbContext;
  }
  public IEnumerable<Dinner> List()
 {
  return _dbContext.Dinners;
 }
}
```
请注意，这非常适用于直接将存储库实现耦合到 EF。如果我需要换出 EF，则只需创建此接口的新实现。此实现类是我的应用程序的基础结构的一部分，这是应用程序中我的类依赖于特定实现的一个地方。
若要在类请求 IDinnerRepository 时将 ASP.NET Core 配置为注入正确实现，我需要将以下代码行添加到之前所示的 ConfigureServices 方法的末尾：

C#
```
services.AddScoped<IDinnerRepository, DinnerRepository>();
```
此语句要求 ASP.NET Core DI 容器在容器解析依赖于 IDinnerRepository 实例的类型时使用 DinnerRepository 实例。作用域意味着一个实例将用于 ASP.NET 处理的每个 Web 请求。还可以使用暂时或单一生存期添加服务。在这种情况下，作用域适用，因为我的 DinnerRepository 依赖于同样使用作用域生存期的 DbContext。下面是可用对象生存期的摘要：
- 暂时： 新类型实例在每次请求类型时使用。
- 作用域： 新类型实例在给定 HTTP 请求中进行第一次请求时创建，然后重用于在该 HTTP 请求期间解析的所有后续类型。
- 单一： 单一类型实例会创建一次，并由该类型的所有后续请求使用。
内置容器支持多种方法，来构造它将提供的类型。最典型的情况是只提供容器和类型，容器将尝试实例化该类型，提供类型运行时需要的任何依赖关系。你还可以提供 lambda 表达式用来构造类型或单一生存期，你可以在注册时在 ConfigureServices 中提供完全构造的实例。
随着依赖关系注入关联，应用程序就可以像以前一样运行。现在，如**图 1** 所示，我可以通过准备就绪的新抽象，使用 IDinner­Repository 接口的虚设或模拟实现对其进行测试，而不在我的测试代码中直接依赖于 EF。
图 1 使用 Mock 对象测试 DinnersController

C#
```
public class DinnersControllerIndex
{
  private List<Dinner> GetTestDinnerCollection()
  {
  return new List<Dinner>()
  {
  new Dinner() {Title = "Test Dinner 1" },
  new Dinner() {Title = "Test Dinner 2" },
  };
  }
  [Fact]
  public void ReturnsDinnersInViewModel()
  {
  var mockRepository = new Mock<IDinnerRepository>();
  mockRepository.Setup(r =>
  r.List()).Returns(GetTestDinnerCollection());
  var controller = new DinnersController(mockRepository.Object, null);
  var result = controller.Index();
  var viewResult = Assert.IsType<ViewResult>(result);
  var viewModel = Assert.IsType<IEnumerable<Dinner>>(
  viewResult.ViewData.Model).ToList();
  Assert.Equal("Test Dinner 1", viewModel.First().Title);
  Assert.Equal(2, viewModel.Count);
  }
}
```
无论 Dinner 实例的列表来自何处，此测试都能正常运行。你可以重写数据访问代码以使用其他数据库、Azure 表存储或 XML 文件，并且控制器也会同样正常运行。当然，在此情况中，并没有执行很多操作，那么，你可能想知道…
## 实际的逻辑会怎么样？
到目前为止，我没有真正实现任何实际的业务逻辑—这只是返回简单数据集合的简单方法。测试的真正价值在于，在遇到逻辑和特殊情况时，你需要对其会按照预期运行满怀信心。为了说明这一点，我打算将一些需求添加到我的 GeekDinner 站点。此站点将公开一个 API，允许任何人访问 dinner 的 RSVP。
但是，dinner 将拥有可选的最大容量，并且 RSVP 不应超过这一容量。请求超过最大容量的 RSVP 的用户不应被添加到候补名单中。最后，dinner 可以指定相对于其开始时间必须接收 RSVP 的最后期限，在此期限后它们将停止接收 RSVP。
我可以将此逻辑全部编码到一个操作中，但我认为这让一个方法承担了太多责任，尤其是 UI 方法应专注于 UI 问题，而不是业务逻辑。控制器应确认它接收的输入有效，并且应确保它返回的响应适合于客户端。在此之外的决策，尤其是业务逻辑，不属于控制器。
放置业务逻辑的最佳位置位于应用程序的域模型中，这不应依赖于基础结构方面的问题（如数据库或 UI）。Dinner 类在管理需求中所述的 RSVP 问题时最具价值，因为它会为 dinner 存储最大容量，并知道目前已完成了多少 RSVP。但是，部分逻辑还依赖于 RSVP 发生的时间以及是否超过最后期限，因此方法也需要访问当前时间。
我可以只使用 DateTime.Now，但这会造成逻辑难以测试，并将我的域模型耦合到系统时钟。另一种方法是使用 IDateTime 抽象并将其注入到 Dinner 实体。但是，根据我的经验，最好使 Dinner 等实体没有依赖关系，尤其是如果你计划使用类似 EF 的 O/RM 工具将这些实体从持久性层中提取出来。我不希望将实体的依赖关系填充为该进程的一部分，EF 肯定不可能在我没有执行其他代码的情况下做到这一点。
此时一个常用的方法是将逻辑从 Dinner 实体中提取出来，并将其放在可轻松注入依赖关系的某类服务（如 DinnerService 或 RsvpService）中。这往往会导致贫乏域模型反模式 ([bit.ly/anemic-model](http://bit.ly/anemic-model))，不过，其中实体具有很少行为或没有行为，只是状态包。不，在这种情况下，解决方案相当简单—方法可以将当前时间作为参数，并让调用代码将其传入。
通过此方法，添加 RSVP 的逻辑非常简单（请参阅**图 2**）。有多个测试可说明此方法按预期运行，这些方法在与本文关联的示例项目中提供。
图 2 域模型中的业务逻辑

C#
```
public RsvpResult AddRsvp(string name, string email, DateTime currentDateTime)
{
  if (currentDateTime > RsvpDeadlineDateTime())
  {
    return new RsvpResult("Failed - Past deadline.");
  }
  var rsvp = new Rsvp()
  {
    DateCreated = currentDateTime,
    EmailAddress = email,
    Name = name
  };
   if (MaxAttendees.HasValue)
  {
  if (Rsvps.Count(r => !r.IsWaitlist) >= MaxAttendees.Value)
  {
  rsvp.IsWaitlist = true;
  Rsvps.Add(rsvp);
  return new RsvpResult("Waitlist");
  }
  }
  Rsvps.Add(rsvp);
  return new RsvpResult("Success");
}
```
通过将此逻辑转换为域模型，我确保我的控制器的 API 方法仍然较小并专注于其自身的问题。因此，可以轻松测试控制器是否执行它应该执行的操作，因为通过此方法创建的路径相对较少。
## 控制器职责
控制器的部分职责是检查 ModelState 并确保其有效。为清楚起见，我在操作方法中执行此工作，但在大型应用程序中，我会通过使用操作筛选器清除每个操作中的重复代码：

C#
```
[HttpPost]
public IActionResult AddRsvp([FromBody]RsvpRequest rsvpRequest)
{
  if (!ModelState.IsValid)
  {
  return HttpBadRequest(ModelState);
  }
```
假定 ModelState 有效，操作下一步必须使用请求中提供的标识符来提取适当的 Dinner 实例。如果操作找不到匹配该 ID 的 Dinner 实例，它应返回“未找到”结果：

C#
```
var dinner = _dinnerRepository.GetById(rsvpRequest.DinnerId);
if (dinner == null)
{
  return HttpNotFound("Dinner not found.");
}
```
在完成这些检查后，操作即可将由请求表示的业务操作委托给域模型，调用你之前看到的 Dinner 类上的 AddRsvp 方法，并在返回 OK 响应前保存域模型的更新状态（在这种情况下，为 dinner 实例及其 RSVP 集合）。

C#
```
var result = dinner.AddRsvp(rsvpRequest.Name,
  rsvpRequest.Email,
  _systemClock.Now);
  _dinnerRepository.Update(dinner);
  return Ok(result);
}
```
请记住，我决定 Dinner 类不应对系统时钟具有依赖关系，而选择将当前时间传入此方法。在控制器中，我为 currentDateTime 参数传入 _systemClock.Now。这是通过 DI 填充的本地字段，这还会防止控制器紧密耦合到系统时钟。
适当的做法是使用控制器上的 DI 而非域实体，因为控制器始终由 ASP.NET 服务容器创建，这将实现控制器在其构造函数中声明的任何依赖关系。_systemClock 是类型 IDateTime 的字段，只需几行代码即可定义和实现此字段。

C#
```
public interface IDateTime
{
  DateTime Now { get; }
}
public class MachineClockDateTime : IDateTime
{
  public DateTime Now { get { return System.DateTime.Now; } }
}
```
当然，我还需要确保将 ASP.NET 容器配置为在类需要 IDateTime 实例时使用 MachineClockDateTime。此操作可以在 Startup 类的 ConfigureServices 中完成，在这种情况下，尽管任何对象生存期都有效，但我选择单一生存期，因为一个 MachineClockDateTime 实例将适用于整个应用程序：

C#
```
services.AddSingleton<IDateTime, MachineClockDateTime>();
```
在准备好这个简单抽象后，我能基于 RSVP 是否过期来测试控制器的行为，并确保返回正确的结果。因为我已经对 Dinner.AddRsvp 方法进行了测试，验证其按预期方式工作，我不需要通过控制器对相同行为进行多次测试来使我确信这一点，在协同工作时，控制器和域模型都能正常工作。
## 后续步骤
下载关联的示例项目，查看 Dinner 和 DinnersController 的单元测试。请记住，相比充斥着依赖于基础结构问题的“新的”或静态方法调用的紧密耦合代码，松散耦合代码通常更容易进行单元测试。应该在你的应用程序中有意而不是意外使用“新关键字就是粘附”和新关键字。在 [docs.asp.net](http://docs.asp.net/) 上了解有关 ASP.NET Core 及其对依赖关系注入的支持。
