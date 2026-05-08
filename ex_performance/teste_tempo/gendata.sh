#!/bin/bash
CPU=15
TAMANHOS="128 200 300 512 1024 2000 2048 4092" # 6000 7000 10000 50000 100000

echo "performance" > /sys/devices/system/cpu/cpufreq/policy${CPU}/scaling_governor

# 1. Cria o cabeçalho da tabela Markdown no arquivo tempo.txt
echo "| n | MatPtr x Vetor (ms) | MatRow x Vetor (ms) | MatPtr x MatPtr (ms) | MatRow x MatRow (ms) |" > tempo.txt
echo "|---|---------------------|---------------------|----------------------|----------------------|" >> tempo.txt

for n in $TAMANHOS
do
    echo "Realizando operações com matrizes de tamanho ${n}..."
    # 2. Adicionado o -q para suprimir a saída padrão do likwid (evita quebrar a tabela)
    likwid-pin -q -c ${CPU} ./operacoes -n ${n} >> tempo.txt
done

echo "powersave" > /sys/devices/system/cpu/cpufreq/policy${CPU}/scaling_governor 

echo ""
echo "Concluído!"