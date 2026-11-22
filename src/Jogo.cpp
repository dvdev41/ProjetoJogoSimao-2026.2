#include "Jogo.h"

#include <algorithm>

namespace Controle
{
    Jogo::Jogo()
        : gerenciadorGrafico_(960, 540, "Ninja Frog - Fase 1"),
          fase1_(gerenciadorGrafico_)
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

            const float tempoQuadro =
                std::min(relogio.restart().asSeconds(), 0.1f);
            fase1_.setTempoQuadro(tempoQuadro);
            fase1_.executar();

            gerenciadorGrafico_.limpar();
            gerenciadorGrafico_.desenharEnte(&fase1_);
            gerenciadorGrafico_.exibir();
        }
    }
}
