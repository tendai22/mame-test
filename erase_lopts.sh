#! /bin/sh
for MF in ./build/projects/sdl/mame/gmake-linux/mame.make ./build/projects/sdl/zexall/gmake-linux/zexall.make ./build/projects/sdl/emuz80/gmake-linux/emuz80.make
do  if test -f $MF; then
        cat  $MF |
        sed 's/-lshared //g
            s/-lportmidi //g
            s/-lfrontend //g
            s/-lbgfx/ /g' > tmp.$$
        mv tmp.$$ $MF
    fi
done
