#!/bin/bash

CMD_DIR=`dirname $0`

PROG=${1:-operacoes}
tipo=${2:-avx}
CPU=${3:-7}

DATA_DIR="resultados/${PROG}"

# Garante que o diretório de resultados existe para o script não quebrar
mkdir -p ${DATA_DIR}

METRICA="ENERGY FLOPS_DP L3 "
TAMANHOS="128 200 300 512 1024 2000 2048 4092"

echo "performance" > /sys/devices/system/cpu/cpufreq/policy${CPU}/scaling_governor

echo "Iniciando testes com Likwid (Energia, FLOPS e L3)..."

for m in ${METRICA}
do
    LIKWID_LOG="${DATA_DIR}/${m}_${tipo}.log"
    # Limpa o log de métricas anteriores, se houver
    echo "Resultados para a Métrica: ${m}" > ${LIKWID_LOG}
    
    for n in $TAMANHOS
    do
        LIKWID_OUT="${DATA_DIR}/${m}_${tipo}_${n}.txt"
        
        echo "--->>  Métrica: $m | N: $n | Comando: ./${PROG} $n" > /dev/tty
        
        # Executa o LIKWID gerando um CSV de saída (-O) para o arquivo temporário LIKWID_OUT
        likwid-perfctr -O -C ${CPU} -g ${m} -o ${LIKWID_OUT} -m ./${PROG} -n ${n} > /dev/null
            
        echo -e "\n===> N: ${n} <==" >> ${LIKWID_LOG}
        # Adiciona a saída gerada ao log principal daquela métrica
        cat ${LIKWID_OUT} >> ${LIKWID_LOG}
        
        # Remove o txt temporário
        rm -f ${LIKWID_OUT}
    done
done

echo "powersave" > /sys/devices/system/cpu/cpufreq/policy${CPU}/scaling_governor 

echo ""
echo "Testes concluídos! Os logs estão na pasta ${DATA_DIR}."