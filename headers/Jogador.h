#ifndef JOGADOR_H
#define JOGADOR_H

#include <SFML/Graphics/RectangleShape.hpp>

class Gerenciador_Grafico;

class Jogador
{
private:
    sf::RectangleShape forma_;
    float velocidadeVertical_;
    bool estaNoChao_;
    bool teclaPuloPressionadaAnteriormente_;

public:
    Jogador(float posicaoX, float topoChao);
    void atualizar(float tempoQuadro, float larguraJanela, float topoChao);
    void desenhar(Gerenciador_Grafico& gerenciadorGrafico) const;
};

#endif
