rm(list=ls())

if (!require("ggplot2")) install.packages("ggplot2"); library("ggplot2")

setwd("~/aulas/quinto_semestre/intro-comp-cientifica/t1/")

dados_otimizados=read.csv("otimizado/resultados_otimizados/dados_tempos.csv")
dados_nao_otimizados = read.csv("nao_otimizado/resultados_nao_otimizados/dados_tempos.csv")

### Cria tabela com as versões ###
dados_otimizados$Versao = "Otimizado"
dados_nao_otimizados$Versao = "Não Otimizado"

dados_completos = rbind(dados_otimizados, dados_nao_otimizados)

cores_versao = c("Otimizado" = "#3357FF", "Não Otimizado" = "#FF5733")

### Comparativo Tempo Newton ###
ggplot(dados_completos)+
  stat_summary(fun = "mean", geom = "line", 
               aes(x=N, y=Tempo_Total, color = Versao), 
               linewidth = 0.5) +
  stat_summary(fun = "mean", geom = "point", 
               aes(x=N, y=Tempo_Total, color = Versao), 
               size = 2) +
  scale_x_continuous(trans='log2')+
  scale_y_continuous(trans='log10')+
  labs(x="Tamanho da Matriz Quadrada",
       y="Tempo Total (ms)", title = "Comparativo Tempo Newton")+
  scale_color_manual(name="Versão",
                     values = cores_versao)+
  theme_bw()

### Comparativo Tempo Jacobiana ###
ggplot(dados_completos)+
  stat_summary(fun = "mean", geom = "line", 
               aes(x=N, y=Tempo_Jacobiana, color = Versao), 
               linewidth = 0.5) +
  stat_summary(fun = "mean", geom = "point", 
               aes(x=N, y=Tempo_Jacobiana, color = Versao), 
               size = 2) +
  scale_x_continuous(trans='log2')+
  scale_y_continuous(trans='log10')+
  labs(x="Tamanho da Matriz Quadrada",
       y="Tempo Total (ms)", title = "Comparativo Tempo Jacobiana")+
  scale_color_manual(name="Versão",
                     values = cores_versao)+
  theme_bw()

### Comparativo Tempo Sistema Linear ###
ggplot(dados_completos)+
  stat_summary(fun = "mean", geom = "line", 
               aes(x=N, y=Tempo_SL, color = Versao), 
               linewidth = 0.5) +
  stat_summary(fun = "mean", geom = "point", 
               aes(x=N, y=Tempo_SL, color = Versao), 
               size = 2) +
  scale_x_continuous(trans='log2')+
  scale_y_continuous(trans='log10')+
  labs(x="Tamanho da Matriz Quadrada",
       y="Tempo Total (ms)", title = "Comparativo Tempo Sistema Linear")+
  scale_color_manual(name="Versão",
                     values = cores_versao)+
  theme_bw()
