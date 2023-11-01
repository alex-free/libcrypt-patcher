#!/bin/bash

total=$(grep -R "((strcmp(bootfile," lcp.c | wc -l)
# 6 game discs share a bootfile

# Jackie Chan Stuntmaster (Europe) (Recalled) / Jackie Chan Stuntmaster (Europe)
# CTR: Crash Team Racing (Europe) / CTR: Crash Team Racing (EDC) (Europe)
# Spyro: Year of the Dragon (Europe) (En,Fr,De,Es,It) (v1.0) / Spyro: Year of the Dragon (Europe) (En,Fr,De,Es,It) (v1.1)
# PGA European Tour Golf (Europe) (Fr, It, Es) / PGA European Tour Golf (Europe) (Fr, It, Es) (Alt)
# OverBlood 2 (Europe) (Disc 1) (v1.0) / OverBlood 2 (Europe) (Disc 1) (v1.1)
# OverBlood 2 (Europe) (Disc 2) (v1.0) / OverBlood 2 (Europe) (Disc 2) (v1.1)
 

total=$(($total + 6))
echo $total
