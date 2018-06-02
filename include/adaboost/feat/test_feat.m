
%function [h,grad_m,luv] = getfeature_10(img,bin_size,nbin,sample_factor)
clc;
im = imread('1.bmp');
bin_size = 4;
nbin=6;
sample_factor = 4;

%sample_factor = 4;
%bin_size = 4;
%nbin =6;
%img = single(imread('E:\luobo\databases\AFLW\aflw-images-3\aflw\data\data\flickr\0\image00002.jpg'));

filter = filterBinomial1d( 1 ); % 半径r=1的1维二项式滤波器
%filter2 = filter*filter';% 半径r=1的2维二项式滤波器
%img = imfilter(img,filter,'conv'); %进行pre-smoothing
im = double(im);
im = im/255.;
%im(:,:,3) = img(:,:,3)/max(max(img(:,:,3)));
img = im;
[m,n,k] = size(img);

%[H,Grad_M] = hog(single(im),bin_size,nbin);
%LUV = rgbConvert(single(im),'luv');

[H,Grad_M] = hog(single(img),bin_size,nbin);
LUV = rgbConvert(img,'luv');

h = H(:,:,1:nbin);
grad_m = imResample(Grad_M,[m,n]/sample_factor);
luv = imResample(LUV,[m,n]/sample_factor);

