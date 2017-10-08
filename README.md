# pprog
The f*cking pprog game.

## How to compile this project
Inside the root directory run `make`. An executable will be created inside
`build/`. You may run it with `./build/game`. Intermediate object files (`*.o`)
will be deleted, however, if by any chance one is left behind, git is
instructed to ignore them. Also, it will ignore anything inside the build
directory except for the directory itself which is required by make. Please, do
not remove `build/.keep`, it makes sure `build/` is committed and ready to use
in compilation.

## A note about image assets

The Sprite library was created for the sole purpose of displaying bitmaps on a
terminal. Performance is still a major unknown. At the moment, sprite is able to
display 8-bit indexed uncompressed BMP3 files on the terminal. Please note that
each pixel is two-characters wide and one character high, so big BMPs will not
fit on the screen unless the font size is made very small. This renders the
text capabilities of the terminal unusable. Text is the thing that a terminal
does right, so using Sprite is not only overcomplicating yourself, but, in many
cases, it may pose a real threat to the usability of your app/game. Please use
it responsibly.

To convert almost any image to a format that is compatible with Sprite you may
use ImageMagick. You may install IM with a package manager if you run Linux or
MacOS. Just run `apt-get install imagemagick` if you're on Ubuntu or the like.
For MacOS you can use [brew.sh](https://brew.sh "Homebrew homepage"). Install
it per the instructions on the website and then run `brew install imagemagick`.
Once done installing you can run `convert -compress None -colors 256
<original.jpeg> BMP3:<compatible.bmp>` to produce an appropriately encoded
image for Sprite.

For documentation on how to handle sprites and use the library in general,
refer to `src/sprite.h`. For more information on the format of a BMP3 file,
refer to `lib/bmp.h`, which contains the structures defined by Microsoft for
the BMP3 file format.

## plot proposal
Our game will be turn-based and will be set on an hexagonal grid. Your task
will be to survive as long as possible, enduring natural disasters, shortage of
resources, etc. At first, you will begin with a minimal amount of resources and
you will need to build, mine and cultivate to be able to survive to the next
round. The game ends when you run out of essential resources. In addition, we
plan on adding attack functionality with a mini game. When you choose to attack
or are attacked from an adjacent cell, the mini game will run and you will be
more or less prepared to play it depending on your available resources. Also,
the game theme is not final, and we are thinking of a more exciting topic for
our story, although the game dynamics will remain the same.

