#include "Entidades/Ente.h"

namespace
{
    const float ACELERACAO_GRAVIDADE = 1100.0f;
    const float VELOCIDADE_PULO = -480.0f;

    void resolverColisaoChao(
        float& posicaoY,
        float altura,
        float topoChao,
        float& velocidadeVertical,
        bool& estaNoChao
    )
    {
        const float limiteChao = topoChao - altura;
        if (posicaoY >= limiteChao)
        {
            posicaoY = limiteChao;
            velocidadeVertical = 0.0f;
            estaNoChao = true;
        }
    }
}

Ente::Ente()
    : velocidadeVertical_(0.0f),
      estaNoChao_(true),
      teclaPuloPressionadaAnteriormente_(false),
      pulosDisponiveis_(2)
{
}

bool Ente::pularSeSolicitado(bool teclaPuloPressionada)
{
    bool puloDuplo = false;
    if (teclaPuloPressionada &&
        !teclaPuloPressionadaAnteriormente_ &&
        pulosDisponiveis_ > 0)
    {
        velocidadeVertical_ = VELOCIDADE_PULO;
        estaNoChao_ = false;
        --pulosDisponiveis_;
        puloDuplo = pulosDisponiveis_ == 0;
    }

    teclaPuloPressionadaAnteriormente_ = teclaPuloPressionada;
    return puloDuplo;
}

void Ente::atualizarGravidade(
    float tempoQuadro,
    float& posicaoY,
    float altura,
    float topoChao
)
{
    if (!estaNoChao_)
    {
        velocidadeVertical_ += ACELERACAO_GRAVIDADE * tempoQuadro;
        posicaoY += velocidadeVertical_ * tempoQuadro;

        if (posicaoY < 0.0f)
        {
            posicaoY = 0.0f;
            if (velocidadeVertical_ < 0.0f)
            {
                velocidadeVertical_ = 0.0f;
            }
        }

        resolverColisaoChao(
            posicaoY,
            altura,
            topoChao,
            velocidadeVertical_,
            estaNoChao_
        );
        if (estaNoChao_)
        {
            pulosDisponiveis_ = 2;
        }
    }
    else
    {
        posicaoY = topoChao - altura;
    }
}