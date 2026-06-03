#!/bin/bash

# Parâmetros iniciais
PROG=${1:-broyden}
tipo=${2:-sem_otimizacao}
CPU=${3:-0}

DATA_DIR="resultados_nao_otimizados"
mkdir -p ${DATA_DIR}

# Grupos do LIKWID solicitados na especificação
METRICA="L2CACHE MEM FLOPS_DP"
TAMANHOS="32 64 128 256 512 1000 2000 4000 8000 9000 10000 20000"

ARQUIVO_LIKWID="${DATA_DIR}/dados_likwid.csv"
ARQUIVO_TEMPOS="${DATA_DIR}/dados_tempos.csv"

# Limpa os arquivos de resultados anteriores, se existirem, e cria cabeçalhos
rm -f ${ARQUIVO_LIKWID} ${ARQUIVO_TEMPOS}

echo "Metrica,N,Tempo_Total,Tempo_Jacobiana,Tempo_SL" > ${ARQUIVO_TEMPOS}
echo "N, L2CACHE, MEM, FLOPS_DP" > ${ARQUIVO_LIKWID}
echo "performance" > /sys/devices/system/cpu/cpufreq/policy${CPU}/scaling_governor

echo "Iniciando testes com Likwid..."

EPSILON=0.0
XO=0.0

for m in ${METRICA}
do
    echo "MÉTRICA: $m"
    
    for n in $TAMANHOS
    do
        echo "Processando N: $n"
        
        ARQUIVO_TXT="${DATA_DIR}/saida_${m}_${n}.txt"

        # 1. Roda o Likwid, passa o stdout pro Python e faz append (>>) no arquivo final de métricas
        echo "$n $EPSILON $XO" | \
        likwid-perfctr -O -C ${CPU} -g ${m} -m ./${PROG} -o "${ARQUIVO_TXT}" | \
        python3 busca_dados.py >> ${ARQUIVO_LIKWID}
            
        # 2. Extrai os tempos do arquivo .txt gerado pelo programa C
        # O 'grep' acha a linha certa e o 'awk' pega a 4ª palavra da linha (o número)
        TEMPO_TOTAL=$(grep "Tempo Total:" "${ARQUIVO_TXT}" | awk '{print $4}')
        TEMPO_JAC=$(grep "Tempo Jacobiana:" "${ARQUIVO_TXT}" | awk '{print $4}')
        TEMPO_SL=$(grep "Tempo SL:" "${ARQUIVO_TXT}" | awk '{print $4}')
        
        # 3. Salva os tempos formatados como CSV no arquivo de tempos
        # Formato salvo: Metrica, Ordem(N), TempoTotal, TempoJacobiana, TempoSL
        echo "${m},${n},${TEMPO_TOTAL},${TEMPO_JAC},${TEMPO_SL}" >> ${ARQUIVO_TEMPOS}
        
    done
done    

# Retorna para powersave para economizar energia/resfriar
echo "powersave" > /sys/devices/system/cpu/cpufreq/policy${CPU}/scaling_governor 

echo "Testes concluídos! Resultados salvos em:"
echo "- Métricas Likwid: ${ARQUIVO_LIKWID}"
echo "- Tempos de Exec:  ${ARQUIVO_TEMPOS}"