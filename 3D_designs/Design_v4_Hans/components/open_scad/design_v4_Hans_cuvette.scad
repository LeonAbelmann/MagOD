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

Wcuv = 13.3; //cuvette width
Hcuv = 30; //cuvette holder height

//translate([-50,0,0]) rotate([180,0,0]) rotate([0,-90,0]) coil_holder();
//translate([0,80,0]) rotate([0,90,-90]) cam_tube_holder();

 
difference(){
    union(){
        
        translate([0,0,-10-T]) cuvette_tube();              
    }
//    
    
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
        slit1_H = 6;                    //primary slit height
        slit1_W = 4;                    //primary slit width
        radius_photo=4.1;               //radius of photodiode
         radius_photo2=3;               //radius of photodiode
        radius_photo3=4.2;               //radius of photodiode
        slit_diff=slit2_H-slit1_H;
        translate([0, 0, slit_H+T])
        union(){
            //Primary slit
            translate([Wcuv/2+T/2, 0 ,0]) 
            cube([T+1, slit1_W, slit1_H], center=true);

            //Scatter slit holder
            union(){
                translate([0, (Wcuv/2+1.55*T) ,0]) 
            rotate([90,90,0])
            cylinder(h=2*T, r1=radius_photo, r2=radius_photo, center=true);
            //cube([T+1, slit1_W, slit1_H], center=true);
             //Scatter slit
            translate([0, (Wcuv/2+1.5*T) ,0]) 
            rotate([90,90,0])
            cylinder(h=4*T, r1=radius_photo2, r2=radius_photo2, center=true);
            //cube([T+1, slit1_W, slit1_H], center=true);
            }
                //Delete hall sensor
            sensor_h = 30;
            sensor_l = 8;
            sensor_w = 8;
            translate([0,-Wcuv/2-T/2-0.5*sensor_l,0.7*sensor_h]) 
            cube([sensor_w, sensor_l, sensor_h], center=true);
            

            //secondary slit
            union(){
            translate([-Wcuv/2-1.5*T,0, 0]) 
            rotate([0,90,0])
            cylinder(h=2*T, r1=radius_photo3, r2=radius_photo3, center=true);
         
            translate([-Wcuv/2-T/2,0, slit1_H/2+slit_diff/2])
            translate([0, 0, -slit2_dist/2])   cube([T+1, slit2_W, slit2_H], center=true);

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



