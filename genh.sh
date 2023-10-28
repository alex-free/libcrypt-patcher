#!/bin/bash

cd "$(dirname "$0")"/ppf-patches
rm -rf ../ppf-c
mkdir ../ppf-c
rm -f ../template-code.txt 
rm -f ../patches.h
for f in *.PPF; do
    #generate headers and includes
    xxd -i "$f" ../ppf-c/"$f".h
    sed -i -e 's|unsigned char|const unsigned char|g' ../ppf-c/"$f".h
    echo "#include \""ppf-c/"$f".h""\" >> ../patches.h
    base=$(basename -s .PPF "$f")

    # change XXXX_XXX.XX to XXXX_XXX_XX
    header=$(echo "$base" | sed 's/\(.\{8\}\)./\1_/')
    echo -e "\telse if((strcmp(bootfile, \""$base"\") == 0))\n\t{\n\t\tapply_ppf("$header"_PPF, "$header"_PPF_len, bin);\n\t}\n" >> ../template-code.txt
done
