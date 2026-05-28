#!/bin/bash

# Diretório onde o script está
CMD_DIR=`dirname $0`

# Parâmetros iniciais
PROG=${1:-broyden}
tipo=${2:-sem_otimizacao}
CPU=${3:-0} # Geralmente usa-se a CPU 0 ou uma fixa para evitar variações

DATA_DIR="resultados"
mkdir -p ${DATA_DIR}

# Grupos do LIKWID solicitados na especificação
METRICA="L3 L2CACHE FLOPS_DP"
# Lista de tamanhos corrigida (sem vírgulas)
TAMANHOS="32 64 128 256 512 1000 2000 4000 8000 9000 10000 20000"

# Configura o governador para performance para garantir estabilidade nos testes
# Requer sudo ou permissão específica
echo "performance" > /sys/devices/system/cpu/cpufreq/policy${CPU}/scaling_governor

echo "Iniciando testes com Likwid..."

for m in ${METRICA}
do
    echo "--- Executando métrica: $m ---"
    
    for n in $TAMANHOS
    do
        echo "Processando N: $n..."
        
        LIKWID_OUT="${DATA_DIR}/${m}_${tipo}_${n}.csv"

        # Explicação das Flags do likwid-perctr:
        # - [-C] => Define uma CPU e a fixa para rodar o executável e realiza as medições só nessa CPU
        # - [-m] => Usa os marcadores do Likwid definidos ao longo do código do executável para realizar as medições em lugares específicos
        # - [-o <filename>] => Armazena os dados em um arquivo (filename) ao invés de usar stdout
        # - [-O] => Printa os resultados em um arquivo .csv
        # - [-g] => Especifica o grupo que deve ser medido

        echo "$n" | \
        likwid-perfctr -O -C ${CPU} -g ${m} -o ${LIKWID_OUT} -m ./${PROG} -o "${DATA_DIR}/saida_${n}.txt" > /dev/null
            
    done
done    
# Retorna para powersave para economizar energia/resfriar
echo "powersave" > /sys/devices/system/cpu/cpufreq/policy${CPU}/scaling_governor 

echo "Testes concluídos! Resultados em: ${DATA_DIR}"