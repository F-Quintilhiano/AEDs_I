
#  TRABALHO AEDs I - Simulação de Propagação de Incêndios

<h1 align='center'>
   Simulação de Propagação de Incêndio Florestal 
</h1>

<div align='center'>

  Algoritmos e Estrutura de Dados I

  CEFET-MG - Campus Divinópolis

  Professor Michel Pires da Silva

  Abril de 2025
</div>

---

<details>
  <summary><b>📑 Sumário</b></summary>
  <ol>
    <li><a href="#☕-introdução">Introdução</a></li>
    <li><a href="#🎯-objetivos">Objetivos</a></li>
    <li><a href="#🖥️-ambiente-de-criação">Ambiente de Criação</a></li>
    <li><a href="#📂-organização-dos-arquivos">Organização dos Arquivos</a></li>
    <li><a href="#💡-implementação">Implementação</a></li>
    <li><a href="#🔥-regras-da-simulação-resumo">Regras da Simulação (Resumo)</a></li>
    <li><a href="#👨🏻💻-compilação-e-execução">Compilação e Execução</a></li>
    <li><a href="#🧪-casos-de-teste--exemplo">Casos de Teste / Exemplo</a></li>
    <li><a href="#📊-análises-e-observações">Análises e Observações</a></li>
    <li><a href="#💡-melhorias-e-trabalhos-futuros">Melhorias e Trabalhos Futuros</a></li>
    <li><a href="#🏁-conclusão">Conclusão</a></li>
    <li><a href="#📪-contato">Contato</a></li>
  </ol>
</details>

---

## - Introdução
Este projeto, desenvolvido para a disciplina de Algoritmos e Estrutura de Dados I no CEFET-MG Campus Divinópolis sob orientação do Prof. Michel Pires da Silva, implementa uma simulação da propagação de incêndios em uma floresta representada por uma matriz bidimensional. A simulação utiliza a linguagem C++ e uma abordagem orientada a objetos para modelar o ambiente, o comportamento do fogo sob diferentes condições de vento (configuráveis via `src/config.hpp`) e a tentativa de sobrevivência de um agente autônomo (animal) que busca escapar das chamas, seguindo regras de prioridade e interagindo com elementos como água. O estado da simulação é lido de um arquivo `input.dat` e a evolução da floresta é registrada a cada iteração em `output.dat`.

---

##  - Objetivos
Conforme especificado no enunciado do trabalho:
- **Modelar** a propagação de um incêndio em uma matriz N x M, representando diferentes estados (vazio, árvore saudável, queimando, queimada, água).
- **Implementar** as regras de transição de estado para o fogo (saudável -> queimando -> queimada).
- **Simular** o efeito do vento na propagação do fogo, com direções configuráveis.
- **Modelar** o comportamento de um animal que busca sobreviver, movendo-se ortogonalmente com base em prioridades (Água > Vazio/Saudável > Queimada) e evitando fogo.
- **Implementar** a interação do animal com a água (célula vira Vazio, vizinhos viram Saudável).
- **Implementar** o mecanismo de "segunda chance" para o animal caso sua posição seja atingida pelo fogo.
- **Implementar** a regra de permanência do animal em local seguro (Vazio) por até 3 iterações.
- **Estruturar** o código de forma modular e organizada.
- **Ler** a configuração inicial de `input.dat` e **salvar** o estado a cada iteração em `output.dat`.
- **Exibir** no final da simulação: caminho do animal, passos totais e status de sobrevivência.
- **Documentar** o projeto (este README) e **analisar** os padrões observados.

---

## - Ambiente de Criação
- **Linguagem:** C++ (utilizando padrão C++11 ou superior)
- **Compilador:** g++ (Clang/LLVM padrão no macOS ou GCC instalado via Homebrew/Xcode)
- **Sistema Operacional:** macOS Monterey (executado em MacBook Pro 2016)
- **IDE:** Visual Studio Code (VS Code)
- **Automação:** GNU Make

---

## - Organização dos Arquivos
O projeto segue a estrutura de pastas padrão:
```bash
.
├── Makefile             # Arquivo para compilação e execução via Make
├── README.md            # Esta documentação
├── build/               # Diretório para arquivos compilados (gerado pelo Make)
│   ├── app
│   └── objects/
├── src/                 # Arquivos de código-fonte
│   ├── config.hpp       # Configurações globais (MAX_ITERACOES, VENTO, etc.)
│   ├── simulation.hpp   # Declaração da classe Simulacao
│   ├── simulation.cpp   # Implementação da classe Simulacao
│   └── main.cpp         # Função principal
├── input.dat            # Arquivo de entrada com a configuração inicial
└── output.dat           # Arquivo de saída com os estados iterativos (gerado)
```

---

## - Implementação

A simulação foi implementada utilizando C++ e Programação Orientada a Objetos. A classe principal `Simulacao` encapsula o estado e a lógica da simulação.

### Representação da Floresta:
Um `std::vector<std::vector<int>> matriz_` armazena o estado de cada célula. Uma matriz auxiliar `proxima_matriz_` é utilizada para calcular o estado da próxima iteração, garantindo atualizações simultâneas.

### Carregamento (`carregar_estado_inicial`):
Lê `input.dat` (dimensões, fogo inicial, matriz). Valida os dados, aloca a `matriz_`, define a célula inicial do fogo como `ARVORE_QUEIMANDO` (2) e localiza a primeira célula `VAZIO` (0) para posicionar o animal internamente (`linha_animal_`, `coluna_animal_`).

### Passo da Simulação (`passo_simulacao`):
Orquestra a ordem dos eventos em cada iteração:
1.  Chama `mover_animal()` (se o animal estiver vivo).
2.  Calcula propagação/envelhecimento do fogo (considerando vento configurado em `config.hpp`) para `proxima_matriz_`.
3.  Atualiza `matriz_` com `proxima_matriz_`.
4.  Verifica se `matriz_[linha_animal_][coluna_animal_] == ARVORE_QUEIMANDO` (2). Se sim, aciona a **segunda chance**:
    *   Chama `mover_animal()` novamente.
    *   Se a posição interna do animal não mudar, define `animal_esta_vivo_ = false` e registra `iteracao_preso_`.
5.  Incrementa `iteracao_atual`.
6.  Retorna `true` se ainda existe fogo (`ARVORE_QUEIMANDO` - 2) na matriz, `false` caso contrário.

### Movimento do Animal (`mover_animal`):
*   Verifica se o animal está vivo (`animal_esta_vivo_`) no início.
*   Avalia os 4 vizinhos ortogonais e suas prioridades: Água (`4`) = 3, Vazio (`0`) / Saudável (`1`) = 2, Queimada (`3`) = 1, Fogo (`2`) = 0.
*   Implementa a regra de **ficar parado:** Se está em Vazio (`0`), `iteracoes_parado_vazio < 3` e a melhor opção de movimento *não* é Água (prioridade <= 2), decide ficar parado.
*   Se decide **mover**: atualiza `linha_animal_`, `coluna_animal_`, incrementa `passos_animal_`, adiciona ao `caminho_animal_`, e reseta `iteracoes_parado_vazio`.
*   Se o destino é **Água (`4`)**: trata a interação (célula da água vira `0`, vizinhos válidos viram `1`) e incrementa `agua_encontrada_`.
*   Se decide/precisa **ficar parado**: adiciona a posição atual ao `caminho_animal_` e incrementa `iteracoes_parado_vazio` (somente se estiver em Vazio).

### Relatório Final (`imprimir_relatorio_final`):
Exibe no console o status final (sobreviveu/morreu), iteração da morte (se aplicável), posição final, passos totais, água encontrada e o caminho completo percorrido.

---

## - Regras da Simulação (Resumo)

### Fogo:
-   **Ignição:** `1` (Saudável) → `2` (Queimando) se vizinho ortogonal for `2` (respeitando a configuração de vento).
-   **Queima:** `2` (Queimando) → `3` (Queimada) após 1 iteração completa em chamas.
-   **Imunidade:** `0` (Vazio), `3` (Queimada), `4` (Água) não pegam fogo.
-   **Propagação:** Padrão ortogonal (4 direções). Com vento ativo (`config.hpp`), segue apenas as direções especificadas.

### Animal:
-   **Início:** Primeira célula `0` (Vazio) encontrada na matriz.
-   **Movimento:** Ortogonal (cima, baixo, esquerda, direita).
-   **Prioridade de Destino:** `4` (Água) > `0` (Vazio) / `1` (Saudável) > `3` (Queimada).
-   **Evitar:** Células `2` (Queimando) como destino.
-   **Interação com Água:** Ao entrar em `4`, a célula vira `0`; vizinhos válidos (não-fogo/água) viram `1`.
-   **Segunda Chance:** Se a posição do animal se torna `2` após a propagação do fogo, ele tenta se mover imediatamente para um vizinho seguro (P > 0). Morre se não conseguir.
-   **Ficar Parado:** Se estiver em uma célula `0` e a melhor opção de movimento não for Água (prioridade <= 2), pode optar por ficar parado por até 3 iterações (`iteracoes_parado_vazio < 3`).

---

## - Compilação e Execução

### Pré-requisitos
- Compilador C++ (g++ ou Clang instalado e configurado).
- Utilitário GNU Make.
- Sistema operacional Linux ou macOS.

### Passos
```bash
# 1. Navegue até a pasta raiz do projeto no terminal
# Exemplo: cd ~/Documentos/Trabalho_I
cd "Trabalho I" # Ou o nome exato da sua pasta

# 2. Limpar compilações anteriores 
make clean

# 3. Compilar o projeto
make

# 4. Executar a simulação
make run
```
A execução lerá o arquivo `input.dat` localizado na mesma pasta do `Makefile` e gerará/sobrescreverá o arquivo `output.dat` com os estados da matriz a cada iteração. O relatório final da simulação será exibido no console.

---

## - Casos de Teste / Exemplo

Utilizando o seguinte `input.dat` (5x5):
```
5 5 1 1
1 1 1 1 4
1 1 1 1 1
1 1 1 0 1
1 0 1 1 1
4 1 0 0 1
```
E com `DIRECAO_VENTO_CONFIGURADA = VENTO_TODOS;` em `src/config.hpp`.

-   **Fogo:** Inicia em (1,1).
-   **Animal:** Inicia em (2,3) (primeiro '0' encontrado).

**Relatório Final Obtido (Exemplo):**

```
--- Relatorio Final da Simulacao ---
Iteracoes executadas: 7
Animal:
  Status: Sobreviveu
  Posicao Final: (4,3)
  Passos Realizados: 4
  Vezes que encontrou agua: 0
  Caminho Percorrido (linha, coluna): (2,3) -> (2,3) -> (2,3) -> (2,3) -> (2,4) -> (3,4) -> (4,4) -> (4,3) -> (4,3)
------------------------------------
```


---

## - Análises e Observações
-   **Impacto do Vento:** A configuração do vento (`DIRECAO_VENTO_CONFIGURADA` em `config.hpp`) altera drasticamente o padrão e a velocidade de propagação. `VENTO_TODOS` resulta em uma expansão mais rápida e abrangente. Ventos direcionais podem criar frentes de fogo lineares ou setoriais, potencialmente extinguindo o fogo mais cedo se o combustível acabar na direção do vento.
-   **Sobrevivência do Animal:** Nos testes realizados com diferentes configurações de vento e mapas iniciais, o animal demonstrou uma notável capacidade de sobrevivência. Isso se deve principalmente:
    *   Ao mecanismo de **segunda chance**, que permite uma reação imediata se atingido pelo fogo.
    *   À permissão para usar células **Queimadas (3)** como rota de fuga de baixa prioridade (P1).
    *   À estratégia de **ficar parado** em locais seguros (Vazio - 0) quando não há perigo iminente ou destino de alta prioridade (Água - 4).
-   **Ordem de Execução:** A regra de que o animal se move *antes* da propagação do fogo a cada iteração é fundamental, permitindo que ele reaja ao estado do fogo da iteração *anterior*.
-   **Condição de Morte:** Embora a lógica para detectar a morte (falha na segunda chance) esteja implementada, os cenários testados não foram suficientes para encurralar o animal de forma a impedir sua fuga, devido às regras de sobrevivência favoráveis.

---

## - Melhorias e Trabalhos Futuros

A implementação atual focou em cumprir os requisitos essenciais do trabalho, modelando as regras de propagação do fogo e o comportamento reativo do animal conforme especificado. No entanto, existem diversas oportunidades para aprimorar e expandir esta simulação:

1.  **Otimização de Performance e Algoritmos de Busca:**
    *   A versão atual usa busca local (vizinhos imediatos) para o animal e não foi otimizada para matrizes extremamente grandes.
    *   **Melhoria:** Implementar **BFS (Busca em Largura)** para encontrar o caminho mais curto para a água/segurança ou **A\* (A-Estrela)** para uma fuga mais estratégica considerando o risco do fogo. A análise e aplicação destes algoritmos são temas relevantes para projetos futuros da disciplina.

2.  **Inteligência Artificial do Animal Aprimorada:**
    *   Dar ao animal um objetivo de longo prazo (ex: sair do mapa) ou memória de áreas perigosas.

3.  **Realismo da Simulação:**
    *   Introduzir probabilidade na ignição, vento dinâmico, ou diferentes tipos de terreno/combustível.

4.  **Interface e Visualização:**
    *   Desenvolver uma interface gráfica (GUI) para visualização em tempo real.


---

## - Conclusão
Este projeto implementou com sucesso a simulação de incêndio florestal e agente autônomo conforme as especificações do trabalho. A utilização de C++ e a estrutura adotada permitiram modelar as regras complexas de interação entre o fogo, o ambiente e o animal. A simulação é capaz de demonstrar diferentes padrões de propagação influenciados pelo vento e o comportamento de fuga do animal. Embora a morte do animal não tenha sido observada nos testes, a lógica para essa condição está implementada, e as regras atuais favorecem sua sobrevivência. O desenvolvimento reforçou conceitos de algoritmos, estruturas de dados (matrizes/vetores 2D) e programação em C++.

---

## - Contato
-   **Nome:** Fabrício Quintilhiano Braga
-   **Email:** [fabricioqbraga@hotmail.com](mailto:fabricioqbraga@hotmail.com)
