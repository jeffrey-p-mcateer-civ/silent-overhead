
import os
import sys
import subprocess
import shutil
import traceback

import distutils
import distutils.ccompiler

try:
    repo_dir = os.path.abspath(os.path.dirname(__file__))
    print(f'repo_dir={repo_dir}')
    print(f'Compiler in use: {distutils.ccompiler.get_default_compiler()}')

    if os.path.exists(repo_dir) and os.path.isdir(repo_dir):
        os.chdir(repo_dir)

    cc = distutils.ccompiler.new_compiler()

    cc.compile(
        ['silent-overhead.cpp'],
        output_dir=os.path.abspath(os.path.dirname(__file__)),
        include_dirs=['.'],
        extra_preargs=[
            '/std:c++20',
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

except:
    msg = traceback.format_exc().lower()
    ignorable = 'compileerror' in msg
    if not ignorable:
        traceback.print_exc()
    sys.exit(1)
