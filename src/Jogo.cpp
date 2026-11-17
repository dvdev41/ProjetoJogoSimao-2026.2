#include "Jogo.h"

#include <SFML/System/Clock.hpp>
#include <stdexcept>

Jogo::Jogo()
    : gerenciadorGrafico_(800, 600, "Aventura do Sapo Ninja"),
      sapoNinja_(80.0f, gerenciadorGrafico_.getPosicaoChao())
{
    const std::string caminhoTextura =
        "assets/Pixel Adventure 1/Free/Traps/Rock Head/Idle.png";
    if (!texturaCabecaPedra_.loadFromFile(caminhoTextura))
    {
        throw std::runtime_error(
            "Nao foi possivel carregar o asset: " + caminhoTextura
        );
    }
    cabecaPedra_.setTexture(texturaCabecaPedra_);
    cabecaPedra_.setScale(2.0f, 2.0f);
    cabecaPedra_.setPosition(
        400.0f,
        gerenciadorGrafico_.getPosicaoChao() -
            cabecaPedra_.getGlobalBounds().height
    );
}

void Jogo::executar()
{
    sf::Clock relogio;

    while (gerenciadorGrafico_.estaAberto())
    {
        gerenciadorGrafico_.processarEventos();
        if (!gerenciadorGrafico_.estaAberto())
        {
            break;
        }

        float tempoQuadro = relogio.restart().asSeconds();
        if (tempoQuadro > 0.1f)
        {
            tempoQuadro = 0.1f;
        }

        sapoNinja_.atualizar(
            tempoQuadro,
            static_cast<float>(gerenciadorGrafico_.getLarguraJanela()),
            gerenciadorGrafico_.getPosicaoChao()
        );
        sapoNinja_.colideCom(cabecaPedra_.getGlobalBounds());

        gerenciadorGrafico_.limpar();
        gerenciadorGrafico_.desenharChao();
        gerenciadorGrafico_.desenhar(cabecaPedra_);
        sapoNinja_.desenhar(gerenciadorGrafico_);
        gerenciadorGrafico_.exibir();
    }
}
