# spring-boot集成graphql入门教程 - z69183787的专栏 - CSDN博客
2018年04月24日 00:19:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3486
[https://my.oschina.net/genghz/blog/1789240](https://my.oschina.net/genghz/blog/1789240)
摘要: 这是一个spring-boot 集成 graphql 的入门教程, 仅供参考学习, 初次写, 请多见谅
本文介绍一个spring-boot + graphql, 是一个 graphql java 入门项目
#### graphql 到底是什么
    graphql 是一种 API 查询语言, 用于服务器端执行按已定义类型系统的查询. GraphQL 不与任何特定的数据库或存储引擎进行绑定, 而是由您的代码和数据支持.(官方描述)
    说白了 就是想要什么, 就传入什么字段, 也就会返回什么字段, 具体字段处理是服务器所提供, 而 graphql 并不会关心怎么服务器怎么处理
例如：
    传统的rest api:  /test/user/{id}  return { id, name, age } 是一成不变的,
    graphql:  findOneUser(id: xx)  return { id, name }  (注: 传输参数id, 指定返回字段 id, name, 当然也可以写{ name, age },完全取决于前端需求 )
#### graphql 的优势
    graphql 大大减少了沟通成本, 避免每次了定义api字段的多少问题, 前端自己选择、组合想要的字段, 生成数据, graphql 提供了GUI 可以很方便的测试, 书写graphql语句, 查看服务提供的 doc, 详细信息请看 [https://graphql.cn/](https://graphql.cn/)
#### 项目构建
spring-boot 是基于 2.0.0版本, 想了解spring-boot [https://gitee.com/geng_hz/Spring-Boot-Reference-Guide](https://gitee.com/geng_hz/Spring-Boot-Reference-Guide)
<parent><groupId>org.springframework.boot</groupId><artifactId>spring-boot-starter-parent</artifactId><version>2.0.0.RELEASE</version></parent>
大神们封装的 spring-boot 依赖
<!-- graphql --><dependency><groupId>com.graphql-java</groupId><artifactId>graphql-spring-boot-starter</artifactId><version>4.0.0</version></dependency><dependency><groupId>com.graphql-java</groupId><artifactId>graphql-java-tools</artifactId><version>4.3.0</version></dependency>
项目的目录( 随便分的包最后会附上链接代码):
![](https://static.oschina.net/uploads/space/2018/0403/150407_uhV0_2725939.png)
root.graphqls 是graphql 服务入口定义:
typeQuery {
    findAllAuthors: [Author]!
    countAuthors: Long!
    findOneAuthor(id: Long!): AuthorfindAllBooks: [Book]!
    countBooks: Long!
}typeMutation {
    newAuthor(firstName: String!, lastName: String!) : Author!
    newBook(title: String!, isbn: String!, pageCount: Int, authorId: Long!) : Book!
    saveBook(input: BookInput!): Book!
    deleteBook(id: ID!) : BooleanupdateBookPageCount(pageCount: Int!, id: Long!) : Book!
}
scheme.graphqls 则是 query/mutation 具体的 scheme 定义字段、类型
typeAuthor {
    id: Long!
    createdTime: String
    firstName: String
    lastName: String
    books: [Book]
}
inputBookInput {
    title: String!
    isbn: String!
    pageCount: Int
    authorId: Long
}
typeBook {
    id: Long!
    title: String!
    isbn: String!
    pageCount: Int
    author: Author
}
Query 是查询入口,  Mutation则是修改入口
例: findOneAuthor 传入一个long id, 返回一个 Author schema, 
graphql 入口定义了, 但这只是一个描述, 我们需要实现 query/mutation中的描述
例如：
publicclassQueryimplementsGraphQLQueryResolver{
    private AuthorRepository authorRepository;
    private BookRepository bookRepository;
    public Author findOneAuthor(Long id) {
        Optional<Author> opt = authorRepository.findById(id);
        return opt.isPresent() ? opt.get() : null;
    }
    public List<Author> findAllAuthors() {
        return authorRepository.findAll();
    }
    publicLong countAuthors() {
        return authorRepository.count();
    }
    public List<Book> findAllBooks() {
        return bookRepository.findAll();
    }
    publicLong countBooks() {
        return bookRepository.count();
    }
}
实现了所有的 Query中的描述(必须全部实现)
schema 一样像我的目录中的一样 AuthorResolver 这是对schema中的描述的实现
注： query/mutation和普通的schema 一样, 只是它们是 graphql服务的入口, resolver实现描述遵循:
1. method <name>(*args)
2.method is<Name>(*args) 仅支持 return boolean
3.method get<Name>(*args)
4.method getField<Name>(*args)
这是种实现, 当提供了resolver时优先使用, 其次是 class this.get<Name>方法
Author.class 中的createdTime 是 Date, 然而 schema Author { createdTime: String }, 所以单独提供AuthorResovler 生成createdTime String, 而其他参数因为与schema Author类型一致，使用Author中的Get方法足够了
@Component@AllArgsConstructor
public class AuthorResolver implements GraphQLResolver<Author> {
    private static final SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    private BookRepository bookRepository;
    publicStringgetCreatedTime(Author author) {
        return sdf.format(author.getCreatedTime());
    }
    public List<Book> getBooks(Author author) {
        return bookRepository.findByAuthorId(author.getId());
    }
}
执行脚本: ./run.sh 启动项目, graphql 的默认endpoint:  /graphql
![](https://static.oschina.net/uploads/space/2018/0403/141435_UjJo_2725939.png)
graphql GUI, 方便的用来编写测试 graphql 和 查看当前服务提供了那些可用的方法, 像这样:
![](https://static.oschina.net/uploads/space/2018/0403/141719_1Cij_2725939.png)
#### demo 代码、GUI奉上：
[https://gitee.com/geng_hz/spring-boot-graphql](https://gitee.com/geng_hz/spring-boot-graphql)
