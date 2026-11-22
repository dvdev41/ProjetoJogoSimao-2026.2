#include "Gerenciadores/Gerenciador_Grafico.h"

#include "Entidades/Ente.h"

#include <stdexcept>

namespace Gerenciadores
{
    Gerenciador_Grafico::Gerenciador_Grafico(
        unsigned int largura,
        unsigned int altura,
        const std::string& titulo
    )
        : janela_(sf::VideoMode(largura, altura), titulo),
          cliquePendente_(false),
          posicaoClique_(0, 0)
    {
        unsigned int i;
        for (i = 0; i < sf::Keyboard::KeyCount; ++i)
        {
            teclasPendentes_[i] = false;
        }

        janela_.setFramerateLimit(60);
        janela_.setKeyRepeatEnabled(false);
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
            else if (evento.type == sf::Event::KeyPressed)
            {
                const int codigo = static_cast<int>(evento.key.code);
                if (codigo >= 0 && codigo < sf::Keyboard::KeyCount)
                {
                    teclasPendentes_[codigo] = true;
                }
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

    bool Gerenciador_Grafico::consumirPressionamentoTecla(
        sf::Keyboard::Key tecla
    )
    {
        const int codigo = static_cast<int>(tecla);
        if (codigo < 0 || codigo >= sf::Keyboard::KeyCount)
        {
            return false;
        }

        const bool pressionada = teclasPendentes_[codigo];
        teclasPendentes_[codigo] = false;
        return pressionada;
    }

    void Gerenciador_Grafico::limpar()
    {
        janela_.clear(sf::Color(35, 55, 55));
    }

    void Gerenciador_Grafico::desenhar(const sf::Drawable& objeto)
    {
        janela_.draw(objeto);
    }

    void Gerenciador_Grafico::desenharEnte(Entidades::Ente* pE)
    {
        if (pE == 0)
        {
            throw std::invalid_argument(
                "Nao e possivel desenhar um Ente nulo."
            );
        }
        pE->desenhar();
    }

    void Gerenciador_Grafico::carregarTextura(
        sf::Texture& textura,
        const std::string& caminho
    ) const
    {
        if (!textura.loadFromFile(caminho))
        {
            throw std::runtime_error(
                "Nao foi possivel carregar o asset: " + caminho
            );
        }
        textura.setSmooth(false);
    }

    void Gerenciador_Grafico::exibir()
    {
        janela_.display();
    }

    void Gerenciador_Grafico::aplicarVista(const sf::View& vista)
    {
        janela_.setView(vista);
    }

    void Gerenciador_Grafico::restaurarVistaPadrao()
    {
        janela_.setView(janela_.getDefaultView());
    }

    unsigned int Gerenciador_Grafico::getLarguraJanela() const
    {
        return janela_.getSize().x;
    }

    unsigned int Gerenciador_Grafico::getAlturaJanela() const
    {
        return janela_.getSize().y;
    }
}
