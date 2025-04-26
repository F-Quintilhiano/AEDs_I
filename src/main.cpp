#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <thread>
#include <chrono>

#include "simulation.hpp"
#include "config.hpp"

int main()
{
    std::cout << "Iniciando Simulador de Incendio..." << std::endl;

    Simulacao sim;

    if (!sim.carregar_estado_inicial("input.dat"))
    {
        std::cerr << "Falha ao carregar o estado inicial. Encerrando." << std::endl;
        return 1;
    }

    std::cout << "Estado inicial carregado. Iteracao 0:" << std::endl;
    sim.imprimir_matriz();

    std::ofstream arquivo_saida("output.dat");
    if (!arquivo_saida.is_open())
    {
        std::cerr << "Aviso: Nao foi possivel limpar/criar o arquivo de saida output.dat" << std::endl;
    }
    else
    {
        arquivo_saida.close();
    }

    sim.salvar_estado_arquivo("output.dat");

    bool continuar_simulacao = true;

    for (int i = 1; i <= MAX_ITERACOES; ++i)
    {

#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

        std::cout << "Executando Iteracao " << i << "..." << std::endl;

        continuar_simulacao = sim.passo_simulacao();
        sim.imprimir_matriz();

        sim.salvar_estado_arquivo("output.dat");

        if (!continuar_simulacao)
        {
            std::cout << "\nIncendio extinto na iteracao " << i << "." << std::endl;
            break;
        }

        if (i == MAX_ITERACOES)
        {
            std::cout << "\nLimite maximo de " << MAX_ITERACOES << " iteracoes atingido." << std::endl;
        }
    }

    sim.imprimir_relatorio_final();

    std::cout << "Simulador finalizado." << std::endl;
    return 0;
}