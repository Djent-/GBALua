#ifndef SMALLFONT_H
#define SMALLFONT_H
/* Tile size:        8*8 */
/* Number of tiles:  114 */
/* Number of colors: 16 */

#define SMALLFONT_NR_OF_TILES 114
#define SMALLFONT_TILESIZE_X 8
#define SMALLFONT_TILESIZE_Y 8
#define SMALLFONT_TILESIZE 16
#define SMALLFONT_PALETTE_COLORS 16

extern unsigned short SmallFontTileData[114][16];
extern unsigned short SmallFontPaletteData[16];
#endif //SMALLFONT_H
