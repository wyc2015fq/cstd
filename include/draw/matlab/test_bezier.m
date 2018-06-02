
clc;
%solve('P=(1-t)^2*A + 2*t*(1-t)*B + t^2*C', 'B')

%[x, y] = solve('P1=(1-t1)^3*A + 3*t1*((1-t1)^2)*x + 3*(t1^2)*(1-t1)*y + t^3*D', 'P2=(1-t2)^3*A + 3*t2*((1-t2)^2)*x + 3*(t2^2)*(1-t2)*y + t2^3*D')

%[x, y] = solve('0=c1+a1*x + b1*y', '0=c2+a2*x + b2*y')
%[x1 x2 x3 x4] = solve('x1+x2=5', 'x3+x4=4',  'x1+x3=7', 'x2+x4=2', 'x1=3');

P = phantom(128); %Generate the Shepp-Logan phantom in a 128x128 array
angle = linspace(0,179,18); %Sampling angles
R = radon(P, angle); %Generate the Radon transform over 180бу,
I1 = iradon(R, angle); %Inverse Radon transform, i.e.,FBP reconstruction
I2 = iradon(R, angle,'linear','Hann'); %backprojection without ramp-filtering
subplot(1,3,1), imshow(P), title('Original')
subplot(1,3,2), imshow(I1), title('Filtered Backprojection')
subplot(1,3,3), imshow(I2,[]), title('Unfiltered Backprojection projection')
%imshow(R/max(max(R)));
