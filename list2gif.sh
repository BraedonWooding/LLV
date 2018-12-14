# Grab file from 'input'
for $file in $@; do
    if [ head -c 1 $file == "=" ]; then
        
    csplit --digits=2  --quiet --prefix=$(basename $file).out $file "//+1" "{*}"
done

convert -size 360x360 xc:white -font "FreeMono" -pointsize 12 -fill black -annotate +15+15 "@ascii.txt" image.png