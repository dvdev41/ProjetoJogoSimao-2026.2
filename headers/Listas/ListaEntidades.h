#ifndef LISTA_ENTIDADES_H
#define LISTA_ENTIDADES_H

#include "Entidades/Entidade.h"
#include "Listas/Lista.h"

namespace Listas
{
    class ListaEntidades
    {
    private:
        Lista<Entidades::Entidade> LEs_;

    public:
        void incluir(Entidades::Entidade* pE);
        void percorrer();
        void desenhar();
        void limpar();
    };
}

#endif
