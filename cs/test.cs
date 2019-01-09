using System;
using System.Drawing;
using System.Text;

namespace Skyiv.Utils
{
  sealed class GB2312Creator
  {
    static void Main(string[] args)
    {
      var count = (args.Length > 0) ? int.Parse(args[0]) : 1800;
      Console.WriteLine(new GB2312Creator().GetGB2312String(count));
    }

    string GetGB2312String(int count)
    {
      var random = new Random();
      var bs = new byte[count * 2];
      for (var i = 0; i < count; i++)
      {
        var c = GetGB2312Char(random);
        bs[i * 2] = (byte)(c.X + 0xa0);
        bs[i * 2 + 1] = (byte)(c.Y + 0xa0);
      }
      return Encoding.GetEncoding("GB2312").GetString(bs);
    }

    Point GetGB2312Char(Random random)
    {
      // 国标一级字(共3755个): 区:16-55, 位:01-94, 55区最后5位为空位
      var 区 = random.Next(40) + 16;
      var 位 = random.Next(区 == 55 ? 89 : 94) + 1;
      return new Point(区, 位);
    }
  }
}