#ifndef GERENCIADOR_GRAFICO_H
#define GERENCIADOR_GRAFICO_H

#include <SFML/Graphics.hpp>

#include <string>

class Gerenciador_Grafico
{
private:
    sf::RenderWindow janela_;
    sf::RectangleShape chao_;

public:
    Gerenciador_Grafico(
        unsigned int largura,
        unsigned int altura,
        const std::string& titulo
    );

    bool estaAberto() const;
    void processarEventos();
    void limpar();
    void desenharChao();
    void desenhar(const sf::Drawable& objeto);
    void exibir();

    unsigned int getLarguraJanela() const;
    float getPosicaoChao() const;
};

#endif
