import cam_to_2D_coners as cam_t2
import from_2D_to_3D as f2t3
import pos_from_3Dtags as pft
from pixel7a_cam_prop import *

fix_arcuo_size = 100. # milimeters
fix_tags = [0]
nut_arucos_size = 40.

expected_cam_pitch = 0
excepted_cam_roll = 0

cam_pitch_tolerance = 100
cam_roll_tolerance = 100

print("!! camera setting are the defaut rear pixel 7a one !!")
print("!! expected cam angle is unset !!")
print("!! fix tags are wrong !!")

fix_th_pos = {
    (0, 0) : (0.,-50.,-50.),
    (0, 1) : (0., 50.,-50.),
    (0, 2) : (0.,-50., 50.),
    (0, 3) : (0., 50., 50.)
}

def update_tags():
    cam_t2.capture_frame()
    cam_t2.find_tags()
    cam_t2.tags = (
        [correct_image_deform(t) for t in cam_t2.tags[0]],
        cam_t2.tags[1]
    )

def process_tags():
    fix_obs_pos = {}
    for corners, tag_id in zip(cam_t2.tags[0], cam_t2.tags[1]):
        if tag_id in fix_tags:
            Xc = im_coordonate_to_unscaled(corners)
            PX = project_on_tag(*Xc, fix_arcuo_size)
            print("deteced fix tag {tag_id} at 3D {PX}")
            for i in range(4):
                fix_obs_pos[(tag_id, i)] = PX[i]
    if len(fix_obs_pos) != 0:
        for key in fix_obs_pos:
            pft.add_th_point(*th_point)
            pft.add_obs_point(*obs_point)
        yaw, pitch, roll, x, y, z= pft.compute()
        pft.flush_th_point()
        pft.flush_obs_point()


if __name__ == "__main__":
    import os
    path = os.getcwd() + "/im2.jpg"
    cam_t2.import_frame_from_file(path)
    cam_t2.find_tags()

