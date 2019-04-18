# centos 5.7 安装PHP扩展memcached memcache redis mongo - weixin_33985507的博客 - CSDN博客
2012年02月10日 16:04:31[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
    tar zxvf libevent-1.4.13-stable.tar.gz
    cd libevent-1.4.13-stable
    ./configure && make -j9 && make install
    tar zxvf memcached-1.4.6.tar.gz
    cd memcached-1.4.6
    ./configure --prefix=/usr/local/memcached
    make -j9 && make install
    # php mecache.so
    tar zxvf memcache-2.2.6.tgz
    cd memcache-2.2.6
/usr/local/zend/bin/phpize
    ./configure --with-php-config=/usr/local/zend/bin/php-config
    make -j8 && make install
    cd ..
    # php memached.so
    tar zxvf libmemcached-0.51.tar.gz
    cd libmemcached-0.51
    ./configure --prefix=/usr/local/libmemcached --with-memcached=/usr/local/memcached/bin/memcached
    make -j9 && make install
    cd ..
    tar zxvf memcached-1.0.2.tgz
    cd memcached-1.0.2
/usr/local/zend/bin/phpize
    ./configure --with-php-config=/usr/local/zend/bin/php-config --with-libmemcached-dir=/usr/local/libmemcached
    make -j9 && make install
    cd ../
wget [http://pecl.php.net/get/mongo-1.2.7.tgz](http://pecl.php.net/get/mongo-1.2.7.tgz)
tar zxvf mongo-1.2.7.tgz
cd mongo-1.2.7
/usr/local/zend/bin/phpize
./configure --with-php-config=/usr/local/zend/bin/php-config
make  && make install
/usr/local/zend/lib/php/20090626/
memcached.so  memcache.so redis.so mongo.so
