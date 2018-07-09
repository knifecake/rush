#
# This script converts a ogg audio file to a wav audio file
#

if [ $# -eq 0 ]
then
        echo "Missing options!"
        echo "(run $0 -h for help)"
#        echo ""
        exit 0
fi
wav=".wav"
ogg=".ogg"
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
                        echo "args.sh [option] <audio_file.ogg>"
                        echo ""
                        echo "   -h     help (this output)"
                        echo "   -c     converts ogg file"
                        echo "   -r     Remove ogg file"
                        exit 0
                        ;;

        esac
done

if [ $CONVERT = "true" ]
then
  name=${2%.*}
  oggdec $name$ogg
fi

if [ $REMOVE = "true" ]
then
  rm $2
fi
