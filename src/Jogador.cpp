#include "Jogador.h"

#include "Gerenciador_Grafico.h"

#include <SFML/Window/Keyboard.hpp>

namespace
{
    const float VELOCIDADE_HORIZONTAL = 280.0f;
    const float ACELERACAO_GRAVIDADE = 1100.0f;
    const float VELOCIDADE_PULO = -480.0f;
}

Jogador::Jogador(float posicaoX, float topoChao)
    : velocidadeVertical_(0.0f),
      estaNoChao_(true),
      teclaPuloPressionadaAnteriormente_(false)
{
    forma_.setSize(sf::Vector2f(44.0f, 44.0f));
    forma_.setFillColor(sf::Color(55, 170, 80));
    forma_.setPosition(posicaoX, topoChao - forma_.getSize().y);
}

void Jogador::atualizar(float tempoQuadro, float larguraJanela, float topoChao)
{
    float direcaoHorizontal = 0.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        direcaoHorizontal -= 1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        direcaoHorizontal += 1.0f;
    }

    sf::Vector2f posicao = forma_.getPosition();
    posicao.x += direcaoHorizontal * VELOCIDADE_HORIZONTAL * tempoQuadro;

    float limiteDireito = larguraJanela - forma_.getSize().x;
    if (limiteDireito < 0.0f)
    {
        limiteDireito = 0.0f;
    }
    if (posicao.x < 0.0f)
    {
        posicao.x = 0.0f;
    }
    else if (posicao.x > limiteDireito)
    {
        posicao.x = limiteDireito;
    }

    const bool teclaPuloPressionada =
        sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    if (teclaPuloPressionada &&
        !teclaPuloPressionadaAnteriormente_ &&
        estaNoChao_)
    {
        velocidadeVertical_ = VELOCIDADE_PULO;
        estaNoChao_ = false;
    }
    teclaPuloPressionadaAnteriormente_ = teclaPuloPressionada;

    if (!estaNoChao_)
    {
        velocidadeVertical_ += ACELERACAO_GRAVIDADE * tempoQuadro;
        posicao.y += velocidadeVertical_ * tempoQuadro;

        if (posicao.y < 0.0f)
        {
            posicao.y = 0.0f;
            if (velocidadeVertical_ < 0.0f)
            {
                velocidadeVertical_ = 0.0f;
            }
        }

        const float baseJogador = posicao.y + forma_.getSize().y;
        if (baseJogador >= topoChao)
        {
            posicao.y = topoChao - forma_.getSize().y;
            velocidadeVertical_ = 0.0f;
            estaNoChao_ = true;
        }
    }
    else
    {
        posicao.y = topoChao - forma_.getSize().y;
    }

    forma_.setPosition(posicao);
}

void Jogador::desenhar(Gerenciador_Grafico& gerenciadorGrafico) const
{
    gerenciadorGrafico.desenhar(forma_);
}
