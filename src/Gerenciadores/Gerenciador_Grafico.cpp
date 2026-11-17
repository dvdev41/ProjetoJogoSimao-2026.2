#include "Gerenciadores/Gerenciador_Grafico.h"

Gerenciador_Grafico::Gerenciador_Grafico(
    unsigned int largura,
    unsigned int altura,
    const std::string& titulo
)
    : janela_(sf::VideoMode(largura, altura), titulo),
      chao_(sf::Vector2f(static_cast<float>(largura), 30.0f))
{
    janela_.setFramerateLimit(60);
    chao_.setPosition(
        0.0f,
        static_cast<float>(altura) - chao_.getSize().y
    );
    chao_.setFillColor(sf::Color(80, 145, 75));
}

bool Gerenciador_Grafico::estaAberto() const
{
    return janela_.isOpen();
}

void Gerenciador_Grafico::processarEventos()
{
    sf::Event evento;
    while (janela_.pollEvent(evento))
    {
        if (evento.type == sf::Event::Closed)
        {
            janela_.close();
        }
    }
}

void Gerenciador_Grafico::limpar()
{
    janela_.clear(sf::Color(130, 195, 235));
}

void Gerenciador_Grafico::desenharChao()
{
    janela_.draw(chao_);
}

void Gerenciador_Grafico::desenhar(const sf::Drawable& objeto)
{
    janela_.draw(objeto);
}

void Gerenciador_Grafico::exibir()
{
    janela_.display();
}

unsigned int Gerenciador_Grafico::getLarguraJanela() const
{
    return janela_.getSize().x;
}

float Gerenciador_Grafico::getPosicaoChao() const
{
    return chao_.getPosition().y;
}
