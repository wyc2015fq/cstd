
#include "graphics.inl"
#include "Liquid.inl"
//#include "test_cube.inl"

int test_graphics()
{
  test_liquid();
  return 0;
}


#if 0
double p(double x, double y, double z)
{
return {x:
    x, y:
    y, z:
      z
         };
}
function s(a, z)
{
  r = w / 10;
  R = w / 3;
  b = -20 * cos(a * 5 + t);
  return p(w / 2 + (R * cos(a) + r * sin(z + 2 * t)) / z + cos(a) * b, h / 2 + (R * sin(a)) / z + sin(a) * b);
}
function q(a, da, z, dz)
{
  var v = [s(a, z), s(a + da, z), s(a + da, z + dz), s(a, z + dz)];
  c.beginPath();
  c.moveTo(v[0].x, v[0].y);

  for (i in v) {
    c.lineTo(v[i].x, v[i].y);
  }

  c.fill();
}
var Z = -0.20;
var t = 0;
function d()
{
  t += 1 / 30.0;
  c.fillStyle = "#000";
  c.fillRect(0, 0, w, h);
  c.fillStyle = "#f00";
  var n = 30;
  var a = 0;
  var da = 2 * Math.PI / n;
  var dz = 0.25;

  for (var z = Z + 8; z > Z; z -= dz) {
    for (var i = 0; i < n; i++) {
      fog = 1 / (MAX((z + 0.7) - 3, 1));

      if (z <= 2) {
        fog = MAX(0, z / 2 * z / 2);
      }

      var k = (205 * (fog * Math.abs(sin(i / n * 2 * 3.14 + t)))) >> 0;
      k *= (0.55 + 0.45 * cos((i / n + 0.25) * Math.PI * 5));
      k = k >> 0;
      c.fillStyle = "rgb(" + k + "," + k + "," + k + ")";
      q(a, da, z, dz);

      if (i % 3 == 0) {
        c.fillStyle = "#000";
        q(a, da / 10, z, dz);
      }

      a += da;
    }
  }

  Z -= 0.05;

  if (Z <= dz) {
    Z += dz;
  }
}
#endif
