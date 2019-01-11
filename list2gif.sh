#!/usr/bin/env bash

# Converts the ascii art into gifs
# You may have to change things like delay/size/font to make it fit/look/work better
# This is more just a helper than anything

if [ -d tmp ]; then
    rm -r tmp
fi
mkdir tmp

for file in $@; do
    regex=$(head -n 1 $file)
    base=$(basename $file)
    echo $regex > tmp/$base
    cat $file >> tmp/$base
    # high amount of digits since we need them for convert
    gcsplit --digits=15  --quiet --prefix=tmp/$base.out tmp/$base "/$regex/+1" "{*}"

    for txt in tmp/*.out*; do
        convert -size 900x900 xc:RoyalBlue4 -font "/Users/Braedon/Library/Fonts/Meslo LG S DZ Regular for Powerline.ttf" -pointsize 14 -fill WhiteSmoke -annotate +15+15 "@$txt" $txt.png &
    done
    wait

    rm tmp/${base}.out000000000000000.png

    convert -delay 100 -loop 0 tmp/*.png $base.gif
    #rm ./tmp/*
done
