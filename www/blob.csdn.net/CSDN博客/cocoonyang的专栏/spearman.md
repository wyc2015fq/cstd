# spearman - cocoonyang的专栏 - CSDN博客





2017年11月09日 20:45:42[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：112








使用JavaScript实现的计算Spearman 相关性系数的函数 

```java
'use strict';

var DEBUG = 0;
module.exports = function( ) {

    this.spearman = function( x, y )
    { 

        // Checking parameters
        if( undefined == typeof( x ) )
        {
             return;
         }
        if( undefined == typeof( y ) )
        {
             return;
         }
        var len_x = x.length;
        var len_y = y.length;
        var len = len_x < len_y ? len_x : len_y;
        if( len <= 0 )
        {
             return;
        }

        var x_mean = 0.0;
        var y_mean = 0.0;

        for(var i = 0; i < len; i++ )
        {
             x_mean += parseFloat(x[i]);
             y_mean += parseFloat(y[i]);
        }
        x_mean = x_mean / len;
        y_mean = y_mean / len;

        var tmp_u = 0.0 ;

        for(var i = 0; i < len; i++ )
        {
             tmp_u += (x_mean - x[i]) * (y_mean - y[i]);
        }
        //console.log( 'tmp_u =', tmp_u );

        var tmp_d_x = 0.0;
        for(var i = 0; i < len; i++ )
        {
             tmp_d_x += (x_mean - x[i]) * (x_mean - x[i]);
        }

        var tmp_d_y = 0.0;
        for(var i = 0; i < len; i++ )
        {
             tmp_d_y += (y_mean - y[i]) * (y_mean - y[i]);
        }

        var rho = tmp_u /Math.sqrt( tmp_d_x * tmp_d_y )

if( DEBUG ){ 
    console.log('             Report   ')
    console.log('-------------------------------')
    console.log( '    Data Set Number =', len  );
    console.log( '    Mean Vaule of X =', x_mean  );
    console.log( '    Mean Vaule of Y =', y_mean  );
    console.log( 'Spearman cor of X-Y =', rho  );
} 

        return rho;
    }

}
```



