# django 多线程 - Arthur的随笔 - CSDN博客
2011年08月23日 15:35:52[largetalk](https://me.csdn.net/largetalk)阅读数：4455
from:  [http://onlypython.group.iteye.com/group/wiki/1516-use-cherrypy-as-django-s-development-server](http://onlypython.group.iteye.com/group/wiki/1516-use-cherrypy-as-django-s-development-server)
该多线程是指多线程development server
1. 
修改Django自带的Development Server为多线程模式，这其实只需要改几行代码就可以了。（详见[http://code.djangoproject.com/ticket/3357](http://code.djangoproject.com/ticket/3357) ）
打开文件 django/core/servers/basehttp.py
第10行：
from BaseHTTPServer import BaseHTTPRequestHandler, HTTPServer
修改为
from BaseHTTPServer import BaseHTTPRequestHandler
在所有的 import 语句之后加上以下代码：
Python代码  ![收藏代码](http://onlypython.group.iteye.com/images/icon_star.png)
- import BaseHTTPServer, SocketServer  
- class HTTPServer(SocketServer.ThreadingMixIn, BaseHTTPServer.HTTPServer):    
- def __init__(self, server_address, RequestHandlerClass=None):    
-         BaseHTTPServer.HTTPServer.__init__(self, server_address, RequestHandlerClass)  
是的，这样就可以了。 但是他还有一个小问题就是它不是线程安全的。可能在请求很多时会出现数据不同步，当然，这一般不是问题，因为我们通常只在自己机器上调试时才用Development Server。
2.
使用CherryPy 作为 Development Server
在安装好CherryPy 之后，将以下代码保存为文件runservercp.py
Python代码  ![收藏代码](http://onlypython.group.iteye.com/images/icon_star.png)
- from django.core.management.base import BaseCommand, CommandError  
- from optparse import make_option  
- import os, sys  
- import cherrypy  
- from cherrypy import wsgiserver  
- class Command(BaseCommand):  
-     option_list = BaseCommand.option_list + (  
-         make_option('--noreload', action='store_false', dest='use_reloader', default=True,  
-             help='Tells Django to NOT use the auto-reloader.'),  
-         make_option('--adminmedia', dest='admin_media_path', default='',  
-             help='Specifies the directory from which to serve admin media.'),  
-     )  
-     help = "Starts a lightweight Web server for development."
-     args = '[optional port number, or ipaddr:port]'
- 
- # Validation is called explicitly each time the server is reloaded.
-     requires_model_validation = False
- 
- def handle(self, addrport='', *args, **options):  
- import django  
- from django.core.servers.basehttp import AdminMediaHandler  
- from django.core.handlers.wsgi import WSGIHandler  
- if args:  
- raise CommandError('Usage is runservercp %s' % self.args)  
- ifnot addrport:  
-             addr = ''
-             port = '8000'
- else:  
- try:  
-                 addr, port = addrport.split(':')  
- except ValueError:  
-                 addr, port = '', addrport  
- ifnot addr:  
-             addr = '127.0.0.1'
- 
- ifnot port.isdigit():  
- raise CommandError("%r is not a valid port number." % port)  
- 
-         use_reloader = options.get('use_reloader', True)  
-         admin_media_path = options.get('admin_media_path', '')  
-         shutdown_message = options.get('shutdown_message', '')  
-         quit_command = (sys.platform == 'win32') and'CTRL-BREAK'or'CONTROL-C'
- 
- def inner_run():  
- from django.conf import settings  
- from django.utils import translation  
- print"\nValidating models..."
- self.validate(display_num_errors=True)  
- print"\nDjango version %s, using settings %r" % (django.get_version(), settings.SETTINGS_MODULE)  
- print"Development server is running at http://%s:%s/" % (addr, port)  
- print"Quit the server with %s." % quit_command  
- 
- # django.core.management.base forces the locale to en-us. We should
- # set it up correctly for the first request (particularly important
- # in the "--noreload" case).
-             translation.activate(settings.LANGUAGE_CODE)  
- 
- try:  
-                 path = admin_media_path or django.__path__[0] + '/contrib/admin/media'
-                 handler = AdminMediaHandler(WSGIHandler(), path)  
-                 cherrypy.config.update({  
- 'server.socket_host': addr,  
- 'server.socket_port': int(port),  
- 'engine.autoreload_on':  use_reloader  
-                     })  
- 
-                 cherrypy.tree.graft(handler, '/')  
- 
-                 cherrypy.engine.start()  
-                 cherrypy.engine.block()  
- finally:  
- if shutdown_message:  
- print shutdown_message  
-                 cherrypy.engine.exit()  
-         inner_run()  
将该文件放到djang源代码目录 django\core\management\commands 下，这样运行命令
python manage.py runservercp
时就使用了CherryPy 作为服务器了，其中命令名是根据文件的名字而定的，因为该文件保存为runservercp.py，所以命令名为 runservercp。
另外放在django的源代码目录的好处是所有的django project都可以使用该命令使用到CherryPy 。
