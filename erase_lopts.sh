#! /bin/sh
MF=./build/projects/sdl/mame/gmake-linux/mame.make
cat  $MF |
sed 's/-lshared //g
    s/-lportmidi //g
    s/-lfrontend //g
    s/-lbgfx/ /g' > tmp.$$
mv tmp.$$ $MF
