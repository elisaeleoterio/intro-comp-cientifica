rm(list=ls())

if (!require("ggplot2")) install.packages("ggplot2"); library("ggplot2")

setwd("~/aulas/quinto_semestre/intro-comp-cientifica/t1/")

dados_otimizados=read.csv("otimizado/resultados_otimizados/dados_likwid_MEM.csv")
dados_nao_otimizados = read.csv("nao_otimizado/resultados_nao_otimizados/dados_likwid_MEM.csv")

### Cria tabela com as versões ###
dados_otimizados$Versao = "Otimizado"
dados_nao_otimizados$Versao = "Não Otimizado"

dados_completos = rbind(dados_otimizados, dados_nao_otimizados)

cores_versao = c("Otimizado" = "#3357FF", "Não Otimizado" = "#FF5733")

### Comparativo Newton ###
ggplot(dados_completos)+
  stat_summary(fun = "mean", geom = "line", 
               aes(x=N, y=Newton, color = Versao), 
               linewidth = 0.5) +
  stat_summary(fun = "mean", geom = "point", 
               aes(x=N, y=Newton, color = Versao), 
               size = 2) +
  scale_x_continuous(trans='log2')+
  scale_y_continuous(trans='log10')+
  labs(x="Tamanho da Matriz Quadrada",
       y="MBytes/s", title = "Comparativo Banda de Memória Newton")+
  scale_color_manual(name="Versão",
                     values = cores_versao)+
  theme_bw()

### Comparativo Jacobiana ###
ggplot(dados_completos)+
  stat_summary(fun = "mean", geom = "line", 
               aes(x=N, y=Jacobiana, color = Versao), 
               linewidth = 0.5) +
  stat_summary(fun = "mean", geom = "point", 
               aes(x=N, y=Jacobiana, color = Versao), 
               size = 2) +
  scale_x_continuous(trans='log2')+
  scale_y_continuous(trans='log10')+
  labs(x="Tamanho da Matriz Quadrada",
       y="MBytes/s", title = "Comparativo Banda de Memória Jacobiana")+
  scale_color_manual(name="Versão",
                     values = cores_versao)+
  theme_bw()

### Comparativo Sistema Linear ###
ggplot(dados_completos)+
  stat_summary(fun = "mean", geom = "line", 
               aes(x=N, y=SL, color = Versao), 
               linewidth = 0.5) +
  stat_summary(fun = "mean", geom = "point", 
               aes(x=N, y=SL, color = Versao), 
               size = 2) +
  scale_x_continuous(trans='log2')+
  scale_y_continuous(trans='log10')+
  labs(x="Tamanho da Matriz Quadrada",
       y="MBytes/s", title = "Comparativo Banda de Memória Sistema Linear")+
  scale_color_manual(name="Versão",
                     values = cores_versao)+
  theme_bw()
