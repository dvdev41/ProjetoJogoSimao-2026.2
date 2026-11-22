#include "Gerenciadores/Gerenciador_Grafico.h"

Gerenciador_Grafico::Gerenciador_Grafico(
    unsigned int largura,
    unsigned int altura,
    const std::string& titulo
)
    : janela_(sf::VideoMode(largura, altura), titulo),
      chao_(sf::Vector2f(static_cast<float>(largura), 30.0f)),
      cliquePendente_(false),
      teclaFPendente_(false)
{
    janela_.setFramerateLimit(60);
    janela_.setKeyRepeatEnabled(false);
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
        else if (evento.type == sf::Event::MouseButtonPressed &&
                 evento.mouseButton.button == sf::Mouse::Left)
        {
            posicaoClique_ = sf::Vector2i(
                evento.mouseButton.x,
                evento.mouseButton.y
            );
            cliquePendente_ = true;
        }
        else if (evento.type == sf::Event::KeyPressed &&
                 evento.key.code == sf::Keyboard::F)
        {
            teclaFPendente_ = true;
        }
    }
}

bool Gerenciador_Grafico::consumirCliqueMouse(sf::Vector2f& posicao)
{
    if (!cliquePendente_)
    {
        return false;
    }

    posicao = janela_.mapPixelToCoords(posicaoClique_);
    cliquePendente_ = false;
    return true;
}

bool Gerenciador_Grafico::consumirPressionamentoF()
{
    const bool pressionada = teclaFPendente_;
    teclaFPendente_ = false;
    return pressionada;
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
