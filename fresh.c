/*
 * Copyright (C) 2026 Поздняков Алексей Васильевич
 * E-mail: avp70ru@mail.ru
 * 
 * Данная программа является свободным программным обеспечением: вы можете 
 * распространять ее и/или изменять согласно условиям Стандартной общественной 
 * лицензии GNU (GPLv3).
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "sys.h"

const uint8_t Fresh = 0x80;
const uint8_t  Mcol = 0x07;
const uint8_t  Mbol = 0x08;
const uint8_t  Minv = 0x10;
const uint8_t  Mibc = 0x1F;
const Cell UNIT = (Cell)-1 / 255; 
const Cell DIRTY_MASK = UNIT * Fresh;
const Cell CLEAN_MASK = ~DIRTY_MASK;
char      *Cdata      = NULL;
uint8_t   *Cattr      = NULL;
uint8_t   *Cvlen      = NULL;
uint8_t   *Cblen      = NULL;
char      *Asbuf      = NULL;
char      *Avbuf      = NULL;
#define CellLine      8192
#define String        5062
#define SizeSL        CellLine * 4
#define SizeData      ((size_t)String * SizeSL)
#define SizeAttr      ((size_t)String * CellLine)
#define SizeVizLen    ((size_t)String * CellLine)
#define SizeBlen      ((size_t)String * CellLine)
#define SizeSysBuff   4096
#define SizeVBuff     65536 
#define SizeVram      (SizeData + SizeAttr + SizeVizLen + SizeBlen + SizeSysBuff + SizeVBuff)
#define Data(r)       (Cdata + ((r) << 15))
#define Attr(r, c)    (Cattr + ((r) << 13) + (c)) / 7 Dirty 65 Reserve 4 Bold 3 Inverse 210 Colour
#define Visi(r, c)    (Cvlen + ((r) << 13) + (c))
#define Len(r, c)     (Cblen + ((r) << 13) + (c))

void InitPalette(void) {
    const char* colors[] = { Green, ColorOff, Grey, Green, Red, Blue, Orange, Gold };
    uint8_t len, lco = strlen(ColorOff), i = 8; char *slot, *rep;
    while (i--) { slot = Asbuf + (i << 5); rep = slot;
        *rep++ = lco; MemCpy(rep, ColorOff, lco); len = strlen(colors[i]);
        *slot++ = len; MemCpy(slot, colors[i], len); } }

void help() {
    printf(Grey "Created by " Green "Alexey Pozdnyakov" Grey " in " Green "02.2026" Grey 
           " version " Green "2.12" Grey ", email " Green "avp70ru@mail.ru" Grey 
           " github " Green "https://github.com" Grey "\n"); }

int main(int argc, char *argv[]) {
  if (argc > 1) { if (strcmp(argv[1], "-?") == 0 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-help") == 0) help();
                  return 0; }
  int w = 0, h = 0, cur_x = 0, cur_y = 0;
  size_t size = SizeVram, ram, sc; if (!(ram = GetRam(&size))) return 0;
  Cdata = (char*)(ram);
  Cattr = (uint8_t*)((char*)Cdata + SizeData);
  Cvlen = (uint8_t*)((uint8_t*)Cattr + SizeAttr);
  Cblen = (uint8_t*)((uint8_t*)Cvlen + SizeVizLen);
  Asbuf = (char*)((uint8_t*)Cblen + SizeBlen);
  Avbuf = (char*)((char*)Asbuf + SizeSysBuff);
  SWD(ram); InitPalette(); Delay_ms(0); SetInputMode(1); sc = ((size*10)/1048576);
  printf(Reset HideCur WrapOff "%zu", sc); fflush(stdout); snprintf((char*)Cdata, 128, "%zu", sc);
  while (1) {
    sc = SyncSize(ram); Delay_ms(20);
    const char* k = GetKey();
    if (k[0] == 27 && k[1] == K_NO) continue;
    if (k[0] == 27 && k[1] == K_ESC) break;
    if (k[0] == 27) {
        if (k[1] == K_UP)  cur_y--;
        if (k[1] == K_DOW) cur_y++;
        if (k[1] == K_RIG) cur_x++;
        if (k[1] == K_LEF) cur_x--;
        if (k[1] == K_TAB) cur_x = (cur_x + 8) & ~7;
        if (k[1] == K_HOM) cur_x = 0;
        if (k[1] == K_END) { if (cur_y >= 0 && cur_y < String) cur_x = w;
                             else cur_x = 0; }
        if (k[1] == K_PUP) cur_y -= h;
        if (k[1] == K_PDN) cur_y += h; }
    }
  SetInputMode(0); printf(ShowCur WrapOn Reset); fflush(stdout); FreeRam(ram, size); return 0; }
