# C# 操作Active Directory - 深之JohnChen的专栏 - CSDN博客

2011年09月05日 23:45:47[byxdaz](https://me.csdn.net/byxdaz)阅读数：10845


活动目录将成为企业基础架构的根本，所有的高级服务都会向活动目录整合，以利用其统一的身份验证、安全管理以及资源公用。

活动目录的首要任务或者说主要目标是 客户端的安全管理，然后是客户端的标准化管理。

**活动目录、域及组策略**

活动目录、域和组策略在很多用户那里都有所运用，如果刚开始接触这些内容时难免会觉得很复杂，这主要是因为专业名词太多，同时也许个人心理因素上存在畏难情绪，因此，在和客户交流过程中，有些发蒙，觉得底气不足，无法和客户继续沟通下去，也就无法了解客户企业完整的网络架构，那就无法从客户的实际环境出发，帮助客户提出一个完备的解决方案。因此，今天我在这里对一些专业术语、易混淆的地方以及难点的地方做一简单诠释 。

**活动目录**
活动目录存储整个网络上资源的信息，便于用户查找、管理和使用这些资源。活动目录是Windows 2000网络中的目录服务。它存储关于网络资源的信息，并使用户或应用程序可以访问这些资源。活动目录使物理网络拓扑和协议透明化，这样网络上的用户可以访问任何资源，而不需要知道资源在什么地方，或物理上它是如何连接到网络上的。
以前我们访问网络资源，一是通过网上邻居，选择某个工作组，进入你所要访问的计算机，并且还需要目标计算机的用户名和密码才能访问上面的资源。或者通过IPC$，输入目标计算机的IP地址和访问盘符，但是同样需要目标计算机的用户名和密码。而通过活动目录，管理员可以把分布在网络各处各台计算机上的资源，比如打印机、共享文件，分门别类的放在一起。活动目录提供对网络资源集中控制，允许用户只登录一次就可以访问整个活动目录的资源了。用户打开网络邻居，所见到的不再是计算机，而是一个个目录文件夹形式：放着打印机资源的目录文件夹名称叫做打印机，技术部门的所有共享文件放在一个称做技术资料的目录文件夹中。这就是活动目录名字的由来。
活动目录的对象代表网络资源，如用户、组、计算机和打印机。而且，网络中所有的服务器、域和站点都作为对象。因为活动目录代表了所有网络资源，只需要一个管理员就可以管理这些资源。

**名词解释：**
轻型目录访问协议（Lightweight Directory Access Protocol,LDAP）
LDAP是用于查询和更新活动目录的目录服务协议。它表明，一个活动目录对象可以由一系列域组件，OU和普通名字来代表，它们组成了活动目录中的命名路径。包括标识名和相对标识名。标识名确定了对象所在的域和可以找到对象的完整路径。比如：yanyi.technology.centerm.com就是一个标识名，在LDAP中表示为CN=yanyi，OU=technology，DC=centerm，DC=com。
名字属性描述
DC         域组件         DNS名字的域组件，如com
OU         组织单元用来包容其他对象的容器
CN         普通名字除了DC和OU的所有对象，如用户和计算机对象
**域（Domain）**域是活动目录中逻辑结构的核心单元。一个域包含许多台计算机，它们由管理者设定，共用一个目录数据库。一个域有一个唯一的名字，给那些由域管理者集中管理的用户帐号和组帐号提供访问通道。
**安全边界（ Secutory BOUndary）**
在Windows 2000网络中，域起着安全边界的作用。安全边界的作用就是保证域的管理者只能在该域里有必要的管理权限。每个域都有自己的安全策略和与其他域的安全联系方式。
**复制单元（Unit of Replication）**
域同时也是一个复制单元。在域中，作为域控制器的计算机包含活动目录的副本。在一个域中，所有域控制器都能够得到活动目录中的变化信息，并把这些变化复制给该域中的其它控制器。（在一个域中，一般都会有两个域控制器，称做DC）。
**组织单元（Organizational Units.OU）**
一个组织单元就是一个可以把对象组织到一个域内的容器对象。一个OU可以包含的对象有用户帐号，组，计算机，打印机和其它OU。
          OU的引入，可以帮助建立基于管理责任的网络管理模型。例如，一个组织可以由一个管理员来负责所有用户的帐户，由另一个管理员负责所有计算机。因此，在这个事例中，可以分别建立一个用户OU和一个计算机OU。另外，我们可以对不同的OU设置不同的组策略进行管理。
**组策略（Group Policy）**
生产上的损失经常是由于用户错误产生的。通过使用组策略来减少用户环境的复杂性，并减小用户不正确得配置这些环境的可能性，可以提高生产率并减少网络所需的技术支持，从而，降低了物主的总体拥有成本。这也是我们推行Windows 终端一直强调的要点，所以，终端和组策略紧密配合，将提高我们工作效率，增强对客户的说服力。因此，掌握组策略，对我们而言是很重要的。
组策略可以应用在整个网络中，也可以仅将它应用在某个特定的用户或计算机小组上。
组策略的设置类型包括：
**管理模板。用户可以获得访问的操作系统的组件和应用程序、控制面板的访问权限以及用户离线文件的控制。**
安全性。即配置本地计算机、域以及网络安全性设置的设置。例如密码策略、IP策略等。
**软件安装。**即软件安装、升级、卸载的集中化管理的设置。
**命令。**即当Windows 2000 运行特定命令时的指定设置。当计算机开机、关机或者当用户登录或退出登录时可以指定命令。
**远程安装服务。**即当运行远程安装服务（RIS）的远程安装向导时控制用户可能的选项设置。
**Internet Explorer维护。**管理和定制基于Windows 2000计算机的Internet Explorer设置。
文件夹重定向。即在网络服务器上存储用户个性化文件夹的位置。这些设置在个性化里创建一个和网络共享文件夹的连接，但文件夹在本地显示。用户可以在网络中的任何一台计算机上对该文件夹进行访问。
**组策略包括计算机的组策略设置和用户的组策略设置。**计算机相关的组策略应用在操作系统初始化和周期性更新循环过程中。用户相关的组策略应用在用户登录计算机和周期性更新循环过程中。
**GPO**称做组策略对象。
**组策略的继承性**
组策略的继承性包括Windows 2000在活动目录中处理GPO的顺序，以及在连接到母容器的GPO中的组策略的继承性。
组策略的执行GPO的顺序是建立在GPO所连接的活动目录容器的基础上的。GPO首先应用于离用户和计算机最远的站点上，然后应用于域，最后是OU。因此，作为成员的用户和计算机的OU的组策略设置是最后执行的组策略设置。
默认GPO是被继承的。继承流程是沿着活动目录从站点到域然后到OU。
如果一个域中存在多个组策略，那么就很有可能出现组策略设置间存在冲突问题。大多数场合下，计算机设置高于用户设置。
当冲突发生时，确定执行哪个组策略设置的原则：
来自母容器的GPO设置和来自子容器的GPO设置冲突。这种情况下，子容器的设置后执行而发挥作用。
连接到同一容器上的不同GPO设置发生冲突。这种情况下，在容器属性对话框中GPO列表中最高位置的GPO的设置后执行并发挥作用。当然，通过改变GPO列表的顺序，可以改变最终发挥作用的GPO设置。
**组策略回送处理模式（Loopback Processing Mode）**
组策略以某种方式应用于用户或计算机，而这种方式取决于用户和计算机对象位于 Active Directory 中的什么位置。 但在某些情况下，用户可能需要仅根据计算机对象的位置来应用策略。 要想仅根据用户登录到哪个计算机来应用组策略对象 (GPO)，可以使用组策略回送处理模式。
该策略将指示系统把该计算机的 GPO 集合应用于登录到受该策略影响的计算机的所有用户。 该策略特别适用于特殊用途的计算机，在这些地方，必须根据使用的计算机来修改用户策略，例如，位于公共场合、实验室和教室中的计算机。
当用户在自己的工作站上工作时，可能希望根据用户对象的位置来应用组策略设置。因此，以用户帐户所在的组织单元 (OU) 为单位来配置策略设置。 但是，有可能出现这样的情况：计算机对象驻留在指定的 OU 中，而且根据计算机对象而不是用户对象来应用策略中的用户设置。
根据正常的组策略处理过程的指定，OU 中的计算机是在计算机启动期间按顺序来应用 GPO 的。 而 OU 中的用户则是在登录期间按顺序来应用 GPO 的，这与他们登录的是哪个计算机无关。
某些情况下，该处理顺序可能是不合适的，例如，一些应用程序已被指派或发布给在某些 OU 中的用户，但当他们登录到在某个特定 OU 中的计算机时，您并不想让这些应用程序安装在该计算机上。使用组策略回送处理支持功能，可以指定采用其它方式来为这个特定 OU 中的计算机的任何用户检索出针对他们的 GPO 列表，这些方式包括：
合并模式
在该模式中，当用户登录时，将通过使用 GetGPOlist 函数正常收集用户的 GPO 列表。 然后，再次调用 GetGPOlist 函数，在这次调用中使用计算机在 Active Directory中的位置。 然后，计算机的 GPO 列表被添加到用户 GPO 的末尾。 这将导致计算机的 GPO 具有比用户的 GPO 更高的优先权。 在该例中，计算机的 GPO 列表被添加到了用户的列表中。
替代模式
在该模式中，不收集用户的 GPO 列表。 只使用基于计算机对象的 GPO 列表。

**普通目录及其面临的困难**

谈到计算机目录及其相关技术，总能让我们想到无时不在使用的文件系统目录、灵巧实用的专用工具集目录、海量存储的网络资源目录等等。是的，这是一个异常熟悉的领域：在文件系统目录里，我们存储文件及其大小、创建日期、类型等信息；在诸如记事本类的专用工具集目录里，我们存放日程安排、联系方式、人员地点等信息；在网络资源目录里，我们以分层架构存放网络上所有对象的相关资料，这些对象包罗了网络里使用的各种资源：共享目录、共享打印机、应用程序、服务、服务器、用户帐号、组、域…可以这样说，从使用计算机的那一刻起，我们就从未离开过目录！曾经，这样的目录让我们随心所欲地处理我们的资源！

然而，正是这样的目录，在Internet下却面临许多麻烦：种类繁多、数量日增的目录让我们很难准确定位想要的资源；系统目录、应用程序特定目录、网络资源目录中到底谁存储了我们需要的信息？如何能用一致的方式登录这些不同的资源？怎样能轻松地维护不同系统上的远程资源？能否通过可视化的程序界面在这些资源间交互？

**初识Active Directory活动目录**

要解决这些问题，你可以使用Micrsoft提供的活动目录Active Directory 对象，它提供一种构造复杂计算机网络的简单方法，用来存储公共文件夹、对象信息、打印机、服务等数据； Active Directory的适用范围很大，它可以用在小自一台计算机，一个计算机网络，大至数个广域网络(WAN)的结合。它可以包含这个范围中所有的对象：文件、打印机、应用程序、服务器、域，以及用户等等。与普通意义上的目录不同的是， Active Directory活动目录以分层树状结构排列成节点树，每个节点表示网络上的一个资源或服务，并且包含一组可检索和操作的属性。Active Directory 是提供复杂网络统一视图的 Windows目录服务，它减少了开发人员必须处理的目录和命名空间的数量。

**Directory Service目录服务**

同时，专门针对Active Directory活动目录的Directory Services目录服务则让目录中的信息可用，Directory Service(目录服务)是让用户很容易地在目录内寻找到所需要的对象的一种服务。通过对Active Directory中数据的整理、规划存储，目录服务使得目录中的信息可用，真正让目录"活动"起来了。理由很明显，Internet网络、WAN局域网络里海量存储的数据往往让你迷失不知所措，再加上这些存放的资料不加以整理，想找到您需要的资料谈何容易！相反地，如果经过适当的规划，事先有系统地去整理这些资料，那就可以在需要时方便快速地寻找到你所要的对象。这样的例子现实生活中也随处可见：查号台算得上是一种目录服务；在Internet上的搜索引擎提供的查询功能究其实也是一种目录服务。目录服务不仅广义地包括了上述的各种目录（共享目录、共享打印机、应用程序、服务，网络服务器、用户帐号、计算机帐号、域、安全规则等等），更重要的它同时也是一种服务，让管理者、用户及应用程序都能利用目录中的资料处理这些对象。

其实，目录服务早就以不同的形式出现许多应用领域：一些操作系统如Microsoft的NT中的NTDS( Windows NT Directory Service), Novell 中的NDS(Novell Directory Service )等都整和了这种运用；另外，它也常集成在应用程序中，如Microsoft Exchange Server等。

作为Active Directory的广泛应用，Microsoft更是以Active Directory及其Service服务为整个分布式运算环境的基础。它可以包含并管理前述不同操作系统及应用程序相关的各种目录服务，提供用户、管理者，及程序开发者一个通用的目录服务，大大减少了企业的负担。

**Active Directory支持的标准——走进ADSI**

为实现活动目录服务，Active Directory支持了以下开放的标准：

LDAP轻型目录访问协议(Lightweight Directory Access Protocol)是用于访问Acitve Directory中数据的一个标准，它同时又是一个编程接口，提供API用来访问Active Directory活动目录；还有一个专门用于用户身份验证的标准是Kerberos协议，Windows 和Unix均支持这一协议以验证用户身份！

另外，为进一步简化程序方式访问Active Directory，Microsoft专门提供了ADSI（Active Directory Service Interface）活动目录服务接口语言，更使得编程人员可以更轻松地访问Active Directory的所有功能！使用ADSI，可以创建执行常见管理任务的应用程序，这些任务包括备份数据库、访问打印机和管理用户帐户等。ADSI 使管理员能够相对方便地定位和管理网络上的资源，不管网络的大小如何。

若要使用 ADSI 技术，在客户端计算机上必须提供有ADSI SDK或ADSI运行库，这可以通过安装 ADSI 2.5 或更高版本来实现。对于 Windows NT 5.0 版、Windows 2000 或 Windows XP，默认安装了 ADSI 2.5。如果使用的是以前版本的 Windows，则您可以自己从 Microsoft Web 站点安装该 SDK。

**Active Directory架构**

Active Directory架构(Schema)包含定义了各种形式的能够在Active Directory中使用的对象类（如共享打印机、共享目录等），它同时也定义了在Active Directory允许存在的各种属性以提供对象的附加信息，如Name、Guid标识等，这种定义目录对象、属性的方式与数据库架构定义数据库结构的方式大体相同。架构信息存储在 Active Directory 层次结构中，可以大大加快在大型目录上进行成员搜索的速度。Active Directory 使用架构定义可以访问网络节点的指定信息。架构提供了一种给存储在目录中的对象类型创建全局定义的方法，这些定义并赋予有意义的名称，如“用户”和“计算机”，更易于用户的访问识别。

.Net架构支持

DirectoryServices名字空间

.Net架构对Active Directory提供了丰富的支持功能，其命名空间 System.DirectoryServices包含的DirctoryEntry、DirectoryEntries以及DirectorySearcher等类库可与任何 Active Directory 服务提供程序（一种识别绑定协议并提供相关服务的程序，下面有专门阐述）一起使用，．NET 框架的这些支持使得操作具有 DirectoryEntry 和 DirectorySearcher 等组件的 ADSI 功能非常容易。

DirectoryEntry组件

DirectoryEntry 组件使从目录访问对象以及使用其数据和行为非常方便。当给 DirectoryEntry 组件指定一个Active Directory 层次结构中的有效目录路径时，它返回可操作的 ADSI COM 对象，这些对象包括用户、计算机、服务、用户帐户和计算机的组织、文件系统以及文件服务操作。DirectoryEntry 类封装 Active Directory 层次结构中的节点或对象，使用此类库绑定到对象、读取属性和更新特性。DirectoryEntry 与帮助器类一起为生存期管理和导航方法提供支持，包括创建、删除、重命名、移动子节点和枚举子级。

可以将 DirectoryEntry 组件绑定到目录中的对象以执行管理任务，如修改属性或监视信息更改。可以使用 DirectoryEntry 组件自动执行常见管理任务，如添加用户和组、管理打印机以及设置网络资源的权限。这样，你可以使用 DirectoryEntry 组件与企业中任何目录系统上的任何资源进行交互。可以向层次结构添加新的节点。当然，如果需要更改 Active Directory 对象的属性值，你必须拥有所绑定到对象的管理权限。

在创建新的对象时，DirectoryEntry 组件会用到 Active DirectorySchema 架构。这时你需要指定一个先前已存在的架构名，并将该对象与之关联。

DirectorySearcher组件

DirectorySearcher 组件使用Active Directory架构信息在目录中执行搜索并获得节点的属性并返回 SearchResult 的实例，这些实例包含在 SearchResultCollection 类的实例中。SearchResult 的实例与 DirectoryEntry 的实例非常类似。明显的差异在于每次访问新对象时，DirectoryEntry 都从 Active Directory 层次结构中检索其信息，而 SearchResult 的数据已经存在于用 DirectorySearcher 执行的查询返回的 SearchResultCollection 中。SearchResult 中只存在那些在查询中通过 DirectorySearcher.PropertiesToLoad 属性的集合指定的属性。

可以使用 DirectorySearcher 类对使用轻量目录访问协议（Lightweight Directory Access Protocol，LDAP）的 Active Directory 层次结构进行搜索以查找特定的服务或对象；可以基于大型目录中对象的属性 (Rich Query) 执行查询，以便按其一个或多个属性值查找特定的对象；可以封装层次结构中的节点，并操作或查询其属性。DirectorySearcher 组件的实例需要 LDAP 提供程序以便对 Active Directory 层次结构执行多格式查询。LDAP 是系统提供的唯一一种支持搜索的 Active Directory 服务接口 (ADSI) 提供程序。

目录服务提供程序

可以使用 DirectoryEntry 组件访问的每种目录系统类型都具有一种特定的目录协议（称为“服务提供程序”），此协议允许您访问和处理该目录的内容。创建ADSI可使开发人员通过单个接口访问所有的协议，从而可以方便地在这些协议之间进行切换。

下表列出了可以访问的服务提供程序以及每种服务提供程序的标识符。

|服务提供程序|路径标识符|
|----|----|
|Windows NT 5.0 版、Windows 2000 或 Windows XP|WinNT://path|
|轻量目录访问协议(LDAP)|LDAP://path|
|Novell NetWare 目录服务|NDS://path|
|Novell Netware 3.x|NWCOMPAT://binderyServer/TopHat|
每种服务提供程序为您提供一组不同的可访问和操作的对象以及关联数据和行为。这些对象与该命名空间的目录树中的项和资源对应。很多提供程序具有相同的对象。例如，所有提供程序授予您访问组对象（表示一个组帐户）和用户对象（表示一个用户帐户）的权限。

对于 Windows NT 服务提供程序，您可以访问域、计算机、打印队列和会话。对于 LDAP 提供程序，您可以访问组织、地址和"根 DS 项"(rootDSE) 对象。LDAP把rootDSE定义为目录服务器中目录树的根。根 DS 项是一组必需的操作属性，用户可以读取这些属性以找出目录和服务器的基本特征：如读取默认命名环境名(default naming context), rootDSE 仅对 LDAP 提供程序是必需的。

绑定及路径

当创建DirectoryEntry组件的实例时，需要指定所使用的服务提供程序的类型以及要操作的对象，并与Active Directory连接，这个过程称为“绑定”。这时，你需要指定所使用的协议（LADP、WinNT等），即服务提供程序指示符。当前的一些服务提供程序包括 Internet 信息服务 (IIS) 、轻量目录访问协议 (LDAP) 和 WinNT等。随后你可能需要指定计算机所在的域名、组名以及计算机名，如果没有显示式指定域名（服务器名），系统就会在整个域中查找与用户绑定过程相关的域控制器，并且使用所找到的第一个域控制器。这样，WinNT下的绑定语法大致为：

1. WinNT: //MyDomain/Group

2. WinNT: //MyDomain/MyComputer/aPrinter/

这个过程中，可能需要指定路径信息，类DirectoryEntry的Path属性唯一地标识网络环境中的路径信息，设置该属性将从目录存储区检索新项，它不更改当前绑定的项的路径。同绑定类似，Path 属性的语法取决于服务提供程序，在WinNT 下连接到计算机上的组语法为：WinNT://domain/computer/group；而IIS下连接到Web目录则为：IIS://LocalHost/W3SVC/1/ROOT/web-directory-name。

ADSI可实现的操作

依赖于.Net架构类库的强大支持，使用ADSI使以下操作变得简单可行：

1、一次登录可处理不同的目录。DirectoryEntry 组件类提供用户名和密码属性，可以在运行时输入这些属性并与绑定到的 Active Directory 对象进行通讯。

2、通过给用户提供各种要使用的协议，使用单个应用程序编程接口 (API) 即可在多个目录系统上执行任务。

3、对目录系统执行"多格式查询"。ADSI 技术允许通过指定两种查询语言，即 SQL 和 LDAP，来搜索对象。

4、通过访问 Active Directory 树，访问和使用用于管理和维护各种复杂网络配置的单个分层结构。

5、将目录信息与数据库（如 SQL Server）进行集成。只要 DirectoryEntry 路径使用 LDAP 提供程序，就可以将它用作 ADO.NET 连接字符串。

C# 操作Active Directory 操作实例

如果你想知道怎么去用C#访问Active Directory 的话，这里将提供一些运用的代码，是关于Active Directory 一系列功能的运用。来源：

[http://www.codeproject.com/script/Articles/ArticleVersion.aspx?aid=67524&av=126482](http://www.codeproject.com/script/Articles/ArticleVersion.aspx?aid=67524&av=126482)

**验证的方法**

Login： 这个方法是通过用户名和密码去验证用户是否存在和用户的活动性

IsAccountActive： 在这个平台上验证这个账户是不是处于激活的状态

IsUserValid：通过用户名和密码登录，确认这个用户是否已经是活动目录的用户

**查询的方法**

GetUser- 如果用户存在会返回 DirectoryEntry Object

GetUserDataSet- This will take a Username and Query the AD for the User. When found, it will transform the results from the Property Collection into a Dataset.

通过用户名在活动目录中查询用户的信息，如果找到则会从属性集合添加到 DataSet中。

**用户账户方法**

SetUserPassword- 设置用户的密码。

EnableUserAccount- 激活用户

ExpireUserPassword- 使用户密码过期。

DisableUserAccount- 冻结账户

MoveUserAccount- 把用户移到新的OU中。

IsAccountLocked- 审核账户是否被锁。

UnlockUserAccount- 把用户解锁

IsUserExpired- 审核账户是否过期。

CreateNewUser- 创建一个新的用户目录对象。

DeleteUser- 通过传入用户名参数从活动目录中删除一个账户 

**Group Metho****ds**

CreateNewGroup- This method will create a New Active Directory Group 

AddUserToGroup- This method will add a User to a group

RemoveUserFromGroup- This method will remove a User from a group

IsUserGroupMember- This method will Validate whether the User is a member of a Group

GetUserGroups- This method will return an ArrayListof a User Group Memberships

**HelperMethods**

GetProperty- This will retrieve the Specified Property Value from the Directory Entry Object

GetProperty_Array- This will retrieve the Specified Property Value if it's an Array Type from the Directory Entry object GetProperty_Byte- This will retrieve the Specified Property Value if it's a Byte Type from the Directory Entry object SetProperty- This will set the Property of the Directory Entry Object

ClearProperty- This method will clear the Property Values

```
using System;
using System.Collections;
using System.Text;
using System.DirectoryServices;
using System.Data;
using System.Configuration;

namespace ADExchangeLib
{
 public class ADMethods : IDisposable
 {
 DirectoryEntry oDE = null;
 DirectoryEntry oDEC = null;
 DirectorySearcher oDS = null;
 SearchResultCollection oResults = null;
 DataSet oDs = null;
 DataSet oDsUser = null;
 DataTable oTb = null;
 DataRow oRwUser = null;
 DataRow oRwResult = null;
 DataRow oNewCustomersRow = null;

 #region Private Variables

 private string sADPath = "";
 private string sADPathPrefix = "";
 private string sADUser = "";
 private string sADPassword = "";
 private string sADServer = "";
 private string sCharactersToTrim = "";

 #endregion

 #region Enumerations

 public enum ADAccountOptions
 {
 UF_TEMP_DUPLICATE_ACCOUNT = 0x0100,
 UF_NORMAL_ACCOUNT = 0x0200,
 UF_INTERDOMAIN_TRUST_ACCOUNT = 0x0800,
 UF_WORKSTATION_TRUST_ACCOUNT = 0x1000,
 UF_SERVER_TRUST_ACCOUNT = 0x2000,
 UF_DONT_EXPIRE_PASSWD = 0x10000,
 UF_SCRIPT = 0x0001,
 UF_ACCOUNTDISABLE = 0x0002,
 UF_HOMEDIR_REQUIRED = 0x0008,
 UF_LOCKOUT = 0x0010,
 UF_PASSWD_NOTREQD = 0x0020,
 UF_PASSWD_CANT_CHANGE = 0x0040,
 UF_ACCOUNT_LOCKOUT = 0X0010,
 UF_ENCRYPTED_TEXT_PASSWORD_ALLOWED = 0X0080,
 UF_EXPIRE_USER_PASSWORD = 0x800000,
 }
 public enum GroupType : uint
 {
 UniversalGroup = 0x08,
 DomainLocalGroup = 0x04,
 GlobalGroup = 0x02,
 SecurityGroup = 0x80000000
 }

 public enum LoginResult
 {
 LOGIN_OK = 0,
 LOGIN_USER_DOESNT_EXIST,
 LOGIN_USER_ACCOUNT_INACTIVE
 }

 #endregion

 #region Methods

 public ADMethods()
 {
 sADPath = ConfigurationSettings.AppSettings["sADPath"].ToString();
 sADUser = ConfigurationSettings.AppSettings["sADUser"].ToString();
 sADPassword = ConfigurationSettings.AppSettings["sADPassword"].ToString();
 sADServer = ConfigurationSettings.AppSettings["sADServer"].ToString();
 }
 //Implement IDisposable.
 public void Dispose()
 {
 Dispose(true);
 GC.SuppressFinalize(this);
 }

 protected virtual void Dispose(bool bDisposing)
 {
 if (bDisposing)
 {

 }
 // Free your own state.
 // Set large fields to null.

 sADPath = null;
 sADUser = null;
 sADPassword = null;
 sADServer = null;
 sCharactersToTrim = null;

 oDE = null;
 oDEC = null;
 oDS = null;
 oResults = null;
 oDs = null;
 oDsUser = null;
 oTb = null;
 oRwUser = null;
 oRwResult = null;
 oNewCustomersRow = null;
 }

 //Use C# Destructor Syntax for Finalization Code.
 ~ADMethods()
 {
 //Simply call Dispose(false).
 Dispose(false);
 }

 #region Validate Methods

 /// <summary>
 /// This Method will verify if the User Account Exists
 /// By Matching both the Username and Password as well as
 /// checking if the Account is Active.
 /// </summary>
 /// <param name="sUserName">Username to Validate</param>
 /// <param name="sPassword">Password of the Username to Validate</param>
 /// <returns></returns>
 public ADMethods.LoginResult Login(string sUserName, string sPassword)
 {
 //Check if the Logon exists Based on the Username and Password
 if (IsUserValid(sUserName, sPassword))
 {
 oDE = GetUser(sUserName);
 if (oDE != null)
 {
 //Check the Account Status
 int iUserAccountControl = Convert.ToInt32(oDE.Properties["userAccountControl"][0]);
 oDE.Close();

 //If the Disabled Item does not Exist then the Account is Active
 if (!IsAccountActive(iUserAccountControl))
 {
 return LoginResult.LOGIN_USER_ACCOUNT_INACTIVE;
 }
 else
 {
 return LoginResult.LOGIN_OK;
 }

 }
 else
 {
 return LoginResult.LOGIN_USER_DOESNT_EXIST;
 }
 }
 else
 {
 return LoginResult.LOGIN_USER_DOESNT_EXIST;
 }
 }

 /// <summary>
 /// This will perform a logical operation on the iUserAccountControl values
 /// to see if the user Account is Enabled or Disabled.
 /// The Flag for Determining if the Account is active is a Bitwise value (Decimal = 2)
 /// </summary>
 /// <param name="iUserAccountControl"></param>
 /// <returns></returns>
 public bool IsAccountActive(int iUserAccountControl)
 {
 int iUserAccountControl_Disabled = Convert.ToInt32(ADAccountOptions.UF_ACCOUNTDISABLE);
 int iFlagExists = iUserAccountControl & iUserAccountControl_Disabled;

 //If a Match is Found, then the Disabled Flag Exists within the Control Flags
 if (iFlagExists > 0)
 {
 return false;
 }
 else
 {
 return true;
 }
 }

 /// <summary>
 /// This will perform a logical operation on the sUserName values
 /// to see if the user Account is Enabled or Disabled.  
 /// The Flag for Determining if the Account is active is a Bitwise value (Decimal = 2)
 /// </summary>
 /// <param name="sUserName">Username to Validate</param>
 /// <returns></returns>
 public bool IsAccountActive(string sUserName)
 {
 oDE = GetUser(sUserName);
 if (oDE != null)
 {

 //to check of the Disabled option exists.
 int iUserAccountControl = Convert.ToInt32(oDE.Properties["userAccountControl"][0]);
 oDE.Close();

 //Check if the Disabled Item does not Exist then the Account is Active
 if (!IsAccountActive(iUserAccountControl))
 {
 return false;
 }
 else
 {
 return true;
 }
 }
 else
 {
 return false;
 }
 }

 /// <summary>
 /// This Method will Attempt to log in a User Based on the Username and Password
 /// to Ensure that they have been set up within the Active Directory.  
 /// This is the basic UserName and Password check.
 /// </summary>
 /// <param name="sUserName">Username to Validate</param>
 /// <param name="sPassword">Password of the Username to Validate</param>
 /// <returns></returns>
 public bool IsUserValid(string sUserName, string sPassword)
 {
 try
 {
 oDE = GetUser(sUserName, sPassword);
 oDE.Close();
 return true;
 }
 catch
 {
 return false;
 }
 }

 #endregion

 #region Search Methods
 /// <summary>
 /// This will return a DirectoryEntry Object if the User Exists
 /// </summary>
 /// <param name="sUserName">Username to Get</param>
 /// <returns></returns>
 public DirectoryEntry GetUser(string sUserName)
 {
 //Create an Instance of the DirectoryEntry
 oDE = GetDirectoryObject();

 //Create Instance of the Directory Searcher
 oDS = new DirectorySearcher();

 oDS.SearchRoot = oDE;
 //Set the Search Filter
 oDS.Filter = "(&(objectClass=user)(sAMAccountName=" + sUserName + "))";
 oDS.SearchScope = SearchScope.Subtree;
 oDS.PageSize = 10000;

 //Find the First Instance
 SearchResult oResults = oDS.FindOne();

 //If found then Return Directory Object, otherwise return Null
 if (oResults != null)
 {
 oDE = new DirectoryEntry
 (oResults.Path, sADUser, sADPassword, AuthenticationTypes.Secure);
 return oDE;
 }
 else
 {
 return null;
 }
 }

 /// <summary>
 /// Override method which will perform query 
 /// based on combination of Username and Password
 /// </summary>
 /// <param name="sUserName">Username to Get</param>
 /// <param name="sPassword">Password for the Username to Get</param>
 /// <returns></returns>
 public DirectoryEntry GetUser(string sUserName, string sPassword)
 {
 //Create an Instance of the DirectoryEntry
 oDE = GetDirectoryObject(sUserName, sPassword);

 //Create Instance of the Directory Searcher
 oDS = new DirectorySearcher();
 oDS.SearchRoot = oDE;

 //Set the Search Filter
 oDS.Filter = "(&(objectClass=user)(sAMAccountName=" + sUserName + "))";
 oDS.SearchScope = SearchScope.Subtree;
 oDS.PageSize = 10000;

 //Find the First Instance
 SearchResult oResults = oDS.FindOne();

 //If a Match is Found, Return Directory Object, Otherwise return Null
 if (oResults != null)
 {
 oDE = new DirectoryEntry
 (oResults.Path, sADUser, sADPassword, AuthenticationTypes.Secure);
 return oDE;
 }
 else
 {
 return null;
 }

 }

 /// <summary>
 /// This will take a Username and Query the AD for the User.  
 /// When found it will Transform the Results from the Property Collection into a Dataset.
 /// </summary>
 /// <param name="sUserName">Username to Get</param>
 /// <returns>Users Dataset</returns>
 public DataSet GetUserDataSet(string sUserName)
 {
 oDE = GetDirectoryObject();

 //Create Instance of the Directory Searcher
 oDS = new DirectorySearcher();
 oDS.SearchRoot = oDE;

 //Set the Search Filter
 oDS.Filter = "(&(objectClass=user)(sAMAccountName=" + sUserName + "))";
 oDS.SearchScope = SearchScope.Subtree;
 oDS.PageSize = 10000;

 //Find the First Instance
 SearchResult oResults = oDS.FindOne();

 //Create Empty User Dataset
 oDsUser = CreateUserDataSet();

 //If Record is not Null, Then Populate DataSet
 if (oResults != null)
 {
 oNewCustomersRow = oDsUser.Tables["User"].NewRow();
 oNewCustomersRow = PopulateUserDataSet(oResults, oDsUser.Tables["User"]);

 oDsUser.Tables["User"].Rows.Add(oNewCustomersRow);
 }
 oDE.Close();

 return oDsUser;

 }

 /// <summary>
 /// This Method will Return a Dataset of User Details 
 /// Based on Criteria passed to the Query
 /// The criteria is in the LDAP format
 /// e.g.
 /// (sAMAccountName='Test Account Name')(sn='Test Surname')
 /// </summary>
 /// <param name="sCriteria">Criteria to use for Searching</param>
 /// <returns>Users Dataset</returns>
 public DataSet GetUsersDataSet(string sCriteria)
 {
 oDE = GetDirectoryObject();

 //Create Instance of the Directory Searcher
 oDS = new DirectorySearcher();
 oDS.SearchRoot = oDE;

 //Set the Search Filter
 oDS.Filter = "(&(objectClass=user)(objectCategory=person)(" + sCriteria + "))";
 oDS.SearchScope = SearchScope.Subtree;
 oDS.PageSize = 10000;

 //Find the First Instance
 oResults = oDS.FindAll();

 //Create Empty User Dataset
 oDsUser = CreateUserDataSet();

 //If Record is not Null, Then Populate DataSet
 try
 {
 if (oResults.Count > 0)
 {
 foreach (SearchResult oResult in oResults)
 {
 oDsUser.Tables["User"].Rows.Add(PopulateUserDataSet(oResult, oDsUser.Tables["User"]));
 }
 }
 }
 catch { }

 oDE.Close();
 return oDsUser;
 }

 #endregion

 #region User Account Methods

 /// <summary>
 /// This Method will set the Users Password based on the User Name
 /// </summary>
 /// <param name="sUserName">The Username to Set the New Password</param>
 /// <param name="sNewPassword">The New Password</param>
 /// <param name="sMessage">Any Messages caught by the Exception</param>
 public void SetUserPassword(string sUserName, string sNewPassword, out string sMessage)
 {
 try
 {
 //Get Reference to User
 string LDAPDomain = "/sAMAccountName=" + sUserName + ",CN=Users," + GetLDAPDomain();
 oDE = GetDirectoryObject(LDAPDomain);
 oDE.Invoke("SetPassword", new Object[] { sNewPassword });
 oDE.CommitChanges();
 oDE.Close();
 sMessage = "";
 }
 catch (Exception ex)
 {
 sMessage = ex.Message;
 }
 }

 /// <summary>
 /// This Method will set the Users Password based on Directory Entry Object
 /// </summary>
 /// <param name="oDE">The Directory Entry to Set the New Password</param>
 /// <param name="sPassword">The New Password</param>
 /// <param name="sMessage">Any Messages caught by the Exception</param>
 public void SetUserPassword(DirectoryEntry oDE, string sPassword, out string sMessage)
 {
 try
 {
 //Set The new Password
 oDE.Invoke("SetPassword", new Object[] { sPassword });
 sMessage = "";

 oDE.CommitChanges();
 oDE.Close();
 }
 catch (Exception ex)
 {
 sMessage = ex.InnerException.Message;
 }
 }

 /// <summary>
 /// This Method will Enable a User Account Based on the Username
 /// </summary>
 /// <param name="sUserName">The Username of the Account to Enable</param>
 public void EnableUserAccount(string sUserName)
 {
 //Get the Directory Entry for the User and Enable the Password
 EnableUserAccount(GetUser(sUserName));
 }

 /// <summary>
 /// This Method will Enable a User Account Based on the Directory Entry Object
 /// </summary>
 /// <param name="oDE">The Directory Entry Object of the Account to Enable</param>
 public void EnableUserAccount(DirectoryEntry oDE)
 {
 oDE.Properties["userAccountControl"][0] = ADMethods.ADAccountOptions.UF_NORMAL_ACCOUNT;
 oDE.CommitChanges();
 oDE.Close();
 }

 /// <summary>
 /// This Method will Force Expire a Users Password based on Directory Entry Object
 /// </summary>
 /// <param name="oDE">The Directory Entry Object of the Account to Expire</param>
 public void ExpireUserPassword(DirectoryEntry oDE)
 {
 //Set the Password Last Set to 0, this will Expire the Password
 oDE.Properties["pwdLastSet"][0] = 0;
 oDE.CommitChanges();
 oDE.Close();
 }

 /// <summary>
 /// This Method will Disable the User Account based on the Username
 /// </summary>
 /// <param name="sUsername">The Username of the Account to Disable</param>
 public void DisableUserAccount(string sUserName)
 {
 DisableUserAccount(GetUser(sUserName));
 }

 /// <summary>
 /// This Method will Disable the User Account based on the Directory Entry Object
 /// </summary>
 /// <param name="oDE">The Directory Entry Object of the Account to Disable</param>
 public void DisableUserAccount(DirectoryEntry oDE)
 {
 oDE.Properties["userAccountControl"][0] = ADMethods.ADAccountOptions.UF_NORMAL_ACCOUNT 
 | ADMethods.ADAccountOptions.UF_DONT_EXPIRE_PASSWD 
 | ADMethods.ADAccountOptions.UF_ACCOUNTDISABLE;
 oDE.CommitChanges();
 oDE.Close();
 }

 /// <summary>
 /// Moves a User Account to a New OU Path
 /// </summary>
 /// <param name="oDE">Directory Entry Object of the User to Move</param>
 /// <param name="sNewOUPath">The New Path</param>
 public void MoveUserAccount(DirectoryEntry oDE, string sNewOUPath)
 {
 DirectoryEntry myNewPath = null;
 //Define the new Path
 myNewPath = new DirectoryEntry("LDAP://" + sADServer + "/" + 
  sNewOUPath, sADUser, sADPassword, AuthenticationTypes.Secure);

 oDE.MoveTo(myNewPath);
 oDE.CommitChanges();
 oDE.Close();
 }

 /// <summary>
 /// This Method checks whether and Account is Locked 
 /// based on the Directory Entry Object
 /// </summary>
 /// <param name="oDE">Directory Entry Object of the Account to check</param>
 /// <returns></returns>
 public bool IsAccountLocked(DirectoryEntry oDE)
 {
 return Convert.ToBoolean(oDE.InvokeGet("IsAccountLocked"));
 }

 /// <summary>
 /// This Method will unlock a User Account based on the Directory Entry Object
 /// </summary>
 /// <param name="oDE">Directory Entry Object of the Account to unlock</param>
 public void UnlockUserAccount(DirectoryEntry oDE)
 {
 SetProperty(oDE, "lockoutTime", "0");
 }

 /// <summary>
 /// This Method checks whether and Account is 
 /// Expired based on the Directory Entry Object
 /// </summary>
 /// <param name="oDE">Directory Entry Object of the Account to check</param>
 /// <returns></returns>
 public bool IsUserExpired(DirectoryEntry oDE)
 {
 int iDecimalValue = int.Parse(GetProperty(oDE, "userAccountControl"));
 string sBinaryValue = Convert.ToString(iDecimalValue, 2);

 //Reverse the Binary Value to get the Location for all 1's
 char[] str = sBinaryValue.ToCharArray();
 Array.Reverse(str);
 string sBinaryValueReversed = new string(str);

 //24th 1 is the Switch for the Expired Account
 if (sBinaryValueReversed.Length >= 24)
 {
 if (sBinaryValueReversed.Substring(24, 1) == "1")
 {
 return true;
 }
 else
 {
 return false;
 }
 }
 return false;
 }

 /// <summary>
 /// This Method will Create a new User Directory Object
 /// </summary>
 /// <param name="sCN">The CN of the New User</param>
 /// <returns></returns>
 public DirectoryEntry CreateNewUser(string sCN)
 {
 //Set the LDAP Path so that the user will be Created under the Users Container
 string LDAPDomain = "/CN=Users," + GetLDAPDomain();

 oDE = GetDirectoryObject();
 oDEC = oDE.Children.Add("CN=" + sCN, "user");
 oDE.Close();
 return oDEC;
 }

 /// <summary>
 /// This Method will Create a new User Directory Object 
 /// based on a Username and LDAP Domain
 /// </summary>
 /// <param name="sUserName">The Username of the New User</param>
 /// <param name="sLDAPDomain">The LDAP Domain for the New User</param>
 /// <returns></returns>
 public DirectoryEntry CreateNewUser(string sUserName, string sLDAPDomain)
 {
 //Set the LDAP qualification so that the user will be Created under the Users Container
 string LDAPDomain = "/CN=Users," + sLDAPDomain;
 oDE = new DirectoryEntry("LDAP://" + sADServer + "/" + 
 sLDAPDomain, sADUser, sADPassword, AuthenticationTypes.Secure);

 oDEC = oDE.Children.Add("CN=" + sUserName, "user");
 oDE.Close();
 return oDEC;
 }

 /// <summary>
 /// This Method will Delete an AD User based on UserNaeme. 
 /// Please be careful when using this
 /// The only way you can restore this object is by Tombstone which will not
 /// Restore every details on the Directory Entry object
 /// </summary>
 /// <param name="sUserName">The Username of the Account to Delete</param>
 /// <returns>True or False if the Delete was successful</returns>
 public bool DeleteUser(string sUserName)
 {
 string sParentPath = GetUser(sUserName).Parent.Path;
 return DeleteUser(sUserName, sParentPath);
 }

 /// <summary>
 /// This Method will Delete an AD User based on Username and specifying the Path. 
 /// Please be careful when using this
 /// The only way you can restore this object is by Tombstone which will not
 /// Restore every details on the Directory Entry object
 /// </summary>
 /// <param name="sUserName">The Username of the Account to Delete</param>
 /// <param name="sParentPath">The Path where the Useraccount is Located on LDAP</param>
 /// <returns></returns>
 public bool DeleteUser(string sUserName, string sParentPath)
 {
 try
 {
 oDE = new DirectoryEntry(sParentPath, sADUser, sADPassword, AuthenticationTypes.Secure);

 oDE.Children.Remove(GetUser(sUserName));

 oDE.CommitChanges();
 oDE.Close();
 return true;
 }
 catch
 {
 return false;
 }
 }

 #endregion

 #region Group Methods

 /// <summary>
 /// This Method will Create a New Active Directory Group
 /// </summary>
 /// <param name="sOULocation">OU Location of the New Group to be Created</param>
 /// <param name="sGroupName">The Group Name</param>
 /// <param name="sDescription">The Group Description</param>
 /// <param name="oGroupTypeInput">The Group Type</param>
 /// <param name="bSecurityGroup">True or False whether the Group is a 
 /// Security Group or a Distribution Group</param>
 /// <returns></returns>
 public DirectoryEntry CreateNewGroup(string sOULocation, string sGroupName, 
 string sDescription, GroupType oGroupTypeInput, bool bSecurityGroup)
 {
 GroupType oGroupType;

 oDE = new DirectoryEntry("LDAP://" + sADServer + "/" + sOULocation, 
 sADUser, sADPassword, AuthenticationTypes.Secure);

 //Check if the Requested group is a Security Group or Distribution Group
 if (bSecurityGroup)
 {
 oGroupType = oGroupTypeInput | GroupType.SecurityGroup;
 }
 else
 {
 oGroupType = oGroupTypeInput;
 }
 int typeNum = (int)oGroupType;

 //Add Properties to the Group
 DirectoryEntry myGroup = oDE.Children.Add("cn=" + sGroupName, "group");
 myGroup.Properties["sAMAccountName"].Add(sGroupName);
 myGroup.Properties["description"].Add(sDescription);
 myGroup.Properties["groupType"].Add(typeNum);
 myGroup.CommitChanges();

 return myGroup;
 }

 /// <summary>
 /// This Method will add a User Based on the Distinguished Name to an AD Group
 /// </summary>
 /// <param name="sDN">The Users Distinguished Name</param>
 /// <param name="sGroupDN">The Groups Distinguished Name</param>
 public void AddUserToGroup(string sDN, string sGroupDN)
 {
 oDE = new DirectoryEntry("LDAP://" + sADServer + "/" + sGroupDN, 
 sADUser, sADPassword, AuthenticationTypes.Secure);

 //Adds the User to the Group
 oDE.Properties["member"].Add(sDN);
 oDE.CommitChanges();
 oDE.Close();
 }

 /// <summary>
 /// This Method will remove a User Based on the Distinguished Name to an AD Group
 /// </summary>
 /// <param name="sDN">The Users Distinguished Name</param>
 /// <param name="sGroupDN">The Groups Distinguished Name</param>
 public void RemoveUserFromGroup(string sDN, string sGroupDN)
 {
 oDE = new DirectoryEntry("LDAP://" + sADServer + "/" + sGroupDN, 
 sADUser, sADPassword, AuthenticationTypes.Secure);

 //Removes the User to the Group
 oDE.Properties["member"].Remove(sDN);
 oDE.CommitChanges();
 oDE.Close();
 }

 /// <summary>
 /// This Method will Validate whether the User is a member of an AD Group
 /// </summary>
 /// <param name="sDN">The Users Distinguished Name</param>
 /// <param name="sGroupDN">The Groups Distinguished Name</param>
 /// <returns></returns>
 public bool IsUserGroupMember(string sDN, string sGroupDN)
 {
 oDE = new DirectoryEntry("LDAP://" + sADServer + "/" + sDN, 
 sADUser, sADPassword, AuthenticationTypes.Secure);

 string sUserName = GetProperty(oDE, "sAMAccountName");

 ArrayList oUserGroups = GetUserGroups(sUserName);
 int iGroupsCount = oUserGroups.Count;

 if (iGroupsCount != 0)
 {
 for (int i = 0; i < iGroupsCount; i++)
 {
 //Check is User is a Member of the AD Group
 if (sGroupDN == oUserGroups[i].ToString())
 {
 return true;
 }
 }
 return false;
 }
 else
 {
 return false;
 }
 }

 /// <summary>
 /// This Method will return an ArrayList of a User
 /// AD Group Memberships
 /// </summary>
 /// <param name="sUserName">The Username to get Group Memberships</param>
 /// <returns></returns>
 public ArrayList GetUserGroups(string sUserName)
 {
 ArrayList oGroupMemberships = new ArrayList();
 return AttributeValuesMultiString("memberOf", sUserName, oGroupMemberships);
 }

 #endregion
#region Helper Methods
 /// <summary>
 /// This will retrieve the Specified Property Value from the Directory Entry Object
 /// </summary>
 /// <param name="oDE">The Directory Object to retrieve from</param>
 /// <param name="sPropertyName">The Property to retrieve</param>
 /// <returns></returns>
 public string GetProperty(DirectoryEntry oDE, string sPropertyName)
 {
 if (oDE.Properties.Contains(sPropertyName))
 {
 return oDE.Properties[sPropertyName][0].ToString();
 }
 else
 {
 return string.Empty;
 }
 } 

 /// <summary>
 /// This will retrieve the Specified Property Value if it's an 
 /// Array Type from the Directory Entry object
 /// </summary>
 /// <param name="oDE">The Directory Object to retrieve from</param>
 /// <param name="sPropertyName">The Property to retrieve</param>
 /// <returns></returns>
 public ArrayList GetProperty_Array(DirectoryEntry oDE, string sPropertyName)
 {
 ArrayList myItems = new ArrayList();
 if (oDE.Properties.Contains(sPropertyName))
 {
 for (int i = 0; i < oDE.Properties[sPropertyName].Count; i++)
 {
 myItems.Add(oDE.Properties[sPropertyName][i].ToString());
 }
 return myItems;
 }
 else
 {
 return myItems;
 }
 }

 /// <summary>
 /// This will retrieve the Specified Property Value if its a 
 /// Byte Type from the Directory Entry object
 /// </summary>
 /// <param name="oDE">The Directory Object to retrieve from</param>
 /// <param name="sPropertyName">The Property to retrieve</param>
 /// <returns></returns>
 public byte[] GetProperty_Byte(DirectoryEntry oDE, string sPropertyName)
 {
 if (oDE.Properties.Contains(sPropertyName))
 {
 return (byte[])oDE.Properties[sPropertyName].Value;
 }
 else
 {
 return null;
 }
 }

 /// <summary>
 /// This is an Override that will Allow a Property to be Extracted Directly
 /// from a Search Result Object
 /// </summary>
 /// <param name="oSearchResult">The Search Result</param>
 /// <param name="sPropertyName">The Property to retrieve</param>
 /// <returns></returns>
 public string GetProperty(SearchResult oSearchResult, string sPropertyName)
 {
 if (oSearchResult.Properties.Contains(sPropertyName))
 {
 return oSearchResult.Properties[sPropertyName][0].ToString();
 }
 else
 {
 return string.Empty;
 }
 }

 /// <summary>
 /// This will Set the Property of the Directory Entry Object
 /// </summary>
 /// <param name="oDE">The Directory Object to Set to</param>
 /// <param name="sPropertyName">The Property Name</param>
 /// <param name="sPropertyValue">The Property Value</param>
 public void SetProperty(DirectoryEntry oDE, string sPropertyName, string sPropertyValue)
 {
 //Check if the Value is Valid
 if (sPropertyValue != string.Empty)
 {
 //Check if the Property Exists
 if (oDE.Properties.Contains(sPropertyName))
 {
 oDE.Properties[sPropertyName].Value = sPropertyValue;
 oDE.CommitChanges();
 oDE.Close();
 }
 else
 {
 oDE.Properties[sPropertyName].Add(sPropertyValue);
 oDE.CommitChanges();
 oDE.Close();
 }
 }
 }

 /// <summary>
 /// This will Set the Property of the Directory Entry Object
 /// </summary>
 /// <param name="oDE">The Directory Object to Set to</param>
 /// <param name="sPropertyName">The Property Name</param>
 /// <param name="bPropertyValue">The Property Value</param>
 public void SetProperty(DirectoryEntry oDE, string sPropertyName, byte[] bPropertyValue)
 {

 //Clear Binary Data if Exists
 oDE.Properties[sPropertyName].Clear();

 //Update Attribute with Binary Data from File
 oDE.Properties[sPropertyName].Add(bPropertyValue);
 oDE.CommitChanges();
 oDE.Dispose();

 }

 /// <summary>
 /// This will Set the Property of the Directory Entry Object if its an Array Type
 /// </summary>
 /// <param name="oDE">The Directory Object to Set to</param>
 /// <param name="sPropertyName">The Property Name</param>
 /// <param name="aPropertyValue">The Property Value in Array List Type</param>
 public void SetProperty(DirectoryEntry oDE, string sPropertyName, 
    ArrayList aPropertyValue)
 {
 //Check if the Value is Valid
 if (aPropertyValue.Count != 0)
 {
 foreach (string sPropertyValue in aPropertyValue)
 {
 oDE.Properties[sPropertyName].Add(sPropertyValue);
 oDE.CommitChanges();
 oDE.Close();
 }
 }
 }

 /// <summary>
 /// This Method will Clear the Property Values
 /// </summary>
 /// <param name="oDE">The Directory Object to Set to</param>
 /// <param name="sPropertyName">The Property Name to be cleared</param>
 public void ClearProperty(DirectoryEntry oDE, string sPropertyName)
 {
 //Check if the Property Exists
 if (oDE.Properties.Contains(sPropertyName))
 {
 oDE.Properties[sPropertyName].Clear();
 oDE.CommitChanges();
 oDE.Close();
 }
 }

 /// <summary>
 /// This is an Internal Method for Retrieving a New Directory Entry Object
 /// </summary>
 /// <returns></returns>
 private DirectoryEntry GetDirectoryObject()
 {
 oDE = new DirectoryEntry(sADPath, sADUser, sADPassword, AuthenticationTypes.Secure);
 return oDE;
 }

 /// <summary>
 /// Override Function that will Attempt a Logon based on the Username and Password
 /// </summary>
 /// <param name="sUserName"></param>
 /// <param name="sPassword"></param>
 /// <returns></returns>
 private DirectoryEntry GetDirectoryObject(string sUserName, string sPassword)
 {
 oDE = new DirectoryEntry(sADPath, sUserName, sPassword, AuthenticationTypes.Secure);
 return oDE;
 }

 /// <summary>
 /// This will Create the Directory Entry based on the Domain Reference
 /// </summary>
 /// <param name="sDomainReference"></param>
 /// <returns></returns>
 private DirectoryEntry GetDirectoryObject(string sDomainReference)
 {
 oDE = new DirectoryEntry(sADPath + sDomainReference, 
 sADUser, sADPassword, AuthenticationTypes.Secure);
 return oDE;
 }

 /// <summary>
 ///This will Create the Directory Entry based on the LDAP Path
 /// </summary>
 /// <param name="sPath"></param>
 /// <returns></returns>
 public DirectoryEntry GetDirectoryObject_ByPath(string sPath)
 {
 oDE = new DirectoryEntry(sADPathPrefix + sPath, sADUser, 
  sADPassword, AuthenticationTypes.Secure);
 return oDE;
 }

 /// <summary>
 /// Additional Override that will Allow oObject to be 
 /// Created based on the Username and Password.
 /// </summary>
 /// <param name="DomainReference"></param>
 /// <param name="sUserName"></param>
 /// <param name="sPassword"></param>
 /// <returns></returns>
 private DirectoryEntry GetDirectoryObject
 (string sDomainReference, string sUserName, string sPassword)
 {
 oDE = new DirectoryEntry
 (sADPath + sDomainReference, sUserName, sPassword, AuthenticationTypes.Secure);
 return oDE;
 }

 /// <summary>
 /// This will retrieve the Distinguished Name from the DirectoryEntry Object
 /// </summary>
 /// <param name="oDE">The Directory Entry Object to get the 
 /// Distinguished Name From</param>
 /// <returns></returns>
 public string GetDistinguishedName(DirectoryEntry oDE)
 {
 if (oDE.Properties.Contains("distinguishedName"))
 {
 return oDE.Properties["distinguishedName"][0].ToString();
 }
 else
 {
 return string.Empty;
 }
 }

 /// <summary>
 /// This will retrieve the Distinguished Name from the User Name
 /// </summary>
 /// <param name="oDE">The User Name to get the Distinguished Name From</param>
 /// <returns></returns>
 public string GetDistinguishedName(string sUserName)
 {
 oDE = GetUser(sUserName);

 if (oDE.Properties.Contains("distinguishedName"))
 {
 return oDE.Properties["distinguishedName"][0].ToString();
 }
 else
 {
 return string.Empty;
 }
 }

 /// <summary>
 /// This Method will Get the Array List of the Directory Object Attribute
 /// </summary>
 /// <param name="sAttributeName"></param>
 /// <param name="sUserName"></param>
 /// <param name="oValuesCollection"></param>
 /// <returns></returns>
 public ArrayList AttributeValuesMultiString
 (string sAttributeName, string sUserName, ArrayList oValuesCollection)
 {
 oDE = GetUser(sUserName);

 PropertyValueCollection oValueCollection = oDE.Properties[sAttributeName];
 IEnumerator oIEn = oValueCollection.GetEnumerator();

 while (oIEn.MoveNext())
 {
 if (oIEn.Current != null)
 {
 if (!oValuesCollection.Contains(oIEn.Current.ToString()))
 {
 oValuesCollection.Add(oIEn.Current.ToString());
 }
 }
 }
 oDE.Close();
 oDE.Dispose();
 return oValuesCollection;
 }

 #endregion

 #region Internal Methods

 /// <summary>
 /// This will read in the ADServer Value from the Web.config and will Return it
 /// as an LDAP Path
 /// e.g.. DC=testing, DC=co, DC=nz.
 /// This is required when Creating Directory Entry other than the Root.
 /// </summary>
 /// <returns></returns>
 private string GetLDAPDomain()
 {
 StringBuilder LDAPDomain = new StringBuilder();
 string[] LDAPDC = sADServer.Split('.');

 for (int i = 0; i < LDAPDC.GetUpperBound(0) + 1; i++)
 {
 LDAPDomain.Append("DC=" + LDAPDC[i]);
 if (i < LDAPDC.GetUpperBound(0))
 {
 LDAPDomain.Append(",");
 }
 }
 return LDAPDomain.ToString();
 }

 /// <summary>
 /// This method will Create a Dataset Structure 
 /// Containing all Relevant Fields for a User Object
 /// </summary>
 /// <returns></returns>
 private DataSet CreateUserDataSet()
 {
 oDs = new DataSet();

 oTb = oDs.Tables.Add("User");

 //Create All the Columns
 oTb.Columns.Add("company");
 oTb.Columns.Add("department");
 oTb.Columns.Add("description");
 oTb.Columns.Add("displayName");
 oTb.Columns.Add("facsimileTelephoneNumber");
 oTb.Columns.Add("givenName");
 oTb.Columns.Add("homePhone");
 oTb.Columns.Add("employeeNumber");
 oTb.Columns.Add("initials");
 oTb.Columns.Add("ipPhone");
 oTb.Columns.Add("l");
 oTb.Columns.Add("mail");
 oTb.Columns.Add("manager");
 oTb.Columns.Add("mobile");
 oTb.Columns.Add("name");
 oTb.Columns.Add("pager");
 oTb.Columns.Add("physicalDeliveryOfficeName");
 oTb.Columns.Add("postalAddress");
 oTb.Columns.Add("postalCode");
 oTb.Columns.Add("postOfficeBox");
 oTb.Columns.Add("sAMAccountName");
 oTb.Columns.Add("sn");
 oTb.Columns.Add("st");
 oTb.Columns.Add("street");
 oTb.Columns.Add("streetAddress");
 oTb.Columns.Add("telephoneNumber");
 oTb.Columns.Add("title");
 oTb.Columns.Add("userPrincipalName");
 oTb.Columns.Add("wWWHomePage");
 oTb.Columns.Add("whenCreated");
 oTb.Columns.Add("whenChanged");
 oTb.Columns.Add("distinguishedName");
 oTb.Columns.Add("info");

 return oDs;
 }

 /// <summary>
 /// This method will Create a Dataset Structure Containing all 
 /// Relevant Fields for a Group Object
 /// </summary>
 /// <param name="sTableName"></param>
 /// <returns></returns>
 private DataSet CreateGroupDataSet(string sTableName)
 {

 oDs = new DataSet();

 oTb = oDs.Tables.Add(sTableName);

 //Create all the Columns
 oTb.Columns.Add("distinguishedName");
 oTb.Columns.Add("name");
 oTb.Columns.Add("friendlyname");
 oTb.Columns.Add("description");
 oTb.Columns.Add("domainType");
 oTb.Columns.Add("groupType");
 oTb.Columns.Add("groupTypeDesc");

 return oDs;
 }

 /// <summary>
 /// This Method will Return a DataRow Object which will be added 
 /// to the User Dataset Object
 /// </summary>
 /// <param name="oUserSearchResult"></param>
 /// <param name="oUserTable"></param>
 /// <returns></returns>
 private DataRow PopulateUserDataSet(SearchResult oUserSearchResult, DataTable oUserTable)
 {
 //Sets a New Empty Row
 oRwUser = oUserTable.NewRow();

 oRwUser["company"] = GetProperty(oUserSearchResult, "company");
 oRwUser["department"] = GetProperty(oUserSearchResult, "department");
 oRwUser["description"] = GetProperty(oUserSearchResult, "description");
 oRwUser["displayName"] = GetProperty(oUserSearchResult, "displayName");
 oRwUser["facsimileTelephoneNumber"] = GetProperty
  (oUserSearchResult, "facsimileTelephoneNumber");
 oRwUser["givenName"] = GetProperty(oUserSearchResult, "givenName");
 oRwUser["homePhone"] = GetProperty(oUserSearchResult, "homePhone");
 oRwUser["employeeNumber"] = GetProperty(oUserSearchResult, "employeeNumber");
 oRwUser["initials"] = GetProperty(oUserSearchResult, "initials");
 oRwUser["ipPhone"] = GetProperty(oUserSearchResult, "ipPhone");
 oRwUser["l"] = GetProperty(oUserSearchResult, "l");
 oRwUser["mail"] = GetProperty(oUserSearchResult, "mail");
 oRwUser["manager"] = GetProperty(oUserSearchResult, "manager");
 oRwUser["mobile"] = GetProperty(oUserSearchResult, "mobile");
 oRwUser["name"] = GetProperty(oUserSearchResult, "name");
 oRwUser["pager"] = GetProperty(oUserSearchResult, "pager");
 oRwUser["physicalDeliveryOfficeName"] = GetProperty
  (oUserSearchResult, "physicalDeliveryOfficeName");
 oRwUser["postalAddress"] = GetProperty(oUserSearchResult, "postalAddress");
 oRwUser["postalCode"] = GetProperty(oUserSearchResult, "postalCode");
 oRwUser["postOfficeBox"] = GetProperty(oUserSearchResult, "postOfficeBox");
 oRwUser["sAMAccountName"] = GetProperty(oUserSearchResult, "sAMAccountName");
 oRwUser["sn"] = GetProperty(oUserSearchResult, "sn");
 oRwUser["st"] = GetProperty(oUserSearchResult, "st");
 oRwUser["street"] = GetProperty(oUserSearchResult, "street");
 oRwUser["streetAddress"] = GetProperty(oUserSearchResult, "streetAddress");
 oRwUser["telephoneNumber"] = GetProperty(oUserSearchResult, "telephoneNumber");
 oRwUser["title"] = GetProperty(oUserSearchResult, "title");
 oRwUser["userPrincipalName"] = GetProperty(oUserSearchResult, "userPrincipalName");
 oRwUser["wWWHomePage"] = GetProperty(oUserSearchResult, "wWWHomePage");
 oRwUser["whenCreated"] = GetProperty(oUserSearchResult, "whenCreated");
 oRwUser["whenChanged"] = GetProperty(oUserSearchResult, "whenChanged");
 oRwUser["distinguishedName"] = GetProperty(oUserSearchResult, "distinguishedName");
 oRwUser["info"] = GetProperty(oUserSearchResult, "info");

 return oRwUser;
 }

 /// <summary>
 /// This Method will Return a DataRow object which will be 
 /// added to the Group Dataset Object
 /// </summary>
 /// <param name="oSearchResult"></param>
 /// <param name="oTable"></param>
 /// <returns></returns>
 private DataRow PopulateGroupDataSet(SearchResult oSearchResult, DataTable oTable)
 {
 //Sets a New Empty Row
 oRwResult = oTable.NewRow();

 string sFullOU = GetProperty(oSearchResult, "distinguishedName");
 string[] splita = sCharactersToTrim.ToString().Split(new Char[] { ';' });
 foreach (string sa in splita)
 {
 sFullOU = sFullOU.Replace(sa, "");
 }

 string sDisplayName = "";
 string sRawString = "";
 string[] split1 = sFullOU.Split(new Char[] { ',' });
 foreach (string s1 in split1)
 {
 sRawString = s1;
 sRawString = sRawString.Replace("OU=", "");
 sRawString = sRawString.Replace("DC=", "");
 sRawString = sRawString.Replace("CN=", "");
 sDisplayName = sRawString + "/" + sDisplayName;
 }

 oRwResult["distinguishedName"] = GetProperty(oSearchResult, "distinguishedName");
 oRwResult["name"] = GetProperty(oSearchResult, "name");
 oRwResult["friendlyname"] = sDisplayName.Substring(0, sDisplayName.Length - 1); ;
 oRwResult["description"] = GetProperty(oSearchResult, "description");
 oRwResult["domainType"] = sADServer;

 string sGroupType = GetProperty(oSearchResult, "groupType");
 oRwResult["groupType"] = sGroupType;

 switch (sGroupType)
 {
 case "2": oRwResult["groupTypeDesc"] = "Global, Distribution"; break;
 case "4": oRwResult["groupTypeDesc"] = "Domain, Distribution"; break;
 case "8": oRwResult["groupTypeDesc"] = "Universal, Distribution"; break;
 case "-2147483640": oRwResult["groupTypeDesc"] = "Universal, Security"; break;
 case "-2147483646": oRwResult["groupTypeDesc"] = "Global, Security"; break;
 case "-2147483644": oRwResult["groupTypeDesc"] = "Domain, Security"; break;
 default: oRwResult["groupTypeDesc"] = ""; break;
 }

 return oRwResult;
 }

 #endregion
 #endregion

 }
}
```

[操作Active Directory C#](http://www.cnblogs.com/sherwinzhu/archive/2008/10/06/1304413.html) 实例代码2

[http://www.cnblogs.com/sherwinzhu/archive/2009/07/02/1304413.html](http://www.cnblogs.com/sherwinzhu/archive/2009/07/02/1304413.html)

活动目录（Active Directory）域故障解决实例

[http://hi.baidu.com/ruijievideo/blog/item/86125d5053e6db3143a75b6c.html](http://hi.baidu.com/ruijievideo/blog/item/86125d5053e6db3143a75b6c.html)

