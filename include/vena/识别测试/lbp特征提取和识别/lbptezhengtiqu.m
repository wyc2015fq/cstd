clc;
%clear all;
path='..\ROI\';
file=dir(path);
count=0;
allfeature=[];
allname=[];
mapping=getmapping(8,'u2');
for tt=3:length(file)
    Iname=strcat(path,file(tt).name);
    disp(file(tt).name);
    I=imread(Iname);
    feaimg=fenkuailbp(I,2,8,mapping,'h',8);
    count=count+1;
    feaimg=single(feaimg(:)');
    allfeature=[allfeature;feaimg];
    name=file(tt).name(1);
    allname=[allname;name];
end
save allfeature allfeature
save name  allname
