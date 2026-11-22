#ifndef GERENCIADOR_COLISOES_H
#define GERENCIADOR_COLISOES_H

#include <SFML/Graphics/Rect.hpp>

namespace Entidades
{
    namespace Personagens
    {
        class Jogador;
    }
}

namespace Gerenciadores
{
    class Gerenciador_Colisoes
    {
    private:
        Entidades::Personagens::Jogador* pJog1_;
        Entidades::Personagens::Jogador* pJog2_;

    public:
        Gerenciador_Colisoes();
        void setJogadores(
            Entidades::Personagens::Jogador* pJog1,
            Entidades::Personagens::Jogador* pJog2
        );
        bool verificarColisao(
            const sf::FloatRect& primeiro,
            const sf::FloatRect& segundo
        ) const;
    };
}

#endif
