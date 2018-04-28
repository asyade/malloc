echo GENERATING OBJ
gcc  -I./libft src/*.c -I./include/ -fPIC -c
echo OBJ GENERATED
rm -rf ./obj/*
mv *.o ./obj
echo GENERATING DLIB
gcc -shared -fPIC -Wl,-soname,libmalloc.so.1 obj/* -o  libmalloc.so.0.1 -lc
echo DLIB GENERATED
