import detect_tag
import find_tag_3D
import numpy as np

arcuo_size = .1

path = "/home/aypiman/Documents/dev/robot 1A/main_robot/vizion/im0.jpg"

pixel_per_radian = 2268*0.739*1.623268
center = (2268/2, 4032/2)


detect_tag.import_frame_from_file(path)
detect_tag.find_tags()

print(detect_tag.tags[0][0][0])

X_c = [
    np.array((
        (x - center[0]) / pixel_per_meter_at_one_meter,
        (y - center[1]) / pixel_per_meter_at_one_meter,
        1.
    ))
    for x, y in detect_tag.tags[0][0][0]
]

X_c = [X_c[i] for i in (0, 1, 3, 2)]

for PX in X_c:
    print(PX)

print("\n","\n")

PX = [*find_tag_3D.compute_tag_points(*X_c, arcuo_size)]

PX = [PX[i] for i in (0, 1, 3, 2)]

d = [
    np.dot(
        PX[i] - PX[(i+1)%4],
        PX[(i+0)%4] - PX[(i+1)%4]
        )**0.5 
    for i in range(4)
]

print(d,"\n")

for PX in PX:
    print(PX)

