#ifndef ENTIDADE_H
#define ENTIDADE_H

#include "Entidades/Ente.h"

#include <SFML/Graphics/Rect.hpp>
#include <sstream>

namespace Entidades
{
    class Entidade : public Ente
    {
    protected:
        float x_;
        float y_;
        std::stringstream buffer_;

        Entidade(float x = 0.0f, float y = 0.0f);

    public:
        virtual ~Entidade();
        virtual void executar() = 0;
        virtual void salvar() = 0;

        void salvarDataBuffer();
        const std::string getDadosBuffer() const;
        virtual sf::FloatRect getLimites() const;
        float getX() const;
        float getY() const;
    };
}

#endif
