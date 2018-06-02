
var fso = new ActiveXObject("Scripting.FileSystemObject");

var a = fso.CreateTextFile("arrinit.inl", true);

for (var i = 1; i < 20; ++i)
{
  var s = "#define ARRINIT" + i + "(P"

  for (var j = 0; j < i; ++j) {
    s += ", V" + j;
  }

  s += ")";

  for (var j = i; j < 20; ++j) {
    s += "    ";
  }

  s += "("

  for (var j = 0; j < i; ++j) {
    s += "((P))[" + j + "]=(V" + j + "),";
  }

  s += "P)"

      a.WriteLine(s);
}
