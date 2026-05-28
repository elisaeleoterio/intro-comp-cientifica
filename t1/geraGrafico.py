#! /usr/bin/env python3
# coding=utf8

# Descomente as 2 linhas abaixo se quiser que Python procure por modulos
# em outros diretórios que não os do sistema e o diretório corrente.
## import sys, os
## sys.path.extend([ '~/lib/python', '.', '..' ])
# Ou em seu ambiente shell, crie e exporte a variavel de ambiente PYTHONPATH.
# Coloque a linha abaixo em seu arquivo '~/.bashrc' ou '~/.profile:
## export PYTHONPATH="${HOME}/lib/python:.:.."
#

# import numpy as np
# import matplotlib.pyplot as plt

from string import *
from math import *
import re, sys, os


#  "CACHE" : "data cache misses",
#  "CACHE" : "data cache miss ratio",
#  "CACHE" : "data cache miss rate",
#  "L2CACHE" : "L2 miss ratio",
#  "L2CACHE" : "L2 miss rate",
#  "L3" : "L3 bandwidth \[*MBytes/s\]*",
#  "MEM" : "Memory bandwidth \[*MBytes/s\]*",
#  "MEM" : "Memory read bandwidth",
#  "MEM" : "Memory write bandwidth",
#  "TIME" : "Runtime \(RDTSC\) \[*s\]*",
#  "ENERGY" : "Power \[*W\]*"

campos = { "L3CACHE" : "L3 miss ratio",
           "FLOPS_DP" : "DP \[*MFLOP/s\]*",
           "FLOPS_AVX" : "AVX DP \[*MFLOP/s\]*",
           "ENERGY" : "Energy \[*J\]*"
        }

# 'STRUCT,Info' inicia Região
# 'TABLE,Region.*Metric,<metrica>' inicia metrica
# '<string>' inicia linha '<string>.*,<valor>,.*', onde
# <string> é obtido a partr de 'campo[]', tendo <metrica> como chave


def lerDados() :
    linha = sys.stdin.readline()
    while linha and (re.match("STRUCT,Info", linha) == None) :
        linha = sys.stdin.readline()            

    for linha in sys.stdin :
        if re.match("TABLE,Region.*Metric,", linha) != None :
            break

    if (linha) :
        linha = linha.split(',')
        metrica = linha[3]
        regiao = linha[1].split('_')
        if regiao[1].isnumeric() :
            ordem = int(regiao[1])
            marker = re.sub("Region ", "", regiao[0])
        else :
            ordem = int(regiao[2])
            marker = re.sub("Region ", "", regiao[0])+'_'+regiao[1]

        for linha in sys.stdin :
            if re.match(campos[metrica], linha) != None :
                break

        if (linha) :
            linha = linha.split(',')
            valor = float(linha[1])

            return [ metrica, ordem, marker, valor ]
            ## return [ metrica, marker, valor ]

    return ''
    
# fim lerDados()

    
# =====================================================
# Inicio Programa principal
# =====================================================
#
# Descomente a linha abaixo se arquivo pode ser importado como módulo
# via 'import .....'
## if __name__ == '__main__':

# Lê saída de likwid-perfctr -O e gera saída para gnuplot gerar
# um gráfico para cada métrica, cada gráfico contendo os dados de todos os markers
# 

# saida = {<metrica> : {<ordem> : [ [<marker>, <valor>] ]} }
saida = {}

# item possui campos: marker, n, metrica, valor.
# 'n' é inteiro, 'valor' é double. 'marker' e 'metrica' são strings

item = lerDados()
while item :
    # print(item)
    if item[0] in saida :
        if item[1] in saida[item[0]] :
            saida[item[0]][item[1]].append(item[2:])
        else :
            saida[item[0]][item[1]] = [item[2:]]
    else :
        saida[item[0]] = {item[1]:[item[2:]]}

    item = lerDados()

# print(saida)
# exit()

    
# gera tabela para uso em gnuplot ou pyplot/matplotlib
gnuplot = ''

for i in saida :
    # print(i)
    ordens = []
    for j in saida[i] :
        ordens.append(j)

    # print(ordens,'\n\n',gnuplot)
    for j in ordens :
        gnuplot += '{0}'.format(j)
        # print('@@@',saida[i][j])
        for k in saida[i][j] :
            gnuplot += ',{0}'.format(k[1])
            
        gnuplot += '\n'
    # FIM for j in ordens

    # grava tabela de pontos
    print (gnuplot, end='')
    
    # plotFile = i+'.csv'
    # fp = open(plotFile, "w")
    # fp.writelines(gnuplot)
    # fp.close()
# FIM for i in saida
