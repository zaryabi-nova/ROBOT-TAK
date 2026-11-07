# Chassis plate 150 x 110 x 3 mm (Chapter 6 sample) - FreeCAD macro
# FreeCAD: Macro > Macros... > select this file > Execute.
# Builds the part and saves STEP and STL next to this file.
import os
import FreeCAD as App
import Part

L, W, T, R = 150.0, 110.0, 3.0, 10.0
M3 = 3.4
HOLES = [(-45, 45, M3), (-25, 45, M3), (-45, -45, M3), (-25, -45, M3),   # motor brackets
         (60, 15, M3), (60, -15, M3),                                    # caster wheel
         (-11.03, -24.11, M3), (-9.76, 24.15, M3), (41.04, -19.03, M3), (41.04, 8.91, M3),  # Arduino Uno
         (-50, 0, 12)]                                                   # cable hole

doc = App.newDocument("ChassisPlate")
plate = Part.makeBox(L, W, T, App.Vector(-L / 2, -W / 2, 0))
vertical = [e for e in plate.Edges if abs(e.Vertexes[0].Z - e.Vertexes[1].Z) > 1e-6]
plate = plate.makeFillet(R, vertical)                       # round the four corners
for x, y, d in HOLES:
    plate = plate.cut(Part.makeCylinder(d / 2, T + 2, App.Vector(x, y, -1)))
obj = doc.addObject("Part::Feature", "ChassisPlate")
obj.Shape = plate
doc.recompute()

folder = os.path.dirname(os.path.abspath(__file__)) if "__file__" in globals() else App.getUserMacroDir(True)
plate.exportStep(os.path.join(folder, "ch06_chassis_plate.step"))
plate.exportStl(os.path.join(folder, "ch06_chassis_plate_freecad.stl"))
print("Saved STEP and STL in", folder)
