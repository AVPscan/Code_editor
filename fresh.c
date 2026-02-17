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
const uint8_t  Mcol = 0x1C;
const uint8_t  Mbol = 0x02;
const uint8_t  Minv = 0x01;
const uint8_t  Mcbi = 0x1F;
const Cell UNIT = (Cell)-1 / 255;
const Cell DIRTY_MASK = UNIT * Fresh;
const Cell CLEAN_MASK = ~DIRTY_MASK;
char      *Cdata      = NULL;
uint16_t  *Coffset    = NULL;
uint8_t   *Cattr      = NULL;
uint8_t   *Cvlen      = NULL;
uint8_t   *Clen       = NULL;
char      *Asbuf      = NULL;
char      *Avbuf      = NULL;
#define CellLine      8192
#define String        5062
#define SizeCOL       CellLine * 2
#define SizeCL        CellLine * 4
#define SizeData      ((size_t)String * SizeCL)
#define SizeOffset    ((size_t)String * SizeCOL)
#define SizeAttr      ((size_t)String * CellLine)
#define SizeVizLen    ((size_t)String * CellLine)
#define SizeLen       ((size_t)String * CellLine)
#define SizeSysBuff   10240
#define SizeVBuff     65536
#define SizeVram      (SizeData + SizeOffset + SizeAttr + SizeVizLen + SizeLen + SizeSysBuff + SizeVBuff)
#define Data(r)       (Cdata + ((r) << 15))
#define Offset(r, c)  (Coffset + ((r) << 14) + (c))
#define Attr(r, c)    (Cattr + ((r) << 13) + (c))   // 7 Dirty 65 Reserve 432 Colour 1 Bold 0 Inverse
#define Visi(r, c)    (Cvlen + ((r) << 13) + (c))
#define Len(r, c)     (Clen + ((r) << 13) + (c))
#define Parse(cbi)    (Asbuf + ((cbi) << 5))        // 0-31 All
#define Colour(col)   (Asbuf + 1024 + ((col) << 5)) // 0 Current 1 Bw 2-7 Palette
#define WindowData    (Asbuf + 1280)

void InitPD(uint8_t col) { col &= Mcol;
  const char* colors[] = { Green, ColorOff, Grey, Green, Red, Blue, Orange, Gold };
  const char* modes[] = { "\011\033[1;7;53;", "\012\033[1;27;55;", "\012\033[22;7;53;", "\013\033[22;27;55;" };
  uint8_t lm, cbi, ca, c = strlen(ColorOff), i = 8; char *ac, *dst;
  while (i--) { ac = Colour(i); dst = ac;
      *dst++ = c; MemCpy(dst, ColorOff, c); ca = strlen(colors[i]);
      *ac++ = ca; MemCpy(ac, colors[i], ca); }
  i = 4; if (col) { ac = Colour(col); dst = Colour(0); *dst++ = *ac; MemCpy(dst , (ac + 1), *ac); }
  while(i) { const char* mode = modes[--i]; lm = *mode++, c = 8; 
      while(c) { ac = Colour(--c); cbi = (c << 2) + i; ca = *ac - 5; ac += 5;
        dst = Parse(cbi); *dst++ = lm + ca; MemCpy(dst, mode, lm); MemCpy(dst + lm, ac, ca); } } }
        
void InitVram(size_t addr, size_t size) { if (!addr || (size < SizeVram)) return;
  Cdata = (char*)(addr); Coffset = (uint16_t*)(Cdata + SizeData); Cattr = (uint8_t*)((char*)Coffset + SizeOffset);
  Cvlen = (uint8_t*)((uint8_t*)Cattr + SizeAttr); Clen = (uint8_t*)((uint8_t*)Cvlen + SizeVizLen);
  Asbuf = (char*)((uint8_t*)Clen + SizeLen); Avbuf = (char*)((char*)Asbuf + SizeSysBuff); InitPD(0); }

void help() {
    printf(Grey "Created by " Green "Alexey Pozdnyakov" Grey " in " Green "02.2026" Grey 
           " version " Green "2.13" Grey ", email " Green "avp70ru@mail.ru" Grey 
           " github " Green "https://github.com" Grey "\n"); }

int main(int argc, char *argv[]) {
  if (argc > 1) { if (strcmp(argv[1], "-?") == 0 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-help") == 0) help();
                  return 0; }
  int16_t w, h, ow, oh, ff; size_t size = SizeVram, ram, sc; if (!(ram = GetRam(&size))) return 0;
  SWD(ram); InitVram(ram,size); SwitchRaw(); Delay_ms(0); ff = SyncSize(ram,0); w = TermCR(&h); sc = ((size*10)/1048576);
  printf(Reset HideCur WrapOff "%zu", sc); fflush(stdout); snprintf((char*)Cdata, 128, "%zu", size);
  while (1) {
    if ((ff = SyncSize(ram,1))) { ow = w; oh = h; w = TermCR(&h); }
    ff = ow + oh;
    Delay_ms(20); const char* k = GetKey();
    if (k[0] == 27) {
        if (k[1] == K_NO) continue;
        if (k[1] == K_ESC) break; }
    }
  SwitchRaw(); printf(ShowCur WrapOn Reset); fflush(stdout); FreeRam(ram, size); return 0; }
