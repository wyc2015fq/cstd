
#if 0

//GMM

function gmmcolor(image_name, maxsize , gmm_file, callback){
	var ngrid = 10;
	
	this.calculate_singlegrid = function(ori_r,ori_g,ori_b)
	{
		
        var ntmp=ngrid + 1;
        var ntmpsqr = ntmp*ntmp;
        var diff_x, diff_y,diff_z;
        var corner_ind;
        var tmpx, tmpy, tmpz;
		
		
        tmpx = ori_r / 255 * ngrid;
        diff_x = tmpx - floor(tmpx);
        tmpx = floor(tmpx);
        if (tmpx == ngrid){
          tmpx = ngrid - 1;
          diff_x = 1;
         }
	   
        tmpy = ori_g / 255 *ngrid;
        diff_y = tmpy - floor(tmpy);
	  tmpy = floor(tmpy);
        if (tmpy == ngrid){
            tmpy = ngrid - 1;
            diff_y = 1;
        }
	  
        tmpz = ori_b / 255 *ngrid;
        diff_z = tmpz - floor(tmpz);
	  tmpz = floor(tmpz);
        if (tmpz == ngrid){
            tmpz = ngrid - 1;
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
		
		return res;
	}
	this.prepare_grid = function()
	{
		//grid_lab
		this.grid_size = (ngrid+1) * (ngrid+1) * (ngrid+1);
		this.grid_lab = new Array(this.grid_size);
		
		var step = 255.0 / ngrid;
		
		var tot = 0;
		for(var i=0;i<ngrid+1;i++)
		for(var j=0;j<ngrid+1;j++)
		for(var k=0;k<ngrid+1;k++)
		{
			this.grid_lab[tot] = RGB2LAB([k*step,j*step,i*step]);
		    tot = tot + 1;
		}
		
		//weight index/map
        this.weightindex = new Array(this.img_area);
        this.weightmap   = new Array(this.img_area);

        for (var i=0; i<this.img_area; i++) {
			var res = this.calculate_singlegrid( this.ori_r[i],this.ori_g[i],this.ori_b[i]);
      
  		  this.weightindex[i] = new Array(8);
		  for(var j=0;j < 8;j++)
		  {
		  	this.weightindex[i][j] = res[j];
		  }
		  this.weightmap[i] = new Array(8);
		  for(var j=0;j < 8;j++)
		  {
		  	this.weightmap[i][j] = res[j+8];
		  }
        }
		
		this.grid_R = new Array(this.grid_size);
		this.grid_G = new Array(this.grid_size);
		this.grid_B = new Array(this.grid_size);
		
		this.tmp_grid_R = new Array(this.grid_size);
		this.tmp_grid_G = new Array(this.grid_size);
		this.tmp_grid_B = new Array(this.grid_size);
		
	}
	
	
	var imageObj = new Image();
	var ptr = this;
	
	imageObj.onload = function(){
		var img_width, img_height;
		
	    if( imageObj.width > imageObj.height )
		{
	    	img_width = maxsize;
        	img_height = ceil(img_width/imageObj.width*imageObj.height);
		}else
		{
	    	img_height = maxsize;
        	img_width = ceil(img_height/imageObj.height*imageObj.width);
		}

		ptr.img_width = img_width;
		ptr.img_height = img_height;

		var newCanvas = $("<canvas>")[0];
		newCanvas.width = img_width;
		newCanvas.height = img_height;
		
		var newContext = newCanvas.getContext("2d");
		newContext.width = img_width;
		newContext.height = img_height;
		
        newContext.drawImage(imageObj, 0,0,img_width,img_height);
		
        var imgData = newContext.getImageData(0,0,img_width,img_height);
        ptr.img_area = img_width * img_height;

		
        ptr.ori_b = new Array(ptr.img_area);
        ptr.ori_g = new Array(ptr.img_area);
        ptr.ori_r = new Array(ptr.img_area);
        var cnt = 0;
        for (var i=0; i<imgData.data.length; i+=4)
        {
            ptr.ori_r[cnt] = imgData.data[i];
            ptr.ori_g[cnt] = imgData.data[i+1];
            ptr.ori_b[cnt] = imgData.data[i+2];
            cnt = cnt + 1;
        }
		
		ptr.prepare_grid();
		
        ptr.res_b = new Array(ptr.img_area);
        ptr.res_g = new Array(ptr.img_area);
        ptr.res_r = new Array(ptr.img_area);
		
	        var request = new XMLHttpRequest();
	        request.open("GET", gmm_file);
	        request.onreadystatechange = function () {
	            if (request.readyState == 4) {
					var line = request.responseText.split("\n");
					cnt = 0;
					ptr.gmm_palette = new Array(5);
					for(var i=0;i<5;i++)
					{
						var gmm_lab = new Array(3);
						
						for(var j=0;j<3;j++)
						{
							gmm_lab[j] = parseFloat(line[cnt]);cnt = cnt + 1;
						}
						
						//alert(gmm_lab);
						ptr.gmm_palette[i] = LAB2RGB(gmm_lab);
					}
					
					var file_ngrid = parseInt(line[cnt]);cnt = cnt + 1;
					
					if(file_ngrid != ngrid)
					{
						alert("Configuration mismatch");
					}
					
					var grid3 = (ngrid+1)*(ngrid+1)*(ngrid+1);
					ptr.gmm_post = new Array(grid3);
					for(var i=0;i < grid3;i++)
					{
						ptr.gmm_post[i] = new Array(5);
						for(var j=0;j<5;j++)
						{
							ptr.gmm_post[i][j] = parseFloat(line[cnt]);cnt = cnt + 1;
						}
						//alert(ptr.gmm_post[i]);
					}
					
					callback();
	            }
	        }
	        request.send();

	};
	imageObj.src = image_name;
	
	this.drawResImage = function(output_canvas)
	{
		var output_context = output_canvas.getContext('2d');
        var output_imgData = output_context.createImageData(this.img_width, this.img_height);
        var cnt = 0;
        for (var i=0; i<output_imgData.data.length; i+=4)
        {
            output_imgData.data[i] = this.res_r[cnt];
            output_imgData.data[i+1]= this.res_g[cnt];
            output_imgData.data[i+2]= this.res_b[cnt];
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
	this.drawRes = function(input_palette, output_palette, palette_size, output_canvas)
	{
		var oldpalette_L = new Array(palette_size+1);
		var oldpalette_A = new Array(palette_size+1);
		var oldpalette_B = new Array(palette_size+1);
		var newpalette_L = new Array(palette_size+1);
		var newpalette_A = new Array(palette_size+1);
		var newpalette_B = new Array(palette_size+1);
		
		var tmplab;
		for(var i=0;i<palette_size;i++)
		{
			tmplab = RGB2LAB( [ input_palette[i*3]*255,input_palette[i*3+1]*255,input_palette[i*3+2]*255 ] );
			oldpalette_L[i] = tmplab[0];oldpalette_A[i] = tmplab[1];oldpalette_B[i] = tmplab[2];
			
			tmplab = RGB2LAB( [ output_palette[i*3]*255,output_palette[i*3+1]*255,output_palette[i*3+2]*255 ] );
			newpalette_L[i] = tmplab[0];newpalette_A[i] = tmplab[1];newpalette_B[i] = tmplab[2];
		}
		
		oldpalette_L[palette_size]=0;
		oldpalette_A[palette_size]=0;
		oldpalette_B[palette_size]=0;
		
		var diffpalette_L = new Array(palette_size+1);
		var diffpalette_A = new Array(palette_size+1);
		var diffpalette_B = new Array(palette_size+1);
		
		for(var i=0;i < palette_size;i++)
		{
			diffpalette_L[i] = newpalette_L[i]-oldpalette_L[i];
			diffpalette_A[i] = newpalette_A[i]-oldpalette_A[i];
			diffpalette_B[i] = newpalette_B[i]-oldpalette_B[i];
		}
		
		diffpalette_L[palette_size]=0;
		diffpalette_A[palette_size]=0;
		diffpalette_B[palette_size]=0;
		
		
		
		for(var i=0;i < this.grid_size;i++ )
		{
			var vsrc = new Array(3);
			vsrc[0] = this.grid_lab[i][0];
			vsrc[1] = this.grid_lab[i][1];
			vsrc[2] = this.grid_lab[i][2];
			
			for(var j=0;j < 5;j++)
			{
				vsrc[0] = vsrc[0] + diffpalette_L[j] * this.gmm_post[i][j];
				vsrc[1] = vsrc[1] + diffpalette_A[j] * this.gmm_post[i][j];
				vsrc[2] = vsrc[2] + diffpalette_B[j] * this.gmm_post[i][j];
			}
			
			var pc = LAB2RGB(vsrc);
			this.grid_R[i] = pc[0];
			this.grid_G[i] = pc[1];
			this.grid_B[i] = pc[2];			
					
			this.grid_R[i] = MAX(0, min(this.grid_R[i],255))
			this.grid_G[i] = MAX(0, min(this.grid_G[i],255))
			this.grid_B[i] = MAX(0, min(this.grid_B[i],255))
		}
			
        for (var i=0; i<this.img_area;i++){
			
			var tmpR = 0;
			var tmpG = 0;
			var tmpB = 0;
         	for (var k=0; k<8; k++){
 				//alert(weightindex[i][k]+" "+weightmap[i][k]);
             	tmpR = tmpR + this.grid_R[this.weightindex[i][k]] * this.weightmap[i][k];
            	tmpG = tmpG + this.grid_G[this.weightindex[i][k]] * this.weightmap[i][k];
            	tmpB = tmpB + this.grid_B[this.weightindex[i][k]] * this.weightmap[i][k];
        	}
			this.res_r[i] = tmpR;
			this.res_g[i] = tmpG;
			this.res_b[i] = tmpB;
 		}
		

		//draw res_rgb
		this.drawResImage(output_canvas);
		
	}
	
	this.drawImage = function(output_canvas)
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
	
	
	//other function
	this.RGB2LAB = function(Q)
	{
		return RGB2LAB(Q);
	}
	this.LAB2RGB = function(Q)
	{
		return LAB2RGB(Q);
	}

	this.getGMM = function(center_num)
	{
		//return gmm palette
		var res = new Array(3*center_num);
		for(var i = 0;i < center_num;i++ )
		{
			var ps = this.gmm_palette[i];
			res[i*3+0] = MAX( 0.0, min( 1.0,ps[0]/255 ) );
			res[i*3+1] = MAX( 0.0, min( 1.0,ps[1]/255 ) );
			res[i*3+2] = MAX( 0.0, min( 1.0,ps[2]/255 ) );
		}
		return res;
	}
	this.fast_calcKMeans = function(center_num, ptLimit)
	{
		return this.getGMM(center_num);
	}
	this.gridacc_kmeans = function(center_num)
	{
		return this.getGMM(center_num);
	}
}
#endif
