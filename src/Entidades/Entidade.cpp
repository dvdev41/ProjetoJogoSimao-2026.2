#include "Entidades/Entidade.h"

namespace Entidades
{
    Entidade::Entidade(float x, float y)
        : Ente(),
          x_(x),
          y_(y),
          buffer_()
    {
    }

    Entidade::~Entidade()
    {
    }

    void Entidade::salvarDataBuffer()
    {
        buffer_.str("");
        buffer_.clear();
    }

    const std::string Entidade::getDadosBuffer() const
    {
        return buffer_.str();
    }

    sf::FloatRect Entidade::getLimites() const
    {
        return sf::FloatRect(x_, y_, 0.0f, 0.0f);
    }

    float Entidade::getX() const
    {
        return x_;
    }

    float Entidade::getY() const
    {
        return y_;
    }
}
