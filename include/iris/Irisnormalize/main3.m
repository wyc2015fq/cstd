%%%%%主函数

%%把图像读入，作为一个矩阵
Classnum=70;
snuml=10;
snumr=4;
row=30;
line=360;

Datel=zeros(row,line,snuml,Classnum);
Maskl=zeros(row,line,snuml,Classnum);

Juanl=zeros(row,line,snuml,Classnum);

%%class L1
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2001L01.jpg');
Datel(:,:,1,1)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2001L01.jpg');
Maskl(:,:,1,1)=T1';

T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2001L02.jpg');
Datel(:,:,2,1)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2001L02.jpg');
Maskl(:,:,2,1)=T1';

T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2001L04.jpg');
Datel(:,:,3,1)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2001L04.jpg');
Maskl(:,:,3,1)=T1';

T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2001L06.jpg');
Datel(:,:,4,1)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2001L06.jpg');
Maskl(:,:,4,1)=T1';

T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2001L08.jpg');
Datel(:,:,5,1)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2001L08.jpg');
Maskl(:,:,5,1)=T1';

T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2001L10.jpg');
Datel(:,:,6,1)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2001L10.jpg');
Maskl(:,:,6,1)=T1';

T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2001L11.jpg');
Datel(:,:,7,1)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2001L11.jpg');
Maskl(:,:,7,1)=T1';

T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2001L14.jpg');
Datel(:,:,8,1)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2001L14.jpg');
Maskl(:,:,8,1)=T1';

T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2001L15.jpg');
Datel(:,:,9,1)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2001L15.jpg');
Maskl(:,:,9,1)=T1';

T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2001L20.jpg');
Datel(:,:,10,1)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2001L20.jpg');
Maskl(:,:,10,1)=T1';

%%%%%%%%class L2
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2002L01.jpg');
Datel(:,:,1,2)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2002L01.jpg');
Maskl(:,:,1,2)=T1';


T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2002L02.jpg');
Datel(:,:,2,2)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2002L02.jpg');
Maskl(:,:,2,2)=T1';

T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2002L03.jpg');
Datel(:,:,3,2)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2002L03.jpg');
Maskl(:,:,3,2)=T1';

T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2002L04.jpg');
Datel(:,:,4,2)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2002L04.jpg');
Maskl(:,:,4,2)=T1';

T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2002L06.jpg');
Datel(:,:,5,2)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2002L06.jpg');
Maskl(:,:,5,2)=T1';

T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2002L08.jpg');
Datel(:,:,6,2)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2002L08.jpg');
Maskl(:,:,6,2)=T1';

T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2002L13.jpg');
Datel(:,:,7,2)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2002L13.jpg');
Maskl(:,:,7,2)=T1';

T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2002L16.jpg');
Datel(:,:,8,2)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2002L16.jpg');
Maskl(:,:,8,2)=T1';

%T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2002L17.jpg');
%Datel(:,:,9,2)=T1';
%T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2002L17.jpg');
%Maskl(:,:,9,2)=T1';

T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2002L20.jpg');
Datel(:,:,10,2)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2002L20.jpg');
Maskl(:,:,10,2)=T1';

%%%%%class L3
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2003L01.jpg');
Datel(:,:,1,3)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2003L01.jpg');
Maskl(:,:,1,3)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2003L02.jpg');
Datel(:,:,2,3)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2003L02.jpg');
Maskl(:,:,2,3)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2003L03.jpg');
Datel(:,:,3,3)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2003L03.jpg');
Maskl(:,:,3,3)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2003L04.jpg');
Datel(:,:,4,3)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2003L04.jpg');
Maskl(:,:,4,3)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2003L06.jpg');
Datel(:,:,5,3)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2003L06.jpg');
Maskl(:,:,5,3)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2003L07.jpg');
Datel(:,:,6,3)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2003L07.jpg');
Maskl(:,:,6,3)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2003L08.jpg');
Datel(:,:,7,3)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2003L08.jpg');
Maskl(:,:,7,3)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2003L16.jpg');
Datel(:,:,8,3)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2003L16.jpg');
Maskl(:,:,8,3)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2003L20.jpg');
Datel(:,:,9,3)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2003L20.jpg');
Maskl(:,:,9,3)=T1';

%%%class L4
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2004L01.jpg');
Datel(:,:,1,4)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2004L01.jpg');
Maskl(:,:,1,4)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2004L02.jpg');
Datel(:,:,2,4)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2004L02.jpg');
Maskl(:,:,2,4)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2004L04.jpg');
Datel(:,:,3,4)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2004L04.jpg');
Maskl(:,:,3,4)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2004L06.jpg');
Datel(:,:,4,4)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2004L06.jpg');
Maskl(:,:,4,4)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2004L08.jpg');
Datel(:,:,5,4)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2004L08.jpg');
Maskl(:,:,5,4)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2004L09.jpg');
Datel(:,:,6,4)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2004L09.jpg');
Maskl(:,:,6,4)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2004L16.jpg');
Datel(:,:,7,4)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2004L16.jpg');
Maskl(:,:,7,4)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2004L17.jpg');
Datel(:,:,8,4)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2004L17.jpg');
Maskl(:,:,8,4)=T1';

%%%%%%%%class L5
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2005L01.jpg');
Datel(:,:,1,5)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2005L01.jpg');
Maskl(:,:,1,5)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2005L02.jpg');
Datel(:,:,2,5)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2005L02.jpg');
Maskl(:,:,2,5)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2005L04.jpg');
Datel(:,:,3,5)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2005L04.jpg');
Maskl(:,:,3,5)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2005L05.jpg');
Datel(:,:,4,5)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2005L05.jpg');
Maskl(:,:,4,5)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2005L06.jpg');
Datel(:,:,5,5)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2005L06.jpg');
Maskl(:,:,5,5)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2005L07.jpg');
Datel(:,:,6,5)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2005L07.jpg');
Maskl(:,:,6,5)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2005L08.jpg');
Datel(:,:,7,5)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2005L08.jpg');
Maskl(:,:,7,5)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2005L10.jpg');
Datel(:,:,8,5)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2005L10.jpg');
Maskl(:,:,8,5)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2005L16.jpg');
Datel(:,:,9,5)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2005L16.jpg');
Maskl(:,:,9,5)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2005L20.jpg');
Datel(:,:,10,5)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2005L20.jpg');
Maskl(:,:,10,5)=T1';

%%%%%%%%%%%%%%class L6
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2006L01.jpg');
Datel(:,:,1,6)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2006L01.jpg');
Maskl(:,:,1,6)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2006L04.jpg');
Datel(:,:,2,6)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2006L04.jpg');
Maskl(:,:,2,6)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2006L08.jpg');
Datel(:,:,3,6)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2006L08.jpg');
Maskl(:,:,3,6)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2006L11.jpg');
Datel(:,:,4,6)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2006L11.jpg');
Maskl(:,:,4,6)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2006L13.jpg');
Datel(:,:,5,6)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2006L13.jpg');
Maskl(:,:,5,6)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2006L15.jpg');
Datel(:,:,6,6)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2006L15.jpg');
Maskl(:,:,6,6)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2006L19.jpg');
Datel(:,:,7,6)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2006L19.jpg');
Maskl(:,:,7,6)=T1';

%%%%%%%%%%class L7
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2007L01.jpg');
Datel(:,:,1,7)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2007L01.jpg');
Maskl(:,:,1,7)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2007L03.jpg');
Datel(:,:,2,7)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2007L03.jpg');
Maskl(:,:,2,7)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2007L05.jpg');
Datel(:,:,3,7)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2007L05.jpg');
Maskl(:,:,3,7)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2007L06.jpg');
Datel(:,:,4,7)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2007L06.jpg');
Maskl(:,:,4,7)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2007L09.jpg');
Datel(:,:,5,7)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2007L09.jpg');
Maskl(:,:,5,7)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2007L10.jpg');
Datel(:,:,6,7)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2007L10.jpg');
Maskl(:,:,6,7)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2007L13.jpg');
Datel(:,:,7,7)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2007L13.jpg');
Maskl(:,:,7,7)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2007L15.jpg');
Datel(:,:,8,7)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2007L15.jpg');
Maskl(:,:,8,7)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2007L16.jpg');
Datel(:,:,9,7)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2007L16.jpg');
Maskl(:,:,9,7)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2007L20.jpg');
Datel(:,:,10,7)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2007L20.jpg');
Maskl(:,:,10,7)=T1';

%%%%%%%%%%%%%%%%class L8
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2008L01.jpg');
Datel(:,:,1,8)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2008L01.jpg');
Maskl(:,:,1,8)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2008L04.jpg');
Datel(:,:,2,8)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2008L04.jpg');
Maskl(:,:,2,8)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2008L05.jpg');
Datel(:,:,3,8)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2008L05.jpg');
Maskl(:,:,3,8)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2008L06.jpg');
Datel(:,:,4,8)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2008L06.jpg');
Maskl(:,:,4,8)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2008L07.jpg');
Datel(:,:,5,8)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2008L07.jpg');
Maskl(:,:,5,8)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2008L08.jpg');
Datel(:,:,6,8)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2008L08.jpg');
Maskl(:,:,6,8)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2008L10.jpg');
Datel(:,:,7,8)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2008L10.jpg');
Maskl(:,:,7,8)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2008L12.jpg');
Datel(:,:,8,8)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2008L12.jpg');
Maskl(:,:,8,8)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2008L17.jpg');
Datel(:,:,9,8)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2008L17.jpg');
Maskl(:,:,9,8)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2008L19.jpg');
Datel(:,:,10,8)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2008L19.jpg');
Maskl(:,:,10,8)=T1';

%%%%%%%class L9
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2009L01.jpg');
Datel(:,:,1,9)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2009L01.jpg');
Maskl(:,:,1,9)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2009L02.jpg');
Datel(:,:,2,9)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2009L02.jpg');
Maskl(:,:,2,9)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2009L04.jpg');
Datel(:,:,3,9)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2009L04.jpg');
Maskl(:,:,3,9)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2009L06.jpg');
Datel(:,:,4,9)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2009L06.jpg');
Maskl(:,:,4,9)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2009L08.jpg');
Datel(:,:,5,9)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2009L08.jpg');
Maskl(:,:,5,9)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2009L09.jpg');
Datel(:,:,6,9)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2009L09.jpg');
Maskl(:,:,6,9)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2009L11.jpg');
Datel(:,:,7,9)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2009L11.jpg');
Maskl(:,:,7,9)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2009L15.jpg');
Datel(:,:,8,9)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2009L15.jpg');
Maskl(:,:,8,9)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2009L17.jpg');
Datel(:,:,9,9)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2009L17.jpg');
Maskl(:,:,9,9)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2009L19.jpg');
Datel(:,:,10,9)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2009L19.jpg');
Maskl(:,:,10,9)=T1';

%%%%%%%%%%class L10
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2010L01.jpg');
Datel(:,:,1,10)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2010L01.jpg');
Maskl(:,:,1,10)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2010L02.jpg');
Datel(:,:,2,10)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2010L02.jpg');
Maskl(:,:,2,10)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2010L03.jpg');
Datel(:,:,3,10)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2010L03.jpg');
Maskl(:,:,3,10)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2010L04.jpg');
Datel(:,:,4,10)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2010L04.jpg');
Maskl(:,:,4,10)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2010L06.jpg');
Datel(:,:,5,10)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2010L06.jpg');
Maskl(:,:,5,10)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2010L08.jpg');
Datel(:,:,6,10)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2010L08.jpg');
Maskl(:,:,6,10)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2010L09.jpg');
Datel(:,:,7,10)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2010L09.jpg');
Maskl(:,:,7,10)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2010L12.jpg');
Datel(:,:,8,10)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2010L12.jpg');
Maskl(:,:,8,10)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2010L16.jpg');
Datel(:,:,9,10)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2010L16.jpg');
Maskl(:,:,9,10)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2010L18.jpg');
Datel(:,:,10,10)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2010L18.jpg');
Maskl(:,:,10,10)=T1';

%%%%%%%%%%class L11
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2011L01.jpg');
Datel(:,:,1,11)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2011L01.jpg');
Maskl(:,:,1,11)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2011L02.jpg');
Datel(:,:,2,11)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2011L02.jpg');
Maskl(:,:,2,11)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2011L04.jpg');
Datel(:,:,3,11)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2011L04.jpg');
Maskl(:,:,3,11)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2011L06.jpg');
Datel(:,:,4,11)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2011L06.jpg');
Maskl(:,:,4,11)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2011L09.jpg');
Datel(:,:,5,11)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2011L09.jpg');
Maskl(:,:,5,11)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2011L10.jpg');
Datel(:,:,6,11)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2011L10.jpg');
Maskl(:,:,6,11)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2011L13.jpg');
Datel(:,:,7,11)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2011L13.jpg');
Maskl(:,:,7,11)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2011L17.jpg');
Datel(:,:,8,11)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2011L17.jpg');
Maskl(:,:,8,11)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2011L16.jpg');
Datel(:,:,9,11)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2011L16.jpg');
Maskl(:,:,9,11)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2011L19.jpg');
Datel(:,:,10,11)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2011L19.jpg');
Maskl(:,:,10,11)=T1';


%%%%%%%%%class L12
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2012L01.jpg');
Datel(:,:,1,12)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2012L01.jpg');
Maskl(:,:,1,12)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2012L03.jpg');
Datel(:,:,2,12)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2012L03.jpg');
Maskl(:,:,2,12)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2012L04.jpg');
Datel(:,:,3,12)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2012L04.jpg');
Maskl(:,:,3,12)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2012L06.jpg');
Datel(:,:,4,12)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2012L06.jpg');
Maskl(:,:,4,12)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2012L09.jpg');
Datel(:,:,5,12)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2012L09.jpg');
Maskl(:,:,5,12)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2012L11.jpg');
Datel(:,:,6,12)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2012L11.jpg');
Maskl(:,:,6,12)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2012L15.jpg');
Datel(:,:,7,12)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2012L15.jpg');
Maskl(:,:,7,12)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2012L17.jpg');
Datel(:,:,8,12)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2012L17.jpg');
Maskl(:,:,8,12)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2012L18.jpg');
Datel(:,:,9,12)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2012L18.jpg');
Maskl(:,:,9,12)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2012L19.jpg');
Datel(:,:,10,12)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2012L19.jpg');
Maskl(:,:,10,12)=T1';

%%%class L13
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2013L02.jpg');
Datel(:,:,1,13)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2013L02.jpg');
Maskl(:,:,1,13)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2013L04.jpg');
Datel(:,:,2,13)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2013L04.jpg');
Maskl(:,:,2,13)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2013L06.jpg');
Datel(:,:,3,13)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2013L06.jpg');
Maskl(:,:,3,13)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2013L08.jpg');
Datel(:,:,4,13)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2013L08.jpg');
Maskl(:,:,4,13)=T1';

%%%%%%%%%%%class L14
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2014L02.jpg');
Datel(:,:,1,14)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2014L02.jpg');
Maskl(:,:,1,14)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2014L04.jpg');
Datel(:,:,2,14)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2014L04.jpg');
Maskl(:,:,2,14)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2014L08.jpg');
Datel(:,:,3,14)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2014L08.jpg');
Maskl(:,:,3,14)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2014L06.jpg');
Datel(:,:,4,14)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2014L06.jpg');
Maskl(:,:,4,14)=T1';

%%%%%%%%class L15
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2017L02.jpg');
Datel(:,:,1,15)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2017L02.jpg');
Maskl(:,:,1,15)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2017L04.jpg');
Datel(:,:,2,15)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2017L04.jpg');
Maskl(:,:,2,15)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2017L06.jpg');
Datel(:,:,3,15)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2017L06.jpg');
Maskl(:,:,3,15)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2017L08.jpg');
Datel(:,:,4,15)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2017L08.jpg');
Maskl(:,:,4,15)=T1';

%%%%%%%%%%class L16
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2018L02.jpg');
Datel(:,:,1,16)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2018L02.jpg');
Maskl(:,:,1,16)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2018L04.jpg');
Datel(:,:,2,16)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2018L04.jpg');
Maskl(:,:,2,16)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2018L06.jpg');
Datel(:,:,3,16)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2018L06.jpg');
Maskl(:,:,3,16)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2018L08.jpg');
Datel(:,:,4,16)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2018L08.jpg');
Maskl(:,:,4,16)=T1';

%%%%%%%%%%%%%class L17
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2019L02.jpg');
Datel(:,:,1,17)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2019L02.jpg');
Maskl(:,:,1,17)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2019L04.jpg');
Datel(:,:,2,17)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2019L04.jpg');
Maskl(:,:,2,17)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2019L06.jpg');
Datel(:,:,3,17)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2019L06.jpg');
Maskl(:,:,3,17)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2019L08.jpg');
Datel(:,:,4,17)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2019L08.jpg');
Maskl(:,:,4,17)=T1';
 
%%%%%%%%%class L18
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2020L02.jpg');
Datel(:,:,1,18)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2020L02.jpg');
Maskl(:,:,1,18)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2020L04.jpg');
Datel(:,:,2,18)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2020L04.jpg');
Maskl(:,:,2,18)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2020L06.jpg');
Datel(:,:,3,18)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2020L06.jpg');
Maskl(:,:,3,18)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2020L08.jpg');
Datel(:,:,4,18)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2020L08.jpg');
Maskl(:,:,4,18)=T1';
 
%%%%%%%class L19
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2021L02.jpg');
Datel(:,:,1,19)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2021L02.jpg');
Maskl(:,:,1,19)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2021L04.jpg');
Datel(:,:,2,19)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2021L04.jpg');
Maskl(:,:,2,19)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2021L06.jpg');
Datel(:,:,3,19)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2021L06.jpg');
Maskl(:,:,3,19)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2021L08.jpg');
Datel(:,:,4,19)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2021L08.jpg');
Maskl(:,:,4,19)=T1';
 
%%%%class L20
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2022L02.jpg');
Datel(:,:,1,20)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2022L02.jpg');
Maskl(:,:,1,20)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2022L04.jpg');
Datel(:,:,2,20)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2022L04.jpg');
Maskl(:,:,2,20)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2022L06.jpg');
Datel(:,:,3,20)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2022L06.jpg');
Maskl(:,:,3,20)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2022L08.jpg');
Datel(:,:,4,20)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2022L08.jpg');
Maskl(:,:,4,20)=T1';
 
%%%%%%%%%class L21
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2023L02.jpg');
Datel(:,:,1,21)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2023L02.jpg');
Maskl(:,:,1,21)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2023L04.jpg');
Datel(:,:,2,21)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2023L04.jpg');
Maskl(:,:,2,21)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2023L06.jpg');
Datel(:,:,3,21)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2023L06.jpg');
Maskl(:,:,3,21)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2023L08.jpg');
Datel(:,:,4,21)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2023L08.jpg');
Maskl(:,:,4,21)=T1';

%%%%%%%%%class L22
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2026L02.jpg');
Datel(:,:,1,22)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2026L02.jpg');
Maskl(:,:,1,22)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2026L04.jpg');
Datel(:,:,2,22)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2026L04.jpg');
Maskl(:,:,2,22)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2026L06.jpg');
Datel(:,:,3,22)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2026L06.jpg');
Maskl(:,:,3,22)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2026L08.jpg');
Datel(:,:,4,22)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2026L08.jpg');
Maskl(:,:,4,22)=T1';

%%%%%%%%%class L23
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2027L02.jpg');
Datel(:,:,1,23)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2027L02.jpg');
Maskl(:,:,1,23)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2027L04.jpg');
Datel(:,:,2,23)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2027L04.jpg');
Maskl(:,:,2,23)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2027L06.jpg');
Datel(:,:,3,23)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2027L06.jpg');
Maskl(:,:,3,23)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2027L08.jpg');
Datel(:,:,4,23)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2027L08.jpg');
Maskl(:,:,4,23)=T1';

%%%%%%%%class L24
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2028L02.jpg');
Datel(:,:,1,24)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2028L02.jpg');
Maskl(:,:,1,24)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2028L04.jpg');
Datel(:,:,2,24)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2028L04.jpg');
Maskl(:,:,2,24)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2028L06.jpg');
Datel(:,:,3,24)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2028L06.jpg');
Maskl(:,:,3,24)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2028L08.jpg');
Datel(:,:,4,24)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2028L08.jpg');
Maskl(:,:,4,24)=T1';

%%%%%%%class L25
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2029L02.jpg');
Datel(:,:,1,25)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2029L02.jpg');
Maskl(:,:,1,25)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2029L04.jpg');
Datel(:,:,2,25)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2029L04.jpg');
Maskl(:,:,2,25)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2029L06.jpg');
Datel(:,:,3,25)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2029L06.jpg');
Maskl(:,:,3,25)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2029L08.jpg');
Datel(:,:,4,25)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2029L08.jpg');
Maskl(:,:,4,25)=T1';

%%%%%%%%class L26
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2030L02.jpg');
Datel(:,:,1,26)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2030L02.jpg');
Maskl(:,:,1,26)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2030L04.jpg');
Datel(:,:,2,26)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2030L04.jpg');
Maskl(:,:,2,26)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2030L06.jpg');
Datel(:,:,3,26)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2030L06.jpg');
Maskl(:,:,3,26)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2030L08.jpg');
Datel(:,:,4,26)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2030L08.jpg');
Maskl(:,:,4,26)=T1';

%%%%%%%%class L27
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2031L02.jpg');
Datel(:,:,1,27)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2031L02.jpg');
Maskl(:,:,1,27)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2031L04.jpg');
Datel(:,:,2,27)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2031L04.jpg');
Maskl(:,:,2,27)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2031L06.jpg');
Datel(:,:,3,27)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2031L06.jpg');
Maskl(:,:,3,27)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2031L08.jpg');
Datel(:,:,4,27)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2031L08.jpg');
Maskl(:,:,4,27)=T1';

%%%%%%%%class L28
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2032L02.jpg');
Datel(:,:,1,28)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2032L02.jpg');
Maskl(:,:,1,28)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2032L04.jpg');
Datel(:,:,2,28)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2032L04.jpg');
Maskl(:,:,2,28)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2032L06.jpg');
Datel(:,:,3,28)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2032L06.jpg');
Maskl(:,:,3,28)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2032L08.jpg');
Datel(:,:,4,28)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2032L08.jpg');
Maskl(:,:,4,28)=T1';

%%%%%%%%%%%%%%%%%class L29
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2035L02.jpg');
Datel(:,:,1,29)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2035L02.jpg');
Maskl(:,:,1,29)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2035L04.jpg');
Datel(:,:,2,29)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2035L04.jpg');
Maskl(:,:,2,29)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2035L06.jpg');
Datel(:,:,3,29)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2035L06.jpg');
Maskl(:,:,3,29)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2035L08.jpg');
Datel(:,:,4,29)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2035L08.jpg');
Maskl(:,:,4,29)=T1';

%%%%%%%%class L30
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2036L02.jpg');
Datel(:,:,1,30)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2036L02.jpg');
Maskl(:,:,1,30)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2036L04.jpg');
Datel(:,:,2,30)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2036L04.jpg');
Maskl(:,:,2,30)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2036L06.jpg');
Datel(:,:,3,30)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2036L06.jpg');
Maskl(:,:,3,30)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2036L08.jpg');
Datel(:,:,4,30)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2036L08.jpg');
Maskl(:,:,4,30)=T1';

%%%%%%class L31
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2037L02.jpg');
Datel(:,:,1,31)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2037L02.jpg');
Maskl(:,:,1,31)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2037L04.jpg');
Datel(:,:,2,31)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2037L04.jpg');
Maskl(:,:,2,31)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2037L06.jpg');
Datel(:,:,3,31)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2037L06.jpg');
Maskl(:,:,3,31)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2037L08.jpg');
Datel(:,:,4,31)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2037L08.jpg');
Maskl(:,:,4,31)=T1';

%%%%%%%%%class L32
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2038L02.jpg');
Datel(:,:,1,32)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2038L02.jpg');
Maskl(:,:,1,32)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2038L04.jpg');
Datel(:,:,2,32)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2038L04.jpg');
Maskl(:,:,2,32)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2038L06.jpg');
Datel(:,:,3,32)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2038L06.jpg');
Maskl(:,:,3,32)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2038L08.jpg');
Datel(:,:,4,32)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2038L08.jpg');
Maskl(:,:,4,32)=T1';

%%%%%class L33
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2039L02.jpg');
Datel(:,:,1,33)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2039L02.jpg');
Maskl(:,:,1,33)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2039L04.jpg');
Datel(:,:,2,33)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2039L04.jpg');
Maskl(:,:,2,33)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2039L06.jpg');
Datel(:,:,3,33)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2039L06.jpg');
Maskl(:,:,3,33)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2039L08.jpg');
Datel(:,:,4,33)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2039L08.jpg');
Maskl(:,:,4,33)=T1';

%%%%%class L34
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2040L02.jpg');
Datel(:,:,1,34)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2040L02.jpg');
Maskl(:,:,1,34)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2040L04.jpg');
Datel(:,:,2,34)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2040L04.jpg');
Maskl(:,:,2,34)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2040L06.jpg');
Datel(:,:,3,34)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2040L06.jpg');
Maskl(:,:,3,34)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2040L08.jpg');
Datel(:,:,4,34)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2040L08.jpg');
Maskl(:,:,4,34)=T1';

%%%%%%class L35
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2041L02.jpg');
Datel(:,:,1,35)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2041L02.jpg');
Maskl(:,:,1,35)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2041L04.jpg');
Datel(:,:,2,35)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2041L04.jpg');
Maskl(:,:,2,35)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2041L06.jpg');
Datel(:,:,3,35)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2041L06.jpg');
Maskl(:,:,3,35)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2041L08.jpg');
Datel(:,:,4,35)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2041L08.jpg');
Maskl(:,:,4,35)=T1';

%%%%%%%class L36
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2044L02.jpg');
Datel(:,:,1,36)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2044L02.jpg');
Maskl(:,:,1,36)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2044L04.jpg');
Datel(:,:,2,36)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2044L04.jpg');
Maskl(:,:,2,36)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2044L06.jpg');
Datel(:,:,3,36)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2044L06.jpg');
Maskl(:,:,3,36)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2044L08.jpg');
Datel(:,:,4,36)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2044L08.jpg');
Maskl(:,:,4,36)=T1';

%%%%%%%%class L37
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2045L02.jpg');
Datel(:,:,1,37)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2045L02.jpg');
Maskl(:,:,1,37)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2045L04.jpg');
Datel(:,:,2,37)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2045L04.jpg');
Maskl(:,:,2,37)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2045L06.jpg');
Datel(:,:,3,37)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2045L06.jpg');
Maskl(:,:,3,37)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2045L08.jpg');
Datel(:,:,4,37)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2045L08.jpg');
Maskl(:,:,4,37)=T1';

%%%%%%%%%class L38
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2046L02.jpg');
Datel(:,:,1,38)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2046L02.jpg');
Maskl(:,:,1,38)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2046L04.jpg');
Datel(:,:,2,38)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2046L04.jpg');
Maskl(:,:,2,38)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2046L06.jpg');
Datel(:,:,3,38)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2046L06.jpg');
Maskl(:,:,3,38)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2046L08.jpg');
Datel(:,:,4,38)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2046L08.jpg');
Maskl(:,:,4,38)=T1';

%%%%%%%%class L39
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2047L02.jpg');
Datel(:,:,1,39)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2047L02.jpg');
Maskl(:,:,1,39)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2047L04.jpg');
Datel(:,:,2,39)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2047L04.jpg');
Maskl(:,:,2,39)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2047L06.jpg');
Datel(:,:,3,39)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2047L06.jpg');
Maskl(:,:,3,39)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2047L08.jpg');
Datel(:,:,4,39)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2047L08.jpg');
Maskl(:,:,4,39)=T1';

%%%%%class L40
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2048L02.jpg');
Datel(:,:,1,40)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2048L02.jpg');
Maskl(:,:,1,40)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2048L04.jpg');
Datel(:,:,2,40)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2048L04.jpg');
Maskl(:,:,2,40)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2048L06.jpg');
Datel(:,:,3,40)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2048L06.jpg');
Maskl(:,:,3,40)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2048L08.jpg');
Datel(:,:,4,40)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2048L08.jpg');
Maskl(:,:,4,40)=T1';

%%%%%%%%class L41
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2049L02.jpg');
Datel(:,:,1,41)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2049L02.jpg');
Maskl(:,:,1,41)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2049L04.jpg');
Datel(:,:,2,41)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2049L04.jpg');
Maskl(:,:,2,41)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2049L06.jpg');
Datel(:,:,3,41)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2049L06.jpg');
Maskl(:,:,3,41)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2049L08.jpg');
Datel(:,:,4,41)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2049L08.jpg');
Maskl(:,:,4,41)=T1';

%%%%%%class L42
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2050L02.jpg');
Datel(:,:,1,42)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2050L02.jpg');
Maskl(:,:,1,42)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2050L04.jpg');
Datel(:,:,2,42)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2050L04.jpg');
Maskl(:,:,2,42)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2050L06.jpg');
Datel(:,:,3,42)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2050L06.jpg');
Maskl(:,:,3,42)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2050L08.jpg');
Datel(:,:,4,42)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2050L08.jpg');
Maskl(:,:,4,42)=T1';

%%%%%%%%class L43
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2053L02.jpg');
Datel(:,:,1,43)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2053L02.jpg');
Maskl(:,:,1,43)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2053L04.jpg');
Datel(:,:,2,43)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2053L04.jpg');
Maskl(:,:,2,43)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2053L06.jpg');
Datel(:,:,3,43)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2053L06.jpg');
Maskl(:,:,3,43)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2053L08.jpg');
Datel(:,:,4,43)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2053L08.jpg');
Maskl(:,:,4,43)=T1';

%%%%%%%class L44
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2054L02.jpg');
Datel(:,:,1,44)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2054L02.jpg');
Maskl(:,:,1,44)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2054L04.jpg');
Datel(:,:,2,44)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2054L04.jpg');
Maskl(:,:,2,44)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2054L06.jpg');
Datel(:,:,3,44)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2054L06.jpg');
Maskl(:,:,3,44)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2054L08.jpg');
Datel(:,:,4,44)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2054L08.jpg');
Maskl(:,:,4,44)=T1';

%%%%%%%%class L45
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2055L02.jpg');
Datel(:,:,1,45)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2055L02.jpg');
Maskl(:,:,1,45)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2055L04.jpg');
Datel(:,:,2,45)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2055L04.jpg');
Maskl(:,:,2,45)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2055L06.jpg');
Datel(:,:,3,45)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2055L06.jpg');
Maskl(:,:,3,45)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2055L08.jpg');
Datel(:,:,4,45)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2055L08.jpg');
Maskl(:,:,4,45)=T1';

%%%%%%%%%class L46
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2056L02.jpg');
Datel(:,:,1,46)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2056L02.jpg');
Maskl(:,:,1,46)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2056L04.jpg');
Datel(:,:,2,46)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2056L04.jpg');
Maskl(:,:,2,46)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2056L06.jpg');
Datel(:,:,3,46)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2056L06.jpg');
Maskl(:,:,3,46)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2056L08.jpg');
Datel(:,:,4,46)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2056L08.jpg');
Maskl(:,:,4,46)=T1';

%%%%%%%%class L47
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2057L02.jpg');
Datel(:,:,1,47)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2057L02.jpg');
Maskl(:,:,1,47)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2057L04.jpg');
Datel(:,:,2,47)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2057L04.jpg');
Maskl(:,:,2,47)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2057L06.jpg');
Datel(:,:,3,47)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2057L06.jpg');
Maskl(:,:,3,47)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2057L08.jpg');
Datel(:,:,4,47)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2057L08.jpg');
Maskl(:,:,4,47)=T1';

%%%%%%%class L48
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2058L02.jpg');
Datel(:,:,1,48)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2058L02.jpg');
Maskl(:,:,1,48)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2058L04.jpg');
Datel(:,:,2,48)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2058L04.jpg');
Maskl(:,:,2,48)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2058L06.jpg');
Datel(:,:,3,48)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2058L06.jpg');
Maskl(:,:,3,48)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2058L08.jpg');
Datel(:,:,4,48)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2058L08.jpg');
Maskl(:,:,4,48)=T1';

%%%%%%%%%%class L49
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2059L02.jpg');
Datel(:,:,1,49)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2059L02.jpg');
Maskl(:,:,1,49)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2059L04.jpg');
Datel(:,:,2,49)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2059L04.jpg');
Maskl(:,:,2,49)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2059L06.jpg');
Datel(:,:,3,49)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2059L06.jpg');
Maskl(:,:,3,49)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2059L08.jpg');
Datel(:,:,4,49)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2059L08.jpg');
Maskl(:,:,4,49)=T1';

%%%%%%class L50
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2062L02.jpg');
Datel(:,:,1,50)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2062L02.jpg');
Maskl(:,:,1,50)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2062L04.jpg');
Datel(:,:,2,50)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2062L04.jpg');
Maskl(:,:,2,50)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2062L06.jpg');
Datel(:,:,3,50)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2062L06.jpg');
Maskl(:,:,3,50)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2062L08.jpg');
Datel(:,:,4,50)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2062L08.jpg');
Maskl(:,:,4,50)=T1';

%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%class R51
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2001R01.jpg');
Datel(:,:,1,51)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2001R01.jpg');
Maskl(:,:,1,51)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2001R04.jpg');
Datel(:,:,2,51)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2001R04.jpg');
Maskl(:,:,2,51)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2001R06.jpg');
Datel(:,:,3,51)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2001R06.jpg');
Maskl(:,:,3,51)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2001R08.jpg');
Datel(:,:,4,51)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2001R08.jpg');
Maskl(:,:,4,51)=T1';

%%%%%%%%%%%%%class R52
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2002R01.jpg');
Datel(:,:,1,52)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2002R01.jpg');
Maskl(:,:,1,52)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2002R04.jpg');
Datel(:,:,2,52)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2002R04.jpg');
Maskl(:,:,2,52)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2002R06.jpg');
Datel(:,:,3,52)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2002R06.jpg');
Maskl(:,:,3,52)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2002R08.jpg');
Datel(:,:,4,52)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2002R08.jpg');
Maskl(:,:,4,52)=T1';

%%%%%%%%%class R53
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2003R01.jpg');
Datel(:,:,1,53)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2003R01.jpg');
Maskl(:,:,1,53)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2003R04.jpg');
Datel(:,:,2,53)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2003R04.jpg');
Maskl(:,:,2,53)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2003R06.jpg');
Datel(:,:,3,53)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2003R06.jpg');
Maskl(:,:,3,53)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2003R08.jpg');
Datel(:,:,4,53)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2003R08.jpg');
Maskl(:,:,4,53)=T1';

%%%%%%%%%%class R54
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2004R01.jpg');
Datel(:,:,1,54)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2004R01.jpg');
Maskl(:,:,1,54)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2004R04.jpg');
Datel(:,:,2,54)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2004R04.jpg');
Maskl(:,:,2,54)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2004R06.jpg');
Datel(:,:,3,54)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2004R06.jpg');
Maskl(:,:,3,54)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2004R08.jpg');
Datel(:,:,4,54)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2004R08.jpg');
Maskl(:,:,4,54)=T1';

%%%%%%%%%%%class R55
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2005R01.jpg');
Datel(:,:,1,55)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2005R01.jpg');
Maskl(:,:,1,55)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2005R04.jpg');
Datel(:,:,2,55)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2005R04.jpg');
Maskl(:,:,2,55)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2005R06.jpg');
Datel(:,:,3,55)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2005R06.jpg');
Maskl(:,:,3,55)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2005R08.jpg');
Datel(:,:,4,55)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2005R08.jpg');
Maskl(:,:,4,55)=T1';

%%%%%%%%%%class R56
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2010R01.jpg');
Datel(:,:,1,56)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2010R01.jpg');
Maskl(:,:,1,56)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2010R04.jpg');
Datel(:,:,2,56)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2010R04.jpg');
Maskl(:,:,2,56)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2010R06.jpg');
Datel(:,:,3,56)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2010R06.jpg');
Maskl(:,:,3,56)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2010R08.jpg');
Datel(:,:,4,56)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2010R08.jpg');
Maskl(:,:,4,56)=T1';

%%%%%%%%class R57
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2011R01.jpg');
Datel(:,:,1,57)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2011R01.jpg');
Maskl(:,:,1,57)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2011R04.jpg');
Datel(:,:,2,57)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2011R04.jpg');
Maskl(:,:,2,57)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2011R06.jpg');
Datel(:,:,3,57)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2011R06.jpg');
Maskl(:,:,3,57)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2011R08.jpg');
Datel(:,:,4,57)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2011R08.jpg');
Maskl(:,:,4,57)=T1';

%%%%%%class R58
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2012R01.jpg');
Datel(:,:,1,58)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2012R01.jpg');
Maskl(:,:,1,58)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2012R04.jpg');
Datel(:,:,2,58)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2012R04.jpg');
Maskl(:,:,2,58)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2012R06.jpg');
Datel(:,:,3,58)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2012R06.jpg');
Maskl(:,:,3,58)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2012R08.jpg');
Datel(:,:,4,58)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2012R08.jpg');
Maskl(:,:,4,58)=T1';

%%%%%%%class R59
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2013R01.jpg');
Datel(:,:,1,59)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2013R01.jpg');
Maskl(:,:,1,59)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2013R04.jpg');
Datel(:,:,2,59)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2013R04.jpg');
Maskl(:,:,2,59)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2013R06.jpg');
Datel(:,:,3,59)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2013R06.jpg');
Maskl(:,:,3,59)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2013R08.jpg');
Datel(:,:,4,59)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2013R08.jpg');
Maskl(:,:,4,59)=T1';

%%%%%%%%%%%%%%%%%%%%class R60
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2014R01.jpg');
Datel(:,:,1,60)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2014R01.jpg');
Maskl(:,:,1,60)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2014R04.jpg');
Datel(:,:,2,60)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2014R04.jpg');
Maskl(:,:,2,60)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2014R06.jpg');
Datel(:,:,3,60)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2014R06.jpg');
Maskl(:,:,3,60)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2014R08.jpg');
Datel(:,:,4,60)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2014R08.jpg');
Maskl(:,:,4,60)=T1';

%%%%%%%%%%class R61
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2019R01.jpg');
Datel(:,:,1,61)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2019R01.jpg');
Maskl(:,:,1,61)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2019R04.jpg');
Datel(:,:,2,61)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2019R04.jpg');
Maskl(:,:,2,61)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2019R06.jpg');
Datel(:,:,3,61)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2019R06.jpg');
Maskl(:,:,3,61)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2019R08.jpg');
Datel(:,:,4,61)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2019R08.jpg');
Maskl(:,:,4,61)=T1';

%%%%%%%%%%%%class R62
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2020R01.jpg');
Datel(:,:,1,62)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2020R01.jpg');
Maskl(:,:,1,62)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2020R04.jpg');
Datel(:,:,2,62)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2020R04.jpg');
Maskl(:,:,2,62)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2020R06.jpg');
Datel(:,:,3,62)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2020R06.jpg');
Maskl(:,:,3,62)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2020R08.jpg');
Datel(:,:,4,62)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2020R08.jpg');
Maskl(:,:,4,62)=T1';

%%%%%%%%class R63
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2021R01.jpg');
Datel(:,:,1,63)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2021R01.jpg');
Maskl(:,:,1,63)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2021R04.jpg');
Datel(:,:,2,63)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2021R04.jpg');
Maskl(:,:,2,63)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2021R06.jpg');
Datel(:,:,3,63)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2021R06.jpg');
Maskl(:,:,3,63)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2021R08.jpg');
Datel(:,:,4,63)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2021R08.jpg');
Maskl(:,:,4,63)=T1';

%%%%%%%%%%class R64
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2022R01.jpg');
Datel(:,:,1,64)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2022R01.jpg');
Maskl(:,:,1,64)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2022R04.jpg');
Datel(:,:,2,64)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2022R04.jpg');
Maskl(:,:,2,64)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2022R06.jpg');
Datel(:,:,3,64)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2022R06.jpg');
Maskl(:,:,3,64)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2022R08.jpg');
Datel(:,:,4,64)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2022R08.jpg');
Maskl(:,:,4,64)=T1';

%%%%%%%%class R65
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2023R01.jpg');
Datel(:,:,1,65)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2023R01.jpg');
Maskl(:,:,1,65)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2023R04.jpg');
Datel(:,:,2,65)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2023R04.jpg');
Maskl(:,:,2,65)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2023R06.jpg');
Datel(:,:,3,65)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2023R06.jpg');
Maskl(:,:,3,65)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2023R08.jpg');
Datel(:,:,4,65)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2023R08.jpg');
Maskl(:,:,4,65)=T1';

%%%%%%%class R66
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2029R01.jpg');
Datel(:,:,1,66)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2029R01.jpg');
Maskl(:,:,1,66)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2029R04.jpg');
Datel(:,:,2,66)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2029R04.jpg');
Maskl(:,:,2,66)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2029R06.jpg');
Datel(:,:,3,66)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2029R06.jpg');
Maskl(:,:,3,66)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2029R08.jpg');
Datel(:,:,4,66)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2029R08.jpg');
Maskl(:,:,4,66)=T1';

%%%%%%%%class R67
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2030R01.jpg');
Datel(:,:,1,67)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2030R01.jpg');
Maskl(:,:,1,67)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2030R04.jpg');
Datel(:,:,2,67)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2030R04.jpg');
Maskl(:,:,2,67)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2030R06.jpg');
Datel(:,:,3,67)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2030R06.jpg');
Maskl(:,:,3,67)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2030R08.jpg');
Datel(:,:,4,67)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2030R08.jpg');
Maskl(:,:,4,67)=T1';

%%%%%%%class R68
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2031R01.jpg');
Datel(:,:,1,68)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2031R01.jpg');
Maskl(:,:,1,68)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2031R04.jpg');
Datel(:,:,2,68)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2031R04.jpg');
Maskl(:,:,2,68)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2031R06.jpg');
Datel(:,:,3,68)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2031R06.jpg');
Maskl(:,:,3,68)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2031R08.jpg');
Datel(:,:,4,68)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2031R08.jpg');
Maskl(:,:,4,68)=T1';

%%%%%%class R69
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2032R01.jpg');
Datel(:,:,1,69)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2032R01.jpg');
Maskl(:,:,1,69)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2032R04.jpg');
Datel(:,:,2,69)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2032R04.jpg');
Maskl(:,:,2,69)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2032R06.jpg');
Datel(:,:,3,69)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2032R06.jpg');
Maskl(:,:,3,69)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2032R08.jpg');
Datel(:,:,4,69)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2032R08.jpg');
Maskl(:,:,4,69)=T1';

%%%%%%%%class R70
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2037R01.jpg');
Datel(:,:,1,70)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2037R01.jpg');
Maskl(:,:,1,70)=T1';
 
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2037R04.jpg');
Datel(:,:,2,70)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2037R04.jpg');
Maskl(:,:,2,70)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2037R06.jpg');
Datel(:,:,3,70)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2037R06.jpg');
Maskl(:,:,3,70)=T1';
 
T1=imread('F:\matwork\20110729\Irisnormalize_pic\S2037R08.jpg');
Datel(:,:,4,70)=T1';
T1=imread('F:\matwork\20110729\IRisnormalize_mask\S2037R08.jpg');
Maskl(:,:,4,70)=T1';




%%%%%%定序编码
tic
for c=1:Classnum
    for i=1:snuml
        Juanl(:,:,i,c)=bianma4(Datel(:,:,i,c));
    end
end   
toc

clear Datel;
%%%%%%%%hamming distance of the same classL
%%%%同类的汉明距离
intradistl=zeros(snuml,snuml,Classnum);

tic
for c=1:Classnum
    intradistl(:,:,c)=Distance1(Juanl(:,:,:,c),Maskl(:,:,:,c));
end
           
toc

%%%%%%%%%%%%%%%%%hamming distance of different classL
%%%%%%%%不同类的汉明距离
zhuhe=Classnum*(Classnum-1)/2;
interdistl=zeros(snuml,snuml,zhuhe);

tic
k=0;
for c1=1:(Classnum-1)
    for c2=(c1+1):Classnum
        k=k+1;
        interdistl(:,:,k)=Distance2(Juanl(:,:,:,c1),Maskl(:,:,:,c1),Juanl(:,:,:,c2),Maskl(:,:,:,c2));
    end
end
toc



%%%%%%%%%%%%%%%%%%%%%%%%%统计均值和方差
%%%%%%the same class
total1=sum(sum(sum(intradistl)));

temp1=intradistl&1;
num1=sum(sum(sum(temp1)));

mean1=total1/num1

temp2=intradistl-temp1*mean1;
temp3=temp2.*temp2;
total2=sum(sum(sum(temp3)));
sigma1=total2/num1

%%%%different class
total3=sum(sum(sum(interdistl)));

temp6=interdistl&1;
num2=sum(sum(sum(temp6)));

mean2=total3/num2

temp4=interdistl-temp6*mean2;
temp5=temp4.*temp4;
total4=sum(sum(sum(temp5)));
sigma2=total4/num2
  
%%%计算DI
t1=abs(mean1-mean2);
t2=sqrt(0.5*sigma1+0.5*sigma2);
DI=t1/t2

%%%%%%计算ROC曲线

tic
stodist1=[];
for i=1:snuml
    for j=1:snuml
        for k=1:zhuhe
            if interdistl(i,j,k)>0
                stodist1=[stodist1 interdistl(i,j,k)];
            end
        end
    end
end
toc

[stodist2 shunxu]=sort(stodist1);

tic
stodist3=[];
for i=1:snuml
    for j=1:snuml
        for k=1:Classnum
            if intradistl(i,j,k)>0
                stodist3=[stodist3 intradistl(i,j,k)];
            end
        end
    end
end
toc
[stodist4 shunxu]=sort(stodist3);

FAR=zeros(1,5);
FRR=zeros(1,5);

for i=1:5
    FAR(i)=0.1^i;
    t=floor(num2*FAR(i));
    thresh=(stodist2(t)+stodist2(t+1))/2;
    m=1;
    while stodist4(m)<thresh
          m=m+1;
    end

    FRR(i)=(num1-m+1)/num1;
end








