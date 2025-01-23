
import os
import sys
import subprocess
import shutil

import distutils
import distutils.ccompiler

print(f'Compiler in use: {distutils.ccompiler.get_default_compiler()}')

cc = distutils.ccompiler.new_compiler()

cc.compile(
    ['silent-overhead.cpp'],
    output_dir=os.path.abspath(os.path.dirname(__file__)),
    include_dirs=['.'],
    extra_preargs=[
        '/std:c++17',
        # '/Wall', '/WX',
        '/EHsc', '-Zi',
    ],
    extra_postargs=[]
)
cc.link_executable(
    ['silent-overhead.obj'],
    'silent-overhead',
)

post_build_cmd = sys.argv[1:]
if len(post_build_cmd) > 0:
    print(f'Running {" ".join(post_build_cmd)}')
    subprocess.run(post_build_cmd)

    