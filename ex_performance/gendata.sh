#!/bin/bash

CMD_DIR=`dirname $0`

PROG=${1:-matmult}
tipo=${2:-avx}
CPU=${3:-3}

DATA_DIR="resultados/${PROG}"
TEMPOS="${DATA_DIR}/Tempos.csv"

METRICA="FLOPS_DP L3 ENERGY"

TAMANHOS="128 200 300 512 1024 2000 2048 4092" # 6000 7000 10000 50000 100000

mkdir -p ${DATA_DIR}

echo "performance" > /sys/devices/system/cpu/cpufreq/policy${CPU}/scaling_governor

make purge ${tipo}

for m in ${METRICA}
do
    LIKWID_LOG="${DATA_DIR}/${m}_${tipo}.log"
    rm -f ${TEMPOS}
    rm -f ${LIKWID_LOG}
    
    for n in $TAMANHOS
    do
	LIKWID_OUT="${DATA_DIR}/${m}_${tipo}_${n}.txt"
	
	echo "--->>  $m: ./${PROG} $n" >/dev/tty
	likwid-perfctr -O -C ${CPU} -g ${m} -o ${LIKWID_OUT} -m ./${PROG} -n ${n} >>${TEMPOS}
	    
	# echo "===> N: ${n} <==" >> ${LIKWID_LOG}
	cat ${LIKWID_OUT} >> ${LIKWID_LOG}
	rm -f ${LIKWID_OUT}
    done

   # ${CMD_DIR}/genplot.py < ${LIKWID_LOG} > ${LIKWID_LOG%%.log}.csv
done

echo "powersave" > /sys/devices/system/cpu/cpufreq/policy${CPU}/scaling_governor 

echo ""
echo ""
