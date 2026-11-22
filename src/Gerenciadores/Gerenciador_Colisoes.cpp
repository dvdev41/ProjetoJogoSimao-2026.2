#include "Gerenciadores/Gerenciador_Colisoes.h"

namespace Gerenciadores
{
    Gerenciador_Colisoes::Gerenciador_Colisoes()
        : pJog1_(0),
          pJog2_(0)
    {
    }

    void Gerenciador_Colisoes::setJogadores(
        Entidades::Personagens::Jogador* pJog1,
        Entidades::Personagens::Jogador* pJog2
    )
    {
        pJog1_ = pJog1;
        pJog2_ = pJog2;
    }

    bool Gerenciador_Colisoes::verificarColisao(
        const sf::FloatRect& primeiro,
        const sf::FloatRect& segundo
    ) const
    {
        return primeiro.intersects(segundo);
    }
}
