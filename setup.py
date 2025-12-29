from setuptools import setup, Extension

setup(
  name="find_rot",
  version="0.1",
  ext_modules=[
    Extension(
      "find_rot",
      sources=[
        "./main_robot/vizion/find_rot/find_rot_py_module.c",
        "./main_robot/vizion/find_rot/core.c",
        "./main_robot/vizion/find_rot/node.c"
      ],
      depends=[
        "./main_robot/vizion/find_rot/find_rot_py_module.c",
        "./main_robot/vizion/find_rot/core.c",
        "./main_robot/vizion/find_rot/node.c"
        "./main_robot/vizion/find_rot/core.h",
        "./main_robot/vizion/find_rot/node.h",
      ],
      include_dirs=["./main_robot/vizion/find_rot"] 
    )
  ]
)
