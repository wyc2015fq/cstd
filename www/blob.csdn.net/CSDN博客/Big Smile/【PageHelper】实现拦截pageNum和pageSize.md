# 【PageHelper】实现拦截pageNum和pageSize - Big Smile - CSDN博客
置顶2018年05月31日 16:38:53[王啸tr1912](https://me.csdn.net/tr1912)阅读数：2775
所属专栏：[java学习](https://blog.csdn.net/column/details/26775.html)
        我们在使用pageHelper的时候，总是需要往后端传pageNum和pageSize，以便我们在后面做分页处理，但是我们在controller中也需要在方法参数中接收并带到我们的数据查询层，中间基本对于这两个变量没有任何操作，这样做是在的很麻烦。今天我们写一个拦截，让这两个变量可以直接从请求中直接到我们的查询层，跳过从controller层到service层的艰辛。
# 一、构造基本类
        首先，我们先构造一个放pageHelper里面必须参数的一个类，相当于一个实体：
```java
import com.github.pagehelper.Page;
import java.io.Serializable;
import java.util.List;
public class PageBean<T> implements Serializable {
    private static final long serialVersionUID = 8656597559014685635L;
    private long total;        //总记录数
    private List<T> list;    //结果集
    private int pageNum;    // 第几页
    private int pageSize;    // 每页记录数
    private int pages;        // 总页数
    private int size;        // 当前页的数量 <= pageSize，该属性来自ArrayList的size属性
    /**
     * 包装Page对象，因为直接返回Page对象，在JSON处理以及其他情况下会被当成List来处理，
     * 而出现一些问题。
     *
     * @param list          page结果
     */
    public PageBean(List<T> list) {
        if (list instanceof Page) {
            Page<T> page = (Page<T>) list;
            this.pageNum = page.getPageNum();
            this.pageSize = page.getPageSize();
            this.total = page.getTotal();
            this.pages = page.getPages();
            this.list = page;
            this.size = page.size();
        }
    }
    public PageBean() {
        super();
    }
    public long getTotal() {
        return total;
    }
    public void setTotal(long total) {
        this.total = total;
    }
    public List<T> getList() {
        return list;
    }
    public void setList(List<T> list) {
        this.list = list;
    }
    public int getPageNum() {
        return pageNum;
    }
    public void setPageNum(int pageNum) {
        this.pageNum = pageNum;
    }
    public int getPageSize() {
        return pageSize;
    }
    public void setPageSize(int pageSize) {
        this.pageSize = pageSize;
    }
    public int getPages() {
        return pages;
    }
    public void setPages(int pages) {
        this.pages = pages;
    }
    public int getSize() {
        return size;
    }
    public void setSize(int size) {
        this.size = size;
    }
    
}
```
# 二、构造page的操作实体
然后我们再定义一个page的存储操作实体，以后我们记录的pageNum和pageSize都是从这里取出的：
```java
public class PaginationContext {
    // 定义threadLocal变量：pageNum和pageSize
    // 通过Filter 赋值
    private static ThreadLocal<Integer> pageNum = new ThreadLocal<Integer>();    // 第几页
    private static ThreadLocal<Integer> pageSize = new ThreadLocal<Integer>();    // 每页记录条数
    /*
     * pageNum ：get、set、remove
     */
    public static int getPageNum() {
        Integer pn = pageNum.get();
        if (pn == null) {
            return 1;
        }
        return pn;
    }
    public static void setPageNum(int pageNumValue) {
        pageNum.set(pageNumValue);
    }
    public static void removePageNum() {
        pageNum.remove();
    }
    /*
     * pageSize ：get、set、remove
     */
    public static int getPageSize() {
        Integer ps = pageSize.get();
        if (ps == null) {
            return 1;
        }
        return ps;
    }
    public static void setPageSize(int pageSizeValue) {
        pageSize.set(pageSizeValue);
    }
    public static void removePageSize() {
        pageSize.remove();
    }
}
```
构造好以上两个类之后，我们就可以进行一系列操作了。
# 三、构造一个请求拦截filter
        这个filter的主要作用就是用来解析我们的请求地址，也就是servlet中的request，然后拆分其中的pageNum参数和pageSize参数，然后进行存储，到pageContext中，如果没有传其他值的话，会设定一些默认值：
```java
import org.apache.commons.lang.StringUtils;
import javax.servlet.*;
import javax.servlet.http.HttpServletRequest;
import java.io.IOException;
/**
 * 读取分页信息
 */
public class PageFilter implements Filter {
    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws IOException, ServletException {
        HttpServletRequest httpRequest = (HttpServletRequest) request;
        PaginationContext.setPageNum(getPageNum(httpRequest));
        PaginationContext.setPageSize(getPageSize(httpRequest));
        try {
            chain.doFilter(request, response);
        }
        // 使用完Threadlocal，将其删除
        finally {
            PaginationContext.removePageNum();
            PaginationContext.removePageSize();
        }
    }
    /**
     * 获得page参数的值
     *
     * @param request
     * @return
     */
    protected int getPageNum(HttpServletRequest request) {
        int pageNum = 1;
        try {
            String pageNums = request.getParameter("pageNum");//统一参数
            if (pageNums != null && StringUtils.isNumeric(pageNums)) {
                pageNum = Integer.parseInt(pageNums);
            }
        } catch (NumberFormatException e) {
            e.printStackTrace();
        }
        return pageNum;
    }
    /**
     * 设置默认每页大小
     *
     * @return
     */
    protected int getPageSize(HttpServletRequest request) {
        int pageSize = 20;    // 默认每页10条记录
        try {
            String pageSizes = request.getParameter("pageSize");//统一参数
            if (pageSizes != null && StringUtils.isNumeric(pageSizes)) {
                pageSize = Integer.parseInt(pageSizes);
            }
        } catch (NumberFormatException e) {
            e.printStackTrace();
        }
        return pageSize;
    }
    @Override
    public void init(FilterConfig filterConfig) throws ServletException {
    }
    @Override
    public void destroy() {
    }
}
```
经过以上的过程，我们一个请求中的pageNum和pageSize就都到了threadLocal里面了，那么我们要怎么使用呢？
# 四、使用
        之前的请求都不用管了，在controller层的时候，我们选择接收的对象为pageBean的赋值构造型的一个实体，例如：
```java
PageBean<TBom> tBoms=null;
try{
     tBoms =bomFacade.findAllParent();
}catch (Exception e){
}
```
TBom为我们的一个实体类。
在service层：
```java
public PageBean<TBom> findAllParent() {
        PageHelper.startPage(PaginationContext.getPageNum(), PaginationContext.getPageSize());  //从threadLocal中获得num和size
        TBomExample tBomExample = new TBomExample();
        tBomExample.setOrderByClause("ordertime");
        TBomExample.Criteria criteria=tBomExample.createCriteria();
        criteria.andPidEqualTo("0000");
        criteria.andIsDeleteEqualTo((byte)0);
        List<TBom> tBoms=tBomDao.selectByExample(tBomExample);
        return new PageBean(tBoms) ;       //返回赋值后的pagebean对象
}
```
       我们可以看到，在service实现层，我们首先给pageHelper的操作类赋值了我们请求拦截时候获得的pageNum和pageSize两个值，然后再进行查询就可以实现我们分页效果了，当然我们最后还要把返回的实体放在我们的PageBean构造的实体中，用来获得除这两个值之外的其他值，以传输给页面。            
