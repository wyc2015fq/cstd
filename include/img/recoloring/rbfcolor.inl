// ***************************************
// **	Color Transfer using variant RBF & GMM.
// **	param: 
// **  	RBF_param_coff: influction parameter in gaussian function of RBF.
// **	Require <script src="js/numeric-1.2.6.js"></script>
// ****************************************
double RBF_param_coff = 5;

//#include "numeric.inl"

/************************
* inverse A
*************************/

#include "cv1/matrix.inl"

int pinv(const double* A, int m, int n, double* B)
{
#if 0
   double* U, *S, *V;
   void* buf;
   if(buf = svd_(A, m, n, &U, &S, &V)) {
     double foo = S[0];
     double tol = MAX(m,n)*epsilon*foo;
     int i, j;
     double* Sinv = MALLOC(double, n);
     for (i=0;i<n*n; ++i) {Sinv[i] = 0;}
     for (i=n-1; i!=-1; i--) {
       if(S[i]>tol) Sinv[i] = 1/S[i];
     }
     for (i=0;i<n; ++i) {
       const double* v1 = V + i*n;
       for (j=0;j<n; ++j) {
         v1[j]*=Sinv[j];
       }
     }
     dotMMsmall(V, n, 0, U, n, 1, n, );
     //numeric.dot(numeric.dot(V,numeric.diag(Sinv)),numeric.transpose(U));
     FREE(buf);
   }
#endif
   cvInvert_64f(m, n, A, n, B, n, CC_SVD);
   return 0;
}

#if 0

/************************
* solve Equation Ax=b
*************************/
function solveEquation(A,b)
{
	var z = numeric.svd(A), foo = z.S[0];
	var U = z.U, S = z.S, V = z.V;

	var m = A.length, n = A[0].length, tol = MAX(m,n)*numeric.epsilon*foo,M = S.length;
	var i,Sinv = new Array(M);
	for (i=M-1;i!==-1;i--) { if(S[i]>tol) Sinv[i] = 1/S[i]; else Sinv[i] = 0; }
	return numeric.dotMV( numeric.dot(numeric.dot(V,numeric.diag(Sinv)),numeric.transpose(U)), b );
}
#endif

// RGB->XYZ->LAB from easyRGB.com 
void RGB2LAB(const uchar* Q, int rindex, double* Lab)
{
	//alert("R2L "+Q);
	double var_R = ( Q[0 ^ rindex] / 255. );        //R from 0 to 255
	double var_G = ( Q[1] / 255. );        //G from 0 to 255
	double var_B = ( Q[2 ^ rindex] / 255. );        //B from 0 to 255
	

	if ( var_R > 0.04045 ) var_R = pow( ( var_R + 0.055 ) / 1.055, 2.4 );
	else                   var_R = var_R / 12.92;
	
	if ( var_G > 0.04045 ) var_G = pow( ( var_G + 0.055 ) / 1.055, 2.4 );
	else                   var_G = var_G / 12.92;
	
	if ( var_B > 0.04045 ) var_B = pow( ( var_B + 0.055 ) / 1.055, 2.4 );
	else                   var_B = var_B / 12.92;

	var_R = var_R * 100;
	var_G = var_G * 100;
	var_B = var_B * 100;

	//Observer. = 2°, Illuminant = D65
	double X = var_R * 0.4124 + var_G * 0.3576 + var_B * 0.1805;
	double Y = var_R * 0.2126 + var_G * 0.7152 + var_B * 0.0722;
	double Z = var_R * 0.0193 + var_G * 0.1192 + var_B * 0.9505;
	
	double var_X = X / 95.047; 
	double var_Y = Y / 100; 
	double var_Z = Z / 108.883;

	if ( var_X > 0.008856 ) var_X = pow(var_X, 1./3 );
	else                    var_X = ( 7.787 * var_X ) + ( 16. / 116. );
	
	if ( var_Y > 0.008856 ) var_Y = pow(var_Y, 1./3 );
	else                    var_Y = ( 7.787 * var_Y ) + ( 16. / 116. );
	
	if ( var_Z > 0.008856 ) var_Z = pow(var_Z, 1./3 );
	else                    var_Z = ( 7.787 * var_Z ) + ( 16. / 116. );

	Lab[0] = ( 116 * var_Y ) - 16; // L
	Lab[1] = 500 * ( var_X - var_Y );// A
	Lab[2] = 200 * ( var_Y - var_Z );//B
	
	//alert("Answer "+[L,A,B]);
	//return [L,A,B];
}

void RGB2LABd(const double* Q, int rindex, double* Lab)
{
  //alert("R2L "+Q);
  double var_R = ( Q[0 ^ rindex] );        //R from 0 to 255
  double var_G = ( Q[1] );        //G from 0 to 255
  double var_B = ( Q[2 ^ rindex] );        //B from 0 to 255
  
  
  if ( var_R > 0.04045 ) var_R = pow( ( var_R + 0.055 ) / 1.055, 2.4 );
  else                   var_R = var_R / 12.92;
  
  if ( var_G > 0.04045 ) var_G = pow( ( var_G + 0.055 ) / 1.055, 2.4 );
  else                   var_G = var_G / 12.92;
  
  if ( var_B > 0.04045 ) var_B = pow( ( var_B + 0.055 ) / 1.055, 2.4 );
  else                   var_B = var_B / 12.92;
  
  var_R = var_R * 100;
  var_G = var_G * 100;
  var_B = var_B * 100;
  
  //Observer. = 2°, Illuminant = D65
  double X = var_R * 0.4124 + var_G * 0.3576 + var_B * 0.1805;
  double Y = var_R * 0.2126 + var_G * 0.7152 + var_B * 0.0722;
  double Z = var_R * 0.0193 + var_G * 0.1192 + var_B * 0.9505;
  
  double var_X = X / 95.047; 
  double var_Y = Y / 100; 
  double var_Z = Z / 108.883;
  
  if ( var_X > 0.008856 ) var_X = pow(var_X, 1./3 );
  else                    var_X = ( 7.787 * var_X ) + ( 16. / 116. );
  
  if ( var_Y > 0.008856 ) var_Y = pow(var_Y, 1./3 );
  else                    var_Y = ( 7.787 * var_Y ) + ( 16. / 116. );
  
  if ( var_Z > 0.008856 ) var_Z = pow(var_Z, 1./3 );
  else                    var_Z = ( 7.787 * var_Z ) + ( 16. / 116. );
  
  Lab[0] = ( 116 * var_Y ) - 16; // L
  Lab[1] = 500 * ( var_X - var_Y );// A
  Lab[2] = 200 * ( var_Y - var_Z );//B
  
  //alert("Answer "+[L,A,B]);
  //return [L,A,B];
}
// LAB->XYZ->RGB from easyRGB.com
void LAB2RGBd(const double* Q, int rindex, double* rgb) {
	//alert("L2R "+Q);
	double var_Y = ( Q[0] + 16. ) / 116.;//L
	double var_X = Q[1] / 500. + var_Y;//A
	double var_Z = var_Y - Q[2] / 200.;//B

	if ( var_Y > 0.206893034422 ) var_Y = pow(var_Y,3.);
	else                      var_Y = ( var_Y - 16. / 116. ) / 7.787;
	if ( var_X > 0.206893034422 ) var_X = pow(var_X,3.);
	else                      var_X = ( var_X - 16. / 116. ) / 7.787;
	if ( var_Z > 0.206893034422 ) var_Z = pow(var_Z,3.);
	else                      var_Z = ( var_Z - 16. / 116. ) / 7.787;

	double X = 95.047 * var_X;
	double Y = 100 * var_Y;
	double Z = 108.883 * var_Z;
	
	var_X = X / 100.;
	var_Y = Y / 100.;
	var_Z = Z / 100.;

	double var_R = var_X *  3.2406 + var_Y * -1.5372 + var_Z * -0.4986;
	double var_G = var_X * -0.9689 + var_Y *  1.8758 + var_Z *  0.0415;
	double var_B = var_X *  0.0557 + var_Y * -0.2040 + var_Z *  1.0570;

	if ( var_R > 0.0031308 ) var_R = 1.055 * pow(var_R,1. / 2.4) - 0.055;
	else                     var_R = 12.92 * var_R;
	if ( var_G > 0.0031308 ) var_G = 1.055 * pow(var_G,1. / 2.4) - 0.055;
	else                     var_G = 12.92 * var_G;
	if ( var_B > 0.0031308 ) var_B = 1.055 * pow(var_B,1. / 2.4) - 0.055;
	else                     var_B = 12.92 * var_B;

	rgb[0 ^ rindex] = var_R * 255.;
	rgb[1] = var_G * 255.;
	rgb[2 ^ rindex] = var_B * 255.;
	//alert("Answer "+[R,G,B]);
	//return [R,G,B];
}

void LAB2RGB(const double* Q, int rindex, uchar* rgb)
{
  double rgb_d[3];
  LAB2RGBd(Q, rindex, rgb_d);
  rgb[0] = BOUND(rgb_d[0], 0, 255);
  rgb[1] = BOUND(rgb_d[1], 0, 255);
  rgb[2] = BOUND(rgb_d[2], 0, 255);
}

/************************************************
* main function: rbfcolor
* param: 
*	image_name: image name
*	maxsize: 	MAX height/width of the processing image 
*	controlmap: used in the gallery version
*	callback: 	callback function
*	ngrid: 		grid size, n*n*n, default = 10 
*************************************************/

// calculate the grid index and weight for and RGB color
void calculate_singlegrid(double ori_r,double ori_g,double ori_b, int ngrid, int* index, double* map)
{
  int ntmp = ngrid + 1;
  int ntmpsqr = ntmp*ntmp;
  int corner_ind;
  double diff_x, diff_y,diff_z;
  double tmpx, tmpy, tmpz;	
		
  tmpx = ori_r * ngrid / 255.;
  diff_x = tmpx - floor(tmpx);
  tmpx = floor(tmpx);
  if (tmpx == ngrid){
    tmpx = ngrid - 1;
    diff_x = 1;
  }
  tmpy = ori_g *ngrid / 255.;
  diff_y = tmpy - floor(tmpy);
  tmpy = floor(tmpy);
  if (tmpy == ngrid){
    tmpy = ngrid - 1;
    diff_y = 1;
  }
  tmpz = ori_b *ngrid / 255.;
  diff_z = tmpz - floor(tmpz);
  tmpz = floor(tmpz);
  if (tmpz == ngrid){
    tmpz = ngrid - 1;
    diff_z = 1;
  }
  
  corner_ind = (int)(tmpz * ntmpsqr + tmpy * ntmp +tmpx);

  index[0] = corner_ind;
  index[1] = corner_ind+ntmpsqr;
  index[2] = corner_ind+ntmp;
  index[3] = corner_ind+ntmp+ntmpsqr;
  index[4] = corner_ind+1;
  index[5] = corner_ind+ntmpsqr+1;
  index[6] = corner_ind+ntmp+1;
  index[7] = corner_ind+ntmp+ntmpsqr+1;
  
  map[0] = (1-diff_x)*(1-diff_y)*(1-diff_z);
  map[1] = (1-diff_x)*(1-diff_y)*diff_z;
  map[2] = (1-diff_x)*diff_y*(1-diff_z);
  map[3] = (1-diff_x)*diff_y*diff_z;
  map[4] = diff_x*(1-diff_y)*(1-diff_z);
  map[5] = diff_x*(1-diff_y)*diff_z;
  map[6] = diff_x*diff_y*(1-diff_z);
  map[7] = diff_x*diff_y*diff_z;
}

#if 0

this.updateMask(image_name)
{
	var imageObj = new Image();
	var ptr = this;

	imageObj.onload(){
		// paint on a virtual canvas
		var img_width, img_height;

		img_width = s->img_width;
		img_height = s->img_height;

		var newCanvas = $("<canvas>")[0];
		newCanvas.width = img_width;
		newCanvas.height = img_height;
	
		var newContext = newCanvas.getContext("2d");
		newContext.width = img_width;
		newContext.height = img_height;
	
	      newContext.drawImage(imageObj, 0,0,img_width,img_height);
	
	      var imgData = newContext.getImageData(0,0,img_width,img_height);

	      var cnt = 0;
	      for (var i=0; i<imgData.data.length; i+=4)
	      {
			//alert([imgData.data[i],imgData.data[i+1] , imgData.data[i+2],imgData.data[i+3]])
	          s->mask[cnt] = MAX( MAX( imgData.data[i],imgData.data[i+1] ), imgData.data[i+2] ) / 255;
	          
			//alert(s->mask[cnt]);
	          cnt = cnt + 1;
	      }
	
		alert("mask loaded");
	};
	imageObj.src = image_name;
}
	

	this.drawImage(output_canvas)
	{
		var output_context = output_canvas.getContext('2d');
        var output_imgData = output_context.createImageData(this.img_width, this.img_height);
        var cnt = 0;
        for (var i=0; i<output_imgData.data.length; i+=4)
        {
            output_imgData.data[i] = this.ori_r[cnt];
            output_imgData.data[i+1]= this.ori_g[cnt];
            output_imgData.data[i+2]= this.ori_b[cnt];
            output_imgData.data[i+3] = 255;
            cnt = cnt + 1;
        }
		var newCanvas = $("<canvas>")
        .attr("width", output_imgData.width)
        .attr("height", output_imgData.height)[0];
		
		newCanvas.getContext("2d").putImageData(output_imgData, 0, 0);
		
		output_context.scale(output_canvas.width / this.img_width,output_canvas.height / this.img_height);
		output_context.drawImage(newCanvas, 0, 0);
		output_context.scale(this.img_width / output_canvas.width , this.img_height / output_canvas.height );
	}
	
	//Kmeans
	this.calcKMeans(center_num)
	{
		var D = new Array(this.img_area);
		for( var i = 0;i < this.img_area;i++ )
		{
			D[i] = RGB2LAB([ this.ori_r[i], this.ori_g[i], this.ori_b[i] ]);
		}
		
		var Center = new Array(center_num);
		var cnt = new Array(center_num);
		var sumD = new Array(center_num);
		for (var i = 0;i < center_num;i++)
		{
			Center[i] = D[ floor(random() * this.img_area) ];
		}
		
		
		for( var iter=0;iter < 20;iter++)
		{
			for(var i=0;i < center_num;i++)
			{
				sumD[i] = [0,0,0];
				cnt[i] = 0;
			}
			
			for(var i = 0;i < this.img_area;i++)
			{
				var min_id = -1;
				var min_v = 1e100;
				for( var j = 0; j < center_num;j++ )
				{
					var r = (D[i][0]-Center[j][0])*(D[i][0]-Center[j][0])+
							(D[i][1]-Center[j][1])*(D[i][1]-Center[j][1])+
							(D[i][2]-Center[j][2])*(D[i][2]-Center[j][2]);
					if( r < min_v )
					{
						min_v = r;
						min_id = j;
					}		
				}
				
				cnt[min_id] = cnt[min_id] + 1;
				sumD[min_id][0] = sumD[min_id][0] + D[i][0];
				sumD[min_id][1] = sumD[min_id][1] + D[i][1];
				sumD[min_id][2] = sumD[min_id][2] + D[i][2];
			}
			
			
			for( var i = 0;i < center_num;i++ ) if(cnt[i]>0)
			{
				Center[i][0] = sumD[i][0] / cnt[i];
				Center[i][1] = sumD[i][1] / cnt[i];
				Center[i][2] = sumD[i][2] / cnt[i];
			}
		}
		
		var res = new Array(3*center_num);
		for(var i = 0;i < center_num;i++ )
		{
			var ps = LAB2RGB(Center[i]);
			res[i*3+0] = ps[0]/255;
			res[i*3+1] = ps[1]/255;
			res[i*3+2] = ps[2]/255;
		}
		
		return res;
	}
	
	this.grid_calcKMeans(center_num)
	{
		var D = new Array(this.grid_size);
		var Dw = new Array(this.grid_size);
		for(var i=0;i < this.grid_size;i++)
		{
			D[i] = this.grid_lab[i];
			Dw[i] = 0;
		}
		for( var i = 0;i < this.img_area;i++ )
		{
			for(var j =0;j<8;j++)
			{
				Dw[this.weightindex[i][j]] = Dw[this.weightindex[i][j]] + this.weightmap[i][j];
			}
		}
		
		var Center = new Array(center_num);
		var cnt = new Array(center_num);
		var sumD = new Array(center_num);
		for (var i = 0;i < center_num;i++)
		{
			Center[i] = D[ floor(random() * this.grid_size) ];
		}
		
		
		for( var iter=0;iter < 20;iter++)
		{
			for(var i=0;i < center_num;i++)
			{
				sumD[i] = [0,0,0];
				cnt[i] = 0;
			}
			
			for(var i = 0;i < this.grid_size;i++)
			{
				var min_id = -1;
				var min_v = 1e100;
				for( var j = 0; j < center_num;j++ )
				{
					var r = (D[i][0]-Center[j][0])*(D[i][0]-Center[j][0])+
							(D[i][1]-Center[j][1])*(D[i][1]-Center[j][1])+
							(D[i][2]-Center[j][2])*(D[i][2]-Center[j][2]);
					if( r < min_v )
					{
						min_v = r;
						min_id = j;
					}		
				}
				
				cnt[min_id] = cnt[min_id] + Dw[i];
				sumD[min_id][0] = sumD[min_id][0] + Dw[i]*D[i][0];
				sumD[min_id][1] = sumD[min_id][1] + Dw[i]*D[i][1];
				sumD[min_id][2] = sumD[min_id][2] + Dw[i]*D[i][2];
			}
			
			
			for( var i = 0;i < center_num;i++ ) if(cnt[i]>0)
			{
				Center[i][0] = sumD[i][0] / cnt[i];
				Center[i][1] = sumD[i][1] / cnt[i];
				Center[i][2] = sumD[i][2] / cnt[i];
			}
		}
		
		var res = new Array(3*center_num);
		for(var i = 0;i < center_num;i++ )
		{
			var ps = LAB2RGB(Center[i]);
			res[i*3+0] = ps[0]/255;
			res[i*3+1] = ps[1]/255;
			res[i*3+2] = ps[2]/255;
		}
		
		return res;
	}
	
	fast_calcKMeans(center_num, ptLimit)
	{
		seedrandom('hello.');
		if(ptLimit > this.img_area)
		{
			ptLimit = this.img_area;
		}
		
		
		var D = new Array(ptLimit);
		for( var i = 0;i < ptLimit;i++ )
		{
			var index = floor( i / (ptLimit-1) * (this.img_area-1)  );
			D[i] = RGB2LAB([ this.ori_r[index], this.ori_g[index], this.ori_b[index] ]);
		}
		
		var Center = new Array(center_num);
		var cnt = new Array(center_num);
		var sumD = new Array(center_num);
		for (var i = 0;i < center_num;i++)
		{
			Center[i] = D[ floor(random() * ptLimit) ];
		}
		
		
		for( var iter=0;iter < 20;iter++)
		{
			for(var i=0;i < center_num;i++)
			{
				sumD[i] = [0,0,0];
				cnt[i] = 0;
			}
			
			for(var i = 0;i < ptLimit;i++)
			{
				var min_id = -1;
				var min_v = 1e100;
				for( var j = 0; j < center_num;j++ )
				{
					var r = (D[i][0]-Center[j][0])*(D[i][0]-Center[j][0])+
							(D[i][1]-Center[j][1])*(D[i][1]-Center[j][1])+
							(D[i][2]-Center[j][2])*(D[i][2]-Center[j][2]);
					if( r < min_v )
					{
						min_v = r;
						min_id = j;
					}		
				}
				
				cnt[min_id] = cnt[min_id] + 1;
				sumD[min_id][0] = sumD[min_id][0] + D[i][0];
				sumD[min_id][1] = sumD[min_id][1] + D[i][1];
				sumD[min_id][2] = sumD[min_id][2] + D[i][2];
			}
			
			
			for( var i = 0;i < center_num;i++ ) if(cnt[i]>0)
			{
				Center[i][0] = sumD[i][0] / cnt[i];
				Center[i][1] = sumD[i][1] / cnt[i];
				Center[i][2] = sumD[i][2] / cnt[i];
			}
		}
		
		var res = new Array(3*center_num);
		for(var i = 0;i < center_num;i++ )
		{
			var ps = LAB2RGB(Center[i]);
			res[i*3+0] = MAX( min( 1.0,ps[0]/255 ) );
			res[i*3+1] = MAX( min( 1.0,ps[1]/255 ) );
			res[i*3+2] = MAX( min( 1.0,ps[2]/255 ) );
		}
		
		return res;
	}
	
	// adapted calculate Kmeans, now it doesn't work good.
	adapted_calcKMeans(grid_size)
	{
		//initialize
		var pt_size = (grid_size+1) * (grid_size+1) * (grid_size+1);
		var D = new Array(pt_size);
		var Dw = new Array(pt_size);
		
		for(var i=0;i < pt_size;i++) Dw[i] = 0;
		
		var step = 255.0 / grid_size;
		
		var tot = 0;
		for(var i=0;i<grid_size+1;i++)
		for(var j=0;j<grid_size+1;j++)
		for(var k=0;k<grid_size+1;k++)
		{
			D[tot] = RGB2LAB([k*step,j*step,i*step]);
		    tot = tot + 1;
		}

		for( var i = 0;i < this.img_area;i++ )
		{
			var ori_r = this.ori_r[i];
			var ori_g = this.ori_g[i];
			var ori_b = this.ori_b[i];
	        var ntmp = grid_size + 1;
	        var ntmpsqr = ntmp*ntmp;
	        var diff_x, diff_y,diff_z;
	        var corner_ind;
	        var tmpx, tmpy, tmpz;
		
		
	        tmpx = ori_r / 255 * grid_size;
	        diff_x = tmpx - floor(tmpx);
	        tmpx = floor(tmpx);
	        if (tmpx == grid_size){
	          tmpx = grid_size - 1;
	          diff_x = 1;
	         }
	   
	        tmpy = ori_g / 255 * grid_size;
	        diff_y = tmpy - floor(tmpy);
		  tmpy = floor(tmpy);
	        if (tmpy == grid_size){
	            tmpy = grid_size - 1;
	            diff_y = 1;
	        }
	  
	        tmpz = ori_b / 255 *grid_size;
	        diff_z = tmpz - floor(tmpz);
		    tmpz = floor(tmpz);
	        if (tmpz == grid_size){
	            tmpz = grid_size - 1;
	            diff_z = 1;
	        }
   
	        corner_ind = tmpz * ntmpsqr + tmpy * ntmp +tmpx;

			var res = new Array(16);
		
			res[0] = corner_ind;
	        res[1] = corner_ind+ntmpsqr;
	        res[2] = corner_ind+ntmp;
	        res[3] = corner_ind+ntmp+ntmpsqr;
	        res[4] = corner_ind+1;
	        res[5] = corner_ind+ntmpsqr+1;
	        res[6] = corner_ind+ntmp+1;
	        res[7] = corner_ind+ntmp+ntmpsqr+1;
 
	        res[8] = (1-diff_x)*(1-diff_y)*(1-diff_z);
	        res[9] = (1-diff_x)*(1-diff_y)*diff_z;
	        res[10] = (1-diff_x)*diff_y*(1-diff_z);
	        res[11] = (1-diff_x)*diff_y*diff_z;
	        res[12] = diff_x*(1-diff_y)*(1-diff_z);
	        res[13] = diff_x*(1-diff_y)*diff_z;
	        res[14] = diff_x*diff_y*(1-diff_z);
	        res[15] = diff_x*diff_y*diff_z;
		
			for(var j=0;j<8;j++)
			{
				Dw[res[j]] = Dw[res[j]] + res[j+8];
			}
		}
		
		
		var center_num = 2;
		var Center = new Array(center_num);
		for (var i = 0;i < center_num;i++)
		{
			var k = floor(random() * this.img_area);
			Center[i] = RGB2LAB([this.ori_r[k],this.ori_g[k],this.ori_b[k]])
			
			/*
			var k = floor(random() * pt_size);
			Center[i] = new Array(3);
			Center[i][0] = D[k][0];
			Center[i][1] = D[k][1];
			Center[i][2] = D[k][2];
			*/
		}

		//alert("initialization");
		while(true)
		{
			var cnt = new Array(center_num);
			var sumD = new Array(center_num);		
		
			for( var iter=0;iter <10;iter++)
			{
				for(var i=0;i < center_num;i++)
				{
					sumD[i] = [0,0,0];
					cnt[i] = 0;
				}
			
				for(var i = 0;i < pt_size;i++)
				{
					var min_id = -1;
					var min_v = 1e100;
					for( var j = 0; j < center_num;j++ )
					{
						var r = (D[i][0]-Center[j][0])*(D[i][0]-Center[j][0])+
								(D[i][1]-Center[j][1])*(D[i][1]-Center[j][1])+
								(D[i][2]-Center[j][2])*(D[i][2]-Center[j][2]);
						if( r < min_v )
						{
							min_v = r;
							min_id = j;
						}		
					}
				
					cnt[min_id] = cnt[min_id] + Dw[i];
					sumD[min_id][0] = sumD[min_id][0] + Dw[i]*D[i][0];
					sumD[min_id][1] = sumD[min_id][1] + Dw[i]*D[i][1];
					sumD[min_id][2] = sumD[min_id][2] + Dw[i]*D[i][2];
				}
			
			
				for( var i = 0;i < center_num;i++ ) if(cnt[i]>0)
				{
					Center[i][0] = sumD[i][0] / cnt[i];
					Center[i][1] = sumD[i][1] / cnt[i];
					Center[i][2] = sumD[i][2] / cnt[i];
				}
			}
			
			
			//alert("check");
			//check whether it's good enough
			var tot = 0;
			var totr = 0;
			for(var u=0;u < center_num;u++)
			for(var v=u+1;v < center_num;v++)
			{
				var r = this.CalculateLABDistance(Center[u][0],Center[u][1],Center[u][2],Center[v][0],Center[v][1],Center[v][2]);
			
				tot = tot + 1;
				totr = totr + sqrt(r);
			}
			totr = totr / tot;
			
			//totr = 65;
			
			var param = RBF_param_coff / (totr*totr);
			//alert(totr);
		
			var threshold = 0.7;
			var ratio = 0.0;
			var good = true; 
			var worst_weight = 1.0;
			var worst_k = 0;
			for(var k=0;k<pt_size;k++)
			{
				var tmpMat = new Array(center_num);
				for(var i=0;i < center_num;i++ )
				{
					tmpMat[i] = new Array(center_num);
				}
				var tmpD = new Array(center_num);
		
				for(var u=0;u < center_num;u++)
				for(var v=0;v < center_num;v++)
				{
					var r = this.CalculateLABDistance(Center[u][0],Center[u][1],Center[u][2],Center[v][0],Center[v][1],Center[v][2]);
				   
				    tmpMat[u][v] = exp( - r * param );
				}
		
				for(var u=0;u < center_num;u++)
				{
					var r = this.CalculateLABDistance(Center[u][0],Center[u][1],Center[u][2],D[k][0],D[k][1],D[k][2]);

					tmpD[u] = exp( - r * param );
				}

				var precompute_pinv = numeric.dotMV( pinv(tmpMat), tmpD );
				
				
				var weight_sum = 0;
				for(var i=0;i < center_num;i++)
				{
					//weight_sum = MAX(weight_sum , precompute_pinv[i]);
					
					weight_sum =weight_sum + precompute_pinv[i];
					//weight_sum =weight_sum + MAX(precompute_pinv[i],0.0);
				}
				if(weight_sum > threshold)
				{
					ratio = ratio + Dw[k];
				}else
				{
					if(Dw[k]>10)
					{
						good = false;
						//break;
					
						if( weight_sum < worst_weight )
						{
							worst_weight = weight_sum;
							worst_k = k;
						}
					}
				}
			}
			
			ratio = ratio / this.img_area;
			
			alert(center_num + " : " + ratio);
			
			if(good) break;
			if (ratio>0.99) break;
			
			center_num = center_num + 1;
			
			var tmpCenter = Center;
			Center = new Array(center_num);
			for (var i = 0;i < center_num-1;i++)
			{
				Center[i] = new Array(3);
				Center[i][0] = tmpCenter[i][0];
				Center[i][1] = tmpCenter[i][1];
				Center[i][2] = tmpCenter[i][2];
			}
			Center[center_num-1] = new Array(3);
			Center[center_num-1][0] = D[worst_k][0];
			Center[center_num-1][1] = D[worst_k][1];
			Center[center_num-1][2] = D[worst_k][2];
		}

		
		
		var res = new Array(3*center_num);
		for(var i = 0;i < center_num;i++ )
		{
			var ps = LAB2RGB(Center[i]);
			res[i*3+0] = min(MAX(ps[0]/255,0.0),1.0);
			res[i*3+1] = min(MAX(ps[1]/255,0.0),1.0);
			res[i*3+2] = min(MAX(ps[2]/255,0.0),1.0);
		}
		
		return res;
	}
     this.grid_calcKMeans(center_num)
     {
         var D = new Array(this.grid_size);
         var Dw = new Array(this.grid_size);
         for(var i=0;i < this.grid_size;i++)
         {
             D[i] = this.grid_lab[i];
             Dw[i] = 0;
         }
         for( var i = 0;i < this.img_area;i++ )
         {
             for(var j =0;j<8;j++)
             {
                 Dw[this.weightindex[i][j]] = Dw[this.weightindex[i][j]] 
+ this.weightmap[i][j];
             }
         }

         var Center = new Array(center_num);
         var cnt = new Array(center_num);
         var sumD = new Array(center_num);
         for (var i = 0;i < center_num;i++)
         {
             Center[i] = D[ floor(random() * this.grid_size) ];
         }


         for( var iter=0;iter < 20;iter++)
         {
             for(var i=0;i < center_num;i++)
             {
                 sumD[i] = [0,0,0];
                 cnt[i] = 0;
             }

             for(var i = 0;i < this.grid_size;i++)
             {
                 var min_id = -1;
                 var min_v = 1e100;
                 for( var j = 0; j < center_num;j++ )
                 {
                     var r = (D[i][0]-Center[j][0])*(D[i][0]-Center[j][0])+
(D[i][1]-Center[j][1])*(D[i][1]-Center[j][1])+
(D[i][2]-Center[j][2])*(D[i][2]-Center[j][2]);
                     if( r < min_v )
                     {
                         min_v = r;
                         min_id = j;
                     }
                 }

                 cnt[min_id] = cnt[min_id] + Dw[i];
                 sumD[min_id][0] = sumD[min_id][0] + Dw[i]*D[i][0];
                 sumD[min_id][1] = sumD[min_id][1] + Dw[i]*D[i][1];
                 sumD[min_id][2] = sumD[min_id][2] + Dw[i]*D[i][2];
             }


             for( var i = 0;i < center_num;i++ ) if(cnt[i]>0)
             {
                 Center[i][0] = sumD[i][0] / cnt[i];
                 Center[i][1] = sumD[i][1] / cnt[i];
                 Center[i][2] = sumD[i][2] / cnt[i];
             }
         }

         var res = new Array(3*center_num);
         for(var i = 0;i < center_num;i++ )
         {
             var ps = LAB2RGB(Center[i]);
             res[i*3+0] = ps[0]/255;
             res[i*3+1] = ps[1]/255;
             res[i*3+2] = ps[2]/255;
         }

         return res;
     }

#endif

int round(double x) {
  return (int)(x+0.5);
}

int gridacc_kmeans(int img_height, int img_width, const uchar* src, int step, int cn, int rindex, int center_num, uchar* res)
{
  int i, j, k;
  int ngrid = 16;
  int grid_size = ngrid * ngrid * ngrid;
  
  double step_size = 255.0 / (ngrid-1);
  int* sample_cnt = MALLOC(int, grid_size);
  double* sample_sum = MALLOC(double, grid_size*3);

  for(i=0;i < grid_size;i++)
  {
    sample_cnt[i] = 0;
    sample_sum[i*3+0] = 0;
    sample_sum[i*3+1] = 0;
    sample_sum[i*3+2] = 0;
  }
  
  for( i = 0; i < img_height; i++ ) {
    const uchar* src1 = src + i*step;
    for (j=0; j<img_width; ++j) {
      double p[3];
      uchar u[3];
      const uchar* ori = src1 + j * cn;
      RGB2LAB(ori, rindex, p);
      //LAB2RGB(p, rindex, u);
      
      int bin1 = round( ori[0 ^ rindex] / step_size );
      int bin2 = round( ori[1] / step_size );
      int bin3 = round( ori[2 ^ rindex] / step_size );
      
      int bin = bin1 * ngrid * ngrid + bin2 * ngrid + bin3;
      
      sample_cnt[bin]++;
      sample_sum[bin*3+0] += p[0];
      sample_sum[bin*3+1] += p[1];
      sample_sum[bin*3+2] += p[2];
    }
  }
  if (0) {
    cvShowMat("src0", "%I8d", img_height, img_width, src+0, step, 3, -1);
    cvShowMat("src1", "%I8d", img_height, img_width, src+1, step, 3, -1);
    cvShowMat("src2", "%I8d", img_height, img_width, src+2, step, 3, -1);
    cvShowMat("sample_cnt", "%I32d", grid_size, 1, sample_cnt, 4, 4, -1);
    cvWaitKey(-1);
  }
  
  int tot = 0;
  for(i=0;i < grid_size;i++) {
    if(sample_cnt[i]>0) {
      tot++;
    }
  }
  
  double* D = MALLOC(double, tot*3);
  tot = 0;
  for(i=0; i < grid_size; i++) {
    if(sample_cnt[i]>0) {
      D[tot*3+0] = sample_sum[i*3+0] / sample_cnt[i];
      D[tot*3+1] = sample_sum[i*3+1] / sample_cnt[i];
      D[tot*3+2] = sample_sum[i*3+2] / sample_cnt[i];
      sample_cnt[tot] = sample_cnt[i];
      tot++;
    }
  }
  
  double* Center = MALLOC(double, (center_num+1)*3);
  double* pickcnt = MALLOC(double, tot);
  for(i=0; i < tot; i++) {
    pickcnt[i] = sample_cnt[i];
  }
  
  for (i = 0;i < center_num;i++) {
    int idx = 0;
    for(j=0;j<tot; j++) {
      if(pickcnt[j] > pickcnt[idx]) {
        idx = j;
      }
    }
    
    Center[i*3+0] = D[idx*3+0];
    Center[i*3+1] = D[idx*3+1];
    Center[i*3+2] = D[idx*3+2];
    for(j=0; j < tot;j++) {
      double dis = 0;
      for(k=0; k<3; k++) {
        double t = D[idx*3+k] - D[j*3+k];
        dis += t*t;
      }
      dis = dis / (80 * 80);
      pickcnt[j] = pickcnt[j] * ( 1 - exp(-dis) );
    }
  }
  
  if (0) {
    cvShowMat("Center", "%f %f %f", center_num, 1, Center, 3*sizeof(*Center), sizeof(*Center), -1);
    cvWaitKey(-1);
  }
  //add black
  uchar black_rgb[4] = {0, 0, 0, 0};
  RGB2LAB(black_rgb, rindex, Center+center_num*3);
  center_num = center_num + 1;
  
  int* cnt = MALLOC(int, center_num);
  double* sumD = MALLOC(double, center_num*3);
  
  int iter;
  for( iter=0;iter < 20;iter++) {
    for(i=0;i < center_num;i++) {
      sumD[i*3+0] = 0;
      sumD[i*3+1] = 0;
      sumD[i*3+2] = 0;
      cnt[i] = 0;
    }
    
    for(i = 0;i < tot;i++) {
      int min_id = -1;
      double min_v = 1e100;
      for(j = 0; j < center_num; j++) {
        double t0 = D[i*3+0]-Center[j*3+0];
        double t1 = D[i*3+1]-Center[j*3+1];
        double t2 = D[i*3+2]-Center[j*3+2];
        double r = t0*t0 + t1*t1 + t2*t2;
        if( r < min_v ) {
          min_v = r;
          min_id = j;
        }
      }
      
      cnt[min_id] += sample_cnt[i];
      sumD[min_id*3+0] += sample_cnt[i] * D[i*3+0];
      sumD[min_id*3+1] += sample_cnt[i] * D[i*3+1];
      sumD[min_id*3+2] += sample_cnt[i] * D[i*3+2];
    }

    for( i = 0; i < center_num; i++ ) {
      if(cnt[i]>0) {
        Center[i*3+0] = sumD[i*3+0] / cnt[i];
        Center[i*3+1] = sumD[i*3+1] / cnt[i];
        Center[i*3+2] = sumD[i*3+2] / cnt[i];
      }
    }
  }

  center_num = center_num - 1;
  for(i = 0;i < center_num;i++ ) {
    uchar* ps = res + i*4;
    LAB2RGB(Center+i*3, rindex, ps);
  }
  FREE(sample_cnt);
  FREE(sample_sum);
  FREE(D);
  FREE(pickcnt);
  FREE(Center);
  FREE(sumD);
  FREE(cnt);
  return 0;
}

double CalculateLABDistance6(double l1,double a1,double b1,double l2,double a2,double b2)
{
		double K1 = 0.045, K2 = 0.015;
    double del_L = l1 - l2;
    double c1 = sqrt(a1*a1+b1*b1); 
    double c2 = sqrt(a2*a2+b2*b2);
    double c_ab = c1 - c2;
    double h_ab = (a1-a2)*(a1-a2)+(b1-b2)*(b1-b2) - c_ab*c_ab;
    return del_L*del_L + c_ab *c_ab /(1+K1*c1)/(1+K1*c1) + h_ab / (1+K2*c1)/(1+K2*c1);
    //return (l1-l2)*(l1-l2)+(a1-a2)*(a1-a2)+(b1-b2)*(b1-b2);
}

double CalculateLABDistance(const double* lab1, const double* lab2) {
  return CalculateLABDistance6(lab1[0], lab1[1], lab1[2], lab2[0], lab2[1], lab2[2]);
}

// variant RBF interpolation
int CalculateSinglePoint(double param, int matrixsize, const double* oldpalette, const double* diffpalette, const double* vsrc, double* SinglePoint )
{
  int i, j, u, v;
	// tmpMat: e^oldpalette distance
	double* tmpMat = MALLOC(double, matrixsize*matrixsize);
  for(u=0;u < matrixsize;u++) {
    double* tmpMat1 = tmpMat + u*matrixsize;
    for(v=0;v < matrixsize;v++) {
      double r = CalculateLABDistance(oldpalette+u*3,oldpalette+v*3);		   
      tmpMat1[v] = exp(-r*param);
    }
  }
  // tmpD: e^distance(vsrc , oldpalette)
  double* tmpD = MALLOC(double, matrixsize);
  for(u=0; u < matrixsize; u++) {
    double r = CalculateLABDistance(oldpalette+u*3, vsrc);
    tmpD[u] = exp(-r*param);
  }
  
  double* tmpInvMat = MALLOC(double, matrixsize*matrixsize);
  pinv(tmpMat, matrixsize, matrixsize, tmpInvMat);
  
  if (0) {
    printf("tmpMat\n");
    for (i=0; i<matrixsize; ++i) {
      for (j=0; j<matrixsize; ++j) {
        printf("%f,", tmpMat[i*matrixsize+j]);
      }
      printf("\n");
    }
    
    printf("\n");
    printf("tmpInvMat\n");
    for (i=0; i<matrixsize; ++i) {
      for (j=0; j<matrixsize; ++j) {
        printf("%f,", tmpInvMat[i*matrixsize+j]);
      }
      printf("\n");
    }
  }
  double* precompute_pinv = MALLOC(double, matrixsize);// = numeric.dotMV( pinv(tmpMat), tmpD );
  for (i=0; i<matrixsize; ++i) {
    double t=0;
    const double* tmpInvMat1 = tmpInvMat + i*matrixsize;
    for (j=0; j<matrixsize; ++j) {
      t += tmpD[j] * tmpInvMat1[j];
    }
    precompute_pinv[i] = t;
  }
  double delta[3] = {0,0,0};
  double scale = 0;
  for(j = 0;j < matrixsize;j++ ) {
    scale = scale + MAX( precompute_pinv[j], 0.0 );
  }
  //if( scale < 1){ scale = 1.0;}
  for( j = 0; j < matrixsize; j++ ) {
    if(precompute_pinv[j]>0) {
      double r = precompute_pinv[j];
      delta[0] += r / scale * diffpalette[j*3+0];
      delta[1] += r / scale * diffpalette[j*3+1];
      delta[2] += r / scale * diffpalette[j*3+2];
    }
  }
  
  SinglePoint[0] = vsrc[0] + delta[0];
  SinglePoint[1] = vsrc[1] + delta[1];
  SinglePoint[2] = vsrc[2] + delta[2];
  FREE(tmpInvMat);
  FREE(precompute_pinv);
  FREE(tmpD);
  FREE(tmpMat);
  return 0;
}

BOOL Bigchange(const double* dir)
{
	return ( fabs(dir[0])>0.5 || fabs(dir[1])>0.5 || fabs(dir[2])>0.5 );
}

BOOL OutBoundary(const double* testrgb )
{
		double out_threshold = 0.5;
    return ( testrgb[0] < -out_threshold || testrgb[0] > 255+out_threshold ||
		    testrgb[1] < -out_threshold || testrgb[1] > 255+out_threshold ||
        testrgb[2] < -out_threshold || testrgb[2] > 255+out_threshold  );
}
BOOL LabOutBoundary(const double* lab ) {
  double rgb[3];
  LAB2RGBd(lab, 0, rgb);
  return OutBoundary(rgb);
}

double FindBoundary(const double* vsrc, const double* dir, double l, double r)
{
  //Assume dir is large
  int iter;
  for(iter = 0;iter < 15; iter++)
  {
    double mid = (l+r)*0.5;
    double lab[3];
    lab[0] = vsrc[0] + mid * dir[0];
    lab[1] = vsrc[1] + mid * dir[1];
    lab[2] = vsrc[2] + mid * dir[2];
    
    if(LabOutBoundary(lab)) {
      r = mid;
    } else {
      l = mid;
    }
  }
  return l;
}

int CalculateGridResult(int palette_size, const double* oldpalette, const double* diffpalette, int grid_size, const double* grid_lab, int rindex, uchar* grid )
{
	//average distance
  int i, j, u, v;
	int tot = 0;
	double totr = 0;
  for(u=0;u < palette_size;u++) {
    for(v=u+1;v < palette_size;v++) {
      double r = CalculateLABDistance(oldpalette+u*3,oldpalette+v*3);
      tot = tot + 1;
      totr = totr + sqrt(r);
    }
  }
	
	if( palette_size > 1 ) {
    totr = totr / tot;
  } else {
		totr = 1.0;
	}
	
  double param = RBF_param_coff / (totr*totr);
	double* tdiff = MALLOC(double, palette_size*3);
	
	for(i=0;i < grid_size;i++ ) {
		const double* vsrc = grid_lab+i*3;

		for(j=0; j < palette_size;j++) {
			const double* dir = diffpalette + j*3;
			if(Bigchange(dir)) {
        double lab[] = {vsrc[0]+dir[0],vsrc[1]+dir[1],vsrc[2]+dir[2]};
				if(LabOutBoundary(lab))	{
          double M[3] = { oldpalette[j*3+0]+dir[0], oldpalette[j*3+1]+dir[1], oldpalette[j*3+2]+dir[2] };
          double Mdir[3] = {vsrc[0]-oldpalette[j*3+0],vsrc[1]-oldpalette[j*3+1],vsrc[2]-oldpalette[j*3+2]};
					double t1 = FindBoundary( M , Mdir, 0, 1 );
          double lab1[] = {M[0]+Mdir[0],M[1]+Mdir[1],M[2]+Mdir[2]};
					if(!LabOutBoundary(lab1))
					{
						printf("M+Mdir Error");
            //return 0;
					}
					
					double t2 = FindBoundary( oldpalette+j*3, dir, 1, 300);	
					
					tdiff[j*3+0] = (dir[0] - (1-t1)*Mdir[0]) ;/// t2;
					tdiff[j*3+1] = (dir[1] - (1-t1)*Mdir[1]) / t2;
					tdiff[j*3+2] = (dir[2] - (1-t1)*Mdir[2]) / t2;
					
					if(t1>1)
					{
						printf("t1>1 at case 1 %f [%f %f %f]", t1, Mdir[0], Mdir[1], Mdir[2]);	
					}
					if(t2<1)
					{
						printf("t2<1 at case 1 %f", t2);
					}
				}else
				{
					double t1 = FindBoundary( vsrc, dir,1,300 );
					double t2 = FindBoundary( oldpalette+j*3, dir, 1,300 );
					
					if(t2<1)
					{
						printf("t2<1 at case 2 %f", t2);
					}
					double lambda = MIN( t1/t2 , 1.0 );
					tdiff[j*3+0] = diffpalette[j*3+0] ;//* lambda;
					tdiff[j*3+1] = diffpalette[j*3+1] * lambda;
					tdiff[j*3+2] = diffpalette[j*3+2] * lambda;
				}
      } else	{
				tdiff[j*3+0] = diffpalette[j*3+0];
				tdiff[j*3+1] = diffpalette[j*3+1];
				tdiff[j*3+2] = diffpalette[j*3+2];
			}
    }
		
		double res[3];
    CalculateSinglePoint( param, palette_size, oldpalette, tdiff, vsrc, res );
		LAB2RGB(res, rindex, grid + i*3);
	}
  FREE(tdiff);
  return 0;
}

int drawRes(const uchar* input_palette, const uchar* output_palette, int palette_size, int rindex,
            int h, int w, const uchar* src, int srcstep, uchar* dst, int dststep, int cn,
            const uchar* mask, int maskstep, double alpha)
{
  int i, j, k;
	//alert(input_palette);
	//alert(output_palette);
	double* oldpalette = MALLOC(double, (palette_size+1)*3);
  double* newpalette = MALLOC(double, (palette_size+1)*3);
  int img_area = h * w;
  int ngrid = 12;
  int cnt = 0;
  
  // preprocessing grid for an input
  // initialize the LAB grid
  int grid_size = (ngrid+1) * (ngrid+1) * (ngrid+1);
  double* grid_lab = MALLOC(double, grid_size*3);
  
  double step = 255.0 / ngrid;
  int tot = 0;
  for(i=0;i<ngrid+1;i++) {
    for(j=0;j<ngrid+1;j++) {
      for(k=0;k<ngrid+1;k++) {
        uchar rgb[] = {k*step,j*step,i*step};
        RGB2LAB(rgb, 0, grid_lab+tot*3);
        tot = tot + 1;
      }
    }
  }
  
  // calculate the weight index and map for image pixel
  int* weightindex = MALLOC(int, img_area*8);
  double* weightmap = MALLOC(double, img_area*8);
  
  for (i=0; i<h; i++) {
    const uchar* src1 = src + i*srcstep;
    for (j=0; j<w; j++, src1+=cn) {
      int ori_r = src1[0 ^ rindex];
      int ori_g = src1[1];
      int ori_b = src1[2 ^ rindex];
      int* weightindex1 = weightindex + (i*w+j)*8;
      double* weightmap1 = weightmap + (i*w+j)*8;
      calculate_singlegrid( ori_r, ori_g, ori_b, ngrid, weightindex1, weightmap1);
    }
  }
  // initialize result grid
  uchar* grid = MALLOC(uchar, grid_size*3);

  for(i=0;i<palette_size;i++) {
    RGB2LAB( input_palette+i*4, rindex, oldpalette+i*3 );
    RGB2LAB( output_palette+i*4, rindex, newpalette+i*3 );
  }

  if (0) {
    palette_size = 5;
    oldpalette[0 ] = 0.25628208292351756;
    oldpalette[1 ] = 0.21817835107769884;
    oldpalette[2 ] = 0.14886595496402188;
    oldpalette[3 ] = 0.42849167659494153;
    oldpalette[4 ] = 0.35299656126459417;
    oldpalette[5 ] = 0.30398255925384277;
    oldpalette[6 ] = 0.4079072793830071;
    oldpalette[7 ] = 0.49572318590551473;
    oldpalette[8 ] = 0.6471133947715362;
    oldpalette[9 ] = 0.7531024708127293;
    oldpalette[10] = 0.5963797804133025;
    oldpalette[11] = 0.47847982378586534;
    oldpalette[12] = 0.6412882916886491;
    oldpalette[13] = 0.6281352300191952;
    oldpalette[14] = 0.6721146269151697;
    ;
    newpalette[0 ] = 0.25628208292351756;
    newpalette[1 ] = 0.21817835107769884+0.2;
    newpalette[2 ] = 0.14886595496402188;
    newpalette[3 ] = 0.42849167659494153;
    newpalette[4 ] = 0.35299656126459417;
    newpalette[5 ] = 0.30398255925384277;
    newpalette[6 ] = 0.4079072793830071;
    newpalette[7 ] = 0.49572318590551473;
    newpalette[8 ] = 0.6471133947715362;
    newpalette[9 ] = 0.7531024708127293;
    newpalette[10] = 0.5963797804133025;
    newpalette[11] = 0.47847982378586534;
    newpalette[12] = 0.6412882916886491;
    newpalette[13] = 0.6281352300191952;
    newpalette[14] = 0.6721146269151697;
    
    for(i=0;i<palette_size;i++) {
      RGB2LABd( oldpalette+i*3, 0, oldpalette+i*3 );
      RGB2LABd( newpalette+i*3, 0, newpalette+i*3 );
    }
  }
  oldpalette[palette_size*3+0]=0;
  oldpalette[palette_size*3+1]=0;
  oldpalette[palette_size*3+2]=0;
  
  double* diffpalette = MALLOC(double, (palette_size+1)*3);
  for(i=0;i < palette_size;i++)
  {
    diffpalette[i*3+0] = newpalette[i*3+0]-oldpalette[i*3+0];
    diffpalette[i*3+1] = newpalette[i*3+1]-oldpalette[i*3+1];
    diffpalette[i*3+2] = newpalette[i*3+2]-oldpalette[i*3+2];
  }
  
  diffpalette[palette_size*3+0]=0;
  diffpalette[palette_size*3+1]=0;
  diffpalette[palette_size*3+2]=0;
  
  //cvShowMat("diffpalette", "%f", palette_size, 3, diffpalette, 8*3, 8, -1);cvWaitKey(-1);
  //var start_time = performance.now();//ms
  CalculateGridResult(palette_size, oldpalette, diffpalette, grid_size, grid_lab, rindex, grid );
  FREE(diffpalette);
  //this.grid_time = (performance.now() - start_time);//ms
  
  for (i=0; i<h; i++) {
    const uchar* src1 = src + i*srcstep;
    uchar* dst1 = dst + i*dststep;
    const uchar* mask1 = mask + i*maskstep;
    for (j=0; j<w; j++, src1+=cn, dst1+=cn) {
      if (!mask || mask1[j]>0) {
        int ori_r = src1[0 ^ rindex];
        int ori_g = src1[1];
        int ori_b = src1[2 ^ rindex];
        double tmp[3] = {0, 0, 0};
        uchar tmp8u[4] = {0, 0, 0};
        int* weightindex1 = weightindex + (i*w+j)*8;
        double* weightmap1 = weightmap + (i*w+j)*8;
        
        for (k=0; k<8; k++) {
          uchar* grid1 = grid + weightindex1[k]*3;
          //alert(weightindex[i][k]+" "+weightmap[i][k]);
          tmp[0] += grid1[0] * weightmap1[k];
          tmp[1] += grid1[1] * weightmap1[k];
          tmp[2] += grid1[2] * weightmap1[k];
        }

        if (mask) {
          uchar r = mask1[j]*alpha;
          tmp[0] = dst1[0] + ((tmp[0]-dst1[0])*r)/255.;
          tmp[1] = dst1[1] + ((tmp[1]-dst1[1])*r)/255.;
          tmp[2] = dst1[2] + ((tmp[2]-dst1[2])*r)/255.;
        }
        dst1[0] = BOUND(tmp[0], 0, 255);
        dst1[1] = BOUND(tmp[1], 0, 255);
        dst1[2] = BOUND(tmp[2], 0, 255);
      }
    }
  }

  //this.interp_time = (performance.now() - start_time - this.grid_time);//ms
  //this.sum_grid_time = this.sum_grid_time + this.grid_time;
  //this.sum_interp_time = this.sum_interp_time + this.interp_time;
  //this.sum_draw_time = this.sum_draw_time + 1;
  
  //draw res_rgb
  //this.drawResImage(output_canvas);
  FREE(diffpalette);
  FREE(grid);
  FREE(weightindex);
  FREE(weightmap);
  FREE(grid_lab);
  FREE(oldpalette);
  FREE(newpalette);
  return 0;
}
	
