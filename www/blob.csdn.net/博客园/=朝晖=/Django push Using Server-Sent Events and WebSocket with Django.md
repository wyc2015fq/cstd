# Django push: Using Server-Sent Events and WebSocket with Django - =朝晖= - 博客园
# [Django push: Using Server-Sent Events and WebSocket with Django](https://www.cnblogs.com/dhcn/p/7124715.html)
http://curella.org/blog/2012/jul/17/django-push-using-server-sent-events-and-websocket/
The goal of this article is to explore and show how it's possible to implement Server-Sent Events and WebSocket with Django.
There are other implementations out there for frameworks that are designed specifically to work in event-based scenario (tornado, [Node.js](http://lib.csdn.net/base/nodejs)), and are probably better suited for implementing these kind of services.
The point of this article is not "you should use Django for that", but a more humble "here's how I made it work with Django".
## The Scenario
Suppose you have a website where users can import their contacts from other services. The importing is handled off-band by some other means (most likely, a celery task), and you want to show your users a notification box when the job is done.
There are currently a few alternative technologies for pushing events to the browser: Server-Sent Events (SSE) and WebSocket.
SSEs are a simpler protocol and are easier to implement, but they provide communication only in one direction, from the server to the browser. WebSocket provides instead a bidirectional channel.
For a simple notification scenario like the above, SSEs provide just what we want, at the expenses of one long-running connection per user.
We will use [Redis](http://lib.csdn.net/base/redis) and its PubSub functionality as a broker between the celery task and Django's web process.
The final code of this article is available as a [repository on GitHub](https://github.com/fcurella/django-push-demo).
## Architecture
Celery Task -> [redis](http://lib.csdn.net/base/redis) -> Django -> Browser
## Running gunicorn
Both technologies require the server to keep the connection open indefinitely.
If we'd run Django under mod_wsgi or the regular dev server, the request-response cycle will be blocked by those always-open requests.
The solution is to use `gevent`. I found that the simplest way to use it is to run Django under gunicorn.
Install gunicorn:
```
$ pip install gunicorn
```
Add gunicorn to your `INSTALLED_APPS`:
```
INSTALLED_APPS = (
    ...,
    'myapp',
    'gunicorn',
)
```
Then, I created a config file for gunicorn at `config/gunicorn`.
```
#!python
from os import environ
from gevent import monkey
monkey.patch_all()
bind = "0.0.0.0:8000"
workers = 1 # fine for dev, you probably want to increase this number in production
worker_class = "gunicorn.workers.ggevent.GeventWorker"
```
You can start the server with:
```
$ gunicorn_django -c config/gunicorn
```
For more info on Django on gunicorn see Django's docs on [How to use Django with Gunicorn](https://docs.djangoproject.com/en/1.4/howto/deployment/wsgi/gunicorn/).
## Server-Sent Events
The browser will issue a GET request to the url `/sse/` (this path is completely arbitrary). The server will respond with a stream of data, without ever closing the connection.
The easiest way to implement SSEs is to use the `django-sse` package, available on PyPi.
```
$ pip install sse django-sse
```
If you want to publish via redis, `django-sse` requires you to specify how to connect:
`settings.py`:
```
REDIS_SSEQUEUE_CONNECTION_SETTINGS = {
    'location': 'localhost:6379',
    'db': 0,
}
```
`django_sse` provides a ready-to-use view that uses redis as message broker.
`myapp/views.py`:
```
from django.views.generic import TemplateView
from django_sse.redisqueue import RedisQueueView
class HomePage(TemplateView):
    template_name = 'index.html'
class SSE(RedisQueueView):
    pass
```
Hook the views up in your `urls.py`:
```
from django.conf.urls import patterns, include, url
from myapp import views
urlpatterns = patterns('',
    url(r'^sse/$', views.SSE.as_view(), name='sse'),  # this URL is arbitrary.
    url(r'^$', views.HomePage.as_view(), name='homepage'),
)
```
### IE Polyfill
Not every browser supports SSEs (most notably, internet Explorer).
For unsupported browser, we can include a [JavaScript](http://lib.csdn.net/base/javascript) polyfill in our page. There are many polyfills available out there, but I've choose to use [`eventsource.js`](https://github.com/Yaffle/EventSource)because it's close to the original API and it looks actively maintained.
After including the polyfill in our HTML we can set up our callback functions on DOMReady. Here I've also uses [jQuery](http://lib.csdn.net/base/jquery) for simplicity.
```
<!doctype html>
<html>
<head>
  <meta charset="utf-8">
  <title>My App</title>
</head>
<body>
  <script src="//ajax.googleapis.com/ajax/libs/jquery/1.7.1/jquery.min.js"></script>
  <script>window.jQuery || document.write('<script src="{{ STATIC_URL }}js/libs/jquery-1.7.1.min.js"><\/script>')</script>
  <script src="{{ STATIC_URL }}js/libs/eventsource.js"></script>
<script>
$().ready(function() {
  var source = new EventSource('/sse/'); // of course this must match the endpoint in your urlconf
  function log() {
    console.log(arguments);
  }
  source.onopen = function() {
    console.log(arguments);
  };
  source.onerror = function () {
    console.log(arguments);
  };
  source.addEventListener('connections', log, false);
  source.addEventListener('requests', log, false);
  source.addEventListener('myevent', function(e) {
    data = JSON.parse(e.data);
    // .. do something..
  }, false);
  source.addEventListener('uptime', log, false);
  source.onmessage = function() {
    console.log(arguments);
  };
});
</script>
</body>
</html>
```
### Publishing events
`django_sse` provides a convenience method to publish messages to`RedisQueueView` subclasses:
```
imoprt json
from django_sse.redisqueue import send_event
send_event('myevent', json.dumps(data))
```
Note that `send_event` allows only text values to be published. Taht's why we are serializing the data to json, and we unserialize it in the event handler with`JSON.parse`.
By default, `django_sse` publishes and listens to the redis channel `see`. If we want to separate messages per user, we can define the `get_redis_channel`method on the view:
```
class SSE(RedisQueueView):
    def get_redis_channel(self):
        return "sse_%s" % self.request.user.username
```
When we want to publish some event to a specific user, all we have to do is to specify the channel when calling `send_event`:
```
send_event('myevent', json.dumps(data), 'sse_%s' % user.username)
```
## WebSocket
Now, suppose you want to notify user A when user B does some kind action.
You could still use SSEs, but every time the scenario happens, you'll end up with three connections: two long-running ones opened by A and B listening for SSEs, and a short one fired by B when POSTing his action.
Since you're already having long-running connections because you need to push events, you may just switch to WebSockets and save that POST.
Since WebSocket is not yet supported by Explorer, we'll have to use an abstraction layer, like `socket.io` or `socks.[js](http://lib.csdn.net/base/javascript)`, that provide alternative transports of messages.
I choose to use `socket.io` mainly because of the `gevent-socketio` library, which integrates pretty easily with Django.
### Using the socketio worker
In order to run `gevent-socketio`, we have to run gunicorn with a specialized worker class.
The `GeventSocketIOWorker` will take care of implementing the socket.io handshake and the new WebSocket Protocol (`ws://`)
In order to use `GeventSocketIOWorker`, I modified the `worker_class`parameter in the config file for unicorn:
```
#!python
from os import environ
from gevent import monkey
monkey.patch_all()
bind = "0.0.0.0:8000"
workers = 1
worker_class = "socketio.sgunicorn.GeventSocketIOWorker" # Note that we are now using gevent-socketio's worker
```
Note that using the `socketio.sgunicorn.GeventSocketIOWorker` is compatible with SSEs, so you could use this worker if you want both protocols running.
`gevent-socketio` allows you to define different Socket.io `namespaces`. This way you can implement different domain-specific logics. For example, you could implement a namespace for users' status (online, away, etc.) and a different chat messages.
Additionally, `gevent-socketio` ships with a couple of namespaces mixing for common situations, like for implementing separate chat rooms.
Let's create a namespace. Our namespace will provide separate chat-rooms, and will process events from our redis queue.
I had to override the `emit_to_room` method because I had the messages delivered more than once when I had more clients connected than the available workers.
`myapp/sockets.py`:
```
from socketio.namespace import BaseNamespace
from socketio.sdjango import namespace
from socketio.mixins import RoomsMixin
from myapp.utils import redis_connection
import json
@namespace('')
class MyNamespace(BaseNamespace, RoomsMixin):
    def listener(self, room):
        # ``redis_connection()`` is an utility function that returns a redis connection from a pool
        r = redis_connection().pubsub()
        r.subscribe('socketio_%s' % room)
        for m in r.listen():
            if m['type'] == 'message':
                data = json.loads(m['data'])
                self.process_event(data)
    def on_subscribe(self, *args):
        for channel in args:
            self.join(channel)
    def join(self, room):
        super(MyNamespace, self).join(room)
        self.spawn(self.listener, room)
        self.emit('joined', room)
    def on_myevent(self, room, *args):
        self.emit_to_room(room, 'myevent', *args)
    def emit_to_room(self, room, event, *args):
        """
        This is almost the same as ``.emit_to_room()`` on the parent class,
        but it sends events only over the current socket.
        This is to avoid a problem when there are more client than workers, and
        a single message can get delivered multiple times.
        """
        pkt = dict(type="event",
                   name=event,
                   args=args,
                   endpoint=self.ns_name)
        room_name = self._get_room_name(room)
        if 'rooms' not in self.socket.session:
            return
        if room_name in self.socket.session['rooms']:
            self.socket.send_packet(pkt)
```
Note that the `join` method we spawn a listener (and thus, a new redis connection) for every room we join. That's the way it's implemented in the chat example at the `gevent-socketio` repository.
If you're worried about having to spawn one process per client per channel, I've included an alternative subclass in the repo that restarts the listener when joining channel. The catch is that there will be a few milliseconds during which the user won't receive message.
I'm also using a pool to recycle Redis connection, The `redis_connection`method creates a new redis object for our already existing connection pool:
`utils.py`:
```
from django.conf import settings
from redis import Redis
from redis import ConnectionPool as RedisConnectionPool
from redis.connection import Connection
WEBSOCKET_REDIS_BROKER_DEFAULT = {
    'HOST': 'localhost',
    'PORT': 6379,
    'DB': 0
}
CONNECTION_KWARGS = getattr(settings, 'WEBSOCKET_REDIS_BROKER', {})
class ConnectionPoolManager(object):
    """
    A singleton that contains and retrieves redis ``ConnectionPool``s according to the connection settings.
    """
    pools = {}
    @classmethod
    def key_for_kwargs(cls, kwargs):
        return ":".join([str(v) for v in kwargs.values()])
    @classmethod
    def connection_pool(cls, **kwargs):
        pool_key = cls.key_for_kwargs(kwargs)
        if pool_key in cls.pools:
            return cls.pools[pool_key]
        params = {
            'connection_class': Connection,
            'db': kwargs.get('DB', 0),
            'password': kwargs.get('PASSWORD', None),
            'host': kwargs.get('HOST', 'localhost'),
            'port': int(kwargs.get('PORT', 6379))
        }
        cls.pools[pool_key] = RedisConnectionPool(**params)
        return cls.pools[pool_key]
def redis_connection():
    """
    Returns a redis connection from one of our pools.
    """
    pool = ConnectionPoolManager.connection_pool(**CONNECTION_KWARGS)
    return Redis(connection_pool=pool)
```
For serving our namespaces, `gevent-socketio` gives us an autodiscovery feature similar to Django's admin:
`urls.py`:
```
from django.conf.urls import patterns, include, url
from myapp import views
import socketio.sdjango
socketio.sdjango.autodiscover()
urlpatterns = patterns('',
    url(r'^sse/$', views.SSE.as_view(), name='sse'),  # this URL is arbitrary.
    # socket.io uses the well-known URL `/socket.io/` for its protocol
    url(r"^socket\.io", include(socketio.sdjango.urls)),
    url(r'^$', views.HomePage.as_view(), name='homepage'),
)
```
On the client side, we need to include the `socket.io`[javascript](http://lib.csdn.net/base/javascript) client, (available at https://github.com/LearnBoost/socket.io-client/).
By default, the client will try to use `flashsockets` under Internet Explorer (because Explorer doesn't support WebSocket).
The problem with `flashsocket` is that the Flash shipped with `socketio-client` makes a request for a policy file, and you'd need to set up a Flash policy server. So I decided to disable this transport and have IE use xhr-polling.
```
socket = io.connect('', {  // first argument is the namespace
  transports: ['websocket', 'xhr-multipart', 'xhr-polling', 'jsonp-polling']  // note ``flashsockets`` is missing
});
socket.on("myevent", function(e) {
  console.log("<myevent> event", arguments);
});
socket.on("message", function(e) {
  console.log("Message", e);
});
socket.on("joined", function(e) {
  console.log("joined", arguments);
});
socket.on("connect", function(e) {
  console.log("Connected", arguments);
  socket.emit('subscribe', 'default_room');
});
socket.on("disconnect", function(e) {
  console.log("Disconnected", arguments);
});
```
### Publishing an event
All we have to do in order to emit an event to our client is pushing a message to the right redis channel.
`utils.py`:
```
# previous code here ...
import json
def emit_to_channel(channel, event, *data):
    r = redis_connection()
    args = [channel] + list(data)
    r.publish('socketio_%s' % channel, json.dumps({'name': event, 'args': args}))
```
## Links & Acknowledgements
I would like to thank Jeff Triplett for the initial feedback on this article, Cody Soyland for his initial article about socket.io and gevent, Andrei Antoukh for accepting my patches for `django_sse`, and Jeffrey Gelens for accepting my patch for `gevent-websocket`.
If you want to read more, here's some links:
- [http://codysoyland.com/2011/feb/6/evented-django-part-one-socketio-and-gevent/](http://codysoyland.com/2011/feb/6/evented-django-part-one-socketio-and-gevent/)
- [http://eflorenzano.com/blog/2011/02/16/technology-behind-convore/](http://eflorenzano.com/blog/2011/02/16/technology-behind-convore/)
- [http://www.gevent.org/](http://www.gevent.org/)
- [http://gunicorn.org/](http://gunicorn.org/)
- [https://bitbucket.org/Jeffrey/gevent-websocket/src](https://bitbucket.org/Jeffrey/gevent-websocket/src)
- [http://gevent-socketio.readthedocs.org/en/latest/index.html](http://gevent-socketio.readthedocs.org/en/latest/index.html)
- [http://www.w3.org/TR/eventsource/](http://www.w3.org/TR/eventsource/)

