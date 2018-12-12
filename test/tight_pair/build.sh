rm -rf Build;
cmake -H. -BBuild -DCMAKE_EXPORT_COMPILE_COMMANDS=1
cd Build;
make -j10;
..
