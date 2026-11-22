#ifndef PERSONAGEM_H
#define PERSONAGEM_H

#include "Entidades/Entidade.h"

namespace Entidades
{
    namespace Personagens
    {
        class Personagem : public Entidade
        {
        protected:
            int num_vidas_;
            float velocidadeVertical_;
            bool estaNoChao_;
            bool teclaPuloPressionadaAnteriormente_;
            unsigned int pulosDisponiveis_;

            Personagem(float x, float y, int vidas);
            bool pularSeSolicitado(bool teclaPuloPressionada);

        public:
            virtual ~Personagem();
            virtual void mover() = 0;
            virtual void executar() = 0;
            virtual void salvar() = 0;

            int getVidas() const;
            void setVidas(int vidas);
        };
    }
}

#endif
