<!DOCTYPE html>
<!-- Author: Shubham Khandelwal. ID: skhandelwal@ucla.edu -->
<html>
<head>
<script src="jquery-1.11.3.min.js" charset="utf-8"></script>
<script src="d3.v3.min.js" charset="utf-8"></script>
<script type="text/javascript">
  <?php
	 
	 $myFile = "bit.txt";
	 $fh = fopen($myFile, 'r');
	 $theData = fgets($fh);
	 $array = str_split($theData,1);
   ?>

$(document).ready(function(){

var arr1 = "<?php echo $array[0] ?>";
var arr2 = "<?php echo $array[1] ?>";
var arr3 = "<?php echo $array[2] ?>";
var arr4 = "<?php echo $array[3] ?>";

var totalWidth = document.body.clientWidth;
var totalHeight = 600;
var color = "";
var svgContainer = d3.select("body").append("svg").attr("width",totalWidth).attr("height",totalHeight);
              

 //Draw the Rectangles
 var width= totalWidth / 5;
 var height =  totalHeight/4;
 var x =totalWidth /2 - width ;
 var y = totalHeight/2-height;

 color = arr1 === "1"?"blue":"white";
 var rectangle1 = svgContainer.append("rect")
                 .attr("x", x)
                 .attr("y", y)
                 .attr("width", width)
                 .attr("height", height)
                 .attr("fill", color)
                 .attr("stroke","#000080")
                 .attr("stroke-width",1);

color = arr2 === "1"?"blue":"white";

 var rectangle2 = svgContainer.append("rect")
                 .attr("x", x+width)
                 .attr("y", y)
                 .attr("width", width)
                 .attr("height", height)
                 .attr("fill", color)
                 .attr("stroke","#000080")
                 .attr("stroke-width",1);

color = arr3 === "1"?"blue":"white";

 var rectangle3 = svgContainer.append("rect")
				 .attr("x", x)
				 .attr("y", y+height)
				 .attr("width", width)
				 .attr("height", height)
				 .attr("fill", color)
				 .attr("stroke","#000080")
				 .attr("stroke-width",1);

color = arr4 === "1"?"blue":"white";


 var rectangle4 = svgContainer.append("rect")
				 .attr("x", x+width)
				 .attr("y",  y+height)
				 .attr("width", width)
				 .attr("height", height)
				 .attr("fill", color)
				 .attr("stroke","#000080")
				 .attr("stroke-width",1);

});

</script>
<meta http-equiv="refresh" content="5">
</head>
<body>
</body>
</html>
