
var fso = new ActiveXObject('Scripting.FileSystemObject');

var a = fso.CreateTextFile('matmulx.txt', true);

var ii = 1;
var n = 7;
var nn = 18;

a.WriteLine('//此文件由math.js自动生成，请勿修改');
a.WriteLine('#ifndef _MATMULX_H_');
a.WriteLine('#define _MATMULX_H_');
a.WriteLine('//矩阵乘法');

var dims = [4, 8, 16];

for (var i = 0; i < 3; ++i)
{
  var n = dims[i];
  var s = 'void matmul_' + n + 'x' + n + '(const float* A, const float* B, float* C) {\n';

  for (var j = 0; j < n; ++j) {
    for (var k = 0; k < n; ++k) {
      s += '  C['+ (j*n+k) +']+=';

      for (var m = 0; m < n; ++m) {
        s += m ? '+' : '';
        s += 'A[' + (j*n+m) + ']*B[' + (k*n+m) + ']';
      }

      s += ';\n';
    }
  }

  s += '}';
  a.WriteLine(s);
}


a.WriteLine('#endif // _MATMULX_H_');


