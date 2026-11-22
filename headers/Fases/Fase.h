#ifndef FASE_H
#define FASE_H

#include "Entidades/Ente.h"
#include "Gerenciadores/Gerenciador_Colisoes.h"
#include "Gerenciadores/Gerenciador_Grafico.h"
#include "Listas/ListaEntidades.h"

namespace Fases
{
    class Fase : public Entidades::Ente
    {
    protected:
        Listas::ListaEntidades lista_ents_;
        Gerenciadores::Gerenciador_Colisoes GC_;
        Gerenciadores::Gerenciador_Grafico& gerenciadorGrafico_;
        float tempoQuadro_;

        explicit Fase(Gerenciadores::Gerenciador_Grafico& gerenciadorGrafico);

    public:
        virtual ~Fase();
        virtual void executar() = 0;
        virtual void desenhar() = 0;
        void setTempoQuadro(float tempoQuadro);
    };
}

#endif
