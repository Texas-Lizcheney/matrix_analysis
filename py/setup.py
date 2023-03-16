from setuptools import setup, Extension


def main():
    Ext_matrix = Extension("matrixcore",
                           libraries=[":libmatrix.a",":libvar.a"],
                           sources=["srcs/stub.cc"],
                           language="c++",
                           extra_compile_args=["--std=c++20"],
                           library_dirs=["libs/"],
                           include_dirs=["include/"])
    setup(name="matrix_analysis", version="1.0.1", ext_modules=[Ext_matrix])


if __name__ == "__main__":
    main()
