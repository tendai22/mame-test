#! /bin/sh
MF=./build/projects/mame/gmake-linux/mame.make
cat  $MF |
sed 's/-lshared //g
    s/-lportmidi //g
    s/-lbgfx/ /g' > tmp.$$
mv tmp.$$ $MF
