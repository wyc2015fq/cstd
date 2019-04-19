# CI模型的加载 - LC900730的博客 - CSDN博客
2017年10月27日 18:42:10[lc900730](https://me.csdn.net/LC900730)阅读数：512
### 加载模型
模型一般会在控制器中加载并且调用，可以如下加载模型：
```php
$this->load->model('model_name');
//如果模型位于子目录下，那么加载时要带上模型所在目录点相对路径，如application/model/blog/Queries.php中，那么：
$this->load->model('blog/model_name');
```
### 加载之后引用
```php
$this->load->model('model_name');
$this->model_name->method();//调用模型中的方法
```
### 模型对象赋值给不同名字的对象，使用第二个参数
```php
$this->load->model('model_name','foobar');
$this->foobar->method();//调用模型中的方法
```
### 模型的自动加载
如果一个模型需要在整个应用中使用，那么可以让ci在初始化自动加载，即在application/config/autoload.php中 
//如：
```php
$autoload['model'] = array('order_search_model');
```
### 连接数据库
当模型加载之后，它并不会自动连接数据库。 
使用标准数据库方法连接数据库可以设置第三个参数为true 
让模型在加载时候自动连接数据库 
$this->load->model(‘model’,”,TRUE);
