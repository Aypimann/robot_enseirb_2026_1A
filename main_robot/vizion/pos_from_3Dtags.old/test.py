import pos_from_3Dtags as pft

print("add_th_point")
pft.add_th_point ( 4,        0, 1)
pft.add_th_point (-2,-2*3**0.5, 1)
pft.add_th_point (-2, 2*3**0.5, 1)
pft.add_th_point ( 0,        0,-3)

print("add_obs_point")
pft.add_obs_point (-1,-2*3**0.5, 1)
pft.add_obs_point (-1, 2*3**0.5, 1)
pft.add_obs_point ( 5,        0, 1)
pft.add_obs_point ( 1,        0,-3)

print("compute")
rot = pft.compute()
print(rot)

print("flush_obs_point")
pft.flush_obs_point()
pft.flush_th_point()
print("end")

