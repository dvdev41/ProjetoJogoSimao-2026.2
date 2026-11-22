#include "Entidades/Ente.h"

#include "Gerenciadores/Gerenciador_Grafico.h"

#include <stdexcept>

namespace Entidades
{
    int Ente::proximoId_ = 1;
    Gerenciadores::Gerenciador_Grafico* Ente::pGG_ = 0;

    Ente::Ente(const sf::Drawable* figura)
        : id_(proximoId_++),
          pFig_(figura)
    {
    }

    Ente::~Ente()
    {
    }

    void Ente::setFigura(const sf::Drawable* figura)
    {
        pFig_ = figura;
    }

    void Ente::desenhar()
    {
        if (pFig_ != 0)
        {
            if (pGG_ == 0)
            {
                throw std::logic_error(
                    "Gerenciador grafico nao foi configurado."
                );
            }
            pGG_->desenhar(*pFig_);
        }
    }

    void Ente::setGG(Gerenciadores::Gerenciador_Grafico* pG)
    {
        pGG_ = pG;
    }

    int Ente::getId() const
    {
        return id_;
    }
}
