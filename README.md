# pprog
[![CircleCI](https://circleci.com/gh/knifecake/pprog.svg?style=svg&circle-token=a738aa38091dce1ca9eead8125f07cdd91df0831)](https://circleci.com/gh/knifecake/pprog)
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
terminal. Performance is still a major unknown. At the moment, Sprite is able
to display RGB and RGBA PNGs, or, more generally, any PNG readable by the
`lib-png` installed on your system.  Please note that each pixel is
two-characters wide and one character high, so big PNGs will not fit on the
screen unless the font size is made very small.  This renders the text
capabilities of the terminal unusable. Text is the thing that a terminal does
right, so using Sprite is not only overcomplicating yourself, but, in many
cases, it may pose a real threat to the usability of your app/game. Please use
it responsibly.

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

