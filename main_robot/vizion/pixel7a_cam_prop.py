# for pixel 7a x1, rear camera
print("!! camera proprieties are inexact")
pixel_per_radian = 2720 # in the center
center = (1134.0, 2016.0) # half resolution

def correct_image_deform(coordonates : list):
    return [(float(x), float(y)) for x, y in coordonates]

