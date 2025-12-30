import find_rot as fr

fr.add_obs_point( 4,        0,-1)
fr.add_obs_point(-2, 2*3**0.5,-1)
fr.add_obs_point(-2,-2*3**0.5,-1)
fr.add_obs_point( 0,        0, 3)

fr.add_th_point ( 4,        0, 1)
fr.add_th_point (-2,-2*3**0.5, 1)
fr.add_th_point (-2, 2*3**0.5, 1)
fr.add_th_point ( 0,        0,-3)

fr.train()

mat = fr.get_final_rotation_matrix()
for i in range(0,9,3):
    print(mat[i:i+3])

