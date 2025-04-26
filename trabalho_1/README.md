# 📄 TRABALHO AEDS I - Simulação de Propagação de Incêndios

<h1 align='center'>
   Simulação de Propagação de Incêndio Florestal 
</h1>

<div align='center'>

  Algoritmos e Estrutura de Dados I
 
  CEFET-MG - Campus Divinópolis 
 
  Professor Michel Pires da Silva

  Abril de 2025   
</div>

<details>
  <summary><b>📑 Sumário</b></summary>
  <ol>
    <li><a href="#-introdução">Introdução</a></li>
    <li><a href="#-objetivos">Objetivos</a></li>
    <li><a href="#-ambiente-de-criação">Ambiente de Criação</a></li>
    <li><a href="#-organização-dos-arquivos">Organização dos Arquivos</a></li>
    <li><a href="#-implementação">Implementação</a></li>
    <li><a href="#-regras-da-simulação">Regras da Simulação</a></li>
    <li><a href="#-compilação-e-execução">Compilação e Execução</a></li>
    <li><a href="#-casos-de-teste--exemplo">Casos de Teste / Exemplo</a></li>
    <li><a href="#-análises-e-observações">Análises e Observações</a></li>
    <li><a href="#-melhorias-e-trabalhos-futuros">Melhorias e Trabalhos Futuros</a></li>
    <li><a href="#-conclusão">Conclusão</a></li>
    <li><a href="#-contato">Contato</a></li>
  </ol>
</details>

## ☕ Introdução

Este projeto, desenvolvido para a disciplina de Algoritmos e Estrutura de Dados I no CEFET-MG Campus Divinópolis sob orientação do Prof. Michel Pires da Silva, implementa uma simulação da propagação de incêndios em uma floresta representada por uma matriz bidimensional. A simulação utiliza a linguagem C++ e uma abordagem orientada a objetos para modelar o ambiente, o comportamento do fogo sob diferentes condições de vento (configuráveis via `src/config.hpp`) e a tentativa de sobrevivência de um agente autônomo (animal) que busca escapar das chamas, seguindo regras de prioridade e interagindo com elementos como água. O estado da simulação é lido de um arquivo `input.dat` e a evolução da floresta é registrada a cada iteração em `output.dat`.

## 🎯 Objetivos

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

## 🖥️ Ambiente de Criação

- **Linguagem:** C++ (utilizando padrão C++11 ou superior)
- **Compilador:** g++ (Clang/LLVM padrão no macOS ou GCC instalado via Homebrew/Xcode)
- **Sistema Operacional:** macOS Monterey (executado em MacBook Pro 2016)
- **IDE:** Visual Studio Code (VS Code)
- **Automação:** GNU Make

## 📂 Organização dos Arquivos

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

## 💡 Implementação

A simulação foi implementada utilizando C++ e Programação Orientada a Objetos. A classe principal `Simulacao` encapsula o estado e a lógica da simulação.

**Resumo:**

- **Representação da Floresta:** `std::vector<std::vector<int>> matriz_` + `proxima_matriz_` para atualização síncrona.
- **Carregamento:** Lê `input.dat`, valida dimensões, inicializa fogo e animal.
- **Passo de Simulação:** Move o animal, propaga fogo, atualiza estados.
- **Movimentação:** Avalia prioridades de vizinhos e movimenta estrategicamente.
- **Relatório Final:** Mostra caminho percorrido, status de sobrevivência e estatísticas.

---

## 🔥 Regras da Simulação

- **Fogo:**
  - Árvore saudável (1) vira queimando (2) se vizinha de queimando (2).
  - Árvore queimando (2) vira queimada (3) após uma iteração.

- **Animal:**
  - Move ortogonalmente.
  - Prioriza: Água (4) > Vazio/Saudável (0/1) > Queimada (3).
  - Evita fogo (2).
  - Pode permanecer parado se estiver seguro em Vazio (0) até 3 iterações.
  - Tem direito a uma segunda tentativa de fuga se for atingido.

---

## 👨🏻‍💻 Compilação e Execução

**Pré-requisitos:**
- Compilador C++ (g++ ou clang++)
- GNU Make

**Comandos:**

```bash
# Navegar até a pasta do projeto
cd "Trabalho I"

# Limpar build anterior (opcional)
make clean

# Compilar o projeto
make

# Executar a simulação
make run
```

---

## 🧪 Casos de Teste / Exemplo

**Exemplo de `input.dat`:**

```
5 5 1 1
1 1 1 1 4
1 1 1 1 1
1 1 1 0 1
1 0 1 1 1
4 1 0 0 1
```

Configuração de vento: `VENTO_TODOS`.

Relatório esperado:

```
--- Relatório Final da Simulação ---
Iterações executadas: 7
Animal:
  Status: Sobreviveu
  Posição Final: (4,3)
  Passos Realizados: 4
  Vezes que encontrou água: 0
  Caminho: (2,3) → (2,3) → (2,3) → (2,3) → (2,4) → (3,4) → (4,4) → (4,3) → (4,3)
-------------------------------------
```

---

## 📊 Análises e Observações

- O vento influencia fortemente o padrão de propagação.
- O animal demonstrou boa sobrevivência, graças à estratégia de segunda chance e movimentação inteligente.
- A ordem dos eventos (mover animal → propagar fogo) é essencial para o sucesso.

---

## 💡 Melhorias e Trabalhos Futuros

- **Algoritmos de Busca:** Implementar BFS ou A* para movimentação estratégica do animal.
- **Vento Dinâmico:** Alterar direção e intensidade do vento durante a simulação.
- **Propagação Probabilística:** Tornar o comportamento do fogo mais realista.
- **Topografia Variada:** Incluir tipos de terreno com diferentes resistências ao fogo.
- **Interface Gráfica:** Utilizar SFML, SDL ou Qt para uma visualização gráfica da simulação.

---

## 🏁 Conclusão

O projeto atingiu com sucesso todos os objetivos estabelecidos, modelando de forma eficiente a interação entre fogo, ambiente e agente autônomo. Conceitos de estruturas de dados, algoritmos e programação C++ foram aplicados e reforçados.

---

## 📪 Contato

**Nome:** Fabrício Quintilhiano Braga  
**Email:** [fabricioqbraga@hotmail.com](mailto:fabricioqbraga@hotmail.com)
