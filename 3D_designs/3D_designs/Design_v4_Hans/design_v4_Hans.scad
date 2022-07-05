$vpd=100;
$fn=80;

T = 3; //wall thickness in mm
depth_coil=5;

coil_w = 10;
coil_h = 5;
coil_r = 21;
coil_r1 = 22+9-depth_coil;
coil_r2 = 8;
hole_r = 0;
coil_spacing = 23.1;
//coil_spacing = 72;

play = 0.2;
play_cuvette = 0.3;

Wcuv = 12.1; //cuvette width
Hcuv = 30; //cuvette holder height

//translate([-50,0,0]) rotate([180,0,0]) rotate([0,-90,0]) coil_holder();
//translate([0,80,0]) rotate([0,90,-90]) cam_tube_holder();

    translate([80,0,0])
    union(){
    
        rotate([0,-90,0])   translate([0,0,coil_spacing+0.5*coil_r-2*T])  coil_spindle(coil_w, coil_h, coil_r, coil_r-2*T); //LED coil
                //add LED holder
            LEDtube_L = 30;
            translate([LEDtube_L/2+frame_len/2-T,0,0]) rotate([0,90,0])
            union(){
                //create extension tube        
                cylinder_hollow(LEDtube_L, coil_r-2*T, coil_r-3*T, true);
                LED_d = 5;
                difference(){
                    //lid of extension tube
                    translate([0, 0, LEDtube_L/2-T/2]) cylinder(h=T, r1=coil_r-2*T, r2=coil_r-2*T, center=true);
                    //hole for LED
                    translate([0, 0, LEDtube_L/2-T/2]) cylinder(h=T, d1=LED_d, d2=LED_d, center=true);
                }
            }
            
        }



difference(){
    union(){
        
        translate([0,0,-10-T]) cuvette_tube();        
        
        //coil sets
        rotate([0,0,0])     translate([0,0,-coil_spacing])  coil_spindle(coil_w, coil_h, coil_r, hole_r); //bottom coil
        rotate([0,180,0])   translate([0,0,-coil_spacing])  coil_spindle(coil_w, coil_h, coil_r, coil_r-2*T); //top coil (with lid hole)
        rotate([0,90,0])    translate([0,0,-coil_spacing])  coil_spindle(coil_w, coil_h, coil_r, hole_r);  //PD coil
        
        rotate([90,90,0])    translate([0,0,-coil_spacing])  coil_spindle(coil_w, coil_h, coil_r, hole_r);  //camera coil

        rotate([-90,-90,0]) translate([0,0,-coil_spacing])  coil_spindle(coil_w, coil_h, coil_r, hole_r); //non-camera coil
        
        //top lid
        translate([0,0,50]) top_lid();
        
        translate([0,0,-37.7]) rotate([0,180,0]) 
        bottom_lid();

        //coil frame (cuvette holder excluded)
        frame_len = 72;
        difference(){
            coil_frame(frame_len);
            //cut out cuvette
            translate([0,0,-10-T]) cuvette_cube();
            //holes
            holeD = 0.9;
            hole_spacing = 23.1;
            rotate([0,0,0])     translate([0,0,-hole_spacing])  coil_holes(coil_w, coil_h, coil_r, hole_r, holeD); //bottom coil
            rotate([0,180,0])   translate([0,0,-hole_spacing])  coil_holes(coil_w, coil_h, coil_r, coil_r-2*T, holeD); //top coil (with lid hole)
            rotate([0,90,0])    translate([0,0,-hole_spacing])  coil_holes(coil_w, coil_h, coil_r, hole_r, holeD);  //PD coil
            rotate([0,-90,0])   translate([0,0,-hole_spacing])  coil_holes(coil_w, coil_h, coil_r, coil_r-2*T, holeD); //LED coil
            rotate([90,90,0])   translate([0,0,-hole_spacing])  coil_holes(coil_w, coil_h, coil_r, coil_r-2*T, holeD); //camera coil
            rotate([-90,-90,0]) translate([0,0,-hole_spacing])  coil_holes(coil_w, coil_h, coil_r, hole_r, holeD); //non-camera coil
        }          
    }
//    
    //camera hole
    translate([0, 20, 0]) cube([1.3*Wcuv, 20, 1.6*Wcuv], center=true);
    
    //camera hole other side
    translate([0, -20, 0]) cube([1.3*Wcuv, 20, 1.6*Wcuv], center=true);

//    
    //open up structure
    //rotate([0,0,90]) translate([0,25,0]) cube([140,50,120],center=true);
}

module top_lid(){
    
    union(){
        translate([0,0,0]) hex_hollow(8, T, coil_r-T, 0);
        translate([0,0,-1.5*T]) hex_hollow(8, 2*T, coil_r-2*T-play/2, coil_r-3*T-play/2);
    }
    
}

module coil_holder(){
   difference(){
        //cylinder(h=T, r1=coil_r-T, r2=coil_r-T, center=true);
        polyN(8, T*2, coil_r-T);
        holeD = 0.9;
        translate([0,0,10]) coil_holes(coil_w, coil_h, coil_r, hole_r, holeD);
    }
    polyN = 7;
    polyH = 30;
    polyR = 3;
    translate([0,0,polyH/2+T/2]) polyNflat(polyN, polyH, polyR);
}
module coil_frame(frame_len){
    
    difference(){
        union(){
            //basic structure
            difference(){
                //outer structure
                union(){
                    rotate([0,0,0])     polyN(8, frame_len, coil_r-T);
                    rotate([0,90,0])    polyN(8, frame_len, coil_r-T);
                    rotate([90,0,0])    polyN(8, frame_len, coil_r-T);
                }
                //free inner structure
                union(){
                        rotate([0,0,0])     polyN(8, frame_len, coil_r-2*T); 
                        rotate([0,90,0])    polyN(8, frame_len, coil_r-2*T); 
                        rotate([90,0,0])    polyN(8, frame_len, coil_r-2*T); 
                }
                
            }  
            
            //create support for cuvette holder
            difference(){
                rotate([0,90,0]) polyN(8, frame_len, coil_r-T);
                rotate([0,90,0]) polyN(8, frame_len, coil_r-2*T);
                translate([0,0,15]) cube(Wcuv, center=true);
            }
            translate([7,0,0]) rotate([0,90,0]) polyN(8,T,coil_r-2*T);
            

        }
        
        //drill cable holes
        hole_d = 3;
        hole_h = T+2;
        translate([-20,0,-coil_r+1.5*T]) cylinder(h=hole_h, d1=hole_d, d2=hole_d, center=true);
        translate([20,0,-coil_r+1.5*T]) cylinder(h=hole_h, d1=hole_d, d2=hole_d, center=true);  
    }
    
}



module coil_spindle(coil_w, coil_h, coil_r, hole_r){   
   
    translate([0,0,-coil_w/2-T])
    union(){
        
        //major cylinder
        cylinder_hollow_hex(coil_w+2*T, coil_r, coil_r-T+play/2, true);
        
        //tapered edge on one side
        intersection(){
            translate([0,0,coil_w/2+T/2]) cylinder_hollow(T, coil_r+coil_h, coil_r-T+play, true);
            translate([0,0,coil_w/2+T/2]) cylinder(h=T, r1=coil_r+coil_h, r2 = coil_r+coil_h-T, center=true);
        }
        
        
        //holed edge on other side
        difference(){
            translate([0,0,-coil_w/2-T/2]) cylinder_hollow_hex(T, coil_r+coil_h, hole_r, true);

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
        rotate([0,0,i*thetaStep]) translate([r,0,-coil_w-T+screwLen/2]) cylinder(h=screwLen, d1=hole_screwD, d2=hole_screwD, center=true);
    }
    
    //screw holes for coil
    hole_screwD1 = holeD;
    screwLen1 = 8;
    Nscrew1 = 1;
    thetaStep1 = 360/Nscrew1;
    for(i=[0:Nscrew1-1]){
        r = coil_r1-1.5*T;
        rotate([0,0,i*thetaStep1]) translate([r,0,-coil_w-T+screwLen/2]) cylinder(h=screwLen1, d1=hole_screwD1, d2=hole_screwD1, center=true);
    }
    
        //screw holes for wiring
    hole_screwD2 = 6;
    screwLen2 = 8;
    Nscrew2 = 1;
    thetaStep2 = 360/Nscrew2;
    for(i=[0:Nscrew2-1]){
        r = coil_r2-1.5*T;
        rotate([0,0,i*thetaStep2]) translate([r,0,-coil_w-T+screwLen2/2]) cylinder(h=screwLen2, d1=hole_screwD2, d2=hole_screwD2, center=true);
    }

}

module cylinder_hollow(height, radius_outer, radius_inner, cent){
    difference(){
        cylinder(h=height, r1=radius_outer, r2=radius_outer, center=cent);
        cylinder(h=height, r1=radius_inner, r2=radius_inner, center=cent);
    }
}

module cylinder_hollow_hex(height, radius_outer, radius_inner, cent){
    difference(){
        cylinder(h=height, r1=radius_outer, r2=radius_outer, center=cent);
        polyN(8, height, radius_inner);
    }
}
module hex_hollow(N, height, radius_outer, radius_inner){
    difference(){
        polyN(N, height, radius_outer);
        polyN(N, height, radius_inner);
    }
}

module cuvette_tube (  ) {

    difference(){

        //Basic tube
        translate([-Wcuv/2-T, -Wcuv/2-T, 0])
        difference(){
            cube([Wcuv+2*T, Wcuv+2*T, Hcuv+T], center=false);
            translate([T-play_cuvette/2, T-play_cuvette/2, T]) cube([Wcuv+play_cuvette, Wcuv+play_cuvette, Hcuv+1], center=false);
        }

        //Includes slits
        slit_H = 10;                    //height of slits (w.r.t. cuvette)
        slit2_H = 3;                 //secondary slit height
        slit2_W = 3;                    //secondary slit width
        slit2_dist = 3;                 //secondary slit spacing
        slit3_W=4;                      //third slit width
        slit3_H=4;                      //third slit height
        slit1_H = 5;                    //primary slit height
        slit1_W = 5;                    //primary slit width
        slit_diff=slit2_H-slit1_H;
        translate([0, 0, slit_H+T])
        union(){
            //Primary slit
            translate([Wcuv/2+T/2, 0 ,0]) 
            cube([T+1, slit1_W, slit1_H], center=true);

            //Scatter slit
            translate([0, (Wcuv/2+T/2) ,0]) 
            cube([T+1, slit1_W, slit1_H], center=true);
            
                //Delete hall sensor
            sensor_h = 30;
            sensor_l = 8;
            sensor_w = 8;
            translate([0,-Wcuv/2-T/2-0.5*sensor_l,0.7*sensor_h]) 
            cube([sensor_w, sensor_l, sensor_h], center=true);
            

            //secondary slit
            translate([-Wcuv/2-T/2,0, slit1_H/2+slit_diff/2])
                union(){
                translate([0, 0, -slit2_dist/2])   cube([T+1, slit2_W, slit2_H], center=true);
  //              translate([0, 0, slit2_dist/2])    cube([T+1, slit2_W, slit2_H], center=true);
               
            }
        }
    }
    
    //Add 'wings'
    wing_h = 28;
    wing_l = 4;
    wing_w = 8;
    translate([0,0,wing_h]) 
    difference(){
        cube([Wcuv+2*T+2*wing_l, wing_w, T], center=true);
        cube([Wcuv+play_cuvette, Wcuv+play_cuvette, T], center=true);
    }
}

module cuvette_cube (  ) {

    translate([-Wcuv/2-T-play/2, -Wcuv/2-T-play/2, 0])
    cube([Wcuv+2*T+play, Wcuv+2*T+play, Hcuv+T], center=false);
    
    //Add 'wings'
    wing_h = 28;
    wing_l = 4;
    wing_w = 8;
    translate([0,0,wing_h]) 
    cube([Wcuv+2*T+2*wing_l+play, wing_w+play, 2*T], center=true);
    


}

module polyNflat(N, H, Rflat){
    polyR = Rflat/cos(180/N);
    linear_extrude(height=H, center=true, convexity=0, twist=0, slices=0) circle(polyR, $fn=N);
}

module polyN(N, H, R){
    linear_extrude(height=H, center=true, convexity=0, twist=0, slices=0) circle(R, $fn=N);
}