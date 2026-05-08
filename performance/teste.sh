#!/bin/bash

METRICA="FLOPS_DP"
CPU=3

if [ "$1" = "-c" ]; then
    rm -f *~ elimGauss *.log
    exit
fi

LIKWID_HOME=/home/soft/likwid
# Inlcusão da biblioteca
CFLAGS="-I${LIKWID_HOME}/include -DLIKWID_PERFMON" 
# Linkar com a biblioteca
LFLAGS="-L${LIKWID_HOME}/lib -llikwid"
SCALING_FREQ="/sys/devices/system/cpu/cpufreq/policy${CPU}/scaling_governor"

likwid-topology -c -g >TOPOLOGIA.log

if [ -w ${SCALING_FREQ} ]; then
    echo "performance" > ${SCALING_FREQ}
fi

gcc ${CFLAGS} -O0 elimGauss.c -o elimGauss ${LFLAGS}
for k in $METRICA
do
    likwid-perfctr -C ${CPU} -g ${k} -m ./elimGauss >${k}_SemOtimiz.log
done

rm -f elimGauss
gcc ${CFLAGS} -O3 elimGauss.c -o elimGauss ${LFLAGS}
for k in $METRICA
do
    likwid-perfctr -C ${CPU} -g ${k} -m ./elimGauss >${k}_Otimiz.log
done

if [ -w ${SCALING_FREQ} ]; then
    echo "powersave" > ${SCALING_FREQ}
fi

echo ''
echo '-----------------------------------'
if [ -f FLOPS_DP_Otimiz.log -a -f TOPOLOGIA.log ]; then
    echo 'Configuração LIKWID: SUCESSO'
    echo ''
    ls FLOPS_*.log TOPOLOGIA.log
    echo ''
else
    echo 'Configuração LIKWID: FALHA'
fi
echo '-----------------------------------'
echo ''