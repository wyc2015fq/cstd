# 关于php中的spl_autoload_register - weixin_33985507的博客 - CSDN博客
2015年12月20日 23:29:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2

### 一、自动加载定义
很多开发者写面向对象的应用程序时对每个类的定义建立一个 PHP 源文件。一个很大的烦恼是不得不在每个脚本开头写一个长长的包含文件列表（每个类一个文件）。
在 PHP 5 中，不再需要这样了。可以定义一个 [__autoload()](http://php.net/manual/zh/function.autoload.php) 函数，它会在试图使用尚未被定义的类时自动调用。通过调用此函数，脚本引擎在 PHP 出错失败前有了最后一个机会加载所需的类
__autoload 是一个魔术方法，尝试加载未定义的类，spl_autoload_register - 注册给定的函数作为 __autoload 的实现
> 
Tip
[spl_autoload_register()](http://php.net/manual/zh/function.spl-autoload-register.php) 提供了一种更加灵活的方式来实现类的自动加载。因此，不再建议使用 [__autoload()](http://php.net/manual/zh/function.autoload.php)函数，在以后的版本中它可能被弃用。
> 
Note:
在 5.3.0 版之前，__autoload 函数抛出的异常不能被 [catch](http://php.net/manual/zh/language.exceptions.php) 语句块捕获并会导致一个致命错误。从 5.3.0+ 之后，__autoload 函数抛出的异常可以被 [catch](http://php.net/manual/zh/language.exceptions.php) 语句块捕获，但需要遵循一个条件。如果抛出的是一个自定义异常，那么必须存在相应的自定义异常类。__autoload 函数可以递归的自动加载自定义异常类。
> 
Note:
自动加载不可用于 PHP 的 CLI [交互模式](http://php.net/manual/zh/features.commandline.php)。
> 
Note:
如果类名比如被用于 [call_user_func()](http://php.net/manual/zh/function.call-user-func.php)，则它可能包含一些危险的字符，比如 *../*。 建议您在这样的函数中不要使用用户的输入，起码需要在 [__autoload()](http://php.net/manual/zh/function.autoload.php) 时验证下输入

### 二、spl_autoload_register
#### 目录
```php
classes --+
          + mail.class.php
          + norman.class.php
          + db.class.php
```
#### 1、Single Autoloads
<?php
    /*** nullify any existing autoloads ***/
    spl_autoload_register(null, false);
    /*** specify extensions that may be loaded ***/
    spl_autoload_extensions('.php, .class.php');
    /*** class Loader ***/
    function classLoader($class)
    {
        $filename = strtolower($class) . '.class.php';
        $file ='classes/' . $filename;
        if (!file_exists($file))
        {
            return false;
        }
        include $file;
    }
    /*** register the loader functions ***/
    spl_autoload_register('classLoader');
    /*** a new instance if norman ***/
    $norman = new norman;
    /*** make norman do something ***/
    $norman->do_something();
?>
#### 2、Mulitple Autoloads
```php
<?php
    /*** nullify any existing autoloads ***/
    spl_autoload_register(null, false);
    /*** specify extensions that may be loaded ***/
    spl_autoload_extensions('.php, .class.php, .lib.php');
    /*** class Loader ***/
    function classLoader($class)
    {
        $filename = strtolower($class) . '.class.php';
        $file ='classes/' . $filename;
        if (!file_exists($file))
        {
            return false;
        }
        include $file;
    }
    function libLoader($class)
    {
        $filename = strtolower($class) . '.lib.php';
        $file ='libs/' . $filename;
        if (!file_exists($file))
        {
            return false;
        }
        include $file;
    }
    /*** register the loader functions ***/
    spl_autoload_register('classLoader');
    spl_autoload_register('libLoader');
    /*** a new instance of norman ***/
    $norman = new norman;
    /*** make norman do some thing ***/
    $norman->do_something();
```
#### 3、Interfaces
接口文件
```php
<?php
    /*
     * icontroller.class.php
     * interface to ensure all classes have an index method
     *
     */
    interface iController
    {
        public function index();
    }
?>
```
autoload文件
```php
<?php
    /*** nullify any existing autoloads ***/
    spl_autoload_register(null, false);
    /*** specify extensions that may be loaded ***/
    spl_autoload_extensions('.php, .class.php');
    /*** class Loader ***/
    function classLoader($class)
    {
        $filename = strtolower($class) . '.class.php';
        $file ='classes/' . $filename;
        if (!file_exists($file))
        {
            return false;
        }
        include $file;
    }
    /*** register the loader functions ***/
    spl_autoload_register('classLoader');
    class blog implements iController
    {
        public function index()
        {
            echo 'hello from the index';
        }
    }
    /*** a new blog instance ***/
    $blog = new blog;
    /*** run the index method ***/
    $blog->index();
?>
```
#### 4、一个标准的示例
```php
spl_autoload_register( 'autoload' );
 
  
  /**
   * autoload
   *
   * @author testdd
   * @param  string $class
   * @param  string $dir
   * @return bool
   */
  function autoload( $class, $dir = null ) {
 
    if ( is_null( $dir ) )
      $dir = '/path/to/project';
 
    foreach ( scandir( $dir ) as $file ) {
 
      // directory?
      if ( is_dir( $dir.$file ) && substr( $file, 0, 1 ) !== '.' )
        autoload( $class, $dir.$file.'/' );
 
      // php file?
      if ( substr( $file, 0, 2 ) !== '._' && preg_match( "/.php$/i" , $file ) ) {
 
        // filename matches class?
        if ( str_replace( '.php', '', $file ) == $class || str_replace( '.class.php', '', $file ) == $class ) {
 
            include $dir . $file;
        }
      }
    }
  }
```
#### 5、框架中的写法
```php
<?php
/**
 * Autoloader
 * @author Jianxiang Qin <TalkativeDoggy@gmail.com>
 * @license http://opensource.org/licenses/BSD-3-Clause New BSD License
 * @version svn:$Id$
 * @package Lotusphp\Autoloader
 */
/**
 * 自动加载类和函数
 * 
 * 按需加载类，每次只加载用到的类。
 * 
 *     函数库文件不是按需加载！若支持加载函数，则所有定义函数的文件都会加载。
 * 
 * 代码中用到一个类或者函数的时候，不需要使用include/require来包含类库文件或者函数库文件。
 * 
 * 基于Autoloader组件的代码中将不用使用include/require。
 * 
 * Autoloader缓存的是绝对路径，能让Opcode Cache有效缓存文件。
 * 
 *     Autoloader要求类的名字唯一，不在意类文件的路径和文件名。目前不支持命名空间(PHP5.3)
 * 
 * 传统的include/require通常存在以下问题。
 * <ul>
 * <li>目录名和文件名变化引起程序代码变化。</li>
 * <li>Windows和Linux对文件路径的大小写和目录分隔符号的处理不同导致代码在不同平台迁移时出现问题。</li>
 * <li>include_path相对路径的性能低（显著地低）。</li>
 * <li>为了保证不重复包含，使用include_once和require_once导致效率低（不是显著的低）。</li>
 * </ul>
 * @author Jianxiang Qin <TalkativeDoggy@gmail.com> Yi Zhao <zhao5908@gmail.com>
 * @category runtime
 * @package Lotusphp\Autoloader
 * @todo 所有class-file mapping当成一个数据写入storeHandle
 */
class LtAutoloader
{
        /** 
         * @var bool true|false 是否自动加载定义了函数的文件。
         * false 只自动加载定义了class或者interface的文件。
         * true （默认） 自动加载定义了函数的文件。
         */
        public $isLoadFunction = true;
        
        /**
         * @var array 要扫描的文件类型
         * 若该属性设置为array("php","inc","php3")，
         * 则扩展名为"php","inc","php3"的文件会被扫描，
         * 其它扩展名的文件会被忽略
         */
        public $allowFileExtension = array('php', 'inc');
        
        /**
         * @var array 不扫描的目录
         * 若该属性设置为array(".svn", ".setting")，
         * 则所有名为".setting"的目录也会被忽略
         */
        public $skipDirNames = array('.svn', '.git');
        /** @var LtStoreFile 存储句柄默认使用 @link LtStoreFile */
        public $storeHandle;
        
        /** @var array 指定需要自动加载的目录列表 */
        public $autoloadPath;
        
        /** @var bool
     * true 开发模式下  每次都会扫描目录列表
     * false 生产环境下 只扫描一次
     */
        public $devMode = true;
        
        /** @var array 函数名 -> 文件路径  映射 */
        private $functionFileMapping = array();
    /** @var array 类名 -> 文件路径  映射 */
    private $classFileMapping = array();
    /** @var array 定义了函数的文件列表 */
    private $functionFiles = array();
    /** @var LtStoreFile 持久存储句柄,存储文件的get_token_all分析结果/filesize/filehash @link LtStoreFile */
    private $persistentStoreHandle;
    /** @var int store name space id */
    private $storeNameSpaceId;
    /** @var int number of parse error */
    private $parseErrorAmount = 0;
    /** @var int number of library files successfully parsed */
    private $libFileAmount = 0;
        /**
         * 递归扫描指定的目录列表，根据@see LtAutoloader::$isLoadFunction是否加载全部的函数定义文件。
         * 注册自动加载函数，按需加载类文件。
         * @return void
         */
        public function init()
        {
        $this->storeNameSpaceId = sprintf("%u", crc32(serialize($this->autoloadPath)));
        if (true != $this->devMode)
        {
            if ($this->storeHandle instanceof LtStore)
            {
                $this->storeHandle->prefix = 'Lt-Autoloader-' . $this->storeNameSpaceId;
            }
            else
            {
                if (null == $this->storeHandle)
                {
                    $this->storeHandle = new LtStoreFile;
                    $this->storeHandle->prefix = 'Lt-Autoloader-' . $this->storeNameSpaceId;
                    $this->storeHandle->useSerialize = true;
                    $this->storeHandle->init();
                }
                else
                {
                    trigger_error("You passed a value to autoloader::storeHandle, but it is NOT an instance of LtStore");
                }
            }
        }
        else
        {
            $this->storeHandle = new LtStoreMemory;
        }
                // Whether scanning directory
                if ($storedMap = $this->storeHandle->get("map"))
        {
            $this->classFileMapping = $storedMap["classes"];
            $this->functionFiles = $storedMap["functions"];
        }
        else
                {
            $this->setPersistentStoreHandle();
                        $autoloadPath = $this->preparePath($this->autoloadPath);
                        foreach($autoloadPath as $path)
                        {
                                if (is_file($path))
                                {
                                        $this->addFileMap($path);
                                }
                        }
                        $this->scanDirs($autoloadPath);
                        unset($autoloadPath);
                }
                // Whether loading function files
                $this->loadFunctionFiles();
                spl_autoload_register(array($this, "loadClass"));
        }
    protected function setPersistentStoreHandle()
    {
        $this->persistentStoreHandle = new LtStoreFile;
        $this->persistentStoreHandle->prefix = 'Lt-parsed-token-' . $this->storeNameSpaceId;
        $this->persistentStoreHandle->useSerialize = true;
    }
        /**
         * Autoloader扫描项目，若某个php文件中定义了函数，则此文件的绝对路径被缓存，
         * 每次执行LtAutoloader->init()方法时，自动include所有定义了函数的php文件。
     * 因为PHP的Autoload机制是针对Class的.function文件没有办法按需加载
         * @return void
         */
        protected function loadFunctionFiles()
        {
                if ($this->isLoadFunction && count($this->functionFiles))
                {
                        foreach ($this->functionFiles as $functionFile)
                        {
                                include_once($functionFile);
                        }
                }
        }
        /**
         * 被注册的自动加载函数
         * @param string $className
         * @return void 
         */
        protected function loadClass($className)
        {
                if ($filePath = $this->getFilePathByClassName($className))
                {
                        include($filePath);
                }
        }
        /**
         * 将目录分隔符号统一成linux目录分隔符号/
         * @param string $path
         * @return boolean
         */
        protected function convertPath($path)
        {
                $path = str_replace("\\", "/", $path);
                if (!is_readable($path))
                {
                        trigger_error("Directory is not exists/readable: {$path}");
                        return false;
                }
                $path = rtrim(realpath($path), '\\/');
                if (preg_match("/\s/i", $path))
                {
                        trigger_error("Directory contains space/tab/newline is not supported: {$path}");
                        return false;
                }
                return $path;
        }
        /**
         * The string or an Multidimensional array into a one-dimensional array
         * 将字符串和多维数组转换成一维数组
         * @param mixed $paths
         * @return array one-dimensional array
         */
        protected function preparePath($paths)
        {
                $oneDPathArray = array();
                if (!is_array($paths))
                {
                        $paths = array($paths);
                }
                $i = 0;
                while (isset($paths[$i]))
                {
                        if (!is_array($paths[$i]) && $path = $this->convertPath($paths[$i]))
                        {
                                $oneDPathArray[] = $path;
                        }
                        else
                        {
                                foreach($paths[$i] as $v)
                                {
                                        $paths[] = $v;
                                }
                        }
                        $i ++;
                }
        unset($paths);
                return $oneDPathArray;
        }
        /**
         * Using iterative algorithm scanning subdirectories
         * save autoloader filemap
         * 递归扫描目录包含子目录，保存自动加载的文件地图。
         * @param array $dirs one-dimensional
         * @return void
     * @todo in_array换成array_key_exists以提升性能
         */
        protected function scanDirs($dirs)
        {
                $i = 0;
                while (isset($dirs[$i]))
                {
                        $dir = $dirs[$i];
                        $files = scandir($dir);
                        foreach ($files as $file)
                        {
                $currentFile = $dir . DIRECTORY_SEPARATOR . $file;
                if (is_file($currentFile))
                {
                    $this->addFileMap($currentFile);
                }
                else if (is_dir($currentFile))
                {
                    if (in_array($file, array(".", "..")) || in_array($file, $this->skipDirNames))
                    {
                        continue;
                    }
                    else
                    {
                        // if $currentFile is a directory, pass through the next loop.
                        $dirs[] = $currentFile;
                    }
                }
                else
                {
                    trigger_error("$currentFile is not a file or a directory.");
                }
                        } //end foreach
                        $i ++;
                } //end while
        if(0 == $this->parseErrorAmount)
        {
            $this->functionFiles = array_unique(array_values($this->functionFileMapping));
            $map = array("classes" => $this->classFileMapping, "functions" => $this->functionFiles);
            if ($this->storeHandle->get("map"))
            {
                $this->storeHandle->update("map", $map);
            }
            else
            {
                $this->storeHandle->add("map", $map);
            }
        }
        else
        {
            trigger_error($this->parseErrorAmount . " error(s) occoured when scanning and parsing your lib files");
        }
        }
    /**
     * 分析出字符串中的类，接口，函数。 
     * @param string $src
     * @return array
     * @todo 若当前文件包含了直接执行的php语句,或者html,输出警告
     * @todo 若当前文件有语法错误,抛出异常
     */
        protected function parseLibNames($src)
        {
                $libNames = array();
                $tokens = token_get_all($src);
                $level = 0;
                $found = false;
                $name = '';
                foreach ($tokens as $token)
                {
                        if (is_string($token))
                        {
                                if ('{' == $token)
                                {
                                        $level ++;
                                }
                                else if ('}' == $token)
                                {
                                        $level --;
                                }
                        }
                        else
                        {
                                list($id, $text) = $token;
                                if (T_CURLY_OPEN == $id || T_DOLLAR_OPEN_CURLY_BRACES == $id)
                                {
                                        $level ++;
                                }
                                if (0 < $level)
                                {
                                        continue;
                                }
                                switch ($id)
                                {
                                        case T_STRING:
                                                if ($found)
                                                {
                                                        $libNames[strtolower($name)][] = $text;
                                                        $found = false;
                                                }
                                                break;
                                        case T_CLASS:
                                        case T_INTERFACE:
                                        case T_FUNCTION:
                                                $found = true;
                                                $name = $text;
                                                break;
                                }
                        }
                }
                return $libNames;
        }
        /**
         * 保存类名、接口名和对应的文件绝对路径。 
         * @param string $className
         * @param string $file
         * @return boolean
         */
        protected function addClass($className, $file)
        {
                $key = strtolower($className);
                if (isset($this->classFileMapping[$key]))
                {
            $existedClassFile = $this->classFileMapping[$key];
                        trigger_error("duplicate class [$className] found in:\n$existedClassFile\n$file\n, or please clear the cache");
                        return false;
                }
                else
                {
            $this->classFileMapping[$key] = $file;
                        return true;
                }
        }
        /**
         * 保存函数名和对应的文件绝对路径
         * @param string $functionName
         * @param string $file
         * @return boolean
         */
        protected function addFunction($functionName, $file)
        {
                $functionName = strtolower($functionName);
                if (isset($this->functionFileMapping[$functionName]))
                {
                        $existedFunctionFile = $this->functionFileMapping[$functionName];
                        trigger_error("duplicate function [$functionName] found in:\n$existedFunctionFile\n$file\n");
                        return false;
                }
                else
                {
                        $this->functionFileMapping[$functionName] = $file;
                        return true;
                }
        }
        /**
         * 将文件添加到自动加载的FileMap，
         * 添加之前会判断自从上次扫描后有没有修改，若没有修改则无需重复添加，
         * 若修改过，则分析文件内容，根据内容中包含的类、接口，函数添加到FileMap
         * @param string $filePath
         * @return boolean
         */
        protected function addFileMap($filePath)
        {
        if (!in_array(pathinfo($filePath, PATHINFO_EXTENSION), $this->allowFileExtension))
        {//init()会调用这个方法, 不要将这个判断移动到scanDir()中
            return false;
        }
        $fileSize = filesize($filePath);
        $fileHash = md5_file($filePath);
        $savedFileInfo = $this->persistentStoreHandle->get($filePath);
                if (!isset($savedFileInfo['file_size']) || $savedFileInfo['file_size'] != $fileSize || $savedFileInfo['file_hash'] != $fileHash)
                {
            if($libNames = $this->parseLibNames(trim(file_get_contents($filePath))))
            {
                $newFileInfo = array('file_size' => $fileSize, 'file_hash' => $fileHash, 'lib_names' => $libNames);
                if (isset($savedFileInfo['file_size']))
                {
                    $this->persistentStoreHandle->update($filePath, $newFileInfo);
                }
                else
                {
                    $this->persistentStoreHandle->add($filePath, $newFileInfo);
                }
            }
                        else
            {
                $this->parseErrorAmount ++;
            }
                }
        else
        {
            $libNames = $savedFileInfo['lib_names'];
        }
        foreach ($libNames as $libType => $libArray)
        {
            $method = "function" == $libType ? "addFunction" : "addClass";
            foreach ($libArray as $libName)
            {
                if (!$this->$method($libName, $filePath))
                {
                    $this->parseErrorAmount ++;
                }
            }
        }
                return true;
        }
    protected function getFilePathByClassName($className)
    {
        $key = strtolower($className);
        if (isset($this->classFileMapping[$key]))
        {
            return $this->classFileMapping[$key];
        }
        else
        {
            return false;
        }
    }
}
```
#### 6、set_include_path 方式
```php
set_include_path(implode(PATH_SEPARATOR, array(get_include_path(), './services', './printers')));
spl_autoload_register();
```
#### 7、PSR-4: Autoloader
http://www.php-fig.org/psr/psr-4/
参考文章
http://www.phpro.org/tutorials/SPL-Autoload.html
https://github.com/qinjx/adv_php_book/blob/master/class_autoload.md
http://php.net/manual/zh/language.oop5.autoload.php
