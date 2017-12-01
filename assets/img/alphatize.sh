#This script converts an rgb or rgba image of whichever extension to a rgba .txt output.

png='.png'
txt='.txt'
name=${1%.*}
convert $1 -alpha set $name$png
convert $name$png $name$txt
