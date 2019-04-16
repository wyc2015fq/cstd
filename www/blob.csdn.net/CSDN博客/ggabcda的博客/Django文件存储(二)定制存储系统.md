# Django文件存储(二)定制存储系统 - ggabcda的博客 - CSDN博客





2017年09月04日 19:17:45[ggabcda](https://me.csdn.net/ggabcda)阅读数：193








要自己写一个存储系统，可以依照以下步骤：

1.写一个继承自django.core.files.storage.Storage`的子类。`


```python
from django.core.files.storage import Storage

class MyStorage(Storage):
    ...
```


2.Django必须可以在无任何参数的情况下实例化MyStorage，所以任何环境设置必须来自django.conf.settings。


```python
from django.conf import settings
from django.core.files.storage import Storage

class MyStorage(Storage):
    def __init__(self, option=None):
        if not option:
            option = settings.CUSTOM_STORAGE_OPTIONS
        ...
```


3.根据Storage的open和save方法源码:


```python
def open(self, name, mode='rb'):
        """
        Retrieves the specified file from storage.
        """
        return self._open(name, mode)


    def save(self, name, content, max_length=None):
        """
        Saves new content to the file specified by name. The content should be
        a proper File object or any python file-like object, ready to be read
        from the beginning.
        """
        # Get the proper name for the file, as it will actually be saved.
        if name is None:
            name = content.name

        if not hasattr(content, 'chunks'):
            content = File(content, name)

        name = self.get_available_name(name, max_length=max_length)
        return self._save(name, content)
```


MyStorage需要实现_open和_save方法。

如果写的是个本地存储系统，还要重写path方法。

4.使用django.utils.deconstruct.deconstructible装饰器，以便在migration可以序列化。

还有，Storage.delete()、Storage.exists()、Storage.listdir()、Storage.size()、Storage.url()方法都会报NotImplementedError，也需要重写。

### Django Qiniu Storage

七牛云有自己的django storage系统，可以看下是怎么运作的，地址https://github.com/glasslion/django-qiniu-storage。

先在环境变量或者settings中配置QINIU_ACCESS_KEY、QINIU_SECRET_KEY、QINIU_BUCKET_NAME、QINIU_BUCKET_DOMAIN、QINIU_SECURE_URL。

使用七牛云托管用户上传的文件，在 settings.py 里设置DEFAULT_FILE_STORAGE：


```python
DEFAULT_FILE_STORAGE = 'qiniustorage.backends.QiniuStorage'
```


使用七牛托管动态生成的文件以及站点自身的静态文件，设置：


```python
STATICFILES_STORAGE  = 'qiniustorage.backends.QiniuStaticStorage'
```


运行python manage.py collectstatic，静态文件就会被统一上传到七牛。

QiniuStorage代码如下：


```python
@deconstructible
class QiniuStorage(Storage):
    """
    Qiniu Storage Service
    """
    location = ""

    def __init__(
            self,
            access_key=QINIU_ACCESS_KEY,
            secret_key=QINIU_SECRET_KEY,
            bucket_name=QINIU_BUCKET_NAME,
            bucket_domain=QINIU_BUCKET_DOMAIN,
            secure_url=QINIU_SECURE_URL):

        self.auth = Auth(access_key, secret_key)
        self.bucket_name = bucket_name
        self.bucket_domain = bucket_domain
        self.bucket_manager = BucketManager(self.auth)
        self.secure_url = secure_url

    def _clean_name(self, name):
        """
        Cleans the name so that Windows style paths work
        """
        # Normalize Windows style paths
        clean_name = posixpath.normpath(name).replace('\\', '/')

        # os.path.normpath() can strip trailing slashes so we implement
        # a workaround here.
        if name.endswith('/') and not clean_name.endswith('/'):
            # Add a trailing slash as it was stripped.
            return clean_name + '/'
        else:
            return clean_name

    def _normalize_name(self, name):
        """
        Normalizes the name so that paths like /path/to/ignored/../foo.txt
        work. We check to make sure that the path pointed to is not outside
        the directory specified by the LOCATION setting.
        """

        base_path = force_text(self.location)
        base_path = base_path.rstrip('/')

        final_path = urljoin(base_path.rstrip('/') + "/", name)

        base_path_len = len(base_path)
        if (not final_path.startswith(base_path) or
                final_path[base_path_len:base_path_len + 1] not in ('', '/')):
            raise SuspiciousOperation("Attempted access to '%s' denied." %
                                      name)
        return final_path.lstrip('/')

    def _open(self, name, mode='rb'):
        return QiniuFile(name, self, mode)

    def _save(self, name, content):
        cleaned_name = self._clean_name(name)
        name = self._normalize_name(cleaned_name)

        if hasattr(content, 'chunks'):
            content_str = b''.join(chunk for chunk in content.chunks())
        else:
            content_str = content.read()

        self._put_file(name, content_str)
        return cleaned_name

    def _put_file(self, name, content):
        token = self.auth.upload_token(self.bucket_name)
        ret, info = put_data(token, name, content)
        if ret is None or ret['key'] != name:
            raise QiniuError(info)

    def _read(self, name):
        return requests.get(self.url(name)).content

    def delete(self, name):
        name = self._normalize_name(self._clean_name(name))
        if six.PY2:
            name = name.encode('utf-8')
        ret, info = self.bucket_manager.delete(self.bucket_name, name)

        if ret is None or info.status_code == 612:
            raise QiniuError(info)

    def _file_stat(self, name, silent=False):
        name = self._normalize_name(self._clean_name(name))
        if six.PY2:
            name = name.encode('utf-8')
        ret, info = self.bucket_manager.stat(self.bucket_name, name)
        if ret is None and not silent:
            raise QiniuError(info)
        return ret

    def exists(self, name):
        stats = self._file_stat(name, silent=True)
        return True if stats else False

    def size(self, name):
        stats = self._file_stat(name)
        return stats['fsize']

    def modified_time(self, name):
        stats = self._file_stat(name)
        time_stamp = float(stats['putTime']) / 10000000
        return datetime.datetime.fromtimestamp(time_stamp)

    def listdir(self, name):
        name = self._normalize_name(self._clean_name(name))
        if name and not name.endswith('/'):
            name += '/'

        dirlist = bucket_lister(self.bucket_manager, self.bucket_name,
                                prefix=name)
        files = []
        dirs = set()
        base_parts = name.split("/")[:-1]
        for item in dirlist:
            parts = item['key'].split("/")
            parts = parts[len(base_parts):]
            if len(parts) == 1:
                # File
                files.append(parts[0])
            elif len(parts) > 1:
                # Directory
                dirs.add(parts[0])
        return list(dirs), files

    def url(self, name):
        name = self._normalize_name(self._clean_name(name))
        name = filepath_to_uri(name)
        protocol = u'https://' if self.secure_url else u'http://'
        return urljoin(protocol + self.bucket_domain, name)
```


配置是从环境变量或者settings.py中获得的：


```python
def get_qiniu_config(name, default=None):
    """
    Get configuration variable from environment variable
    or django setting.py
    """
    config = os.environ.get(name, getattr(settings, name, default))
    if config is not None:
        if isinstance(config, six.string_types):
            return config.strip()
        else:
            return config
    else:
        raise ImproperlyConfigured(
            "Can't find config for '%s' either in environment"
            "variable or in setting.py" % name)


QINIU_ACCESS_KEY = get_qiniu_config('QINIU_ACCESS_KEY')
QINIU_SECRET_KEY = get_qiniu_config('QINIU_SECRET_KEY')
QINIU_BUCKET_NAME = get_qiniu_config('QINIU_BUCKET_NAME')
QINIU_BUCKET_DOMAIN = get_qiniu_config('QINIU_BUCKET_DOMAIN', '').rstrip('/')
QINIU_SECURE_URL = get_qiniu_config('QINIU_SECURE_URL', 'False')
```


　重写了_open和_save方法：


```python
def _open(self, name, mode='rb'):
        return QiniuFile(name, self, mode)

    def _save(self, name, content):
        cleaned_name = self._clean_name(name)
        name = self._normalize_name(cleaned_name)

        if hasattr(content, 'chunks'):
            content_str = b''.join(chunk for chunk in content.chunks())
        else:
            content_str = content.read()

        self._put_file(name, content_str)
        return cleaned_name
```


使用的put_data方法上传文件，相关代码如下：


```python
def put_data(
        up_token, key, data, params=None, mime_type='application/octet-stream', check_crc=False, progress_handler=None,
        fname=None):
    """上传二进制流到七牛

    Args:
        up_token:         上传凭证
        key:              上传文件名
        data:             上传二进制流
        params:           自定义变量，规格参考 http://developer.qiniu.com/docs/v6/api/overview/up/response/vars.html#xvar
        mime_type:        上传数据的mimeType
        check_crc:        是否校验crc32
        progress_handler: 上传进度

    Returns:
        一个dict变量，类似 {"hash": "<Hash string>", "key": "<Key string>"}
        一个ResponseInfo对象
    """
    crc = crc32(data) if check_crc else None
    return _form_put(up_token, key, data, params, mime_type, crc, progress_handler, fname)

def _form_put(up_token, key, data, params, mime_type, crc, progress_handler=None, file_name=None):
    fields = {}
    if params:
        for k, v in params.items():
            fields[k] = str(v)
    if crc:
        fields['crc32'] = crc
    if key is not None:
        fields['key'] = key

    fields['token'] = up_token
    url = config.get_default('default_zone').get_up_host_by_token(up_token) + '/'
    # name = key if key else file_name

    fname = file_name
    if not fname or not fname.strip():
        fname = 'file_name'

    r, info = http._post_file(url, data=fields, files={'file': (fname, data, mime_type)})
    if r is None and info.need_retry():
        if info.connect_failed:
            url = config.get_default('default_zone').get_up_host_backup_by_token(up_token) + '/'
        if hasattr(data, 'read') is False:
            pass
        elif hasattr(data, 'seek') and (not hasattr(data, 'seekable') or data.seekable()):
            data.seek(0)
        else:
            return r, info
        r, info = http._post_file(url, data=fields, files={'file': (fname, data, mime_type)})

    return r, info


def _post_file(url, data, files):
    return _post(url, data, files, None)

def _post(url, data, files, auth, headers=None):
    if _session is None:
        _init()
    try:
        post_headers = _headers.copy()
        if headers is not None:
            for k, v in headers.items():
                post_headers.update({k: v})
        r = _session.post(
            url, data=data, files=files, auth=auth, headers=post_headers,
            timeout=config.get_default('connection_timeout'))
    except Exception as e:
        return None, ResponseInfo(None, e)
    return __return_wrapper(r)


def _init():
    session = requests.Session()
    adapter = requests.adapters.HTTPAdapter(
        pool_connections=config.get_default('connection_pool'), pool_maxsize=config.get_default('connection_pool'),
        max_retries=config.get_default('connection_retries'))
    session.mount('http://', adapter)
    global _session
    _session = session
```


最终使用的是requests库上传文件的，统一适配了链接池个数、链接重试次数。



