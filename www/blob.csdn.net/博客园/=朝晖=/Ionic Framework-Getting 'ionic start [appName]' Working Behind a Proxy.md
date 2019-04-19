# Ionic Framework - Getting 'ionic start [appName]' Working Behind a Proxy - =朝晖= - 博客园
# [Ionic Framework - Getting 'ionic start [appName]' Working Behind a Proxy](https://www.cnblogs.com/dhcn/p/7115412.html)
This is a quick hacky way to get the **ionic start [appName] **command working from behind a proxy. I ran into the problem going through the steps on the [Getting Started with Ionic](http://ionicframework.com/getting-started/) page. For some reason the ionic cmd doesn't use the npm proxy or git proxy settings.
I did this on a Windows 7 machine but the same should work in another OS just the file locations will be different.
**1. Add the proxy url to request.js** located at 'C:\Users\[username]\AppData\Roaming\npm\node_modules\ionic\node_modules\request\request.js'. Here's the update including the lines just before and after so you can find where to put it.
```js
if (self.strictSSL === false) {
    self.rejectUnauthorized = false
  }
  // hack to add proxy
  self.proxy = 'http://10.68.0.5:8080';
  if (self.proxy) {
    if (typeof self.proxy == 'string') self.proxy = url.parse(self.proxy)
    // do the HTTP CONNECT dance using koichik/node-tunnel
    if (http.globalAgent && self.uri.protocol === "https:") {
      var tunnelFn = self.proxy.protocol === "http:"
                   ? tunnel.httpsOverHttp : tunnel.httpsOverHttps
      var tunnelOptions = { proxy: { host: self.proxy.hostname
                                   , port: +self.proxy.port
                                   , proxyAuth: self.proxy.auth
                                   , headers: { Host: self.uri.hostname + ':' +
                                        (self.uri.port || self.uri.protocol === 'https:' ? 443 : 80) }}
                          , rejectUnauthorized: self.rejectUnauthorized
                          , ca: this.ca }
      self.agent = tunnelFn(tunnelOptions)
      self.tunnel = true
    }
  }
```
**2. Add****the proxy url to registry.js **located at 'C:\Users\[username]\AppData\Roaming\npm\node_modules\cordova\node_modules\plugman\src\registry\registry.js', again I've included the lines around the hack so you can find where to put it.
```js
headers.accept = "application/json"
  headers["user-agent"] = settings['user-agent'] ||
                          'node/' + process.version
  // hack to add proxy
  var p = settings.proxy || 'http://10.68.0.5:8080'
  var sp = settings['https-proxy'] || p
  opts.proxy = remote.protocol === "https:" ? sp : p
  // figure out wth 'what' is
  if (what) {
    if (Buffer.isBuffer(what) || typeof what === "string") {
      opts.body = what
      headers["content-type"] = "application/json"
      headers["content-length"] = Buffer.byteLength(what)
    } else {
      opts.json = what
    }
  }
```
That's it! You should now be able to create ionic apps without an issue.
Just in case people are searching for how to do this by googling the command line errors, I've included them below.
**First error**
```plain
D:\Projects>ionic start myApp tabs
Running start task...
Creating Ionic app in folder D:\Projects\myApp based on tabs project
DOWNLOADING: https://github.com/driftyco/ionic-app-base/archive/master.zip
TypeError: Cannot read property 'statusCode' of undefined
    at Request._callback (C:\Users\watmorj\AppData\Roaming\npm\node_modules\ionic\lib\ionic.js:241:13)
    at self.callback (C:\Users\watmorj\AppData\Roaming\npm\node_modules\ionic\node_modules\request\request.js:129:22)
    at Request.EventEmitter.emit (events.js:95:17)
    at ClientRequest.self.clientErrorHandler (C:\Users\watmorj\AppData\Roaming\npm\node_modules\ionic\node_modules\request\request.js:239:10)
    at ClientRequest.EventEmitter.emit (events.js:95:17)
    at CleartextStream.socketErrorListener (http.js:1547:9)
    at CleartextStream.EventEmitter.emit (events.js:95:17)
    at Socket.onerror (tls.js:1445:17)
    at Socket.EventEmitter.emit (events.js:117:20)
    at net.js:441:14
```
**Second error**
```plain
Initializing cordova project.
Fetching plugin "org.apache.cordova.device" via plugin registry
Error: Failed to fetch package information for org.apache.cordova.device
    at C:\Users\watmorj\AppData\Roaming\npm\node_modules\cordova\node_modules\plugman\src\registry\registry.js:32:20
    at Request.cb [as _callback] (C:\Users\watmorj\AppData\Roaming\npm\node_modules\cordova\node_modules\plugman\src\registry\registry.js:251:9)
    at self.callback (C:\Users\watmorj\AppData\Roaming\npm\node_modules\cordova\node_modules\plugman\node_modules\request\index.js:148:22)
    at Request.EventEmitter.emit (events.js:117:20)
    at ClientRequest.self.clientErrorHandler (C:\Users\watmorj\AppData\Roaming\npm\node_modules\cordova\node_modules\plugman\node_modules\request\index.js:257:10)
    at ClientRequest.EventEmitter.emit (events.js:95:17)
    at Socket.socketErrorListener (http.js:1547:9)
    at Socket.EventEmitter.emit (events.js:95:17)
    at net.js:441:14
    at process._tickCallback (node.js:415:13)
ERROR: Unable to add plugins. Perhaps your version of Cordova is too old. Try updating (npm install -g cordova), removing this project folder, and trying again.
Exiting.
```
