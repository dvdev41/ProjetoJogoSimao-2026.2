#ifndef GERENCIADOR_GRAFICO_H
#define GERENCIADOR_GRAFICO_H

#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <string>

namespace Entidades
{
    class Ente;
}

namespace Gerenciadores
{
    class Gerenciador_Grafico
    {
    private:
        sf::RenderWindow janela_;
        bool cliquePendente_;
        sf::Vector2i posicaoClique_;
        bool teclasPendentes_[sf::Keyboard::KeyCount];

    public:
        Gerenciador_Grafico(
            unsigned int largura,
            unsigned int altura,
            const std::string& titulo
        );

        bool estaAberto() const;
        void processarEventos();
        bool consumirCliqueMouse(sf::Vector2f& posicao);
        bool consumirPressionamentoTecla(sf::Keyboard::Key tecla);
        void limpar();
        void desenhar(const sf::Drawable& objeto);
        void desenharEnte(Entidades::Ente* pE);
        void carregarTextura(sf::Texture& textura, const std::string& caminho) const;
        void exibir();
        void aplicarVista(const sf::View& vista);
        void restaurarVistaPadrao();

        unsigned int getLarguraJanela() const;
        unsigned int getAlturaJanela() const;
    };
}

#endif
