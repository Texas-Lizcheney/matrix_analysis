python ./py/setup.py build_ext --inplace
mv core.cpython-311-x86_64-linux-gnu.so ./matrix_analysis
python ./test.py