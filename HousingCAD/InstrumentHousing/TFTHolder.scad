/* Holder for AdaFruit 1596: 5" TFT screen and Adafruit 1590: RA8875 driver board*/

 
//$vpd=400; //Viewport, defines the zoom factor after auto reload
$fn=80; //High res
//$fn = 20; //Low res for testing

//Dimensions
t_top   = 2.5; //Thickness of top cover (original perspex was 3 mm)

//Print
if(true){
     rotate([0,180,0]){
	  translate([0,0,-t_top/2]) top_cover();
	  bottom_holder();
	  }
     }

//View
if(false){
     translate([0,0,-t_top/2]) top_cover();
     bottom_holder();
}

module top_cover(){
     union(){
	  top();
	  // Close holes
	  translate([ 63.9, 35.3, -t_top/2]) cylinder(h=t_top, r=2);
	  translate([ 63.9,-35.3, -t_top/2]) cylinder(h=t_top, r=2);
	  translate([-63.9, 35.3, -t_top/2]) cylinder(h=t_top, r=2);		
	  translate([-63.9,-35.3, -t_top/2]) cylinder(h=t_top, r=2);
     }
}

module bottom_holder(){
     intersection(){
	  cube(size = [127,83,30], center=true);
	  hlt();
	  }
}


module top() {
     linear_extrude(height = t_top, center = true, convexity = 10)
	  translate([0,-100,0]) import (file = "top_s_filled.svg", center=true);
}


module hlt() {
     /* Thanks to Johnny : https://www.thingiverse.com/thing:4287980*/
     translate([-144.5 - 70,-10 -45,0]) import(file = "Halterung_V5.4.stl", center=true);
	  }

module cuvette_insert(Wcuv,slit_H) {
     radius=coil_r-T;
     difference()
     {
	  translate([-WcuvOut/2, -WcuvOut/2, -radius+T])
	  {/*Hollow cube with inner ridge Wcuv and with bottom */
	       /* such that distance to center of slits is slit_H */
	       difference(){
		    cube([WcuvOut, WcuvOut, Hcuv], center=false);

		    /* hollow inside */
		    translate([(WcuvOut-Wcuv)/2,
			       (WcuvOut-Wcuv)/2,
			       radius-T-slit_H])
			 cube([Wcuv, Wcuv, Hcuv+2],
			      center=false);

		    /* text on side */
		    translate([WcuvOut-0.49,WcuvOut/2,Hcuv-3])
			 rotate([90,0,90])
			 textemboss(str(Wcuv));
		    translate([0.49,WcuvOut/2,Hcuv-3])
			 rotate([90,0,-90])
			 textemboss(str(slit_H));

		    /* small holes for easy removal */
		    translate([Wcuv/2+T/2,0,Hcuv-4])
			 rotate([90,0,0])
			 cylinder(h=2*T,r1=1,r2=1,center=true);
		    translate([Wcuv/2+T/2,Wcuv,Hcuv-4])
			 rotate([90,0,0])
			 cylinder(h=2*T,r1=1,r2=1,center=true);
	       }		    
	  }
	  union(){
	       // LED slit (entry for light)
	       translate([Wcuv/2, 0 ,0])
		    cube([2*T+1, slit1_W+play,
			  slit1_H+play], center=true);
	       //Photodiode slit
	       translate([-Wcuv/2-T/2,0, slit1_H/2+slit_diff/2])
		    translate([1, 0, -slit2_dist/2])
		    cube([T+1, slit2_W+play, slit2_H+play], center=true);
	       //Scatter diode slit
	       translate([0, (Wcuv/2+1.5*T)-1 ,0])
		    rotate([90,90,0])
		    cylinder(h=3*T, r1=radius_photo2+play,
			     r2=radius_photo2+play, center=true);
	  }
     }
}

module textemboss(string) {
     linear_extrude(height = 0.5) {
	  text(string, size = 4, halign = "center", valign = "center", $fn = 16);}
}



