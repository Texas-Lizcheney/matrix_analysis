make clean
rm -r ./build/
rm -r ./libs/
mkdir libs
make
python ./py/setup.py build_ext --inplace
mv matrixcore.cpython-311-x86_64-linux-gnu.so ./matrix_analysis
python -m unittest