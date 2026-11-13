#include "Jogo.h"

#include <SFML/System/Clock.hpp>

Jogo::Jogo()
    : gerenciadorGrafico_(800, 600, "Jogo do Simao"),
      jogador_(80.0f, gerenciadorGrafico_.getPosicaoChao())
{
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

        jogador_.atualizar(
            tempoQuadro,
            static_cast<float>(gerenciadorGrafico_.getLarguraJanela()),
            gerenciadorGrafico_.getPosicaoChao()
        );

        gerenciadorGrafico_.limpar();
        gerenciadorGrafico_.desenharChao();
        jogador_.desenhar(gerenciadorGrafico_);
        gerenciadorGrafico_.exibir();
    }
}
