#!/bin/bash

total=$(grep -R "((strcmp(bootfile," lcp.c | wc -l)
# 8 game discs share a bootfile

# Jackie Chan Stuntmaster (Europe) (Recalled) / Jackie Chan Stuntmaster (Europe)
# CTR: Crash Team Racing (Europe) / CTR: Crash Team Racing (EDC) (Europe)
# Spyro: Year of the Dragon (Europe) (En,Fr,De,Es,It) (Rev 0) / Spyro: Year of the Dragon (Europe) (En,Fr,De,Es,It) (Rev 1)
# PGA European Tour Golf (Europe) (Fr, It, Es) / PGA European Tour Golf (Europe) (Fr, It, Es) (Alt)
# Les Cochons de Guerre  (France) Rev 0 / Les Cochons de Guerre  (France) (Rev 1) - Both version have same exact data track bin file, just different audio track checksums 
# Formula One 99 (Europe) (En,Fr,De,It) / Formula One 99 (Europe) (En,Fr,De,It) (Beta 9/9/1999)
# LMA Manager 2001 (Europe) / LMA Manager 2001 (Europe) (Beta 02/26/2001)
# Space Debris (Germany) / Space Debris (Germany) (Beta 11/11/1999)

total=$(($total + 8))
echo $total
