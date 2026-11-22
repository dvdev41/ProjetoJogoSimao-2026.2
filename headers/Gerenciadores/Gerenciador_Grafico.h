#ifndef GERENCIADOR_GRAFICO_H
#define GERENCIADOR_GRAFICO_H

#include <SFML/Graphics.hpp>

#include <string>

class Gerenciador_Grafico
{
private:
    sf::RenderWindow janela_;
    sf::RectangleShape chao_;
    bool cliquePendente_;
    sf::Vector2i posicaoClique_;
    bool teclaFPendente_;

public:
    Gerenciador_Grafico(
        unsigned int largura,
        unsigned int altura,
        const std::string& titulo
    );

    bool estaAberto() const;
    void processarEventos();
    bool consumirCliqueMouse(sf::Vector2f& posicao);
    bool consumirPressionamentoF();
    void limpar();
    void desenharChao();
    void desenhar(const sf::Drawable& objeto);
    void exibir();

    unsigned int getLarguraJanela() const;
    float getPosicaoChao() const;
};

#endif
