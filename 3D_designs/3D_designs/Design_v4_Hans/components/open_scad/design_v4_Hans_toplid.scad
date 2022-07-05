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






difference(){
    union(){
        //top lid
        translate([0,0,0]) top_lid();  
    }
}

module top_lid(){
    
    union(){
        translate([0,0,0]) hex_hollow(8, T, coil_r-T, 0);
        translate([0,0,-1.5*T]) hex_hollow(8, 2*T, coil_r-2*T-play/2, coil_r-3*T-play/2);
    }
    
}







module hex_hollow(N, height, radius_outer, radius_inner){
    difference(){
        polyN(N, height, radius_outer);
        polyN(N, height, radius_inner);
    }
}




module polyNflat(N, H, Rflat){
    polyR = Rflat/cos(180/N);
    linear_extrude(height=H, center=true, convexity=0, twist=0, slices=0) circle(polyR, $fn=N);
}

module polyN(N, H, R){
    linear_extrude(height=H, center=true, convexity=0, twist=0, slices=0) circle(R, $fn=N);
}