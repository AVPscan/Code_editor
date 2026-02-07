/*
 * Copyright (C) 2026 Поздняков Алексей Васильевич
 * E-mail: avp70ru@mail.ru
 * 
 * Данная программа является свободным программным обеспечением: вы можете 
 * распространять ее и/или изменять согласно условиям Стандартной общественной 
 * лицензии GNU (GPLv3).
 */
 
#include <stdio.h>
#include <time.h>               // ОБЯЗАТЕЛЬНО для struct timespec
#include "sys.h"

int main() {
  size_t sz = 2048;
  if (!GetBuff(&sz)) return 0;
  SWD(); os_sync_size(); delay_ms(0); int mode = 0;
  SetInputMode(1); printf(HCur SCur); fflush(stdout);
  while (1) {
    struct timespec real_start, real_end;
    clock_gettime(CLOCK_MONOTONIC, &real_start);
    delay_ms(60);
    clock_gettime(CLOCK_MONOTONIC, &real_end);
    double real_ms = (real_end.tv_sec - real_start.tv_sec) * 1000.0 + 
                 (real_end.tv_nsec - real_start.tv_nsec) / 1000000.0;
    const char* k = GetKey();
    if (k[0] == 27 && k[1] == K_NO) continue;
    os_sync_size();
    if (k[0] == 'p') mode = (mode + 1) & 1;
    if (k[0] != 27) printf (LCur Cam "%d %d" Cna " тоси%sбоси " Cap "%s", TS.w, TS.h, Button("Погнали",mode), k);
    else printf (LCur Cam "%d %d" Cna " тоси%sбоси " Cap "(%d)", TS.w, TS.h, Button("Погнали",mode), k[1]);
    printf(" [Ms: %f]       ", real_ms); fflush(stdout); if (k[0] == 27 && k[1] == K_ESC) break; }
  SetInputMode(0); printf(ShCur Crs); fflush(stdout); return 0; }

