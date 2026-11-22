#include "Listas/ListaEntidades.h"

namespace Listas
{
    void ListaEntidades::incluir(Entidades::Entidade* pE)
    {
        LEs_.incluir(pE);
    }

    void ListaEntidades::percorrer()
    {
        Elemento<Entidades::Entidade>* atual = LEs_.getPrimeiro();
        while (atual != 0)
        {
            atual->getInfo()->executar();
            atual = atual->getProximo();
        }
    }

    void ListaEntidades::desenhar()
    {
        Elemento<Entidades::Entidade>* atual = LEs_.getPrimeiro();
        while (atual != 0)
        {
            atual->getInfo()->desenhar();
            atual = atual->getProximo();
        }
    }

    void ListaEntidades::limpar()
    {
        LEs_.limpar();
    }
}
