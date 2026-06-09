#!/usr/bin/env bash

CMD=(
    turbostat
    -q
    -S
    -J
    --show PkgTmp,Pkg_J
    -i 1
)


#uncomment for power instead of energy
#CMD=(
#    turbostat
#    -q
#    -S
#    --show PkgTmp,PkgWatt
#    -i 1
#)


TITLE="PkgTmp (C) - Thin / Pkg_J (J) - Thick"

#Uncomment to indicate power measurement
#TITLE="PkgTmp (C) - Thin / PkgWatt (Watt) - Thick"

#false call to get sudo
sudo ps

sudo stdbuf -oL "${CMD[@]}" 2>/dev/null \
    | stdbuf -oL tail -n +2 \
    | ttyplot -2 -t "$TITLE"
