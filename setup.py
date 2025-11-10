import setuptools
from pybind11.setup_helpers import Pybind11Extension, build_ext

#definition of C++ extension module

ext_modules=[
    Pybind11Extension(
        "recon",
        sources=['src/pybindings.cpp'],
        extra_compile_args=["-std=c++17"],
    ),
]

setuptools.setup(
    name="recon",
    version="1.0.0",
    ext_modules=ext_modules,
    cmdclass={"build_ext":build_ext},
    zip_safe=False,)