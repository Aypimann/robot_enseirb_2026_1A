from setuptools import setup, Extension

setup(
  name="pos_from_3Dtags",
  version="0.1",
  ext_modules=[
    Extension(
      "pos_from_3Dtags",
      sources=[
        "./main_robot/vizion/pos_from_3Dtags/pos_from_3Dtags.c",
        "./main_robot/vizion/pos_from_3Dtags/core.c",
        "./main_robot/vizion/pos_from_3Dtags/rot.c",
        "./main_robot/vizion/pos_from_3Dtags/pos.c",
        "./main_robot/vizion/pos_from_3Dtags/node.c",
      ],
      depends=[
        "./main_robot/vizion/pos_from_3Dtags/core.h",
        "./main_robot/vizion/pos_from_3Dtags/rot.h",
        "./main_robot/vizion/pos_from_3Dtags/pos.h",
        "./main_robot/vizion/pos_from_3dtags/node.h",
      ],
      include_dirs=["./main_robot/vizion/compute_final_pos"] 
    )
  ]
)
