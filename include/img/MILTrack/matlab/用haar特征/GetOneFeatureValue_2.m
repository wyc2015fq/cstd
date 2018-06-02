function y = GetOneFeatureValue_2( FeatureType, FeatureStPos, FeatureScale, InterData )
%函数功能：返回每个特征在积分图中对应的特征值

%%%%%%%%%%%%%%%%%%%%%%%
%  如果计算的是A类特征 % 
%%%%%%%%%%%%%%%%%%%%%%%
if (FeatureType == 0)

        posLeftTop0Y = FeatureStPos.y;
        posLeftTop0X = FeatureStPos.x;
        
        posMiddleTop0Y = FeatureStPos.y;
        posMiddleTop0X = posLeftTop0X + FeatureScale.x; 
        
        posRightTop0Y = FeatureStPos.y;
        posRightTop0X = posMiddleTop0X + FeatureScale.x;
        
        posLeftBottom0Y = posLeftTop0Y + FeatureScale.y;
        posLeftBottom0X = posLeftTop0X;
        
        posMiddleBottom0Y = posLeftBottom0Y;
        posMiddleBottom0X = posMiddleTop0X;
        
        posRightBottom0Y = posLeftBottom0Y;
        posRightBottom0X = posRightTop0X;

%         y = InterData ( posLeftTop0Y,posLeftTop0X ) + InterData ( posRightTop0Y,posRightTop0X ) ...
%             - InterData (posLeftBottom0Y,posLeftBottom0X) - InterData (posRightBottom0Y,posRightBottom0X)...
%             + 2 * (InterData (posMiddleBottom0Y,posMiddleBottom0X) - InterData (posMiddleTop0Y,posMiddleTop0X));
        y = 1*(InterData(posLeftTop0Y,posLeftTop0X)+InterData(posRightBottom0Y,posRightBottom0X)-InterData(posLeftBottom0Y,posLeftBottom0X)-InterData(posRightTop0Y,posRightTop0X))...
            -2*(InterData(posMiddleTop0Y,posMiddleTop0X)+InterData(posRightBottom0Y,posRightBottom0X)-InterData(posMiddleBottom0Y,posMiddleBottom0X)-InterData(posRightTop0Y,posRightTop0X ));
end
%%%%%%%%%%%%%%%%%%%%%%%
%  如果计算的是B类特征 % 
%%%%%%%%%%%%%%%%%%%%%%%
if (FeatureType == 1)

        posLeftTop1Y = FeatureStPos.y;
        posLeftTop1X = FeatureStPos.x;
        
        posRightTop1Y = posLeftTop1Y;
        posRightTop1X = posLeftTop1X + FeatureScale.x;
        
        posLeftMiddle1Y = posLeftTop1Y + FeatureScale.y;
        posLeftMiddle1X = posLeftTop1X;
        
        posRightMiddle1Y = posLeftMiddle1Y;
        posRightMiddle1X = posRightTop1X;
        
        posLeftBottom1Y = posLeftMiddle1Y + FeatureScale.y;
        posLeftBottom1X = posLeftTop1X;
        
        posRightBottom1Y = posLeftBottom1Y;
        posRightBottom1X = posRightTop1X;

%         y = InterData (posLeftTop1Y,posLeftTop1X) + InterData (posLeftBottom1Y,posLeftBottom1X)...
%             - InterData (posRightTop1Y,posRightTop1X) - InterData (posRightBottom1Y,posRightBottom1X)...
%             + 2 * (InterData (posRightMiddle1Y,posRightMiddle1X) - InterData (posLeftMiddle1Y,posLeftMiddle1X));
        y = 1*(InterData(posLeftTop1Y,posLeftTop1X)+InterData(posRightBottom1Y,posRightBottom1X)-InterData(posLeftBottom1Y,posLeftBottom1X)-InterData(posRightTop1Y,posRightTop1X))...
            -2*(InterData(posLeftMiddle1Y,posLeftMiddle1X)+InterData(posRightBottom1Y,posRightBottom1X)-InterData(posLeftBottom1Y,posLeftBottom1X)-InterData(posRightMiddle1Y,posRightMiddle1X));
            
            
end
%%%%%%%%%%%%%%%%%%%%%%%
%  如果计算的是C类特征 % 
%%%%%%%%%%%%%%%%%%%%%%%
if(FeatureType == 2)

        posLeftTop2Y = FeatureStPos.y;
        posLeftTop2X = FeatureStPos.x;
        
        posMiddleLTop2Y = posLeftTop2Y;
        posMiddleLTop2X = posLeftTop2X + FeatureScale.x;
        
        posMiddleRTop2Y = posLeftTop2Y;
        posMiddleRTop2X = posMiddleLTop2X + FeatureScale.x;
        
        posRightTop2Y = posLeftTop2Y;
        posRightTop2X = posMiddleRTop2X + FeatureScale.x;
        
        posLeftBottom2Y = posLeftTop2Y + FeatureScale.y;
        posLeftBottom2X = posLeftTop2X;
        
        posMiddleLBottom2Y = posLeftBottom2Y;
        posMiddleLBottom2X = posMiddleLTop2X;
        
        posMiddleRBottom2Y = posLeftBottom2Y;
        posMiddleRBottom2X = posMiddleRTop2X;
        
        posRightBottom2Y = posLeftBottom2Y;
        posRightBottom2X = posRightTop2X;

%         y = InterData (posLeftTop2Y,posLeftTop2X) + InterData (posRightBottom2Y,posRightBottom2X)...
%              - InterData (posRightTop2Y,posRightTop2X) - InterData (posLeftBottom2Y,posLeftBottom2X)...
%              + 2*(InterData (posMiddleRTop2Y,posMiddleRTop2X) + InterData (posMiddleLBottom2Y,posMiddleLBottom2X)... 
%              - InterData (posMiddleLTop2Y,posMiddleLTop2X) - InterData (posMiddleRBottom2Y,posMiddleRBottom2X));

        y = 1*(InterData(posLeftTop2Y,posLeftTop2X)+InterData(posRightBottom2Y,posRightBottom2X)-InterData(posLeftBottom2Y,posLeftBottom2X)-InterData(posRightTop2Y,posRightTop2X))...
            -3*(InterData(posMiddleLTop2Y,posMiddleLTop2X)+InterData(posMiddleRBottom2Y,posMiddleRBottom2X)-InterData(posMiddleLBottom2Y,posMiddleLBottom2X)-InterData(posMiddleRTop2Y,posMiddleRTop2X));
end

%%%%%%%%%%%%%%%%%%%%%%%
%  如果计算的是D类特征 % 
%%%%%%%%%%%%%%%%%%%%%%%
if (FeatureType == 3)

        posLeftTop3Y = FeatureStPos.y;
        posLeftTop3X = FeatureStPos.x;
        
        posMiddleTop3Y = posLeftTop3Y;
        posMiddleTop3X = posLeftTop3X + FeatureScale.x;
        
        posRightTop3Y = posLeftTop3Y;
        posRightTop3X = posMiddleTop3X + FeatureScale.x;
        
        posLeftMiddle3Y = posLeftTop3Y + FeatureScale.y;
        posLeftMiddle3X = posLeftTop3X;
        
        posMiddleMiddle3Y = posLeftMiddle3Y;
        posMiddleMiddle3X = posMiddleTop3X;
        
        posRightMiddle3Y = posLeftMiddle3Y;
        posRightMiddle3X = posRightTop3X;
        
        posLeftBottom3Y =  posLeftMiddle3Y + FeatureScale.y;
        posLeftBottom3X = posLeftTop3X;
        
        posMiddleBottom3Y = posLeftBottom3Y;
        posMiddleBottom3X = posMiddleTop3X;
        
        posRightBottom3Y = posLeftBottom3Y;
        posRightBottom3X = posRightTop3X;

%         y = 2 * (2*InterData (posMiddleMiddle3Y,posMiddleMiddle3X) - InterData (posMiddleTop3Y,posMiddleTop3X)...
%            - InterData (posLeftMiddle3Y,posLeftMiddle3X) - InterData (posRightMiddle3Y,posRightMiddle3X)...
%            - InterData (posMiddleBottom3Y,posMiddleBottom3X)) + InterData (posLeftTop3Y,posLeftTop3X)...
%            + InterData (posRightTop3Y,posRightTop3X) + InterData (posLeftBottom3Y,posLeftBottom3X)...
%            +  InterData (posRightBottom3Y,posRightBottom3X);
        y = 1*(InterData(posLeftTop3Y,posLeftTop3X)+InterData(posRightBottom3Y,posRightBottom3X)-InterData(posLeftBottom3Y,posLeftBottom3X)-InterData(posRightTop3Y,posRightTop3X))...
            -2*(InterData(posLeftMiddle3Y,posLeftMiddle3X)+InterData(posMiddleBottom3Y,posMiddleBottom3X)-InterData(posLeftBottom3Y,posLeftBottom3X)-InterData(posMiddleTop3Y,posMiddleTop3X))...
            -2*(InterData(posMiddleTop3Y,posMiddleTop3X)+InterData(posRightMiddle3Y,posRightMiddle3X)-InterData(posRightTop3Y,posRightTop3X)-InterData(posMiddleMiddle3Y,posMiddleMiddle3X));
end
%%%%%%%%%%%%%%%%%%%%%%%
%  如果计算的是E类特征 % 
%%%%%%%%%%%%%%%%%%%%%%%
if (FeatureType == 4)

        posLeftTop4Y = FeatureStPos.y;   
        posLeftTop4X = FeatureStPos.x;
        
        posRightTop4Y = posLeftTop4Y;   
        posRightTop4X = posLeftTop4X + FeatureScale.x;

        posLeftTMiddle4Y = posLeftTop4Y + FeatureScale.y;
        posLeftTMiddle4X = posLeftTop4X;
        
        posRightTMiddle4Y = posLeftTMiddle4Y;
        posRightTMiddle4X = posRightTop4X;

        posLeftBMiddle4Y = posLeftTMiddle4Y + FeatureScale.y;
        posLeftBMiddle4X = posLeftTop4X;
        
        posRightBMiddle4Y = posLeftBMiddle4Y;
        posRightBMiddle4X = posRightTop4X;

        posLeftBottom4Y = posLeftBMiddle4Y + FeatureScale.y;
        posLeftBottom4X = posLeftTop4X;
        
        posRightBottom4Y = posLeftBottom4Y;
        posRightBottom4X = posRightTop4X;

%         y = InterData (posLeftTop4Y,posLeftTop4X) + InterData (posRightBottom4Y,posRightBottom4X)...
%                 - InterData (posRightTop4Y,posRightTop4X) - InterData (posLeftBottom4Y,posLeftBottom4X)...
%                 + 2*(InterData (posRightTMiddle4Y,posRightTMiddle4X) + InterData (posLeftBMiddle4Y,posLeftBMiddle4X)...
%                 - InterData (posLeftTMiddle4Y,posLeftTMiddle4X) - InterData (posRightBMiddle4Y,posRightBMiddle4X)); 
       y = 1*(InterData(posLeftTop4Y,posLeftTop4X)+InterData(posRightBottom4Y,posRightBottom4X)-InterData(posLeftBottom4Y,posLeftBottom4X)-InterData(posRightTop4Y,posRightTop4X))...
           -3*(InterData(posLeftTMiddle4Y,posLeftTMiddle4X)+InterData(posRightBMiddle4Y,posRightBMiddle4X)-InterData(posLeftBMiddle4Y,posLeftBMiddle4X)-InterData (posRightTMiddle4Y,posRightTMiddle4X));
end
