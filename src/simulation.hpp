#ifndef SIMULATION_HPP
#define SIMULATION_HPP
#include <vector>
#include <string>

using namespace std;

class Simulacao
{
public:
    Simulacao();

    void imprimir_relatorio_final() const;

    bool carregar_estado_inicial(const string &nome_arquivo);

    bool passo_simulacao();

    void imprimir_matriz() const;

    void salvar_estado_arquivo(const string &nome_arquivo) const;

private:
    int linhas_;
    int colunas_;
    int linha_fogo_inicial_;
    int coluna_fogo_inicial_;

    void mover_animal();

    int linha_animal_;
    int coluna_animal_;
    int passos_animal_;
    int agua_encontrada_;
    int iteracao_preso_;
    bool animal_esta_vivo_;
    bool animal_inicializado_;

    vector<pair<int, int> > caminho_animal_;

    int iteracao_atual;
    int iteracoes_parado_vazio;

    vector<vector<int> > matriz_;

    vector<vector<int> > proxima_matriz_;
};

#endif