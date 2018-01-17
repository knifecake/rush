/*
 * PPROG Game – Morzilla Firefox 2018
 *
 * Authors: Miguel Baquedano, Sergio Cordero, Elias Hernandis
 *          and Rafael Sánchez.
 *
 * Lead author: <replace me>
 */


/*
 * Font loader. Loads charachters in assets/font folder.
 */

 #include "../lib/sprite.h"


/*
 * Loads the list of sprites containing the font. Receives as an argument
 * the first and the last char to be loaded. Returns NULL on error.
 */
Sprite **load_font(char, char);

/*
 * Frees allocated memory by load_font. Receives as an argument the first
 * and the last char to be free'd.
 */
void destroy_font(Sprite **, char, char);
