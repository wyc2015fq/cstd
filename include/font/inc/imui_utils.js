
var fso = new ActiveXObject('Scripting.FileSystemObject');

var a = fso.CreateTextFile('imui_utils.txt', true);

var ii = 1;
var n = 10;
var nn = 20;

a.WriteLine('//此文件由 imui_utils.js自动生成，请勿修改');
a.WriteLine('#ifndef _IMUI_UTILS_H_');
a.WriteLine('#define _IMUI_UTILS_H_');

a.WriteLine('//IMDRAW_FUNDEF');

for (var i = ii; i < nn; ++i)
{
  var s = '#define IMDRAW_FUNCDEF' + i + '(NAME, T, A';
  for (var j = 0; j < i; ++j) {
    s += ', T' + j + ', A' + j
  }
  s += ')  IM_STATIC void NAME(ImDrawContent* g';
  for (var j = 0; j < i; ++j) {
    s += ', T' + j + ' A' + j;
  }

  s += ') { char* va = (char*)ImGui_AllocPush(g->buf, g->len, sizeof(T)';

  for (var j = 0; j < i; ++j) {
    s += '+sizeof(T' + j + ')';
  }
  s += '); va_arg(va,T) = A;';
  
  for (var j = 0; j < i; ++j) {
    s += 'va_arg(va,T' + j + ') = A' + j + ';';
  }
  s += '}';
  a.WriteLine(s);
  
  
  
  s = '#define IMDRAW_CALLDEF' + i + '(NAME, T, A';
  for (var j = 0; j < i; ++j) {
    s += ', T' + j + ', A' + j
  }
  s += ') ';
  for (var j = 0; j < i; ++j) {
    s += ', T' + j + ' A' + j;
  }

  s += '); va_arg(va,T) = A;';
  
  for (var j = 0; j < i; ++j) {
    s += 'va_arg(va,T' + j + ') = A' + j + ';';
  }
  s += '}';
  a.WriteLine(s);
  
}

a.WriteLine('#endif // _IMUI_UTILS_H_');


