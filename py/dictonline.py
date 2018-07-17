# -*- coding: utf-8 -*-  
import urllib.request, simplejson  

if 0:
	while True:  
		url = 'http://fanyi.youdao.com/openapi.do?key=79379998&keyfrom=justdoit&type=data&doctype=json&version=1.1&q='+ input(">>")  
		print( urllib.request.urlopen(url).read().decode(''))
		#print(simplejson.loads(urllib.request.urlopen(url).read()).get('translation','')[0].encode("utf-8")  )


print(12)
url = 'http://fanyi.youdao.com/openapi.do?key=79379998&keyfrom=justdoit&type=data&doctype=json&version=1.1&q=hello'
print( simplejson.loads(urllib.request.urlopen(url).read()).get('translation','')[0])