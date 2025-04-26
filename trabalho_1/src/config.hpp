#ifndef CONFIG_HPP
#define CONFIG_HPP

const int MAX_ITERACOES = 100;

const int VAZIO = 0;
const int ARVORE_SAUDAVEL = 1;
const int ARVORE_QUEIMANDO = 2;
const int ARVORE_QUEIMADA = 3;
const int AGUA = 4;

const int VENTO_NENHUM = 0;
const int VENTO_CIMA = 1;
const int VENTO_DIREITA = 2;
const int VENTO_BAIXO = 4;
const int VENTO_ESQUERDA = 8;
const int VENTO_TODOS = 15;

const int DIRECAO_VENTO_CONFIGURADA = VENTO_TODOS;

#endif