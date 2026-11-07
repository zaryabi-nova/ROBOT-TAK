// Chassis plate 150 x 110 x 3 mm (Chapter 6 sample) - OpenSCAD, parametric
// Open in OpenSCAD, press F6 (render), then File > Export > STL or 3MF.
L = 150; W = 110; T = 3; R = 10;       // plate size and corner radius (mm)
M3 = 3.4;                               // clearance hole for M3 screws
holes = [                               // [x, y, diameter] from the plate centre
  [-45,  45, M3], [-25,  45, M3], [-45, -45, M3], [-25, -45, M3],   // motor brackets
  [ 60,  15, M3], [ 60, -15, M3],                                    // caster wheel
  [-11.03, -24.11, M3], [-9.76, 24.15, M3], [41.04, -19.03, M3], [41.04, 8.91, M3],  // Arduino Uno
  [-50, 0, 12]                                                       // cable hole
];
$fn = 64;
difference() {
  linear_extrude(T)
    offset(r = R) square([L - 2*R, W - 2*R], center = true);       // rounded rectangle
  for (h = holes) translate([h[0], h[1], -1]) cylinder(d = h[2], h = T + 2);
}
