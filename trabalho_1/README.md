
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
    <li><a href="#-análises-e-observações">Melhorias e Trabalhos Futuros</a></li>
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

💡 IMPLEMENTAÇÃO

A simulação foi implementada utilizando C++ e Programação Orientada a Objetos. A classe principal Simulacao encapsula o estado e a lógica da simulação.

Representação da Floresta: Um std::vector<std::vector<int>> matriz_ armazena o estado de cada célula. Uma matriz auxiliar proxima_matriz_ é utilizada para calcular o estado da próxima iteração, garantindo atualizações simultâneas.
Carregamento (carregar_estado_inicial): Lê input.dat (dimensões, fogo inicial, matriz). Valida os dados, aloca a matriz_, define a célula inicial do fogo como ARVORE_QUEIMANDO (2) e localiza a primeira célula VAZIO (0) para posicionar o animal internamente (linha_animal_, coluna_animal_).
Passo da Simulação (passo_simulacao): Orquestra a ordem dos eventos:
Chama mover_animal() (se vivo).
Calcula propagação/envelhecimento do fogo (considerando vento) para proxima_matriz_.
Atualiza matriz_ com proxima_matriz_.
Verifica se matriz_[linha_animal_][coluna_animal_] == 2. Se sim, aciona a segunda chance: chama mover_animal() novamente; se a posição não mudar, define animal_esta_vivo_ = false e registra iteracao_preso_.
Incrementa iteracao_atual.
Retorna true se ainda existe fogo (2), false caso contrário.
Movimento do Animal (mover_animal):
Verifica se o animal está vivo.
Avalia vizinhos ortogonais e suas prioridades (Água=3, Vazio/Saudável=2, Queimada=1, Fogo=0).
Implementa a regra de ficar parado: Se está em VAZIO, iteracoes_parado_vazio < 3 e a melhor opção não é Água (prioridade <= 2), decide ficar parado.
Se move: atualiza posição interna, incrementa passos_animal_, registra no caminho, reseta iteracoes_parado_vazio. Se o destino é Água, trata a interação (célula vira 0, vizinhos viram 1).
Se fica parado: registra no caminho, incrementa iteracoes_parado_vazio (se estiver em VAZIO).
Relatório Final (imprimir_relatorio_final): Exibe no console status final, iteração da morte (se houver), posição final, passos, água encontrada e caminho percorrido.

🔥 REGRAS DA SIMULAÇÃO (RESUMO)

Fogo:
1 -> 2 (vizinho 2, respeita vento).
2 -> 3 (após 1 iteração).
Propagação padrão ortogonal; vento configurável em config.hpp.
Animal:
Começa no primeiro 0.
Move ortogonalmente.
Prioridade: 4 > 0/1 > 3. Evita 2.
Água: Célula 4 vira 0, vizinhos 1.
Segunda Chance: Se atingido por 2, tenta mover. Morre se cercado.
Ficar Parado: Pode ficar em 0 por até 3 turnos se não houver água próxima como melhor opção.

👨🏻‍💻 COMPILAÇÃO E EXECUÇÃO

Pré-requisitos
Compilador C++ (g++ ou Clang).
GNU Make.

Passos
# 1. Navegue até a pasta do projeto no terminal
cd "Trabalho I" # Ou o nome da sua pasta

# 2. Limpar compilações anteriores (opcional)
make clean

# 3. Compilar o projeto
make

# 4. Executar a simulação
make run

A execução lerá input.dat e gerará/sobrescreverá output.dat. O relatório final aparecerá no console.

🧪 CASOS DE TESTE / EXEMPLO

Utilizando o seguinte input.dat (5x5):

5 5 1 1
1 1 1 1 4
1 1 1 1 1
1 1 1 0 1
1 0 1 1 1
4 1 0 0 1

DIRECAO_VENTO_CONFIGURADA = VENTO_TODOS; em config.hpp.

Fogo: Inicia em (1,1).
Animal: Inicia em (2,3) (primeiro '0' encontrado).
Relatório Final Esperado (Console):

--- Relatorio Final da Simulacao ---
Iteracoes executadas: 7
Animal:
  Status: Sobreviveu
  Posicao Final: (4,3) 
  Passos Realizados: 4 
  Vezes que encontrou agua: 0 
  Caminho Percorrido (linha, coluna): (2,3) -> (2,3) -> (2,3) -> (2,3) -> (2,4) -> (3,4) -> (4,4) -> (4,3) -> (4,3)
------------------------------------



📊 ANÁLISES E OBSERVAÇÕES

O vento tem um impacto significativo na forma e velocidade da propagação do fogo. A ausência de vento (VENTO_TODOS) leva à maior área queimada, enquanto ventos direcionais podem extinguir o fogo mais rapidamente.
O animal demonstrou alta capacidade de sobrevivência nos testes realizados. A combinação da "segunda chance" reativa, a capacidade de usar células Queimadas (3) como rota de fuga de baixa prioridade, e a estratégia de poder esperar em células Vazias (0) mostraram-se eficazes para evitar a morte por fogo ou exaustão nos cenários testados.
A ordem dos eventos (animal move, fogo propaga, segunda chance) é fundamental para a dinâmica observada.
A implementação da lógica de morte parece correta, mas os cenários testados não foram suficientes para acioná-la devido às regras de sobrevivência do animal.

💡 MELHORIAS E TRABALHOS FUTUROS

A implementação atual focou em cumprir os requisitos essenciais do trabalho, modelando as regras de propagação do fogo e o comportamento reativo do animal conforme especificado. No entanto, existem diversas oportunidades para aprimorar e expandir esta simulação:

Otimização de Performance e Algoritmos de Busca:
A presente versão não priorizou a otimização de performance computacional para cenários de larga escala, nem utilizou algoritmos de busca avançados para o planejamento de rotas do animal. O movimento do animal é baseado em uma busca local e gulosa (greedy), analisando apenas vizinhos imediatos.
Melhoria: Poderiam ser implementados algoritmos como Busca em Largura (BFS) para encontrar o caminho mais curto até a água ou uma zona segura, ou o A (A-Estrela)* para uma busca heurística que considere a distância e o risco do fogo, tornando o comportamento do animal mais estratégico e eficiente.
A análise e aplicação desses algoritmos de otimização e pathfinding são temas relevantes a serem explorados em projetos futuros da disciplina, visando soluções mais robustas e performáticas.
Inteligência Artificial do Animal Aprimorada:
Além do pathfinding, o animal poderia ter um "objetivo" de longo prazo (ex: alcançar uma borda específica do mapa considerada segura) em vez de apenas reagir localmente.
Poderia aprender com o ambiente ou ter memória de áreas perigosas que não sejam apenas a última posição visitada.
Realismo da Simulação:
Propagação Probabilística: Introduzir um fator de probabilidade para que uma árvore saudável pegue fogo, dependendo de fatores como tipo de vegetação (não modelado) ou umidade (parcialmente modelado pela água).
Vento Dinâmico: Permitir que a direção e/ou intensidade do vento mudem durante a simulação.
Topografia e Combustível: Adicionar diferentes tipos de terreno que influenciem a velocidade de propagação do fogo.
Interface e Visualização:
Desenvolver uma interface gráfica (GUI) usando bibliotecas como SFML, SDL ou Qt para uma visualização mais clara e interativa da simulação, em vez da saída no console.

🏁 CONCLUSÃO

Este projeto implementou com sucesso a simulação de incêndio florestal e agente autônomo conforme as especificações do trabalho. A utilização de C++ e a estrutura adotada permitiram modelar as regras complexas de interação entre o fogo, o ambiente e o animal. A simulação é capaz de demonstrar diferentes padrões de propagação influenciados pelo vento e o comportamento de fuga do animal. Embora a morte do animal não tenha sido observada nos testes, a lógica para essa condição está implementada e as regras atuais favorecem sua sobrevivência. O desenvolvimento reforçou conceitos de algoritmos, estruturas de dados (matrizes/vetores 2D) e programação em C++.

📪 CONTATO

Nome: Fabrício Quintilhiano Braga
Email: fabricioqbraga@hotmail.com


