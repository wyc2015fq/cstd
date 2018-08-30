
pow = Math.pow;
floor = Math.floor;
sqrt = Math.sqrt;
tanh = Math.tanh;

var shift = 13;
var shift1 = pow(2, shift);

var fso = new ActiveXObject("Scripting.FileSystemObject");
var a = fso.CreateTextFile("invvar_table.inl", true);

var ivar2;
invvar_table_inited = 1;
  a.WriteLine((1 << 13) +',');
for ( ivar2 = 1; ivar2 < ( 2 << 13 ); ++ivar2 ) {
//  invvar_table[ ivar2 ] = INVSQRT( ivar2 );
//  a.WriteLine(',');
var ivar3 = floor(sqrt(1.*( 1 << 13 )*( 1 << 13 )/ivar2));
//WScript.Echo('['+ivar2+'] = \t'+ivar3);
  a.WriteLine(ivar3+',');
}

  