function [rowcenter colcenter M00 probmap] = zhongxin(y_min,y_max,x_min,x_max,probmap,I1,histogram_h,histogram_s,histogram_v);

A=10;
M00 = 0;
M10 = 0;
M01 = 0;
[m,n,l]=size(I1);


for c = x_min:x_max
    for r = y_min:y_max
        M00 = M00 + probmap(r, c);
    end
end

for c = x_min:x_max
    for r = y_min:y_max
        M10 = M10 + c*probmap(r,c);
        M01 = M01 + r*probmap(r,c);
    end
end


if M00==0
        I1_hsv=rgb2hsv(I1);
        h=I1_hsv(:,:,1);s=I1_hsv(:,:,2);v=I1_hsv(:,:,3);
        h=h*255;s=s*255;v=v*255;
        h=uint8(h);s=uint8(s);v=uint8(v);
        [m,n]=size(h);
        probmap_h = zeros(m,n);probmap_s = zeros(m,n);probmap_v= zeros(m,n);
        for i=1:m
            for j=1:n
                probmap_h(i,j)=histogram_h(h(i,j)+1,1);
                probmap_s(i,j)=histogram_s(s(i,j)+1,1);
                probmap_v(i,j)=histogram_v(v(i,j)+1,1);
            end
        end
        probmap=probmap_h.*probmap_s.*probmap_v;
        probmap=bwmorph(probmap,'dilate');

        for c = x_min:x_max
            for r = y_min:y_max
                M00 = M00 + probmap(r, c);
            end
        end

        for c = x_min:x_max
            for r = y_min:y_max
                M10 = M10 + c*probmap(r,c);
                M01 = M01 + r*probmap(r,c);
            end
        end
end

while M00==0%while—≠ª∑∑¿÷πM00=0
    M10 = 0;
    M01 = 0;
    x_min=x_min-A;
    x_max=x_max+A;
    y_min=y_min-A;
    y_max=y_max+A;


    if y_min<1,a=y_max-y_min;y_min=1;y_max=y_min+a;if y_max>m,y_max=m;end;end%∑¿÷π‘ΩΩÁ
    if y_max>m,b=y_max-y_min;y_max=m;y_min=y_max-b;if y_min<1,y_min=1;end;end
    if x_min<1,c=x_max-x_min;x_min=1;x_max=x_min+c;if x_max>n,x_max=n;end;end
    if x_max>n,d=x_max-x_min;x_max=n;x_min=x_max-d;if x_min<1,x_min=1;end;end

    for c = x_min:x_max
        for r = y_min:y_max
            M00 = M00 + probmap(r, c);
        end
    end

    for c = x_min:x_max
        for r = y_min:y_max
            M10 = M10 + c*probmap(r,c);
            M01 = M01 + r*probmap(r,c);
        end
    end
end

colcenter = M10/M00;
rowcenter = M01/M00;
