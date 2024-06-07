#ifndef AUTOMATO_H
#define AUTOMATO_H

#include <allegro5/allegro.h> // Inclua os cabeçalhos da Allegro aqui
#include <allegro5/allegro_primitives.h>

// Declaração da estrutura que representa o autômato celular
typedef struct {
    // Campos necessários para representar o autômato celular
    int dimensao;
    int **celulas; // matriz para armazenar o estado das células
    int **proximaGeracao; // matriz para armazenar o estado da próxima geração
} AutomatoCelular;

// Funções públicas do TAD AutomatoCelular
// São obrigatórias de acordo com o que foi solicitado no enunciado do trabalho
AutomatoCelular* alocarReticulado(int dimensao);
void desalocarReticulado(AutomatoCelular* automato);
AutomatoCelular* leituraReticulado(const char* nomeArquivo, int *numGeracoes);
int contarCelulasVivasVizinhas(AutomatoCelular* automato, int linha, int coluna);
void evoluirReticulado(AutomatoCelular* automato, int geracoes);
void imprimeReticulado(ALLEGRO_DISPLAY *display, AutomatoCelular *automato);

#endif


