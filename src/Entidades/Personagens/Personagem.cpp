#include "Entidades/Personagens/Personagem.h"

namespace Entidades
{
    namespace Personagens
    {
        namespace
        {
            const float VELOCIDADE_PULO = -520.0f;
        }

        Personagem::Personagem(float x, float y, int vidas)
            : Entidade(x, y),
              num_vidas_(vidas),
              velocidadeVertical_(0.0f),
              estaNoChao_(true),
              teclaPuloPressionadaAnteriormente_(false),
              pulosDisponiveis_(2)
        {
        }

        Personagem::~Personagem()
        {
        }

        bool Personagem::pularSeSolicitado(bool teclaPuloPressionada)
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

        int Personagem::getVidas() const
        {
            return num_vidas_;
        }

        void Personagem::setVidas(int vidas)
        {
            num_vidas_ = vidas;
        }
    }
}
