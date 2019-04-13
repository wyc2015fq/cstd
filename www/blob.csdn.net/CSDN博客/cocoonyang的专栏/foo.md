
# foo - cocoonyang的专栏 - CSDN博客


2016年01月06日 06:41:20[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：563








```python
//
//
//

key_point_coord = new Array();

key_point_coord.push( ['Front_Bumper_LH_Point', [0, 0, 10]] );
key_point_coord.push( ['Front_Frame_LH_Pillar_Joint', [80, 0,10]] );
 
key_point_coord.push( ['LH_Pillar_A_Upper_Point', [90, 0,30]] );
 
key_point_coord.push( ['LH_Pillar_A_Low_Point', [90, 0,0]] );
 
key_point_coord.push( ['LH_Pillar_B_Upper_Point', [110, 0,30]] );
 
key_point_coord.push( ['LH_Pillar_B_Low_Point', [110, 0,0]] );

key_point_coord.push( ['LH_Pillar_C_Upper_Point', [140, 0,30]] );
 
key_point_coord.push( ['LH_Pillar_C_Low_Point', [140, 0,0]] );

for( var point in key_point_coord )
{
  console.log( key_point_coord[ point ][0] +'_x = ' + key_point_coord[ point ][1][0] );
  console.log( key_point_coord[ point ][0] +'_y = ' + key_point_coord[ point ][1][1]   );
  console.log( key_point_coord[ point ][0] +'_z = ' + key_point_coord[ point ][1][2]   );
}
 
for( var point in key_point_coord )
{
    var n = parseInt(point) + 1
    console.log( 'N, ' + n + ', ' + key_point_coord[ point ][0] + '_x, ' + key_point_coord[ point ][0] + '_y, ' + key_point_coord[ point ][0] + '_z '  );
}
```








