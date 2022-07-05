/* Magnetic OD meter
 Version 7.2: Nov 2020, Leon Abelmann
 Enlarged openings for pico adapters
 Extruded bottom part so that wires can be guided out
 Enlarged slit for NTC
 
 Version 7.1: July 2020, Leon Abelmann
 Differences: Reduced coil diameter a little, so that the coils fit better. Also got rid of guessed length for frame, now it is nicely calculated.

Version 7:
- Adapted coil holder for Janzten Audio 000-0996 2.9 mH 2.1 Ohm coils

 Version 6:
 - Double cuvette holder. Now you can take out inner cuvette without having to move the photodiode
 
Version 5: May 2018, Leon Abelmann
 Differences with version 4 of Hans Keizer
 - Made top lid longer, to accomodate cuvettes with plug
 - Made the insert 0.3 mm bigger to 12.3, so that cuvettes slide in more easily
*/
 
$vpd=300; //Viewport, defines the zoom factor after auto reload
$fn=80; //High res
//$fn = 20; //Low res for testing

//General
T = 3; //wall thickness in mm
play = 0.4; // Space between the parts (so that they don't jam)

//Coils
coil_w = 20.5;//Width of the coil, so space available for the windings
coil_h = 6;   // Height of the coil windings
coil_r = 20.7;  // Inner diameter of coils
hole_r = 0.8; //Radius of holes for wiring

//Frame
frame_len = 2*(coil_w+coil_r+2*T-play);//Length of the frame
hex_r = 21; /* Radius of the hexagons that hold the coils. Should be smaller than the coil inner radius coil_r! */

//Cuvette
Wcuv = 12.6;         //cuvette width (can vary from cuvette to cuvette!)
play_cuvette = 0.3;  //how much room do you want around the cuvette
Hcuv = 40;           //cuvette holder height
WcuvOut = Wcuv+T;    //cuvette holder outer width
slit_H = 10;         //height of slits (w.r.t. cuvette)
slit1_H = 5;         //LED slit height
slit1_W = 5;         //LED slit width
slit2_H = 3;         //Photodiode slit height
slit2_W = 3;         //Photodiode slit width
slit2_dist = 3;      //secondary slit spacing
slit3_W = 4;         //Scatter diode slit width
slit3_H = 4;         //Scatter diode slit height
radius_photo = 4.1;  //radius of photodiode
radius_photo2 = 3;   //radius of photodiode
radius_photo3 = 4.2; //radius of photodiode
LEDtube_L = 3+16.5;      //height of the tube that holds the LED assembly (W+height LED assembly print - thickness one print)
width_pico4 = 3; //width of Molex Pico-Clasp 4p connector
height_pico4 = 5;//height of Molex Pico-Clasp 4p connector
width_pico6  = 5; //width of Molex Pico-Clasp 6p connector
height_pico6 = 8; //height of Molex Pico-Clasp 6p connector
NTC_t = 1.0;  //Thickness of NTC sensor
NTC_d = 8.5;  //Diameter of NTC sensor
width_NTC = 3;//Width of NTC sensor
height_NTC = NTC_d+1;//Height of NTC sensor


slit_diff=slit2_H-slit1_H; //Help
	  

/* for printing */
coil_shift = coil_r+T+1;// how far out the coils are retracted
show_bottom = true; // Show coil closing rings

// Individual components in one field
if(true){
     //Print area for MarkedForged Onyx (320x132x154mm)
     //color("black") cube([320,132,1],center=true);
     
     // Shift 1: Functional parts
     //translate([10,0,frame_len/2]) frame();
     //translate([-70,30,0]) led_holder();
     //translate([-70,-30,0]) coil(); // need three of those
     //translate([-130,30,0]) coil();
     //translate([-130,-30,0]) top_coil();
     //translate([90,30,0]) coil();
     //translate([90,-30,0]) bottom_coil();
     //translate([140,30,0]) top_lid();
     //translate([140,-30,slit_H+T+slit1_H/2]) cuvette_insert(Wcuv,slit_H);
     translate([0,15,0]) top_lid();
     translate([0,-15,slit_H+T+slit1_H/2]) cuvette_insert(Wcuv,slit_H);

     
     // Shift 2: Coil covers
     xoff = 70;
     //yoff = 30;//For MarkedForged
     yoff = 30 + 60;//For Thingiverse
     //translate([-xoff,-yoff,0]) coil_spindle_bottom();
     //translate([-xoff, yoff,0]) coil_spindle_bottom();
     //translate([0, yoff,0]) coil_spindle_bottom();
     //translate([0,-yoff,0]) coil_spindle_bottom();

     //translate([xoff, yoff,0]) coil_spindle_bottom();
     //translate([xoff,-yoff,0]) coil_spindle_bottom();

}


// Generate entire design
if(false){
     difference() // Cut out box to see inside
     {
	  union(){
	       color("black") translate([0,0,0])
		    frame();
	       color("blue") translate([0,0,0])
		    cuvette_insert(Wcuv,slit_H);
	       translate([coil_shift,0,0]) rotate([0,90,0])
		    led_holder();
	       translate([0,0,coil_shift])
		    top_coil()  ; //top coil (with lid hole)
	       translate([0,0,-coil_shift]) rotate([180,0,0])
		    bottom_coil(); //bottom coil (with hole for mounting)
	       translate([0,-coil_shift,0]) rotate([90,0,0])
	       	    coil();  // NTC
	       translate([0,+coil_shift,0]) rotate([-90,0,0])
	       	    coil();  // scatter
	       translate([-coil_shift,0,0]) rotate([0,-90,0])
	       	    coil(); //photo-diode
	       translate([0,0,60])    top_lid();
	  }
	  translate([0,0,-90])
	       cube([150,150,180],center=false);
     }
}


//inspect frame with cross-section:
/* difference() */
/* { */
/*      union() */
/* 	  { */
/* 	       frame(); */
/* 	       //translate([0,0,-slit_H]) */
/* 	       //cuvette_insert(); */
/* 	  } */
/*      translate([-50,-50,-50]) */
/*      //translate([-50,-50,-50]) */
/* 	  cube([100,50,100],center=false); */
/* } */



// frame (center part)
module frame(){
     union(){
	  coil_frame(frame_len);
	  cuvette_holder();
     }
}

// Coil with led holder
module led_holder(){
     union(){
	  coil_spindle(lid=false);
	  //add LED holder tube
	  translate([0,0,LEDtube_L/2+coil_w+T])
	       union(){
	       //create extension tubes
	       cylinder_hollow(LEDtube_L, coil_r-2*T, coil_r-3*T, true);
	       //backsidelid of extension tube
	       translate([0, 0, LEDtube_L/2-T/2])
		    cylinder(h=T, r1=coil_r-2*T, r2=coil_r-2*T,
			     center=true);
	       //frontside rim to attach to coil_spindle
	       translate([0,0,-LEDtube_L/2+T/2])
		    cylinder_hollow(T,coil_r-T/2-play,coil_r-2*T,true);
	  }
     }
}//end module ledholder

// Coil on top with opening for lid
module top_coil(){
     coil_spindle(lid=false);
}

/* Coil on bottom with hole for fixation entire frame. It extends a little to make room for the coil wires and has a 4 mm hole to fix the entire measurement head */
module bottom_coil(){
     difference(){
	  union(){
	       coil_spindle(lid=true);
	       t=1;
	       translate([0,0,coil_w+2*T+t/2]){
		    cylinder(h=t, r1=coil_r-T/2-play, r2=coil_r-T/2-play, center=true);
	       }
	  }
	  cylinder(h=100,r1=2,r2=2,center=true);//hole for mounting
     }
}
     
// Coil with no special function
module coil(){
     coil_spindle();
}


/* Cuvette insert */
/* Designed to have different inserts for differen cuvette sizes Wcuv and
 different distances between bottom insert and photodiode slits slit_H*/
// Wcuv: space left for the cuvette. The Hellma are 12.4 mm.
// Slit_H
module cuvette_insert(Wcuv,slit_H) {
     radius=hex_r-T;
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

module top_lid(){
     top_lid_height = 15; //Increase to accommodate for cuvette with plug
     translate([0,0,top_lid_height])
     	  union(){
     	       polyN(8, T, coil_r-T);
     	       translate([0,0,-top_lid_height/2])
     	       	    hex_hollow(top_lid_height, hex_r-2*T-play/2,
     	       		       hex_r-3*T-play/2);
	       knob();
     }
}



// *************************** subunits *********************************
// Hexagon frame that holds the coils
module coil_frame(frame_len){
     difference(){
	  union(){
	       //basic structure
	       difference()
	       //union()
	       {
		    //outer structure
		    union(){
			 rotate([0,0,0])
			      polyN(8, frame_len, hex_r-T);
			 rotate([0,90,0])
			      polyN(8, frame_len, hex_r-T);
			 rotate([90,0,0])
			      polyN(8, frame_len, hex_r-T);
		    }
		    //free inner structure
		    union(){
			 rotate([0,0,0])
			      polyN(8, 1.1*frame_len, hex_r-2*T);
			 // 1.1 to make subtraction nicer
			 rotate([0,90,0])
			      polyN(8, 1.1*frame_len, hex_r-2*T); 
			 rotate([90,0,0])
			      polyN(8, 1.1*frame_len, hex_r-2*T);
		    }  
	       }  	       
	  }
	  //holes for Molex 4p connectors (RefDiode side)
	  translate([21,0,-hex_r+1.5*T])
	       cube([width_pico6, height_pico6, T+4],center=true);

	  //hole for Molex 4p connector photodiode
	  translate([-21,0,-hex_r+1.5*T])
	       cube([width_pico4, height_pico4, T+4],center=true);
	  //hole for Molex 4p scatter photodiode
	  translate([0,21,-hex_r+1.5*T])
	       rotate([0,0,90])
	       cube([width_pico4, height_pico4, T+4],center=true);
	  //hole for NTC
	  translate([0,-20,-hex_r+1.5*T])
	       rotate([0,0,90])
	       cube([width_NTC, height_NTC, T+4],center=true);
	  /* translate([0,-hex_r+1.5*T,20]) */
	  /*      rotate([90,90,0]) */
	  /*      cube([width_connector, height_connector, T+2],center=true); */
	  /* translate([0,-hex_r+1.5*T,-20]) */
	  /*      rotate([90,90,0]) */
	  /*      cube([width_connector, height_connector, T+2],center=true); */

	  /* The PCB for the LED is inbetween the coil and the frame. Subtract 1.5 mm */
	  translate([(frame_len/2),0,0])
	       cube([1.5,100,100],center=true);
     }
}

module cuvette_holder (  ) {
     radius=hex_r-T;
     hw = radius-(WcuvOut+play)/2;//Wall thickness
     difference(){
	  //Center part with cut out for cuvette insert
	  difference(){
	       // Central block
	       union(){
		    polyN(8,2*radius,radius);
		    rotate([0,90,0]) polyN(8,2*radius,radius);
		    rotate([90,0,0]) polyN(8,2*radius,radius);
	       }
	       // Cut out holder
	       translate([-(WcuvOut+play)/2, -(WcuvOut+play)/2,-radius+T])
		    cube([WcuvOut+play, WcuvOut+play, 2*radius],
			 center=false);
	  }
	  
	  // Slit for the LED and photodiodes:
	  union(){
	       // Slit for LED
	       translate([radius-hw/2,0,0])
		    cube([hw+1, slit1_W, slit1_H], center=true);
	       
 	       //Photodiode slit plus seat
     	       union(){
		    //Seat for photodiode (1 mm deep)
		    seat=1;
     		    translate([-radius,0, 0])
     			 rotate([0,90,0])
     			 cylinder(h=2*seat, r1=radius_photo3,
				  r2=radius_photo3, center=true);
		    //Slit for photodiode
     		    translate([-radius,0, slit1_H/2+slit_diff/2])
     			 translate([hw/2, 0, -slit2_dist/2])
			 cube([hw+1, slit2_W, slit2_H], center=true);
	       }

	       //Scatter photodiode slit plus seat
     	       union(){
		    //Seat for scatter diode (0.5 mm deep)
		    seat=1;
     		    translate([0, radius ,0])
     			 rotate([90,0,0])
     			 cylinder(1, r1=radius_photo,
				  r2=radius_photo, center=true);
     		    //Slit for scatter diode
     		    translate([0, radius-hw/2 ,0])
     			 rotate([90,0,0])
     			 cylinder(h=hw+1, r1=radius_photo2,
				  r2=radius_photo2, center=true);
	       }
	  
     	       //Location for Hall sensor
     	       sensor_h = 30;
     	       sensor_l = 2;
     	       sensor_w = 8;
     	       translate([0,-WcuvOut/2-T/2-0.5*sensor_l,0.7*sensor_h])
     		    cube([sensor_w, sensor_l, sensor_h], center=true);

	       NTC_l = hw-1; /* Depth of NTC sensor (how far you can stick it in */
	       translate([NTC_d/2,-radius+NTC_l-0.1,0])
		    rotate([0,0,180])
		    cube([NTC_d,NTC_l,NTC_t],center=false);

	       // Hole in the bottom for molex connector or shaking pin
	       translate([0,0,-radius+T/2])
	       cube([width_pico4, height_pico4, T+1],center=true);

	  }
     }
}


module coil_holder(){
     difference(){
	  polyN(8, T*2, coil_r-T);
	  holeD = 0.9;
	  translate([0,0,10])
	       coil_holes(coil_w, coil_h, coil_r, hole_r, holeD);
     }
     polyN = 7;
     polyH = 30;
     polyR = 3;
     translate([0,0,polyH/2+T/2])
	  polyNflat(polyN, polyH, polyR);
}

/* The coil spindle is two parts that slide over each other (top and bottom), the coils is inbetween these two.*/

/* Top: If parameter lid is true, backside is closed.*/
module coil_spindle_top(lid){
     // Rotate and translate how you want it printed: (outcomment for design)
     rotate([180,0,0]) translate([0,0,-(coil_w+2*T)/2])
	  difference(){
	  union(){
	       // major cylinder
	       cylinder_hollow_hex(coil_w+2*T, coil_r, hex_r-T+play/2, true);
	       // tapered edge
	       intersection(){
		    translate([0,0,coil_w/2+T/2])
			 cylinder_hollow(T, coil_r+coil_h, coil_r-T+play,
					 true);
		    translate([0,0,coil_w/2+T/2])
			 cylinder(h=T, r1=coil_r+coil_h, r2 = coil_r+coil_h-T,
				  center=true);
	       }
	       if (lid){
		    // closing lid on back (optional)
		    translate([0,0,-coil_w/2-T])
			 cylinder_hollow(T, coil_r, 0,center=true);
	       }
	  }
	  /* Subtract cylinder for bottom part to slide on.
	     Has some extra play so that assembly is smooth */
	  translate([0,0,-coil_w/2])
	       cylinder_hollow(coil_w, coil_r+coil_h+1, coil_r-T/2-play,
			       true);
     }
}

module coil_spindle_bottom(){
     extra_height = 2; // to make room for a hole for the wiring
     difference(){
	  union(){
	       cylinder(coil_w/2+T,r=coil_r,true); // cylinder
	       cylinder(T, r=coil_r+coil_h + extra_height, true);// disk
	  }
	  translate([0,0,-1])
	       cylinder(coil_w,r=coil_r-T/2,true); //center hole
	  translate([coil_r+hole_r,0,-0.1])
	       cylinder(T+0.4,r=hole_r,true); // inner hole for wire   
	  translate([coil_r + coil_h - hole_r,0,-0.1])
	       cylinder(T+0.4,r=hole_r,true); // inner hole for wire   
     }
}


/* For printing, assemble coil top and bottom together */
module coil_spindle(lid=true, hole_radius=0){
     coil_spindle_top(lid,hole_radius);
     if (show_bottom) {
	  color("red")
	       rotate([180,0,0])
	       translate([0,0,-coil_w-2*T-1])
	       coil_spindle_bottom();
     }
}



module coil_holes(coil_w, coil_h, coil_r, hole_r, holeD){
     //screw holes
     hole_screwD = holeD;
     screwLen = 8;
     Nscrew = 4;
     thetaStep = 360/Nscrew;
     for(i=[0:Nscrew-1]){
	  r = coil_r-1.5*T;
	  rotate([0,0,i*thetaStep])
	       translate([r,0,-coil_w-T+screwLen/2])
	       cylinder(h=screwLen, d1=hole_screwD, d2=hole_screwD, center=true);
     }
     

     holeD=4;
     //hole for mounting bottom coil and pushing out PCBs
     translate([0,0,-coil_w+T])
	  cylinder(h=T+2, d1=holeD, d2=holeD, center=true);
}

module cylinder_hollow(height, radius_outer, radius_inner, cent){
     difference(){
	  cylinder(h=height, r1=radius_outer, r2=radius_outer, center=cent);
	  cylinder(h=1.1*height, r1=radius_inner, r2=radius_inner, center=cent);//1.1 to get cleaner difference
     }
}

module cylinder_hollow_hex(height, radius_outer, radius_inner, cent){
    difference(){
	 cylinder(h=height, r1=radius_outer, r2=radius_outer, center=cent);
	 polyN(8, 1.1*height, radius_inner);//1.1 for clean difference
    }
}

module hex_hollow(height, radius_outer, radius_inner, cent){
	  difference(){
	       polyN(8, height, radius_outer);
	       polyN(8, 1.1*height, radius_inner);
	  }
}


module textemboss(string) {
     linear_extrude(height = 0.5) {
	  text(string, size = 4, halign = "center", valign = "center", $fn = 16);}
}



//inspect cuvette_holder with cross-section:
/* difference() */
/* { */
/*      union() */
/* 	  { */
/* 	       cuvette_holder(); */
/* 	       //translate([0,0,-slit_H]) */
/* 	       cuvette_insert(); */
/* 	  } */
/*      translate([0,-25,-25]) */
/*      //translate([-50,-25,-25]) */
/* 	  cube([50,50,50],center=false); */
/* } */


module polyNflat(N, H, Rflat){
     polyR = Rflat/cos(180/N);
     linear_extrude(height=H, center=true, convexity=0, twist=0, slices=0) circle(polyR, $fn=N);
}

module polyN(N, H, R){
     linear_extrude(height=H, center=true, convexity=0, twist=0, slices=0) circle(R, $fn=N);
}

module knob(){
// https://www.thingiverse.com/thing:42220/files

// How long should the base be? (in mm)
01_stem_height = 5; // [0:40]
stem_height = 01_stem_height + 0.0;

// How thick should the base be? (in mm)
02_stem_diameter = 5; // [10:50]
stem_radius = 02_stem_diameter/2 + 0.0;

// How different is the top of the base compared to the bottom?
03_taper_amount = 0.9; // [1.5:A Lot Larger, 1.2:A Little Larger, 1:No Difference, 0.8:A Little Smaller, 0.5:A Lot Smaller]
taper_amount = 03_taper_amount + 0.0;

// Angled top and bottom edges on stem?
04_angled_edges = "No"; // [No, Yes]
stem_chamfer = str(04_angled_edges, "");

// Is the base Straight or Curved?
05_stem_shape = "Straight"; // [Straight, Curved]
stem_shape = str(05_stem_shape,"");

// How far does the screw stick out of the front of your cabinet? (in mm)
06_screw_depth = 0; // [3:15]
screw_depth = 06_screw_depth + 1.0;

// What is the diameter (width) of threaded part of the screw? (in mm)
07_screw_diameter = 0;
screw_radius = (07_screw_diameter+.75)/2.0;

// What type of ornament on the knob?
08_ornament_style = "UFO"; // [None, Ball, Mushroom, UFO, Blimp, Half Blimp, Square, Triangle, Rectangle, Circle, Hexagon]
ornament_style = str(08_ornament_style,"");

// How big do you want the ornament? (in mm)
09_ornament_size = 12; // [5:50]
ornament_size = (09_ornament_size/2) + 0.0;

// Special indent effect?
10_indent_ornament = "Bump"; // [None, Bump, Reverse]
indent_ornament = str(10_indent_ornament,"");

difference() {
	union() {
		if (stem_height > 0) {
			createStem();
		}
		if (ornament_style != "None") {
			if (ornament_style == "Ball") {
				createBallOrnament();
			}
			if (ornament_style == "Mushroom") {
				createHalfBallOrnament();
			}
			if (ornament_style == "UFO") {
				createUfoOrnament();
			}
			if (ornament_style == "Blimp") {
				createBlimpOrnament();
			}
			if (ornament_style == "Half Blimp") {
				createHalfBlimpOrnament();
			}
			if (ornament_style == "Triangle") {
				createShapeOrnament(3);
			}
			if (ornament_style == "Circle") {
				createShapeOrnament(36);
			}
			if (ornament_style == "Hexagon") {
				createShapeOrnament(6);
			}
			if (ornament_style == "Rectangle") {
				createRectangleOrnament();
			}
			if (ornament_style == "Square") {
				createShapeOrnament(4);
			}
		}
	}
	if ((stem_height > 0) || (ornament_style != "None")) {
		createScrewHole();
	}
}

module createStem() {
	difference() {
		cylinder(r1=max(stem_radius,screw_radius+1),r2=max(stem_radius*taper_amount,screw_radius+1),h=stem_height,$fn=48);
		
		if (stem_shape == "Curved") {
			translate([0,0,stem_height/1.55])
			rotate_extrude(convexity = 10, $fn=24)
			scale([1,(stem_height*0.775)/stem_radius,1])
			translate([stem_radius*1.65, 0, stem_height])
			circle(r = stem_radius, $fn=24);
		}
		
		if (stem_chamfer == "Yes") {
			// bottom
			rotate_extrude(convexity = 10, $fn=64)
			translate([stem_radius+1, 0, -10])
			circle(r = 2, $fn=6);
			// top
			rotate_extrude(convexity = 10, $fn=64)
			translate([(stem_radius+1)*taper_amount, stem_height, -10])
			circle(r = 2*taper_amount, $fn=6);
		}

		if (ornament_style == "None" && indent_ornament != "None") {
			if (indent_ornament == "Bump") {
				rotate_extrude(convexity = 10, $fn=64)
				translate([(stem_radius*taper_amount)-3, stem_height, -10])
				circle(r = 0.5, $fn=6);
			}
			if (indent_ornament == "Reverse") {
				translate([0,0,stem_height-0.5])
				cylinder(r1=stem_radius*taper_amount-3,r2=stem_radius*taper_amount-3,h=stem_height,$fn=48);
			
			}
		}
	}
}

module createScrewHole() {
	if (stem_height > 5 ) {
		translate([0,0,-2]) cylinder(r1=screw_radius,r2=screw_radius,h=min(screw_depth+2,stem_height),$fn=24);
	} else {
		translate([0,0,-80]) cylinder(r1=screw_radius,r2=screw_radius,h=85,$fn=24);
	}
}

module createBallOrnament() {
	if ((ornament_size) >= stem_radius*taper_amount)
	{
		translate([0,0,(stem_height+(sqrt(pow(ornament_size,2)-pow(stem_radius*taper_amount,2)) ) )])
		difference() {
		 union() {
				if (indent_ornament == "Bump") {
					sphere(r=ornament_size-1, $fn=36);
				}
			difference() {
				sphere(r=ornament_size, $fn=36);
				if (indent_ornament != "None") {
					translate([0,0,max(stem_height*0.5,ornament_size-1)]) sphere(r=ornament_size-1, $fn=36);
				}
			}
		}

		}
	}
 	else {
		translate([0,0,stem_height])
		 union() {
			if (indent_ornament == "Bump") {
				sphere(r=ornament_size-1, $fn=36);
			}
			difference() {
				sphere(r=ornament_size, $fn=36);
				if (indent_ornament != "None") {
					translate([0,0,10]) sphere(r=ornament_size-1, $fn=36);
				}
			}
		}
	}

}

module createHalfBallOrnament() {
	translate([0,0,stem_height])
	difference() {
		union() {
			if (indent_ornament == "Bump") {
				sphere(r=ornament_size-1, $fn=36);
			}
			difference() {
				sphere(r=ornament_size, $fn=36);
				if (indent_ornament != "None") {
					translate([0,0,ornament_size-1]) sphere(r=ornament_size-1, $fn=36);
				}
			}
		}
		translate([-35,-35,-70]) cube([70,70,70], center=false);
	}

}


module createUfoOrnament() {
	if ((ornament_size) >= stem_radius*taper_amount)
	{
		scale([1,1,0.2])
		translate([0,0,((stem_height*5)+(sqrt(pow(ornament_size,2)-pow(stem_radius*taper_amount,2)) ) )])
		union() {
			if (indent_ornament == "Bump") {
				sphere(r=ornament_size-1, $fn=36);
			}
			difference() {
				sphere(r=ornament_size, $fn=36);
				if (indent_ornament != "None") {
					translate([0,0,ornament_size-1]) sphere(r=ornament_size-1, $fn=36);
				}
			}
		}
	}
	else {
		scale([1,1,0.2])
		translate([0,0,stem_height*5])
		union() {
			if (indent_ornament == "Bump") {
				sphere(r=ornament_size-1, $fn=36);
			}
			difference() {
				sphere(r=ornament_size, $fn=36);
				if (indent_ornament != "None") {
					translate([0,0,ornament_size-1]) sphere(r=ornament_size-1, $fn=36);
				}
			}
		}
	}
}

module createBlimpOrnament() {
	translate([0,0,(stem_height+(sqrt(pow(ornament_size,2)-pow(stem_radius*taper_amount,2)) )/2 )])
	scale([1,0.5,0.5])
	union() {
		if (indent_ornament == "Bump") {
			sphere(r=ornament_size-1, $fn=36);
		}
		difference() {
			sphere(r=ornament_size, $fn=36);
			if (indent_ornament != "None") {
				translate([0,0,ornament_size-1]) sphere(r=ornament_size-1, $fn=36);
			}
		}
	}
}

module createHalfBlimpOrnament() {
		scale([1,0.5,0.5]) translate([0,0,stem_height*2])
		difference() {
		union() {
			if (indent_ornament == "Bump") {
				sphere(r=ornament_size-1, $fn=48);
			}
			difference() {
				sphere(r=ornament_size, $fn=48);
				if (indent_ornament != "None") {
					translate([0,0,ornament_size-1]) sphere(r=ornament_size-1, $fn=48);
				}
			}
		}
		translate([-35,-35,-70]) cube([70,70,70], center=false);
	}
}

module createShapeOrnament(sides, scale) {
	translate([0,0,stem_height])
	scale([scale,scale,1])
	difference() {
		cylinder(r=ornament_size, h=ornament_size/3, $fn=sides);
		if (indent_ornament != "None") {
			translate([0,0,ornament_size/3-1])
			difference() {
				cylinder(r=ornament_size-4, h=10, $fn=sides);
				if (indent_ornament == "Bump") {
					cylinder(r=ornament_size-6, h=10, $fn=sides);
				}
			}
		}
	}
}
module createRectangleOrnament() {
	translate([0,0,stem_height])
	difference() {
		translate([-ornament_size,-ornament_size/2,0])
		cube(size=[ornament_size*2,ornament_size,ornament_size/5], center=false);
		if (indent_ornament != "None") {
			translate([-ornament_size+2,-ornament_size/2+2,ornament_size/5-1])
			difference() {
				cube(size=[ornament_size*2-4,ornament_size-4,ornament_size/5], center=false);
				if (indent_ornament == "Bump") {
					translate([2,2,0])
					cube(size=[ornament_size*2-8,ornament_size-8,ornament_size/5], center=false);
				}
			}
		}
	}
}

}
