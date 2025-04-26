#include "simulation.hpp" 
#include "config.hpp"   
#include <fstream>    
#include <iostream>    
#include <stdexcept>   

Simulacao::Simulacao(): 
linhas_(0), 
colunas_(0), 


linha_animal_(-1),
coluna_animal_(-1),
passos_animal_(0),
agua_encontrada_(0),
iteracao_preso_(-1),
animal_esta_vivo_(true),
animal_inicializado_(false),
iteracao_atual(0),
iteracoes_parado_vazio(0){

}

bool Simulacao::carregar_estado_inicial(const string& nome_arquivo){

    ifstream arquivo_entrada(nome_arquivo);

    if(!arquivo_entrada.is_open()){
        cerr<< "Erro: Não foi possível abrir o arquivo de entrada: " << nome_arquivo << endl;
        return false;
    }
    
    if(!(arquivo_entrada >> linhas_ >> colunas_ >> linha_fogo_inicial_ >> coluna_fogo_inicial_)){
        cerr << "Erro: Falha ao ler as dimensões ou posição inicial do fogo no arquivo." << endl;
        
        arquivo_entrada.close();
        return false;
    }


    if (linhas_<=0 || colunas_ <=0){
        cerr << "Erro: Dimensoes da matriz invalidas. " << endl;
        arquivo_entrada.close();
        return false;
    }
    try {
      
        matriz_.resize(linhas_, std::vector<int>(colunas_));
    } catch (const std::bad_alloc& e) {
      
        std::cerr << "Erro: Falha ao alocar memoria para a matriz: " << e.what() << std::endl;
         arquivo_entrada.close();
        return false;
    }

  
    for (int i = 0; i < linhas_; i++){
        for (int j = 0; j < colunas_; j++){
            if(!(arquivo_entrada >> matriz_ [i][j])){
                cerr << "Erro: falha ao ler o valor da celula ["<< i <<"]["<< j << "]. " <<endl;
                arquivo_entrada.close();
                return false;
            }
        }
    }

    arquivo_entrada. close();
    cout << "Arquivo" <<nome_arquivo<< "lido com sucesso. Dimensões: "
         <<linhas_ << "X" << colunas_ << "." << endl;
         
    if (linha_fogo_inicial_ >= 0 && linha_fogo_inicial_ < linhas_ &&
        coluna_fogo_inicial_ >= 0 && coluna_fogo_inicial_ < colunas_)
    {
        
        if (matriz_[linha_fogo_inicial_][coluna_fogo_inicial_] == ARVORE_SAUDAVEL ||
            matriz_[linha_fogo_inicial_][coluna_fogo_inicial_] == VAZIO)       {
             cout << "Iniciando fogo na posicao: (" << linha_fogo_inicial_ << "," << coluna_fogo_inicial_ << ")" << endl;
             matriz_[linha_fogo_inicial_][coluna_fogo_inicial_] = ARVORE_QUEIMANDO; 
        } else {
             cerr << "Aviso: Nao foi possivel iniciar fogo em (" << linha_fogo_inicial_ << "," << coluna_fogo_inicial_
                  << "). Estado atual da celula: " << matriz_[linha_fogo_inicial_][coluna_fogo_inicial_] << endl;
            
        }
    } else {
        cerr << "Erro: Coordenadas iniciais do fogo (" << linha_fogo_inicial_ << "," << coluna_fogo_inicial_
             << ") estao fora dos limites da matriz." << endl;
       
        return false; 
    }
    
          
    for (int i = 0; i < linhas_; ++i) {
        for (int j = 0; j < colunas_; ++j) {
            if (matriz_[i][j] == VAZIO) {
                linha_animal_ = i;
                coluna_animal_ = j;
                animal_inicializado_ = true; 
                caminho_animal_.push_back(make_pair(linha_animal_, coluna_animal_));
                std::cout << "Animal inicializado na posicao: (" << linha_animal_ << "," << coluna_animal_ << ")" << std::endl;
                goto animal_encontrado; 
            }
        }
    }
    animal_encontrado: 
    if (!animal_inicializado_) {
        std::cerr << "Aviso: Nenhuma celula VAZIO encontrada para iniciar o animal." << std::endl;
        animal_esta_vivo_ = false; 
    }


        
    return true;

}

void Simulacao :: imprimir_matriz() const {
        cout << "--- Estado Atual da Floresta ---" << endl;
        for (int i=0; i<linhas_; i++){
            for (int j=0; j<colunas_; j++){
                
                cout << matriz_[i][j] << " ";

            }
            cout << endl;
        }
        cout << "----------------------------------------" << endl;
    }

bool Simulacao::passo_simulacao(){
    if(!animal_esta_vivo_){

    } else {
        mover_animal();
    }
    
    proxima_matriz_.resize(linhas_, vector<int>(colunas_)); 
    proxima_matriz_ = matriz_; 

    
    
    for(int i=0; i<linhas_; i++){
        for(int j=0; j<colunas_; j++){

        
        if(matriz_[i][j]== ARVORE_QUEIMANDO){
            proxima_matriz_[i][j] = ARVORE_QUEIMADA;
                
        }
        
        else if(matriz_[i][j]== ARVORE_SAUDAVEL){

            
            bool pegou_fogo = false;

            
            if ((DIRECAO_VENTO_CONFIGURADA & VENTO_CIMA) && (i > 0) && (matriz_[i - 1][j] == ARVORE_QUEIMANDO)) {
                pegou_fogo = true;
            }
            
            if (!pegou_fogo && (DIRECAO_VENTO_CONFIGURADA & VENTO_DIREITA) && (j < colunas_ - 1) && (matriz_[i][j + 1] == ARVORE_QUEIMANDO)) {
                pegou_fogo = true;
            }
            
            if (!pegou_fogo && (DIRECAO_VENTO_CONFIGURADA & VENTO_BAIXO) && (i < linhas_ - 1) && (matriz_[i + 1][j] == ARVORE_QUEIMANDO)) {
                pegou_fogo = true;
            }
            
            if (!pegou_fogo && (DIRECAO_VENTO_CONFIGURADA & VENTO_ESQUERDA) && (j > 0) && (matriz_[i][j - 1] == ARVORE_QUEIMANDO)) {
                pegou_fogo = true;
            }
           
            
            if(pegou_fogo){
                proxima_matriz_[i][j] = ARVORE_QUEIMANDO;
            }
        }
    }
        

    }
    
    matriz_ = proxima_matriz_;
   
    if (animal_esta_vivo_ && animal_inicializado_) { 
        if (matriz_[linha_animal_][coluna_animal_] == ARVORE_QUEIMANDO) {
            std::cout << "!!! Animal atingido pelo fogo em (" << linha_animal_ << "," << coluna_animal_ << ") na iteracao " << iteracao_atual +1 << "! Tentando escapar (Segunda Chance)..." << std::endl;

           
            int linha_anterior = linha_animal_;
            int coluna_anterior = coluna_animal_;
            mover_animal(); 

           
            if (linha_animal_ == linha_anterior && coluna_animal_ == coluna_anterior) {
               
                std::cout << "!!! Animal nao conseguiu escapar e morreu na iteracao " << iteracao_atual +1 << "." << std::endl;
                animal_esta_vivo_ = false;
                iteracao_preso_ = iteracao_atual;
            } else {
                 std::cout << "!!! Animal escapou para (" << linha_animal_ << "," << coluna_animal_ << ")." << std::endl;
                 
            }
        }
    }
            


    iteracao_atual++;
   
    bool fogo_ainda_existe = false;
    for (int i = 0; i < linhas_; ++i) {
       for (int j = 0; j < colunas_; ++j) {
            if(matriz_[i][j] == ARVORE_QUEIMANDO) {
                fogo_ainda_existe = true;
                break;
            }
       }
       if(fogo_ainda_existe) break;
    }

   return fogo_ainda_existe;
}

void Simulacao::salvar_estado_arquivo(const std::string& nome_arquivo) const {
    
    std::ofstream arquivo_saida(nome_arquivo, std::ios::app);

    if (!arquivo_saida.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo de saida: " << nome_arquivo << std::endl;
        return;
    }

    
    arquivo_saida << "Iteracao " << iteracao_atual << ":" << std::endl;

    
    for (int i = 0; i < linhas_; ++i) {
        for (int j = 0; j < colunas_; ++j) {
            arquivo_saida << matriz_[i][j] << (j == colunas_ - 1 ? "" : " "); 
        }
        arquivo_saida << std::endl; 
    }
    arquivo_saida << std::endl; 

   
}

void Simulacao::mover_animal() {

    if (!animal_inicializado_ || !animal_esta_vivo_) {
        return;
    }

    
    int linha_anterior = linha_animal_;
    int coluna_anterior = coluna_animal_;
    
    int estado_celula_atual = matriz_[linha_anterior][coluna_anterior]; 

    int d_linha[] = {-1, 0, 1, 0}; 
    int d_coluna[] = {0, 1, 0, -1};

    int melhor_linha = -1;
    int melhor_coluna = -1;
    int melhor_prioridade = -1;

    
    for (int k = 0; k < 4; ++k) {
        int viz_l = linha_animal_ + d_linha[k];
        int viz_c = coluna_animal_ + d_coluna[k];
        int prioridade_atual = 0;

        if (viz_l >= 0 && viz_l < linhas_ && viz_c >= 0 && viz_c < colunas_) {
            int estado_vizinho = matriz_[viz_l][viz_c];

            if (estado_vizinho == AGUA) {
                prioridade_atual = 3;
            } else if (estado_vizinho == VAZIO || estado_vizinho == ARVORE_SAUDAVEL) {
                prioridade_atual = 2;
            } else if (estado_vizinho == ARVORE_QUEIMADA) {
                prioridade_atual = 1;
            }
            

            if (prioridade_atual > melhor_prioridade) {
                melhor_prioridade = prioridade_atual;
                melhor_linha = viz_l;
                melhor_coluna = viz_c;
            }
        }
    } 

    
    bool deve_ficar_parado = false;

    
    if (estado_celula_atual == VAZIO && iteracoes_parado_vazio < 3 && melhor_prioridade <= 2 && melhor_prioridade > 0) {
         deve_ficar_parado = true;
    }

    
    if (melhor_prioridade > 0 && !deve_ficar_parado) {
        
        linha_animal_ = melhor_linha;
        coluna_animal_ = melhor_coluna;
        passos_animal_++;
        caminho_animal_.push_back(make_pair(linha_animal_, coluna_animal_));

        
        iteracoes_parado_vazio = 0;

        
        if (matriz_[linha_animal_][coluna_animal_] == AGUA) { 
             agua_encontrada_++;
             matriz_[linha_animal_][coluna_animal_] = VAZIO; 

             
             for (int k = 0; k < 4; ++k) {
                 int umido_l = linha_animal_ + d_linha[k];
                 int umido_c = coluna_animal_ + d_coluna[k];
                 if (umido_l >= 0 && umido_l < linhas_ && umido_c >= 0 && umido_c < colunas_) {
                    if (matriz_[umido_l][umido_c] != ARVORE_QUEIMANDO && matriz_[umido_l][umido_c] != AGUA) {
                         matriz_[umido_l][umido_c] = ARVORE_SAUDAVEL;
                    }
                 }
             }
        }
    } else { 
        
        caminho_animal_.push_back(make_pair(linha_animal_, coluna_animal_)); 

        
        if (estado_celula_atual == VAZIO) {
             iteracoes_parado_vazio++;
        } else {
             iteracoes_parado_vazio = 0; 
        }
        
    }
} 
void Simulacao::imprimir_relatorio_final() const {
    std::cout << "\n--- Relatorio Final da Simulacao ---" << std::endl;
    std::cout << "Iteracoes executadas: " << iteracao_atual << std::endl;

    if (!animal_inicializado_) {
        std::cout << "Animal: Nao foi possivel inicializar (sem posicao inicial)." << std::endl;
    } else {
        std::cout << "Animal:" << std::endl;
        if (animal_esta_vivo_) {
            std::cout << "  Status: Sobreviveu" << std::endl;
            std::cout << "  Posicao Final: (" << linha_animal_ << "," << coluna_animal_ << ")" << std::endl;
        } else {
            std::cout << "  Status: Morreu ou ficou preso" << std::endl;
            std::cout << "  Ultima Posicao: (" << linha_animal_ << "," << coluna_animal_ << ")" << std::endl;
            std::cout << "  Iteracao da Morte/Prisao: " << iteracao_preso_ << std::endl;
        }
        std::cout << "  Passos Realizados: " << passos_animal_ << std::endl;
        std::cout << "  Vezes que encontrou agua: " << agua_encontrada_ << std::endl;
        std::cout << "  Caminho Percorrido (linha, coluna): ";
        for (size_t i = 0; i < caminho_animal_.size(); ++i) {
            std::cout << "(" << caminho_animal_[i].first << "," << caminho_animal_[i].second << ")" << (i == caminho_animal_.size() - 1 ? "" : " -> ");
        }
        std::cout << std::endl;
    }
    std::cout << "------------------------------------" << std::endl;
}


