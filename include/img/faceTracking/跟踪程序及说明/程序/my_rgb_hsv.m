function I2=rgv_hsv(I)
I1=double(I);
R=I1(:,:,1);G=I1(:,:,2);B=I1(:,:,3);
[m,n]=size(R);

H=zeros(m,n);S=zeros(m,n);V=zeros(m,n);

xiao = min(min(R, G), B );
da = max(max( R, G), B );
V = da;

delta=da - xiao+1;
if da~=0
    S = delta ./da;
else
    S=0;
end


for i=1:m
    for j=1:n
        if( R(i,j) == da(i,j) )
            H(i,j) = ( G(i,j) - B(i,j) )/delta(i,j);
        else if( G(i,j) == da(i,j) )
                H(i,j) = 2 + ( B(i,j) - R(i,j) ) / delta(i,j);
             else
                H(i,j) = 4 + ( R(i,j) - G(i,j) ) / delta(i,j);
            end
        end
        H(i,j)=H(i,j)*60;
        if( H(i,j) < 0 )
            H(i,j) =H(i,j)+ 360;
        end
    end
end

H=H/max(max(H));S=S/max(max(S));V=V/max(max(V));
I2(:,:,1)=H;I2(:,:,2)=S;I2(:,:,3)=V;
%imshow(I2);



% I3=rgb2hsv(I);
% figure,imshow(I3);