make clean
rm -r ./build/
rm -r ./libs/
mkdir -p libs
make
python ./py/setup.py build_ext --inplace
mv core.cpython-311-x86_64-linux-gnu.so ./matrix_analysis
cd ./py
python ./test.py
cd ..