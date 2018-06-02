
function make_block(w, h, x0, y0, cx, cy, tx, ty)
{
  var s="";
  for (y = y0; y <= h - cy; y += ty) {
    for (x = x0; x <= w - cx; x += tx) {
      //printf("%d %d\n", x, y);
      s += ""+ x+ ",";
      s += ""+ y+ ",";
      s += ""+ (x+cx)+ ",";
      s += ""+ (y+cy)+ ",";
	  s+="\r\n";
    }
  }
  return s;
}

var fso = new ActiveXObject('Scripting.FileSystemObject');
var a = fso.CreateTextFile('iris_block_rc.txt', true);

s = make_block(45, 30, 0, 0, 8, 16, 1, 2);
a.WriteLine(s);
