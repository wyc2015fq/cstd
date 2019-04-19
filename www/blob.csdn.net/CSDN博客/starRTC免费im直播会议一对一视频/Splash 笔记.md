# Splash 笔记 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年11月23日 15:05:14[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：172
javascript rendering service
a lightweight web browser
can execute custom JavaScript in page context
get detailed rendering info in HAR format([http://www.softwareishard.com/blog/har-12-spec/](http://www.softwareishard.com/blog/har-12-spec/))
endpoints 末端that
 provide all
Splash features are [execute](http://splash.readthedocs.io/en/stable/api.html#execute) and [run](http://splash.readthedocs.io/en/stable/api.html#run);
 they allow to execute arbitrary
Lua rendering scripts.
**render.html**
Return the HTML of the javascript-rendered page.
curl'http://localhost:8050/render.html?url=http://domain.com/page-with-javascript.html&timeout=10&wait=0.5'
**render.json**
Return a json-encoded dictionary with information about javascript-rendered webpage.
 It can include HTML, PNG and other information, based on arguments passed.
**html：**Whether
 to include HTML in output
**iframes ：**Whether
 to include information about child frames in output. 
**Response Object**
contain information about a response.
**response.info**
A Lua table with response data in [HAR
 response](http://www.softwareishard.com/blog/har-12-spec/#response) format.
**response.body**
Raw response body (a [binary
 object](http://splash.readthedocs.io/en/stable/scripting-binary-data.html#binary-objects)).
