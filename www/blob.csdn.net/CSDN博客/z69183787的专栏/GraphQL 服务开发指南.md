# GraphQL 服务开发指南 - z69183787的专栏 - CSDN博客
2018年04月24日 00:20:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1257
[https://www.v2ex.com/t/355632](https://www.v2ex.com/t/355632)
最近项目上在用 GraphQL ，总结了些经验分享给大家。欢迎大家提提意见：）
文章我也发在了简书上 @@ [戳这里](http://www.jianshu.com/p/b420a33eeeab)
# GraphQL 服务开发指南
2015 年 7 月， Facebook 发 GraphQL 布并开源了 GraphQL ， GraphQL 作为负责前后端交互的协议，很好的解决了单一的后端服务在面对多前端（ Android ， iOS ， mobile web ， PC web ）的场景下，能够针对同一场景提供不同数据以满足客户端应用展示的需要。
GraphQL 是一种类 json 的语言，有自己的一套语法来请求获得客户端指定的数据或者进行增删改操作，而服务器端则根据客户端的请求封装数据，以 json 格式返回给前端。 GraphQL 的语法可以参考[http://graphql.org/learn](http://graphql.org/learn)。
我们假设现在有一个电商服务需要同时有 iOS ， Android 和 PC web 三种客户端，该电商支持多种分类的商品的线上交易。作为该电商的用户可以在任意一种客户端上根据不同的分类浏览商品列表，查看商品详情，选择商品将其放入购物车并下单、购买。购买成功后，商品通过快递送到用户下单时填写的地址去。
我们将通过框架 graphql-java 来实现基于 GraphQL 的 BFF service 以应对三种客户端的数据请求。
## 抽象出合理的数据结构
GraphQL 需要服务器端预先定义出一系列数据结构，而客户端则根据定义的数据结构根据业务展示需求选择性的查询所需要的字段。因此在使用 GraphQL 时，第一步需要根据业务场景抽象出合理的数据结构。 这里我们可以使用*Domain-driven Design*的方法针对用户场景对数据建模，并从数据中选择出用户需要了解的数据，隐藏用户不应该知道的数据。我们可以得到以下数据结构:
```php
class Category {
	private String id;
	private String name;
	private List<Product> product;
}
class Product {
	private String id;
	private String name;
	private String description;
	private String thumbnail;
	private List<Sku> skus;
}
class Sku {
	private String id;
	private List<OptionPair> options;
	private Integer stock;
	private BigDecimal price;
}
class OptionPair {
	private String key;
	private String value;
}
class Order {
	private String id;
	private String userName;
	private String userMobile;
	private String address;
	private OrderStatus status;
	private BigDecimal price;
	private List<OrderLine> orderLines;
	private Date createTime;
	private Date purchaseTime;
	private Date finishTime;
}
class OrderLine {
	private String skuId;
	private String name;
	private Integer quantity;
	private BigDecimal price;
}
```
其中数据结构中含有 id 的 entity 可以当作*Aggregate Root*.客户端通过 GraphQL 查询数据的入口可以从上面列出的 entity 开始查询。
## 使用 graphql-java 实现服务端
### 定义基本类型
#### graphql-java 中的 ScalarType
在 graphql-java 中，除了 GraphQL 文档中说明的最基本的类型 *GraphQLInt*, *GraphQLFloat*, *GraphQLString*, *GraphQLBoolean*, *GraphQLID*之外，还包含了*GraphQLLong*, *GraphQLBigInteger*, *GraphQLBigDecimal*, *GraphQLByte*, *GraphQLShort*和*GraphQLChar*方便开发者使用。
**其中需要注意的是，当对 field 选用了*GraphQLID*时，只会接受*String*和*Integer*类型的值并将其转换为*String*传递出去, 而通常数据库默认定义的 id 是*Long*，如果用*GraphQLID*的话可能会出错。**
graphql-java 中，也可以自定义*ScalarType*比如定义*GraphQLDate*并将其 serialized, deserialized 为*timestamp*：
```java
public static final GraphQLScalarType GraphQLDate = new GraphQLScalarType("Date", "Built-in Date as timestamp", new Coercing() {
	@Override
	public Long serialize(Object input) {
		if (input instanceof Date) {
			return ((Date) input).getTime();
		}
		return null;
	}
	@Override
	public Date parseValue(Object input) {
		if (input instanceof Long) {
			return new Date((Long) input);
		}
		return null;
	}
	@Override
	public Date parseLiteral(Object input) {
		if (input instanceof IntValue) {
			return new Date(((IntValue) input).getValue().longValue());
		}
		return null;
	}
});
```
#### GraphQLEnumType
在构建 GraphQL 的数据结构之前，首先要做一些基础的准备工作，比如将 Java 数据结构中定义的*enum*类型转换为*GraphQLEnumType*
创建*GraphQLEnumType*可以使用函数`newEnum`，比如*OrderStatus*：
```
private GraphQLEnumType orderStatusEnum = newEnum()
		.name("OrderStatus")
		.description("order status")
		.value("OPEN", OrderStatus.OPEN, "unpaid order")
		.value("CLOSED", OrderStatus.CLOSED, "closed order")
		.value("CANCELLED", OrderStatus.CANCELLED, "cancelled order")
		.value("FULFILLED", OrderStatus.FULFILLED, "finished order")
		.build();
```
函数`value`声明：
```
public Builder value(String name)
public Builder value(String name, Object value)
public Builder value(String name, Object value, String description)
public Builder value(String name, Object value, String description, String deprecationReason)
```
当只传 name 时， name 就为 value 。
### GraphQLObjectType
现在我们可以把我们的数据结构定义为*GraphQLObjectType*了。定义在*GraphQLObjectType*里的每一个 field 都可以被前端得到，所以不应该在这里定义不希望被前端获取的字段，仅以 Order 为例
```
GraphQLObjectType orderLineType = newObject()
	.name("OrderLine")
	.field(field -> field.type(GraphQLID).name("productId"))
	.field(field -> field.type(GraphQLID).name("skuId"))
	.field(field -> field.type(GraphQLString).name("productName"))
	.field(field -> field.type(GraphQLString).name("skuName"))
	.field(field -> field.type(GraphQLInt).name("quantity"))
	.field(field -> field.type(GraphQLBigDecimal).name("price"))
	.build();
				
GraphQLObjectType orderType = newObject()
	.name("Order")
	.description("order")
	.field(field -> field.type(GraphQLID).name("id"))
	.field(field -> field.type(GraphQLString).name("userName"))
	.field(field -> field.type(GraphQLString).name("userMobile"))
	.field(field -> field.type(GraphQLString).name("address"))
	.field(field -> field.type(orderStatusEnum).name("status"))
	.field(field -> field.type(new GraphQLList(orderLineType)).name("orderLines"))
	.field(field -> field.type(GraphQLDate).name("purchaseTime"))
	.field(field -> field.type(GraphQLDate).name("finishTime"))
	.field(field -> field.type(GraphQLDate).name("timeCreated"))
	.build();
```
如果*GraphQLObjectType*的 field name 和 entity 的 field 类型一致的话， graphql-java 会自动做 mapping 。
### 查询
#### 带参数的查询
通常我们会创建一个用于查询的跟节点，客户端所有使用 GraphQL 进行查询的起始位置就在跟节点
```
public GraphQLObjectType getQueryType() {
		return newObject()
			.name("QueryType")
			.field(field -> field.type(orderType).name("order")
				.argument(argument -> argument.name("id").type(GraphQLID))
				.dataFetcher(dynamicDataFetcher::orderFetcher))
			.build();
```
这里我们在 QueryType 这个 object 中声明了一个类型为*orderType*的 field 叫*order*，获得*order*需要 argument *id*，同时声明了*order*的 data fetcher 。
```
public Order orderFetcher(DataFetchingEnvironment env) {
		String id = env.getArgument("id");
		return getOrder(id);
	}
```
`orderFetcher`接收一个*DataFetchingEnvironment*类型的参数，其中可以使用该参数的`getArgument`方法得到对应的传入参数，也可以使用`getSource`的到调用
 data fetcher 当前层的数据结构 比如*product*:
```
GraphQLObjectType productType = newObject()
	.name("Product")
	.description("product")
	.field(field -> field.type(GraphQLID).name("id"))
	.field(field -> field.type(GraphQLID).name("categoryId"))
	.field(field -> field.type(new GraphQLTypeReference("category")).name("category")
		.dataFetcher(productDataFetcher::categoryDataFetcher))
	...
	...
	.build();
public Category categoryDataFetcher(DataFetchingEnvironment env) {
	Product product = (Product)env.getSource()).getCategoryId();
	return getCategory(product.getCategoryId());
}
```
这里，我们通过`env.getSource()`方法拿到了*product*的数据结构，并根据已有的 categoryId 去查找 category 。
**注意 productType 的定义，我们在同时提供了 categoryId 和 category 两个 field ，是为了避免在用户需要得到 categoryId 的时候在做一次 data fetcher 的操作。同时，为了避免循环引用，我们使用了*GraphQLTypeReference*定义 category 的类型。**
### Mutation
GraphQL 同时支持写的操作，和查询一样，我们可以定义一个用于写数据的跟节点，在定义的 data fetcher 视线里进行数据的修改，并返回需要的属性。我们可以使用*GraphQLObjectType*定义更为复杂的传入参数：
```
private static final GraphQLInputObjectType inputOrderLineType = newInputObject()
	.name("InputOrderLineType")
	.field(field -> field.name("productId").type(GraphQLID))
	.field(field -> field.name("skuId").type(GraphQLID))
	.field(field -> field.name("quantity").type(GraphQLInt))
	.field(field -> field.name("price").type(GraphQLBigDecimal))
	.build();
private static final GraphQLInputObjectType inputOrderType = newInputObject()
	.name("InputOrderType")
	.field(field -> field.name("storeId").type(GraphQLID))
	.field(field -> field.name("orderLines").type(new GraphQLList(inputOrderLineType)))
	.build();
```
**要注意的是，当我们在 data fetcher 里得到*GraphQLInputObjectType*的参数的时候，得到的是一个类型为*LinkedHashMap*的数据。**
#### 提供 GraphQL API
API 层的代码如下
```
@Component
@Path("graphql")
@Consumes(MediaType.APPLICATION_JSON)
@Produces(MediaType.APPLICATION_JSON)
public class GraphQlApi {
	private static final Logger LOG = LoggerFactory.getLogger(GraphQlApi.class);
	
	@Autowired
	private QueryType queryType;
	@Autowired
	private MutationType mutationType;
	
	private GraphQL getGraphQl() {
		return new GraphQL(getSchema());
	}
	private GraphQLSchema getSchema() {
		return GraphQLSchema.newSchema()
			.query(queryType.getQueryType())
			.mutation(mutationType.getMutationType())
			.build();
	}
	@POST
	public Response executeOperation(Map body, @Context ContainerRequestContext request) {
		String query = (String) body.get("query");
		Map<String, Object> variables = (Map<String, Object>) body.get("variables");
		ExecutionResult executionResult = getGraphQl().execute(query, request, variables == null ? Maps.newHashMap() : variables);
		Map<String, Object> result = new LinkedHashMap<>();
		if (executionResult.getErrors().size() > 0) {
			LOG.warn("GraphQL command execute error, command: {} cause: {}", body, executionResult.getErrors());
			result.put("errors", executionResult.getErrors());
		}
		result.put("data", executionResult.getData());
		return Response.ok().entity(result).build();
	}
```
`execute`方法接收三个参数，其中第二个参数为 context ，我们将 request 直接传递了进去，用于之后的权限验证。
#### 权限验证
当用户访问一些敏感数据的时候，我们可能要对用户的权限进行验证，这时我们可以在 data fetcher 的实现里利用上面调用`execute`时传递的 context 进行验证了：
```java
public UserInfo userInfoFetcher(DataFetchingEnvironment env) {
	final ContainerRequestContext requestContext  = (ContainerRequestContext) env.getContext();
	// Using requestContext check permission here.
    ...
}
```
#### ErrorHandler
在执行 GraphQL 命令时，会进行 GraphQL Schema 和 GraphQL 命令的语法检查，并且会 handler 所有 data fetcher 的异常，最后转为*GraphQLError*的 list 放进*ExecutionResult*并返回给结果。*GraphQLError*接口声明如下：
```java
public interface GraphQLError {
    String getMessage();
    List<SourceLocation> getLocations();
    ErrorType getErrorType();
}
```
很多时候*GraphQLError*其实并不能满足实际情况的需要。所以需要做一些转换已满足使用需求。现提供一种思路如下:
```cpp
private List<Json> customError(ExecutionResult executionResult) {
    return executionResult.getErrors()
        .stream()
        .map(this::handleError)
        .map(this::toJson)
        .collect(Collectors.toList());
}
private Throwable handleError(GraphQLError error) {
    switch (error.getErrorType()) {
        case DataFetchingException:
            return ((ExceptionWhileDataFetching) error).getException();
        case ValidationError:
        case InvalidSyntax:
            return new Exception(error.getMessage());
        default:
            return new UnknownException();
    }
}
private Json toJson(Throwable throwable) {
    final Json json = Json.read(json(throwable));
    json.delAt("stackTrace");
    json.delAt("localizedMessage");
    json.delAt("cause");
    return json;
}
```
## GraphQL 的提速
GraphQL 的协议允许在调用`query`命令时用并行查询，而`mutation`时则禁止使用并行操作，如要实现`query`的并行，可以如下配置：
```java
ThreadPoolExecutor threadPoolExecutor = new ThreadPoolExecutor(
        2, /* core pool size 2 thread */
        2, /* max pool size 2 thread */
        30, TimeUnit.SECONDS,
        new LinkedBlockingQueue<Runnable>(),
        new ThreadPoolExecutor.CallerRunsPolicy());
GraphQL graphQL = GraphQL.newObject(StarWarsSchema.starWarsSchema)
        .queryExecutionStrategy(new ExecutorServiceExecutionStrategy(threadPoolExecutor))
        .mutationExecutionStrategy(new SimpleExecutionStrategy())
        .build();
```
而 data fetcher 的 cache 等操作，则需要开发者自行完成。
## 提供 GraphQL 的文档
运用`argument`和`dataFetcher`，我们可以定义出一个庞大的数据图，而前端则根据该数据图自行定义查询。可以使用工具[`graphdoc`](https://github.com/2fd/graphdoc) 来生成
 GraphQL 的文档提供给前端使用。 graphdoc 的用法很简单：
```php
# Install graphdoc
npm install -g @2fd/graphdoc
# Generate documentation from live endpoint
graphdoc -e https://your.api.uri/graphql -o ./graphql-schema
```
当然，如果客户端使用的 GraphQL 框架为`Apollo Client`，因此前端开发中测试与文档查看也可以使用 Chrome 浏览器的[```
Apollo
 Client Developer Tools
```](https://chrome.google.com/webstore/detail/apollo-client-developer-t/jdkknkkbebbapilgoeccciglkfbmbnfm) 插件。
