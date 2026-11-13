#ifndef JOGO_H
#define JOGO_H

#include "Gerenciador_Grafico.h"
#include "Jogador.h"

class Jogo
{
private:
    Gerenciador_Grafico gerenciadorGrafico_;
    Jogador jogador_;

public:
    Jogo();
    void executar();
};

#endif
