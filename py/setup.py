from setuptools import setup, Extension
import numpy
import os

np_api = numpy.get_include()+"/numpy"


def main():
    Ext_matrix = Extension("core",
                           libraries=[":libvar.a",
                                      ":libmatrix.a",
                                      ":libvector.a",
                                      ":libutility.a",
                                      ":liberrordouble.a",
                                      ":libmodulemethods.a"],
                           sources=["srcs/stub.cc"],
                           language="c++",
                           extra_compile_args=["--std=c++20"],
                           library_dirs=[f"{os.getcwd()}/libs"],
                           include_dirs=[np_api, f"{os.getcwd()}/include"])
    setup(name="matrix_analysis", version="2.2.3", ext_modules=[Ext_matrix])


if __name__ == "__main__":
    main()
