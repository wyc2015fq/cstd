# 近看 Spring MVC 3.1.2.RELEASE 的 ContentNegotiatingViewResolve - z69183787的专栏 - CSDN博客
2014年12月03日 09:49:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1686
诞生自 3.0 的 ContentNegotiatingViewResolver 从字义不太容易知道在做什么或怎么用，那就从 Source code 好好来研究一下。
每个 ViewResolver 的重点就是 resolveViewName()。
public View resolveViewName(String viewName, Locale locale) throws Exception {
  RequestAttributes attrs = RequestContextHolder.getRequestAttributes();
  Assert.isInstanceOf(ServletRequestAttributes.class, attrs);
  List<MediaType> requestedMediaTypes = getMediaTypes(((ServletRequestAttributes) attrs).getRequest());
  if (requestedMediaTypes != null) {
    List<View> candidateViews = getCandidateViews(viewName, locale, requestedMediaTypes);
    View bestView = getBestView(candidateViews, requestedMediaTypes);
    if (bestView != null) {
      return bestView;
    }
  }
  if (this.useNotAcceptableStatusCode) {
    // ...
    return NOT_ACCEPTABLE_VIEW;
  }
  else {
    logger.debug("No acceptable view found; returning null");
    return null;
  }
}
得到三个线索，分别为
**getMediaTypes()、getCandidateViews() 与 getBestView()**。
除此之后，可以知道如果找不到合适的 View 对象，有两条路可以走，如果将 useNotAcceptableStatusCode  设为 true（预设为 false），那就会丢出 406 Not Acceptable 错误，如果设为 false，那就回传 null，表示交由下一个 View Resolver 处理，这就是 View Resolver Chaining。
getBestView() 
getBestView() 是 private 而且没有加批注，所以在 JavaDoc 里看不到。
private View getBestView(List<View> candidateViews, List<MediaType> requestedMediaTypes) {
  for (View candidateView : candidateViews) {
    if (candidateView instanceof SmartView) {
      SmartView smartView = (SmartView) candidateView;
      if (smartView.isRedirectView()) {
        // ...
        return candidateView;
      }
    }
  }
  for (MediaType mediaType : requestedMediaTypes) {
    for (View candidateView : candidateViews) {
      if (StringUtils.hasText(candidateView.getContentType())) {
        MediaType candidateContentType = MediaType.parseMediaType(candidateView.getContentType());
        if (mediaType.includes(candidateContentType)) {
          // ...
          return candidateView;
        }
      }
    }
  }
  return null;
}
内容分成两部份，第一部份在判断是否为 RedirectView（SmartView 唯一的实作），是的话就直接送走，不用往下走了，第二部份才是重点。 
ContentNegotiatingViewResolver 从字义直翻是「内容谈判」，不懂，那叫做「内容协调」，还是不懂，那叫做「依据 request 决定 response」，嗯，废话,，哪个 request 不是这样！
不完全是，看例子。
http://.../hello.html
http://.../hello.pdf
http://.../hello.xml
看似三个 url，需要三个 requestMapping 处理，但其实产出给 View 的内容是一样的，那可不可以用同一个 requestMapping 处理，然后依据「某些讯息」回传不同格式的 View 对象呢？这就是 ContentNegotiatingViewResolver 要做的事情。
ContentNegotiatingViewResolver 右手拿着 request 想要的格式，左手握着可能的 View 对象（怎么来的？稍后讲），交叉比对，若有符合就回传，举例来说，request 想要 PDF，就去 View 里找有没有提供 PDF 格式的 View 对象，若有则回传，若没有呢？再看 request 里有没有第二志愿、第三志愿，如果都没有，那就爆了，这就是 getBestView() 第二部份在做的事情。
从 getBestView() 往前追踪，传进来的两个参数分别为 List<View> candidateViews 与 List<MediaType> requestedMediaTypes，先看第二个参数，也就是 ContentNegotiatingViewResolver 右手拿的东西，也是前面提到的「
**依据『某些讯息』回传不同格式的 View 对象**」的讯息来源。 
这就可以追踪到 getMediaType() 这个 method。 
**getMediaType()**
protected List<MediaType> getMediaTypes(HttpServletRequest request) {
  if (this.favorPathExtension) {
    String requestUri = urlPathHelper.getLookupPathForRequest(request);
    String filename = WebUtils.extractFullFilenameFromUrlPath(requestUri);
    MediaType mediaType = getMediaTypeFromFilename(filename);
    if (mediaType != null) {
      // ...
      return Collections.singletonList(mediaType);
    }
  }
  if (this.favorParameter) {
    if (request.getParameter(this.parameterName) != null) {
      String parameterValue = request.getParameter(this.parameterName);
      MediaType mediaType = getMediaTypeFromParameter(parameterValue);
      if (mediaType != null) {
        // ...
        return Collections.singletonList(mediaType);
      }
    }
  }
  if (!this.ignoreAcceptHeader) {
    String acceptHeader = request.getHeader(ACCEPT_HEADER);
    if (StringUtils.hasText(acceptHeader)) {
      // ...
    }
  }
  if (this.defaultContentType != null) {
    // ...
    return Collections.singletonList(this.defaultContentType);
  }
  else {
    return Collections.emptyList();
  }
}
getMediaType()  有四种选项，依序为扩展名、request 参数、Accept Header 与默认值。 
**扩展名**
预设开启（favorPathExtension），也是建议使用的方式，因为扩展名可以让浏览器知道回传的文件类型，而使用正确的应用程序来开启。
http://.../hello.html
http://.../hello.pdf
http://.../hello.xml
**request 参数**
预设关闭（favorParameter），需搭配 parameterName（预设为 format）使用。
http://.../hello?format=html
http://.../hello?format=pdf
http://.../hello?format=xml
这边有个奇怪的地方，ContentNegotiatingViewResolver 有个 mediaTypes 属性，是用来设定 ContentNegotiatingViewResolver 需要处理的扩展名，奇怪的地方在于前一项「扩展名」不需要 mediaTypes，而这一项「request 参数」一定需要。
protected MediaType getMediaTypeFromFilename(String filename) {
  String extension = StringUtils.getFilenameExtension(filename);
  if (!StringUtils.hasText(extension)) {
    return null;
  }
  extension = extension.toLowerCase(Locale.ENGLISH);
  MediaType mediaType = this.mediaTypes.get(extension);
  if (mediaType == null) {
    String mimeType = getServletContext().getMimeType(filename);
    if (StringUtils.hasText(mimeType)) {
      mediaType = MediaType.parseMediaType(mimeType);
    }
    // ...
    if (mediaType != null) {
      this.mediaTypes.putIfAbsent(extension, mediaType);
    }
  }
  return mediaType;
}
从扩展名取 MediaType 时，如果不存在 mediaTypes 属性定义范围内，则会去 ServletContext 里找，这边一般的档案都应该找得到，找到之后，甚至回存到 mediaTypes 里。
protected MediaType getMediaTypeFromParameter(String parameterValue) {
  return this.mediaTypes.get(parameterValue.toLowerCase(Locale.ENGLISH));
}
但是从 request 参数里找 MediaType 时，则完全不管找不到的状况。 
换句话说，也就是说，
**使用 request 参数指定扩展名时，一定要将这个扩展名也定义在 mediaTypes 属性里**。 
唯一我能想到的合理解释就是，使用 request 参数不一定要用「真实的扩展名」，也许可以用暗号，然后在 mediaTypes 里再加上暗号与真实 MediaType 的对照。
**Accept Header**
预设开启（ignoreAcceptHeader），但无法从网址中指定，所以常用在由程序送出 request 这类的 Web service。 
**默认值**
由 defaultContentType 设定。 
**getCandidateViews()**
看完 ContentNegotiatingViewResolver  右手的 MediaType，来看看左手的 View 对象，View 对象有两个来源，分别为其他的 ViewResolver 与默认值。
其他的 ViewResolver？神奇吧，ContentNegotiatingViewResolver 除了默认的 View 对象，也就是由 defaultViews 定义的 View 对象以外，本身不事生产，而是向其他的 ViewResolver 要 View 对象。
ContentNegotiatingViewResolver 预设是由 Spring 找出所有其他的 ViewResolver 来建立 View 对象，但是也可以透过 viewResolvers 属性明确指定使用哪些 ViewResolver。
也因为这个特性，ContentNegotiatingViewResolver 的 order 属性必须有相当高的优先权，至少得在被用到的 ViewResolver 之前，不然就发挥不了作用了，默认值为 Ordered.HIGHEST_PRECEDENCE，也就是最小值 Integer.MIN_VALUE。
private List<View> getCandidateViews(String viewName, Locale locale, List<MediaType> requestedMediaTypes)
    throws Exception {
  List<View> candidateViews = new ArrayList<View>();
  for (ViewResolver viewResolver : this.viewResolvers) {
    View view = viewResolver.resolveViewName(viewName, locale);
    if (view != null) {
      candidateViews.add(view);
    }
    for (MediaType requestedMediaType : requestedMediaTypes) {
      List<String> extensions = getExtensionsForMediaType(requestedMediaType);
      for (String extension : extensions) {
        String viewNameWithExtension = viewName + "." + extension;
        view = viewResolver.resolveViewName(viewNameWithExtension, locale);
        if (view != null) {
          candidateViews.add(view);
        }
      }
    }
  }
  if (!CollectionUtils.isEmpty(this.defaultViews)) {
    candidateViews.addAll(this.defaultViews);
  }
  return candidateViews;
}
先要到所有的 ViewResolver，当然本身除外，然后用 viewName 去找，找到后再用 MediaType 去过滤，都符合的就取得 View 对象候选资格，全部的 ViewResolver 看过之后，再加入由 defaultViews 属性定义的 View 对象（这属性好用吗？），就是左手的 View 对象。
