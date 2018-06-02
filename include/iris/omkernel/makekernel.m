%%%%构造高斯核%%%%%%%%%%%%%%%%%%%%%
function output=makekernel(width,disx,disy,delta)
size=((width-1)/2);
[x,y]=meshgrid(-size:size,-size:size);
ks=(1/sqrt(2*pi*delta));
x=x-disx;
y=y-disy;
arg= -(x.*x + y.*y)/(2*delta*delta);
z=ks.*exp(arg);
z(z<eps*max(z(:)))=0;
if sum(z(:))~=0
    z=z/sum(z(:));%归一化
end
output=z;
