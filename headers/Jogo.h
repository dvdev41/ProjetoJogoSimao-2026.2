#ifndef JOGO_H
#define JOGO_H

#include "Gerenciadores/Gerenciador_Grafico.h"
#include "Fases/Fase1.h"

#include <SFML/System/Clock.hpp>

namespace Controle
{
    class Jogo
    {
    private:
        Gerenciadores::Gerenciador_Grafico gerenciadorGrafico_;
        Fases::Fase1 fase1_;

    public:
        Jogo();
        void executar();
    };
}

#endif
