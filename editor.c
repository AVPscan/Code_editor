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

void help() {
    printf(Cnn "Created by " Cna "Alexey Pozdnyakov" Cnn " in " Cna "02.2026" Cnn 
           " version " Cna "1.00" Cnn ", email " Cna "avp70ru@mail.ru" Cnn 
           " github " Cna "https://github.com" Cnn "\n"); }

int main(int argc, char *argv[]) {
  if (argc > 1) { if (strcmp(argv[1], "-?") == 0) help(); return 0; }
  size_t sz = 65536; int w, h, mode = 0; double real_ms; struct timespec real_start, real_end, cs, ce;
  size_t my_buf = GetBuff(&sz); if (!my_buf) return 0;
  SWD(); delay_ms(0); os_sync_size(); w = GetWH(&h); write(1, "\033[?1049h", 8); memset((void*)my_buf, ' ', w * h); 
  clock_gettime(CLOCK_MONOTONIC, &cs); write(1, (void*)my_buf, w * h); clock_gettime(CLOCK_MONOTONIC, &ce); write(1, "\033[?1049l", 8);
  double c_ms = (ce.tv_sec - cs.tv_sec) * 1000.0 + (ce.tv_nsec - cs.tv_nsec) / 1000000.0;
  int PUMP_C = (int)((w * h) / (c_ms > 0.1 ? c_ms : 0.1));
  SetInputMode(1); printf(HCur SCur); fflush(stdout);
  while (1) {
    clock_gettime(CLOCK_MONOTONIC, &real_start); os_sync_size(); w = GetWH(&h);int wait = (w * h) / (PUMP_C > 0 ? PUMP_C : 1);
    delay_ms(wait > 60 ? wait : 60); clock_gettime(CLOCK_MONOTONIC, &real_end);
    real_ms = (real_end.tv_sec - real_start.tv_sec) * 1000.0 + (real_end.tv_nsec - real_start.tv_nsec) / 1000000.0;
    const char* k = GetKey(); if (k[0] == 27 && k[1] == K_NO) continue;
    if (k[0] == 'p') mode = (mode + 1) & 1;
    printf (LCur Cam "%ld" Cnu " %d %d" Cna " тоси%sбоси " Cap " %s " Cpr "C:%d ", sz, w, h, Button("Погнали",mode), k, PUMP_C);
    printf(Cna " [Ms: " Cnu "%f" Cna "]       ", real_ms); fflush(stdout); 
    if (k[0] == 27 && k[1] == K_ESC) break; }
  SetInputMode(0); printf(ShCur Crs); fflush(stdout); FreeBuff(); return 0; }
