#ifndef LISTA_H
#define LISTA_H

#include "Listas/Elemento.h"

#include <stdexcept>

namespace Listas
{
    template <class TL>
    class Lista
    {
    private:
        Elemento<TL>* pPrimeiro_;
        Elemento<TL>* pUltimo_;

        Lista(const Lista<TL>&);
        Lista<TL>& operator=(const Lista<TL>&);

    public:
        Lista()
            : pPrimeiro_(0),
              pUltimo_(0)
        {
        }

        ~Lista()
        {
            limpar();
        }

        void incluir(TL* p)
        {
            if (p == 0)
            {
                throw std::invalid_argument(
                    "Nao e possivel incluir um ponteiro nulo na lista."
                );
            }

            Elemento<TL>* novo = new Elemento<TL>(p);
            if (pUltimo_ == 0)
            {
                pPrimeiro_ = novo;
            }
            else
            {
                pUltimo_->setProx(novo);
            }
            pUltimo_ = novo;
        }

        void limpar()
        {
            Elemento<TL>* atual = pPrimeiro_;
            while (atual != 0)
            {
                Elemento<TL>* proximo = atual->getProximo();
                delete atual;
                atual = proximo;
            }
            pPrimeiro_ = 0;
            pUltimo_ = 0;
        }

        Elemento<TL>* getPrimeiro() const
        {
            return pPrimeiro_;
        }
    };
}

#endif
