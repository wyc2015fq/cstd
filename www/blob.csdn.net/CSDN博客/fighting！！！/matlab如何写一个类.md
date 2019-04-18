# matlab如何写一个类 - fighting！！！ - CSDN博客
2017年11月03日 20:21:00[dujiahei](https://me.csdn.net/dujiahei)阅读数：1832
                
matlab如何写一个类 - MagiCube - 博客园
http://www.cnblogs.com/magic-cube/archive/2011/11/08/2241580.html
先看一个matlab中的类代码：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
classdef TensileData
   properties
      Material = 'carbon steel';
      SampleNumber = 0;
      Stress
      Strain
   end
   properties (Dependent)
      Modulus
   end
   methods
      function td = TensileData(material,samplenum,stress,strain)
         if nargin > 0
            td.Material = material;
            td.SampleNumber = samplenum;
            td.Stress = stress;
            td.Strain = strain;
         end
      end % TensileData
   end
   methods
      function obj = set.Material(obj,material)
         if ~(strcmpi(material,'aluminum') ||...
               strcmpi(material,'stainless steel') ||...
               strcmpi(material,'carbon steel'))
            error('Material must be aluminum, stainless steel, or carbon steel')
         end
         obj.Material = material;
      end % Material set function
      function modulus = get.Modulus(obj)
         ind = find(obj.Strain > 0); % Find nonzero strain
         modulus = mean(obj.Stress(ind)./obj.Strain(ind));
      end % Modulus get function
      function obj = set.Modulus(obj,~)
         fprintf('%s%d\n','Modulus is: ',obj.Modulus)
         error('You cannot set Modulus explicitly');
      end
      function disp(td)
         fprintf(1,'Material: %s\nSample Number: %g\nModulus: %1.5g\n',...
            td.Material,td.SampleNumber,td.Modulus);
      end % disp
      function plot(td,varargin)
         plot(td.Strain,td.Stress,varargin{:})
         title(['Stress/Strain plot for Sample ',num2str(td.SampleNumber)])
         xlabel('Strain %')
         ylabel('Stress (psi)')
      end % plot
   end
   methods (Access = 'private') % Access by class members only
      function m = CalcModulus(td)
         % Over-simplified calculation of Elastic Modulus
         ind = find(td.Strain > 0); % Find nonzero strain
         m = mean(td.Stress(ind)./td.Strain(ind));
      end % CalcModulus
   end
end % classdef
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
在上述代码中，
classdef TensileData
　　...
end
是定义一个TensileData类。代码：
 properties
      Material = 'carbon steel';
      SampleNumber = 0;
      Stress
      Strain
   end
 是定义这个类的属性，也就是C++中类的成员变量。但是和C++不同的是，matlab中的类定义还有一个比较特别的地方的，就是定义依赖属性，如下述代码：
 properties (Dependent)
      Modulus
   end
这表示Modulus这个属性是个依赖属性，它的值是通过其他属性计算得到的，其中Dependent的默认属性值为True。
它的值是通过下面函数实现的：
% Modulus get function
function modulus = get.Modulus(obj)
         ind = find(obj.Strain > 0); % Find nonzero strain
         modulus = mean(obj.Stress(ind)./obj.Strain(ind));
end
类的方法（函数）的定义是以methods ... end的的形式出现的。如下面的类方法的定义：
methods
      function td = TensileData(material,samplenum,stress,strain)
         if nargin > 0
            td.Material = material;
            td.SampleNumber = samplenum;
            td.Stress = stress;
            td.Strain = strain;
         end
      end 
end
该函数块定义了TensileData构造函数方法。上述代码中的最后一个方法 methods (Access = 'private')
中的Access = 'private'表示该方法仅能被类本身访问和修改，是个私有成员方法。其中属性Access又可分为
SetAccess和GetAccess，属性值和Access相同。
在disp函数语句中出现“...”表示下一行和当前行是连接在一起的。如：
 function disp(td)
         fprintf(1,'Material: %s\nSample Number: %g\nModulus: %1.5g\n',...
            td.Material,td.SampleNumber,td.Modulus);
 end
就表示第二行和第三行是连在一起的。 
