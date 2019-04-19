# HttpClient和HttpGet 参数的优先级 - zhanlijun - 博客园
# [HttpClient和HttpGet 参数的优先级](https://www.cnblogs.com/LBSer/p/3295584.html)
#### [http://www.cnblogs.com/LBSer/p/3295584.html](http://www.cnblogs.com/LBSer/p/3295584.html)
一般在使用HttpClient时，我们提前设置好参数，比如超时时间（一般socket超时和连接超时）
private DefaultHttpClient createHttpClient() { //代码1
        ThreadSafeClientConnManager connectMag = new ThreadSafeClientConnManager();
        ...
        client = new DefaultHttpClient(connectMag);
        client.getParams().setParameter(CoreProtocolPNames.USER_AGENT,
                "...");
        client.getParams().setIntParameter(CoreConnectionPNames.SO_TIMEOUT,
                2000);
        client.getParams().setIntParameter(
                CoreConnectionPNames.CONNECTION_TIMEOUT, 1000);
        return client;
    }
但是我们也可以通过HttpUriRequest来设置参数，比如HttpGet、HttpPost。
httpGet.getParams().setIntParameter(
                    CoreConnectionPNames.SO_TIMEOUT, 5000);
            httpGet.getParams().setIntParameter(
                    CoreConnectionPNames.CONNECTION_TIMEOUT, 5000);
httpclient.execute(httpGet, new BasicResponseHandler());
这里的问题是：当我们既在HttlClent设置了超时时间，又在HttpGet设置了超时时间，那么到底以哪个设置为准？
仔细查看代码，发现httpclient.execute最终调用了以下代码，创建了RequestDirector director，在创建director中通过determineParams(request))函数设置了参数。
public final HttpResponse execute(HttpHost target, HttpRequest request,
                                      HttpContext context)
        throws IOException, ClientProtocolException {
        if (request == null) {
            throw new IllegalArgumentException
                ("Request must not be null.");
        }
        // a null target may be acceptable, this depends on the route planner
        // a null context is acceptable, default context created below
        HttpContext execContext = null;
        RequestDirector director = null;
        // Initialize the request execution context making copies of
        // all shared objects that are potentially threading unsafe.
        synchronized (this) {
            HttpContext defaultContext = createHttpContext();
            if (context == null) {
                execContext = defaultContext;
            } else {
                execContext = new DefaultedHttpContext(context, defaultContext);
            }
            // Create a director for this request
            director = createClientRequestDirector(
                    getRequestExecutor(),
                    getConnectionManager(),
                    getConnectionReuseStrategy(),
                    getConnectionKeepAliveStrategy(),
                    getRoutePlanner(),
                    getProtocolProcessor(),
                    getHttpRequestRetryHandler(),
                    getRedirectStrategy(),
                    getTargetAuthenticationHandler(),
                    getProxyAuthenticationHandler(),
                    getUserTokenHandler(),
                    determineParams(request)); //设置了参数
        }
        try {
            return director.execute(target, request, execContext);
        } catch(HttpException httpException) {
            throw new ClientProtocolException(httpException);
        }
    }
那determineParams(request))函数干了什么呢？其实是创建了个HttpParams，也就是ClientParamsStack（ClientParamsStack extends AbstractHttpParams，而AbstractHttpParams implements HttpParams）。
ClientParamsStack拿来干什么用的呢？Represents a stack of parameter collections. When retrieving a parameter, the stack is searched in a fixed order and the first match returned. Setting parameters via the stack is not supported. To minimize overhead, the stack has a fixed size and does not maintain an internal array. The supported stack entries, sorted by increasing priority （摘自：http://hc.apache.org/httpcomponents-client-ga/httpclient/apidocs/org/apache/http/impl/client/ClientParamsStack.html）
上面大意是：ClientParamsStack是个参数栈，这个参数栈里有四个参数，参数优先级是越来越高的，i.e. applicationParams < clientParams < requestParams < overrideParams，从这里可以看出requestParams优先级比clientParams高（在本例中，requestParams是从HttpGet设置的，而clientParams是HttpClient设置的），也就是说当HttpGet和HttpClient同时设置了超时时，以HttpGet设置的为准！
protected HttpParams determineParams(HttpRequest req) {
        return new ClientParamsStack
            (null, getParams(), req.getParams(), null);
    }
public ClientParamsStack(HttpParams aparams, HttpParams cparams,
                             HttpParams rparams, HttpParams oparams) {
        applicationParams = aparams;
        clientParams      = cparams;
        requestParams     = rparams;
        overrideParams    = oparams;
    }
既然各个参数有优先级，那么优先级是如何实现的呢？其实原理很简单，也就是按overrideParams、requestParams、clientParams、applicationParams的顺序依次判断，如果不为空就返回。（注：getParameter()函数经常被底层实现用到）
public Object getParameter(String name) {
        if (name == null) {
            throw new IllegalArgumentException
                ("Parameter name must not be null.");
        }
        Object result = null;
        if (overrideParams != null) {
            result = overrideParams.getParameter(name);
        }
        if ((result == null) && (requestParams != null)) {
            result = requestParams.getParameter(name);
        }
        if ((result == null) && (clientParams != null)) {
            result = clientParams.getParameter(name);
        }
        if ((result == null) && (applicationParams != null)) {
            result = applicationParams.getParameter(name);
        }
        return result;
    }
