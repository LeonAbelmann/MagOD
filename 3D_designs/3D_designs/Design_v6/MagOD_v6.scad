/* Magnetic OD meter
 Version 6: July 2019, Leon Abelmann
 Differences:
 - Double cuvette holder. Now you can take out inner cuvette without having to move the photodiode
 
 Version 5: May 2018, Leon Abelmann
 Differences with version 4 of Hans Keizer
 - Made top lid longer, to accomodate cuvettes with plug
 - Made the insert 0.3 mm bigger to 12.3, so that cuvettes slide in more easily
*/
 
$vpd=400; //Viewport, defines the zoom factor after auto reload
$fn=80; //High res
//$fn = 20; //Low res for testing

//General
T = 3; //wall thickness in mm
play = 0.2; // Space between the parts (so that they don't jam)

//Frame
frame_len = 72;//Length of the frame

//Cuvette
Wcuv = 12.5;         //cuvette width (can vary from cuvette to cuvette!)
play_cuvette = 0.3;  //how much room do you want around the cuvette
Hcuv = 40;           //cuvette holder height
WcuvOut = Wcuv+T;  //cuvette holder outer width
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
width_connector = 3; //width of Molex 4p connector
height_connector = 5;//height of Molex 4p connector
slit_diff=slit2_H-slit1_H; //Help

//Coils
depth_coil=5;
coil_w = 10;
coil_h = 5;
coil_r = 21;
coil_r1 = 22+9-depth_coil;
coil_r2 = 8;
hole_r = 0;
coil_spacing = 23.1;
//coil_spacing = 72;


// Generate entire design
difference() // Cut out box to see inside
{
     union(){
	  translate([0,0,0])   frame();
	  translate([0,0,0])   cuvette_insert(Wcuv,slit_H);
	  translate([50,0,0])  led_holder();
rotate([0,180,0]) translate([0,0,-coil_spacing]) top_coil()  ; //top coil (with lid hole)
rotate([0,0,0]) translate([0,0,-coil_spacing])  coil(); //bottom coil
rotate([0,90,0]) translate([0,0,-coil_spacing])  coil();  //PD coil
rotate([90,90,0]) translate([0,0,-coil_spacing])  coil();  //camera coil
rotate([-90,-90,0]) translate([0,0,-coil_spacing])  coil(); //non-camera coil
translate([0,0,60])    top_lid();
     }
     translate([-50,0,-50])
	  cube([50,100,150],center=false);
}

     
// Individual components in one field
/* translate ([0,0,36]) frame(); */
/* translate([0,65,15]) rotate([0,90,0]) led_holder(); */
/* translate([45,-50,15]) top_coil(); */
/* translate([-65,-65,15]) coil(); //need 4 of those */
/* translate([-65,-10,15]) coil(); */
/* translate([-65,45,15]) coil(); */
/* translate([-65,100,15]) coil(); */
/* translate([0,-55,slit_H+T+slit1_H/2]) cuvette_insert(); */
/* translate ([56,0,T/2]) rotate([180,0,0]) top_lid(); // need 2 of those */
/* translate ([56,40,0]) rotate([180,0,0]) top_lid(); // need 2 of those */

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
     difference(){
	  union(){
	       difference(){
		    // Hexagons for coils and cuvette holder
		    radius=coil_r+T;
		    union(){
			 coil_frame(frame_len);
			 cuvette_holder();
			 }
		    //cut out holes
		    holeD = 0.9;
		    hole_spacing = 23.1;
		    //cut out space for:
		    //bottom coil
		    rotate([0,0,0])
			 translate([0,0,-hole_spacing])
			 coil_holes(coil_w, coil_h, coil_r, hole_r, holeD);
		    //top coil with lid hole
		    rotate([0,180,0])
			 translate([0,0,-hole_spacing])
			 coil_holes(coil_w, coil_h, coil_r, coil_r-2*T, holeD);
		    //PD coil
		    rotate([0,90,0])
			 translate([0,0,-hole_spacing])
			 coil_holes(coil_w, coil_h, coil_r, hole_r, holeD);
		    //LED coil
		    rotate([0,-90,0])
			 translate([0,0,-hole_spacing])
			 coil_holes(coil_w, coil_h, coil_r, coil_r-2*T, holeD);
		    //camera coil
		    rotate([90,90,0])
			 translate([0,0,-hole_spacing])
			 coil_holes(coil_w, coil_h, coil_r, coil_r-2*T, holeD);
		    //non-camera coil
		    rotate([-90,-90,0])
			 translate([0,0,-hole_spacing])
			 coil_holes(coil_w, coil_h, coil_r, hole_r, holeD); 
	       }          
	  }
     }
}//End frame


// Coil with led holder
module led_holder(){
     union(){
	  rotate([0,-90,0])
	       translate([0,0,coil_spacing+0.5*coil_r-2*T])
	       coil_spindle(coil_w, coil_h, coil_r, coil_r-2*T); //LED coil
	  //add LED holder
	  LEDtube_L = 20;
	  translate([-LEDtube_L/2+coil_h+T,0,0])
	       rotate([0,90,0])
	       union(){
	       //create extension tube        
	       cylinder_hollow(LEDtube_L, coil_r-2*T, coil_r-3*T, true);
	       LED_d = 5;
	       difference(){
                    //lid of extension tube
                    translate([0, 0, LEDtube_L/2-T/2])
			 cylinder(h=T, r1=coil_r-2*T, r2=coil_r-2*T, center=true);
                    //hole for LED
                    translate([0, 0, LEDtube_L/2-T/2])
			 cylinder(h=T+1, d1=LED_d, d2=LED_d, center=true);
	       }
	  }
     }
}//end module ledholder

// Coil on top with opening for lid
module top_coil(){
     coil_spindle(coil_w, coil_h, coil_r, coil_r-2*T);
}

module top_lid(){
     top_lid_height = 15; //Increase to accommodate for cuvette with plug
     union(){
	  polyN(8, T, coil_r-T);
	  translate([0,0,-top_lid_height/2])
	       hex_hollow(top_lid_height, coil_r-2*T-play/2,
			  coil_r-3*T-play/2);
     }
}

// Coil other coils without special treatment
module coil(){
     coil_spindle(coil_w, coil_h, coil_r, hole_r);
}



// *************************** subunits *********************************
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
			      polyN(8, frame_len, coil_r-T);
			 rotate([0,90,0])
			      polyN(8, frame_len, coil_r-T);
			 rotate([90,0,0])
			      polyN(8, frame_len, coil_r-T);
		    }
		    //free inner structure
		    union(){
			 rotate([0,0,0])
			      polyN(8, 1.1*frame_len, coil_r-2*T);
			 // 1.1 to make subtraction nicer
			 rotate([0,90,0])
			      polyN(8, 1.1*frame_len, coil_r-2*T); 
			 rotate([90,0,0])
			      polyN(8, 1.1*frame_len, coil_r-2*T);
		    }  
	       }  	       
	  }
	  
	  //holes for Molex 4p connectors
	  translate([-20,0,-coil_r+1.5*T])
	       cube([width_connector, height_connector, T+2],center=true);
	  translate([20,0,-coil_r+1.5*T])
	       cube([width_connector, height_connector, T+2],center=true);
	  translate([0,20,-coil_r+1.5*T])
	       rotate([0,0,90])
	       cube([width_connector, height_connector, T+2],center=true);
	  translate([0,-20,-coil_r+1.5*T])
	       rotate([0,0,90])
	       cube([width_connector, height_connector, T+2],center=true);
	  translate([0,-coil_r+1.5*T,20])
	       rotate([90,90,0])
	       cube([width_connector, height_connector, T+2],center=true);
	  translate([0,-coil_r+1.5*T,-20])
	       rotate([90,90,0])
	       cube([width_connector, height_connector, T+2],center=true);
     }
}


module coil_spindle(coil_w, coil_h, coil_r, hole_r){   
     translate([0,0,-coil_w/2-T])
	  union(){
	  //major cylinder
	  cylinder_hollow_hex(coil_w+2*T, coil_r, coil_r-T+play/2, true);
	  //tapered edge on one side
	  intersection(){
	       translate([0,0,coil_w/2+T/2])
	  	    cylinder_hollow(T, coil_r+coil_h, coil_r-T+play, true);
	       translate([0,0,coil_w/2+T/2])
	  	    cylinder(h=T, r1=coil_r+coil_h, r2 = coil_r+coil_h-T, center=true);
	  }
	  //holed edge on other side
	  difference(){
	       translate([0,0,-coil_w/2-T/2])
	  	    cylinder_hollow_hex(T, coil_r+coil_h, hole_r, true);
	       coil_holes(coil_w, coil_h, coil_r, hole_r, 1.2);
	  }
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
     
     //screw holes for coil
     hole_screwD1 = holeD;
     screwLen1 = 8;
     Nscrew1 = 1;
     thetaStep1 = 360/Nscrew1;
     for(i=[0:Nscrew1-1]){
	  r = coil_r1-1.5*T;
	  rotate([0,0,i*thetaStep1])
	       translate([r,0,-coil_w-T+screwLen/2])
	       cylinder(h=screwLen1, d1=hole_screwD1,
			d2=hole_screwD1, center=true);
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

/* Designed to have different inserts for differen cuvette sizes Wcuv and
 different distances between bottom insert and photodiode slits slit_H*/
// Wcuv: space left for the cuvette. The Hellma are 12.4 mm.
// Slit_H
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

module cuvette_holder (  ) {
     radius=coil_r-T;
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

	       // Slit for NTC
	       NTC_t = 0.8;  //Thickness of NTC sensor
	       NTC_d = 8.5;  //Diameter of NTC sensor
	       NTC_l = hw-1;   //Depth of NTC sensor (how far you can stick it in
	       translate([NTC_d/2,-radius+NTC_l-0.1,0])
		    rotate([0,0,180])
		    cube([NTC_d,NTC_l,NTC_t],center=false);

	       // Hole in the bottom for molex connector or shaking pin
	       translate([0,0,-radius+T/2])
	       cube([width_connector, height_connector, T+1],center=true);

	  }
     }
}

module polyNflat(N, H, Rflat){
     polyR = Rflat/cos(180/N);
     linear_extrude(height=H, center=true, convexity=0, twist=0, slices=0) circle(polyR, $fn=N);
}

module polyN(N, H, R){
     linear_extrude(height=H, center=true, convexity=0, twist=0, slices=0) circle(R, $fn=N);
}

