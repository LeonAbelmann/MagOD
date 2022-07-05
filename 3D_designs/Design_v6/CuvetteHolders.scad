/* Magnetic OD meter
 Version 6: July 2019, Leon Abelmann
 Differences:
 - Double cuvette holder. Now you can take out inner cuvette without having to move the photodiode
 
 Version 5: May 2018, Leon Abelmann
 Differences with version 4 of Hans Keizer
 - Made top lid longer, to accomodate cuvettes with plug
 - Made the insert 0.3 mm bigger to 12.3, so that cuvettes slide in more easily
*/
 
//$vpd=400; //Viewport, defines the zoom factor after auto reload
$fn=80; //High res
//$fn = 20; //Low res for testing

//General
T = 3; //wall thickness in mm
play = 0.2; // Space between the parts (so that they don't jam)

//Cuvette
Wcuv = 12.4;       //cuvette width (now parameter)
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

// Individual components in one field
translate([0,0,slit_H+T+slit1_H/2])
cuvette_insert(12.5,10); // (Inner width, height center slit to bottom) 

translate([0,20,slit_H+T+slit1_H/2])
cuvette_insert(12.6,10); // (Inner width, height center slit to bottom)

translate([0,40,slit_H+T+slit1_H/2])
cuvette_insert(12.7,10); // (Inner width, height center slit to bottom)

translate([0,60,slit_H+T+slit1_H/2])
cuvette_insert(12.7,4); // (Inner width, height center slit to bottom)


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



