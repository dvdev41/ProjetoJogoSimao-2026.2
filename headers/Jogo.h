#ifndef JOGO_H
#define JOGO_H

#include "Gerenciadores/Gerenciador_Grafico.h"
#include "Entidades/Personagens/Jogador.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class Jogo
{
private:
    Gerenciador_Grafico gerenciadorGrafico_;
    SapoNinja sapoNinja_;
    sf::Texture texturaCabecaPedra_;
    sf::Sprite cabecaPedra_;

public:
    Jogo();
    void executar();
};

#endif
