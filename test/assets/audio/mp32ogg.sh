#
# This script converts a mp3 audio file to a ogg audio file
#

if [ $# -eq 0 ]
then
        echo "Missing options!"
        echo "(run $0 -h for help)"
#        echo ""
        exit 0
fi
wav=".wav"
REMOVE="false"
CONVERT="false"
while getopts "hcr" OPTION; do
        case $OPTION in
                c)
                        CONVERT="true"
                        ;;
                r)
                        REMOVE="true"
                        ;;

                h)
                        echo "Usage:"
                        echo "args.sh [option] <audio_file.mp3>"
                        echo ""
                        echo "   -h     help (this output)"
                        echo "   -c     converts mp3 file"
                        echo "   -r     Remove mp3 file"
                        exit 0
                        ;;

        esac
done

if [ $CONVERT = "true" ]
then
  name=${2%.*}
  mpg123 --wav $name$wav $2
  oggenc $name$wav
  rm $name$wav
fi

if [ $REMOVE = "true" ]
then
  rm $2
fi
