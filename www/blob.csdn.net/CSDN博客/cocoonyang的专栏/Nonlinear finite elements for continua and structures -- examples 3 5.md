# Nonlinear finite elements for continua and structures -- examples 3.5 - cocoonyang的专栏 - CSDN博客





2017年02月17日 11:07:06[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：836
个人分类：[Nonlinear FEM](https://blog.csdn.net/cocoonyang/article/category/6730557)











```java
//
//  p96 Example 3.5
//   

const PI = 3.1415926;

//
// X, Y -- initial coordinates
// t    -- time coordinate
// a, b -- parameters
// 
function x_coord( a, b, t, X, Y)
{
    var theta = PI * t / 2;
    var result = ( 1 + a * t ) * X * Math.cos( theta ) - ( 1 + b * t ) * Y * Math.sin( theta );
    return result;
}

//
// X, Y -- initial coordinates
// t    -- time coordinate
// a, b -- parameters
// 
function y_coord( a, b, t, X, Y)
{
    var theta = PI * t / 2;
    var result = ( 1 + a * t ) * X * Math.sin( theta ) + ( 1 + b * t ) * Y * Math.cos( theta );
    return result;
}





function element( a, b, t, X, Y)
{
    var theta = PI * t / 2;
    var At = 1 + a * t;
    var Bt = 1 + b * t;
    var s  = Math.sin( theta )
    var c  = Math.cos( theta )

    var F11 =  At * c ;
    var F12 = -Bt * s ;
    var F21 =  At * s ;
    var F22 =  Bt * c ;


    console.log( 'F = ' );
    console.log( '[ ', parseFloat(F11).toFixed(3), ' ', parseFloat(F12).toFixed(3) );
    console.log( '  ', parseFloat(F21).toFixed(3), ' ', parseFloat(F22).toFixed(3), ']' );


    var E11 = 2 * a * t + a * t * a * t ;
    var E22 = 2 * b * t + b * t * b * t ;

    console.log( 'E = ' );
    console.log( '[ ', parseFloat(E11).toFixed(3), ' ', 0 );
    console.log( '  ',  0, ' ', parseFloat(E22).toFixed(3), ']' );
}



 
function show()
{
    
    for( time = 0; time < 1.0; time+= 0.1 )
    {
        var x0 = x_coord( para_a, para_b, time, 0, 0 );
        var y0 = y_coord( para_a, para_b, time, 0, 0 );    
        //console.log("x0 = ", x , " y0 = ", y );

        var x1 = x_coord( para_a, para_b, time, X_init, Y_init );
        var y1 = y_coord( para_a, para_b, time, X_init, Y_init );    
        //console.log("x1 = ", x , " y1 = ", y );
        
       var new_x0 = (parseFloat(x0) + parseFloat(200.0) ).toFixed(3);
       var new_y0 = (parseFloat(y0) + parseFloat(200.0) ).toFixed(3);

       var new_x1 = (parseFloat(x1) + parseFloat(200.0) ).toFixed(3);
       var new_y1 = (parseFloat(y1) + parseFloat(200.0) ).toFixed(3);

       console.log( 'M ', new_x0, ' ', new_y0, ' L ', new_x1, ' ', new_y1  );

       var length = Math.sqrt(  x1 *  x1 +  y1 *  y1 );
       console.log( 'element length = ', length );


       element( para_a, para_b, time, X_init, Y_init );
       console.log( ); 

    }

}

var para_a = 10;
var para_b = 10;
var time;
var X_init = 10;
var Y_init = 10;

show();
```



Results:



```cpp
M  200.000   200.000  L  210.000   210.000
element length =  14.142135623730951
F =
[  1.000   0.000
   0.000   1.000 ]
E =
[  0.000   0
   0   0.000 ]

M  200.000   200.000  L  216.625   222.882
element length =  28.284271247461902
F =
[  1.975   -0.313
   0.313   1.975 ]
E =
[  3.000   0
   0   3.000 ]

M  200.000   200.000  L  219.261   237.802
element length =  42.426406871192846
F =
[  2.853   -0.927
   0.927   2.853 ]
E =
[  8.000   0
   0   8.000 ]

M  200.000   200.000  L  217.481   253.800
element length =  56.568542494923804
F =
[  3.564   -1.816
   1.816   3.564 ]
E =
[  15.000   0
   0   15.000 ]

M  200.000   200.000  L  211.062   269.840
element length =  70.71067811865474
F =
[  4.045   -2.939
   2.939   4.045 ]
E =
[  24.000   0
   0   24.000 ]

M  200.000   200.000  L  200.000   284.853
element length =  84.85281374238569
F =
[  4.243   -4.243
   4.243   4.243 ]
E =
[  35.000   0
   0   35.000 ]

M  200.000   200.000  L  184.514   297.776
element length =  98.99494936611666
F =
[  4.114   -5.663
   5.663   4.114 ]
E =
[  48.000   0
   0   48.000 ]

M  200.000   200.000  L  165.039   307.600
element length =  113.13708498984761
F =
[  3.632   -7.128
   7.128   3.632 ]
E =
[  63.000   0
   0   63.000 ]

M  200.000   200.000  L  142.216   313.407
element length =  127.27922061357854
F =
[  2.781   -8.560
   8.560   2.781 ]
E =
[  80.000   0
   0   80.000 ]

M  200.000   200.000  L  116.875   314.412
element length =  141.4213562373095
F =
[  1.564   -9.877
   9.877   1.564 ]
E =
[  99.000   0
   0   99.000 ]

M  200.000   200.000  L  90.000   310.000
element length =  155.56349186104043
F =
[  0.000   -11.000
   11.000   0.000 ]
E =
[  120.000   0
   0   120.000 ]
```











