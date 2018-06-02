
	// Palette-based Photo Recoloring

	// for display UI
	//var input_width		= '60px';
	//var input_height	= '55px';
	//var display_size	= 600.0;
	
	//var output_canvas;
	//var mytransfer;
	
	//var fb;
	//var fb_selected;
	//var fb_dummyinput;
	
	//var input_palette;
	//var output_palette;
	//var output_palette_array;
	
	//var palette_size = 5;
	
	//Initialize Output image UI
#if 0
	//Show the result on the output image UI
	function ShowOutput()
	{
		mytransfer.drawRes(input_palette, output_palette, palette_size, output_canvases);
	}

	//get palette color from UI
	function GetPaletteFromArray(palette_array)
	{
		var res = new Array(3*palette_size);
		for(var i=0; i<palette_size; i++)
		{
			var rgb = fb.unpack( palette_array[i].value );
			res[i*3] 	= rgb[0];
			res[i*3+1] 	= rgb[1];
			res[i*3+2] 	= rgb[2];
		}
		return res;
	}
	//set palette ui
	function SetPaletteArray(palette_array,palette)
	{
		for(var i=0;i < palette_size;i++ )
		{
			var newcolor = fb.pack([palette[i*3],palette[i*3+1],palette[i*3+2]]);
			
			palette_array[i].value = newcolor;
	        $(palette_array[i]).css({ backgroundColor: newcolor, color: newcolor });
		}
		if(fb.callback) { fb.linkTo(fb.callback); }
	}

	function SmoothL(x,d)
	{
		var lambda = 0.2 * Math.log(2);
		return Math.log( Math.exp(lambda * x) + Math.exp(lambda * d) - 1 ) / lambda - x;
	}
	
	var old_L,old_A,old_B,expected_L;
	// Modifying luminance to maintain monotonicity
	function modifyNewpalette()
	{
		for (var oid=0;oid < palette_size;oid++) if(output_palette_array[oid] == fb_selected)
		{
			var newRGB = fb.unpack(fb_selected.value);
			//alert(newRGB);
			
			var newLAB = mytransfer.RGB2LAB( [newRGB[0]*255,newRGB[1]*255,newRGB[2]*255] );
			
			var delta = old_L[oid] - newLAB[0];

			for(var i = 0; i < palette_size;i++) if(i!=oid)
			{				
				if(old_L[i] < old_L[oid])
				{
					old_L[i] = newLAB[0] - SmoothL(delta, old_L[oid] - old_L[i]);
				}else
				{	
					old_L[i] = newLAB[0] + SmoothL(-delta, old_L[i] - old_L[oid]);
				}
			}
			
			old_L[oid] = newLAB[0];
			old_A[oid] = newLAB[1];
			old_B[oid] = newLAB[2];		 
		}


		for(var i=0;i < palette_size;i++)
		{
			var pc = mytransfer.LAB2RGB([ old_L[i],old_A[i],old_B[i] ]);
			pc[0] = Math.min( 1.0, Math.max(pc[0]/255,0) );
			pc[1] = Math.min( 1.0, Math.max(pc[1]/255,0) );
			pc[2] = Math.min( 1.0, Math.max(pc[2]/255,0) );
			
			output_palette[3*i]   = pc[0];
			output_palette[3*i+1] = pc[1];
			output_palette[3*i+2] = pc[2];
		}
		
		SetPaletteArray(output_palette_array,output_palette);
		
	}
	
	function updateNewpalette()
	{
		modifyNewpalette();
		//output_palette = GetPaletteFromArray(output_palette_array);
		ShowOutput();
	}
	
	//Effect of focus event
	function focus_template(input)
	{
		return function(event)
		{			
			$('#picker').css('opacity', 1.0);
	        if (fb_selected) 
			{
	            $(fb_selected).removeClass('colorwell-selected');
	        }
	  		fb.linkTo(input);
	        $(fb_selected = input).addClass('colorwell-selected');
		}
	}
	
	//Set Palette UI
	function SetUI()
	{
		var output_container = $('#output_palette');
    output_container.empty();
		output_container.css('margin-left','65px');

		
		output_palette_array = new Array(palette_size);
		for(var i=palette_size-1; i>=0; i--)
		{
			var input = document.createElement('INPUT');
			input.style.width		= input_width;
			input.style.height		= input_height;
			input.style.marginTop   = '2px';
			input.style.marginRight = '2px';
	
			input.disabled = false;
			input.onchange = updateNewpalette;
	
			output_palette_array[i] = input;
			input.value = fb.pack([output_palette[i*3],output_palette[i*3+1],output_palette[i*3+2]]);
			fb.linkTo(input);
			input.onfocus = focus_template(input);
			
			output_container.append(input);
			output_container.append('<br>');
		}
		
		fb_dummyinput = document.createElement('INPUT');
		fb.linkTo(fb_dummyinput);
	}

	//Reset Palette
	function Reset()
	{
		output_palette = new Array(3*palette_size);
		for(var i=0;i < 3 * palette_size;i++)
		{
			output_palette[i] = input_palette[i];
		}
		
		for(var i=0;i < palette_size;i++)
		{
			var pc = mytransfer.RGB2LAB([output_palette[3*i]*255,output_palette[3*i+1]*255,output_palette[3*i+2]*255]);
			old_L[i] = pc[0];
			old_A[i] = pc[1];
			old_B[i] = pc[2];
			expected_L[i] = pc[0];
		}
		
		SetPaletteArray(output_palette_array,output_palette);
		ShowOutput();
	}
	
	var IndexSortOrder;
	var loaded = false;
	var cur_url = null;
  
  function InitializeOutputUI()
  {
    var container 	= $('#output_image_zone'); container.empty();
    output_canvases = document.createElement('canvas');
    
    var lambda = Math.min(600.0 / mytransfer.img_height, 760.0 / mytransfer.img_width);
    output_canvases.width  	= Math.floor( mytransfer.img_width * lambda );
    output_canvases.height 	= Math.floor( mytransfer.img_height * lambda );
    
    container.append(output_canvases);
  }


	//Construct Color transfer for given image
	function getTransfer(imgurl){
		cur_url = imgurl;
		mytransfer = new rbfcolor(imgurl, display_size, [] , function(){
			InitializeOutputUI();
	
			loaded = true;
			input_palette = mytransfer.gridacc_kmeans(palette_size); 	
			
			IndexSortOrder = new Array(palette_size);
			var input_L = new Array(palette_size);
			for(var i=0;i < palette_size;i++ )
			{
				IndexSortOrder[i] = i;
				input_L[i] = mytransfer.RGB2LAB( [input_palette[3*i]*255, input_palette[3*i+1]*255, input_palette[3*i+2]*255] )[0];
			}
	
			for(var i=0;	i < palette_size; i++ )
			for(var j=i+1;	j < palette_size; j++ ) if( input_L[IndexSortOrder[i]] > input_L[IndexSortOrder[j]] )
			{
				var tmp = IndexSortOrder[i];
				IndexSortOrder[i] = IndexSortOrder[j];
				IndexSortOrder[j] = tmp;
			}
			//alert(IndexSortOrder);
	
			var tmp_palette = new Array(3*palette_size);
	
			for(var i=0; i<3*palette_size; i++)
			{
				tmp_palette[i] = input_palette[i];
			}
			for(var i=0; i<palette_size; i++)
			{
				var j = IndexSortOrder[i];
				input_palette[3*i] 		= tmp_palette[3*j];
				input_palette[3*i+1] 	= tmp_palette[3*j+1];
				input_palette[3*i+2] 	= tmp_palette[3*j+2];
			}
			
			output_palette = new Array(3*palette_size);
			for(var i=0; i<3*palette_size; i++)
			{
				output_palette[i] = input_palette[i];
			}
			old_L = new Array(palette_size);
			old_A = new Array(palette_size);
			old_B = new Array(palette_size);
			expected_L = new Array(palette_size);

			for(var i=0;i < palette_size;i++)
			{
				var pc = mytransfer.RGB2LAB([output_palette[3*i]*255,output_palette[3*i+1]*255,output_palette[3*i+2]*255]);
				old_L[i] = pc[0];
				old_A[i] = pc[1];
				old_B[i] = pc[2];
				expected_L[i] = pc[0];
			}

			//Set UI according to input/output palette
			SetUI();
			ShowOutput();
	
			$('#picker').css('opacity', 0.25);
		});
	}
    
	//Set palette UI
	function reload_palette()
   	{
		IndexSortOrder = new Array(palette_size);
		var input_L = new Array(palette_size);
		for(var i=0; i<palette_size; i++ )
		{
			IndexSortOrder[i] = i;
			input_L[i] = mytransfer.RGB2LAB( [input_palette[3*i]*255, input_palette[3*i+1]*255, input_palette[3*i+2]*255] )[0];
		}

	
		for(var i=0;   i<palette_size; i++ )
		for(var j=i+1; j<palette_size; j++ ) if( input_L[IndexSortOrder[i]] > input_L[IndexSortOrder[j]] )		
		{
			var tmp = IndexSortOrder[i];
			IndexSortOrder[i] = IndexSortOrder[j];
			IndexSortOrder[j] = tmp;
		}
		var tmp_palette = new Array(3*palette_size);

		for(var i=0; i<3*palette_size; i++ )
		{
			tmp_palette[i] = input_palette[i];
		}
		for(var i=0;i < palette_size;i++ )
		{
			var j = IndexSortOrder[i];
			input_palette[3*i] 		= tmp_palette[3*j];
			input_palette[3*i+1] 	= tmp_palette[3*j+1];
			input_palette[3*i+2] 	= tmp_palette[3*j+2];
		}
		output_palette = new Array(3*palette_size);
		for(var i=0;i < 3 * palette_size;i++)
		{
			output_palette[i] = input_palette[i];
		}
		old_L = new Array(palette_size);
		old_A = new Array(palette_size);
		old_B = new Array(palette_size);
		expected_L = new Array(palette_size);

		for(var i=0;i < palette_size;i++)
		{
			var pc = mytransfer.RGB2LAB([output_palette[3*i]*255,output_palette[3*i+1]*255,output_palette[3*i+2]*255]);
			old_L[i] = pc[0];
			old_A[i] = pc[1];
			old_B[i] = pc[2];
			expected_L[i] = pc[0];
		}

		//Set UI according to input/output palette
		SetUI();
		ShowOutput();
   }
   
   function changePaletteFromStr(res)
   {
	   palette_size = res.length;
	   for(var i=0;i < palette_size;i++)
	   {
		   var newRGB = fb.unpack(res[i]);
		  input_palette[3*i] = newRGB[0];
		  input_palette[3*i+1] = newRGB[1];
		  input_palette[3*i+2] = newRGB[2];
	   }
	   reload_palette();
   }
   
   
   function closeWindow()
   {
	   $('#global_div').css('opacity', 1.0);
	   $( "#dialog" ).dialog( "close" );
   }
   
   
   
	var dialog_palette_size;   
	var dialog_display_width, dialog_display_height;
	var dialog_output_canvases;
	var dialog_imgData;

	var dialog_fb_selected = null;
	var dialog_output_palette_array;
	var dialog_edited;
	var dialog_fb;
   
	var size_array;
	// Change palette size
	function changepsize()
	{
  	  	var size_select = document.getElementById("psizeselect");
  		dialog_palette_size = size_array[ size_select.selectedIndex ];
  
  	  	dialog_Make_Palette();
	}
	
	// UI effect for focus event in dialog window
	function dialog_focus_template(input)
	{
		return function(event){
			$('#dialog_picker').css('opacity', 1.0);			
        	if (dialog_fb_selected) 
			{
            	$(dialog_fb_selected).removeClass('colorwell-selected');
          	}
		  	dialog_fb.linkTo(input);
		  	//fb.color = fb_selected.value;
		  
        	$(dialog_fb_selected = input).addClass('colorwell-selected');
		}
	}
	//update palette in dialog
	function dialog_update(newcolor)
	{
		for (var oid=0;oid < dialog_palette_size;oid++) if(dialog_output_palette_array[oid] == dialog_fb_selected)
		{
			dialog_output_palette_array[oid].value = newcolor;
        	$(dialog_output_palette_array[oid]).css({
          	  backgroundColor: newcolor,
          	  color: newcolor
        	});
			dialog_edited[oid] = true; 
			$(dialog_output_palette_array[oid]).css('border','3px solid #4A4A4A');  
			dialog_fb.linkTo(dialog_output_palette_array[oid]);
			break;
		}
	}


	function dialog_updatepalette()
	{
		dialog_update(dialog_fb.color);
	}
	//Initalize palette in dialog
	function dialog_Make_Palette()
	{
		var output_container = $('#dialog_output_palette'); output_container.empty();

		dialog_output_palette_array = new Array(dialog_palette_size);
		dialog_edited 				= new Array(dialog_palette_size);
		for(var i=0; i<dialog_palette_size; i++)
		{
			var input = document.createElement('INPUT');
			input.style.width 		= 55;
			input.style.height 		= 50;
			input.style.marginTop 	= '4px';
			input.style.marginRight = '4px';
	
			input.disabled = false;
			input.onchange = dialog_updatepalette;
			input.value = "#EDEDED";
        	$(input).css({backgroundColor: '#EDEDED', color: '#EDEDED'});

			dialog_output_palette_array[i] = input;
			input.onfocus = dialog_focus_template(input);
			dialog_edited[i] = false;
			output_container.append(input);
			if (i%2 == 1){	output_container.append('<br>'); }	
		
		}
		dialog_fb.linkTo(dialog_updatepalette);
	}
	
	//Automatically fill un-assigned entry in palette
	function AutoFill()
	{
		//...
          var ngrid = 16;

          var grid_size = ngrid * ngrid * ngrid;

          var step_size = 255.0 / (ngrid-1);
          var sample_cnt = new Array(grid_size);
          var sample_sum = new Array(grid_size);
          for(var i=0;i < grid_size;i++)
          {
              sample_cnt[i] = 0;
              sample_sum[i] = new Array(3);
              sample_sum[i][0] = 0;
              sample_sum[i][1] = 0;
              sample_sum[i][2] = 0;
          }

          for( var i = 0;i < dialog_imgData.data.length / 4;i++ )
          {
			  var pixel = [ dialog_imgData.data[i*4], dialog_imgData.data[i*4+1], dialog_imgData.data[i*4+2] ]
              var p = RGB2LAB(pixel);

              var bin1 = Math.round( pixel[0] / step_size );
              var bin2 = Math.round( pixel[1] / step_size );
              var bin3 = Math.round( pixel[2] / step_size );

              var bin = bin1 * ngrid * ngrid + bin2 * ngrid + bin3;

              sample_cnt[bin] = sample_cnt[bin] + 1;
              sample_sum[bin][0] = sample_sum[bin][0] + p[0];
              sample_sum[bin][1] = sample_sum[bin][1] + p[1];
              sample_sum[bin][2] = sample_sum[bin][2] + p[2];
          }

          var tot = 0;
          for(var i=0;i < grid_size;i++) if(sample_cnt[i]>0)
          {
              tot = tot + 1;
          }

          var D = new Array(tot);
          tot = 0;
          for(var i=0;i < grid_size;i++) if(sample_cnt[i]>0)
          {
              D[tot] = new Array(3);
              D[tot][0] = sample_sum[i][0] / sample_cnt[i];
              D[tot][1] = sample_sum[i][1] / sample_cnt[i];
              D[tot][2] = sample_sum[i][2] / sample_cnt[i];
              sample_cnt[tot] = sample_cnt[i];

              tot = tot + 1;
          }

          var Center = new Array(dialog_palette_size+1);

          var pickcnt = new Array(tot);
          for(var i=0;i < tot;i++)
          {
              pickcnt[i] = sample_cnt[i];
          }
		  
		  for(var i=0;i < dialog_palette_size;i++) if(dialog_edited[i])
		  {
			  var center_rgb = fb.unpack( dialog_output_palette_array[i].value );
			  var center_lab = RGB2LAB( [center_rgb[0]*255,center_rgb[1]*255,center_rgb[2]*255] );
			  
			  Center[i] = center_lab;
			  
			  //shrink
              for(var j=0; j < tot;j++)
              {
                  var dis = 0;
                  for(var k=0;k<3;k++)
                  {
                      dis = dis + (center_lab[k] - D[j][k])*(center_lab[k] - D[j][k]);
                  }
                  dis = dis / (80 * 80);

                  pickcnt[j] = pickcnt[j] * ( 1 - Math.exp(-dis) );
              }
		  }

          for (var i = 0;i < dialog_palette_size;i++) if(!dialog_edited[i])
          {
              var idx = 0;
              for(var j=0;j<tot;j++) if(pickcnt[j] > pickcnt[idx])
              {
                  idx = j;
              }

              Center[i] = D[idx];
              for(var j=0; j < tot;j++)
              {
                  var dis = 0;
                  for(var k=0;k<3;k++)
                  {
                      dis = dis + (D[idx][k] - D[j][k])*(D[idx][k] - D[j][k]);
                  }
                  dis = dis / (80 * 80);

                  pickcnt[j] = pickcnt[j] * ( 1 - Math.exp(-dis) );
              }
          }

          //add black
          Center[dialog_palette_size] = RGB2LAB([0,0,0]);

          var cnt = new Array(dialog_palette_size+1);
          var sumD = new Array(dialog_palette_size+1);

          for( var iter=0;iter < 20;iter++)
          {
              for(var i=0;i < dialog_palette_size+1;i++)
              {
                  sumD[i] = [0,0,0];
                  cnt[i] = 0;
              }

              for(var i = 0;i < tot;i++)
              {
                  var min_id = -1;
                  var min_v = 1e100;
                  for( var j = 0; j < dialog_palette_size+1;j++ )
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

                  cnt[min_id] = cnt[min_id] + sample_cnt[i];
                  sumD[min_id][0] = sumD[min_id][0] + sample_cnt[i] * D[i][0];
                  sumD[min_id][1] = sumD[min_id][1] + sample_cnt[i] * D[i][1];
                  sumD[min_id][2] = sumD[min_id][2] + sample_cnt[i] * D[i][2];
              }


              for( var i = 0;i < dialog_palette_size;i++ ) if(cnt[i]>0 && !dialog_edited[i])
              {
                  Center[i][0] = sumD[i][0] / cnt[i];
                  Center[i][1] = sumD[i][1] / cnt[i];
                  Center[i][2] = sumD[i][2] / cnt[i];
              }
          }


		  for(var i=0;i < dialog_palette_size;i++) if(!dialog_edited[i])
		  {
              var ps = LAB2RGB(Center[i]);
			  
              ps[0] = Math.max(0.0, Math.min(1.0, ps[0]/255) );
              ps[1] = Math.max(0.0, Math.min(1.0, ps[1]/255) );
              ps[2] = Math.max(0.0, Math.min(1.0, ps[2]/255) );
			  
			  var newcolor = fb.pack(ps);
			  
			  dialog_output_palette_array[i].value = newcolor;
  	   	   	  $(dialog_output_palette_array[i]).css({
  	          	backgroundColor: newcolor,
  	          	color: newcolor
  	          });
          }
	}
	
	//Ok button in dialog window
	function OK()
	{
		AutoFill();
		var res = new Array(dialog_palette_size);
		for(var i=0;i < dialog_palette_size;i++)
		{
			res[i] = dialog_output_palette_array[i].value;
		}

		changePaletteFromStr(res);
	
		closeWindow();
	}

	function Cancel()
	{
		closeWindow();
	}

	//Create Dialog window
   function create_dialog()
   {
	   dialog_palette_size = palette_size;
	   
	var size_select = document.getElementById("psizeselect");
	$(size_select).empty();
	size_array = [2,3,4,5,6,7];
	
	//size_select.selectedIndex = 0;
	for(var k=0;k < size_array.length;k++)
	{
		var i = size_array[k];
	
		var opt = document.createElement("option");
		opt.text = i.toString();
		size_select.add(opt);
	}
	for(var k=0;k < size_array.length;k++)
	if(size_array[k]==dialog_palette_size)
	size_select.selectedIndex = k;
	
	
	dialog_fb = $.farbtastic('#dialog_picker');
	//load image
	var imageObj = new Image();
	imageObj.onload = function(){
		var img_width, img_height;
		
		var lambda = Math.min(300.0 / imageObj.height ,500.0 /  imageObj.width );
	
		dialog_display_width = Math.floor( imageObj.width * lambda );
		dialog_display_height = Math.floor( imageObj.height * lambda );
	
	
	    var container = $('#dialog_output_image_zone');
		container.empty();
		dialog_output_canvases = document.createElement('canvas');
		
		dialog_output_canvases.width = dialog_display_width;
		dialog_output_canvases.height = dialog_display_height;
	
		var newContext = dialog_output_canvases.getContext("2d");
		newContext.width = dialog_display_width;
		newContext.height = dialog_display_height;
	
        newContext.drawImage(imageObj, 0,0,dialog_display_width,dialog_display_height);
	
        dialog_imgData = newContext.getImageData(0,0,dialog_display_width,dialog_display_height);
		container.append(dialog_output_canvases);
		
		
		dialog_output_canvases.addEventListener("mousedown", function(event)
		{
				var canvasX, canvasY;
		
				var canoffset = dialog_output_canvases.getBoundingClientRect();

				canvasX = event.clientX + document.body.scrollLeft + document.documentElement.scrollLeft - Math.floor(canoffset.left) - 1;
				canvasY = event.clientY + document.body.scrollTop + document.documentElement.scrollTop - Math.floor(canoffset.top);
			
				var index = canvasX + canvasY * dialog_display_width;
			
				var new_rgb = [ dialog_imgData.data[4 * index]/255, dialog_imgData.data[4 * index+1]/255, dialog_imgData.data[4 * index+2]/255 ];
				var newcolor = dialog_fb.pack(new_rgb);

				//update
				dialog_update(newcolor);
		});
		
		
		
		//make palette
		dialog_Make_Palette();
		$('#dialog_picker').css('opacity', 0.25);	
		
	};
	
	imageObj.src = cur_url;
	
	$('#dialog').on("beforeunload", function() { 
	    Cancel();
	})
	
   }
   
   function Editpalette()
   {
	 if( cur_url )
	 {
		$( "#dialog" ).dialog( "open" );
		$('#global_div').css('opacity', 0.25);
		
		create_dialog();
	 }
   }
   
   function SelectImage()
   {
   	$( "#select_dialog" ).dialog("open");
   }

	function getInstruction()
   	{
	   $( "#help_dialog" ).dialog("open");
   	}
    function uploadFile(){
		
        var file    = document.querySelector('input[type=file]').files[0];
		var reader  = new FileReader();
		if(file)
		{
			reader.onloadend = function()
			{
				palette_size = 5;
				getTransfer(reader.result);
				$( "#select_dialog" ).dialog( "close" );
			}
			reader.readAsDataURL(file);
		}
	}
	function PutSampleImage(im_id)
	{
		if(im_id==IMAGESET.length)
		{
			return;
		}
		
		var imageObj = new Image();
		imageObj.onload = function(){
			var img_width, img_height;
			
			//restrict height
			var lambda = 100.0 / imageObj.height;
	
			var dialog_display_width = Math.floor( imageObj.width * lambda );
			var dialog_display_height = Math.floor( imageObj.height * lambda );
	
	
		    var container = $('#select_image_zone');
			
			var sample_output_canvases = document.createElement('canvas');
			sample_output_canvases.style.margin='10px';
		
			sample_output_canvases.width = dialog_display_width;
			sample_output_canvases.height = dialog_display_height;
	
			var newContext = sample_output_canvases.getContext("2d");
			newContext.width = dialog_display_width;
			newContext.height = dialog_display_height;
	
	        newContext.drawImage(imageObj, 0,0,dialog_display_width,dialog_display_height);

			container.append(sample_output_canvases);
		
		
			sample_output_canvases.addEventListener("mouseup", function(event)
			{
				palette_size = 5;
				getTransfer(IMAGESET[im_id]);
				
				$( "#select_dialog" ).dialog( "close" );
				//close
			});

			PutSampleImage(im_id+1);
		};
	
		imageObj.src = IMAGESET[im_id];
	}
    $(function() {
		$( "#dialog" ).dialog({
			autoOpen: false,
			width: 800,
			close: function()
			{
				Cancel();
			},
			show: {
				effect: "fade",
				duration: 500
			},
			hide: {
				effect: "fade",
				duration: 500
			},
		});
			
		$( "#help_dialog" ).dialog({
			autoOpen: false,
			width: 500,
			show: {
				effect: "fade",
				duration: 500
			},
			hide: {
				effect: "fade",
				duration: 500
			},
		});
				
		$( "#select_dialog" ).dialog({
			autoOpen: false,
			width: 800,
			show: {
				effect: "fade",
				duration: 500
			},
			hide: {
				effect: "fade",
				duration: 500
			},
		});
	//Put sample Image
		PutSampleImage(0);

	    fb = $.farbtastic('#picker');

		getTransfer(IMAGESET[0]);//default Image
    });
	</script>
</head>

<body max-width="1280">

 <div id="global_div"  style="width:1000px;height:650px;margin:0 auto" >
	 <h4 style="font-family:arial; font-size:20px; color: #5A5A5A; line-height:30px;"> <center>Palette-based Photo Recoloring</center></h4>
	 <div style="width:800px; height:650px; float: left;">
	 	<div id="output_image_zone" > </div>
	 	<div id="button_div" style="width:100%; height:40px;margin-top:10px;align:center;">
			<label class="buttonstyle" > Choose Photo
				<input type="button" id="select_but" onclick="SelectImage()" value="Select Image" style="display:none;" > </input>
	        </label>
			<label class="buttonstyle"> Reset
		   	 	<input type="button" id="reset_but" onclick="Reset()" value="Reset" style="display:none;" > </input>
	        </label>
			<label class="buttonstyle" > Edit Palette
		   	 	<input type="button" id="strokeButton" onclick="Editpalette()" value="Edit Palette" style="display:none;" > </input>
	        </label>
			<label class="buttonstyle" > Help
		   	 	<input type="button" id="instButton" onclick="getInstruction()" value="Help" style="display:none;"> </input> 
	   		</label>
	  
	 	</div>
 	</div>
	 
	<div id="palette_div" style="width:200px; height:560px; float: left;">
	 	<div id="picker" style="width:200px; height:200px; clear:both;"></div>
	 	<div id="output_palette" style="width:200px; height:300px; margin:20px auto auto auto;clear:both; " ></div>
	</div>

 </div>
 
 <div id="dialog" title="Edit Palette">
 	  <div id="dialog_global_div" style="margin:0 auto;align:center;">
 	 	 <div style="width:500px; float: left;">
 	 	 	<div id="dialog_output_image_zone" style="width:100%; height:300px;" > </div>
 			<div style="width:440px; height:100px;"> <p> Click a square on the right to select it (blue border). Its color can be set using the color picker (top right) or by clicking in the image (top left).</p>
<p>
If you click either AutoFill or OK, any unspecified palette entries will be chosen automatically.</p> </div>
 	 	 	<div id="dialog_button_div" style="width:100%; height:40px;margin-top:10px;">
 		   	 	<button class="buttonstyle buttonstyle1" type="button" id="ok_but" onclick="OK()">OK</button>
 		   		<button class="buttonstyle buttonstyle1" type="button" id="cancel_but" onclick="Cancel()">Cancel</button>
				<button class="buttonstyle buttonstyle1" type="button" id="af_but" onclick="AutoFill()">AutoFill</button>
 				<select class="buttonstyle buttonstyle1" id="psizeselect" onchange="changepsize()"> </select>
 			</div>
 		</div>
 		<div id="dialog_palette_div" style="width:200px; height:500px; float: left;">
 		 	<div id="dialog_picker" style="width:200px; height:200px; clear:both;"></div>
 		 	<div id="dialog_output_palette" style="width:160px; height:200px; margin:40px auto auto 40px;clear:both;" ></div>
 		</div>
 	</div>
 </div>
 
 <div id="help_dialog" title="Instructions" style="font-family:arial; font-size:15px;">
 <p>
	 <h4>Basic Usage:</h4>
	 <ul>
		 <li>Click a palette color on the right to select a range of similar colors in the images.</li>
		 <li>Use the color picker to modify the palette color. Notice that this adjusts corresponding colors in the photo.</li>
		 <li>You can click <label class="buttonstyle">Reset</label> at any time to undo your changes.</li>
	 </ul>

	 <h4>Choosing Photos</h4>
	 <ul>
		 <li>Click <label class="buttonstyle">Choose Photo</label> to either:</li>
		 <ul>
			 <li>Use one of the pre-loaded photos, or</li>
			 <li>Click <label class="buttonstyle">Choose File</label> to pick any photo on your computer.</li>
		 </ul>
	 </ul>

	 <h4>Editing the palette</h4>
	 The number of palette entries determines how local or global your changes will be in color space. A small number of palette entries corresponds to more global changes and vice versa.
	 <ul>
	 <li>Click <label class="buttonstyle">Edit Palette</label></li>
	 <li>Choose the number of palette entries from the pull-down menu.</li>
	 <li>Click on any slot on the right. Now you can specify that palette entry either by using the color picker or by clicking on the image.</li>
	 <li>Any remaining unspecified palette entries can be filled automatically by clicking <label class="buttonstyle">AutoFill</label></li>
	 <li>Click <label class="buttonstyle">OK</label> to confirm the new palette.</li>
	 </ul>
 </p>
 </div>
 
 <div id="select_dialog" title="Choose Photo">
	 <div id="select_dialog_button_div" style="width:100%; height:40px;margin:20px;align:center;">
	 <label class="buttonstyle"> Choose file
	 	<input   type="file" onchange="uploadFile()" data-role="button" data-inline="true" data-mini="true" data-corners="false" style="display:none;" >
	 	</input>
 	</label>
	<span>
		Choose your own file with this button, or click any demo photo below. <br>
	</span>
	</div>
	<div id="select_image_zone" style="margin:20px"> </div>
 </div>
 
#endif
