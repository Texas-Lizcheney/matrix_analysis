from setuptools import setup, Extension


def main():
    Ext = Extension("matrixcore", libraries=[":libmatrix.a"],
                    sources=["srcs/stub.cc"],
                    language="c++",
                    extra_compile_args=["--std=c++20"],
                    library_dirs=["libs/"],
                    include_dirs=["include/"])
    setup(name="matrix_analysis", version="1.0.0", ext_modules=[Ext])


if __name__ == "__main__":
    main()
