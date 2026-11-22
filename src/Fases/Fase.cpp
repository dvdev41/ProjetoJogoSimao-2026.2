#include "Fases/Fase.h"

namespace Fases
{
    Fase::Fase(Gerenciadores::Gerenciador_Grafico& gerenciadorGrafico)
        : Entidades::Ente(),
          lista_ents_(),
          GC_(),
          gerenciadorGrafico_(gerenciadorGrafico),
          tempoQuadro_(0.0f)
    {
        Entidades::Ente::setGG(&gerenciadorGrafico_);
    }

    Fase::~Fase()
    {
        lista_ents_.limpar();
    }

    void Fase::setTempoQuadro(float tempoQuadro)
    {
        tempoQuadro_ = tempoQuadro;
    }
}
