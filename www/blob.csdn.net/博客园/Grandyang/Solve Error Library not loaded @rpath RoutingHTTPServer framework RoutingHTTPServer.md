# Solve Error: Library not loaded: @rpath/RoutingHTTPServer.framework/RoutingHTTPServer - Grandyang - 博客园







# [Solve Error: Library not loaded: @rpath/RoutingHTTPServer.framework/RoutingHTTPServer](https://www.cnblogs.com/grandyang/p/8203024.html)







在配置WebDriverAgent的时候，可能会遇到如下的错误：



2018-01-04 09:53:42.759370-0600 WebDriverAgentRunner-Runner[318:13300] +[CATransaction synchronize] called within transaction

2018-01-04 09:53:43.090645-0600 WebDriverAgentRunner-Runner[318:13300] Running tests...

2018-01-04 09:53:43.354179-0600 WebDriverAgentRunner-Runner[318:13300] The bundle “WebDriverAgentRunner” couldn’t be loaded because it is damaged or missing necessary resources. Try reinstalling the bundle.

2018-01-04 09:53:43.354342-0600 WebDriverAgentRunner-Runner[318:13300] (dlopen_preflight(/var/containers/Bundle/Application/41C8100D-E3BF-4D1B-9A26-BA9C9E3479F8/WebDriverAgentRunner-Runner.app/PlugIns/WebDriverAgentRunner.xctest/WebDriverAgentRunner): Library not loaded: @rpath/RoutingHTTPServer.framework/RoutingHTTPServer

  Referenced from: /private/var/containers/Bundle/Application/41C8100D-E3BF-4D1B-9A26-BA9C9E3479F8/WebDriverAgentRunner-Runner.app/PlugIns/WebDriverAgentRunner.xctest/Frameworks/WebDriverAgentLib.framework/WebDriverAgentLib

  Reason: no suitable image found.  Did find:

/private/var/containers/Bundle/Application/41C8100D-E3BF-4D1B-9A26-BA9C9E3479F8/WebDriverAgentRunner-Runner.app/PlugIns/WebDriverAgentRunner.xctest/Frameworks/WebDriverAgentLib.framework/Frameworks/RoutingHTTPServer.framework/RoutingHTTPServer: required code signature missing for '/private/var/containers/Bundle/Application/41C8100D-E3BF-4D1B-9A26-BA9C9E3479F8/WebDriverAgentRunner-Runner.app/PlugIns/WebDriverAgentRunner.xctest/Frameworks/WebDriverAgentLib.framework/Frameworks/RoutingHTTPServer.framework/RoutingHTTPServer'

)



Solution：

Go to **TARGETS** -> **WebDriverAgentRunner** -> **Build Phases** -> **Copy frameworks**

Add **RoutingHTTPServer.framework**





![](https://images2017.cnblogs.com/blog/391947/201801/391947-20180105105206956-987354785.png)














