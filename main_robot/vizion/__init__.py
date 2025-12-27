import cam_to_2D_coners as cam_t2
import from_2D_to_3D as f2t3
print(53*'!')
print("!! camera setting are the defaut rear pixel 7a one !!")
print(53*'!')
from pixel7a_cam_prop import *

fix_arcuo_size = 100. # milimeters
print("fix tags are wrong !")
fix_tags = [0]

nut_arucos_size = 40.

def update_tags():
    cam_t2.capture_frame()
    cam_t2.find_tags()
    cam_t2.tags = None

def process_tags():
    for corners, tag_id in zip(cam_t2.tags[0], cam_t2.tags[1]):
        if tag_id in fix_tags:
            pass


if __name__ == "__main__":
    import os
    path = os.getcwd() + "/im2.jpg"
    cam_t2.import_frame_from_file(path)
    cam_t2.find_tags()

