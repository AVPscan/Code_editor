/*
 * Copyright (C) 2026 Поздняков Алексей Васильевич
 * E-mail: avp70ru@mail.ru
 * 
 * Данная программа является свободным программным обеспечением: вы можете 
 * распространять ее и/или изменять согласно условиям Стандартной общественной 
 * лицензии GNU (GPLv3).
 */
 
#include <stdio.h>
#include <time.h>   // ОБЯЗАТЕЛЬНО для struct timespec
#include "sys.h"

int main() {
  size_t sz = 2048; int w, h, mode = 0; double real_ms; struct timespec real_start, real_end;
  if (!GetBuff(&sz)) return 0;
  SWD(); delay_ms(0); SetInputMode(1); printf(HCur SCur); fflush(stdout);
  while (1) {
    clock_gettime(CLOCK_MONOTONIC, &real_start); delay_ms(60); clock_gettime(CLOCK_MONOTONIC, &real_end);
    real_ms = (real_end.tv_sec - real_start.tv_sec) * 1000.0 + (real_end.tv_nsec - real_start.tv_nsec) / 1000000.0;
    const char* k = GetKey(); if (k[0] == 27 && k[1] == K_NO) continue;
    os_sync_size(); w = GetWH(&h); if (k[0] == 'p') mode = (mode + 1) & 1;
    if (k[0] != 27) printf (LCur Cam "%ld" Cnu " %d %d" Cna " тоси%sбоси " Cap " %s  ", sz, w, h, Button("Погнали",mode), k);
    else printf (LCur Cam "%ld" Cnu " %d %d" Cna " тоси%sбоси " Cap "(%d)", sz, w, h, Button("Погнали",mode), k[1]);
    printf(Cna " [Ms: " Cnu "%f" Cna "]       ", real_ms); fflush(stdout); if (k[0] == 27 && k[1] == K_ESC) break; }
  SetInputMode(0); printf(ShCur Crs); fflush(stdout); FreeBuff(); return 0; }
