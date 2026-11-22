#ifndef ENTE_H
#define ENTE_H

class Ente
{
protected:
    float velocidadeVertical_;
    bool estaNoChao_;
    bool teclaPuloPressionadaAnteriormente_;
    unsigned int pulosDisponiveis_;

    Ente();
    void atualizarGravidade(
        float tempoQuadro,
        float& posicaoY,
        float altura,
        float topoChao
    );
    bool pularSeSolicitado(bool teclaPuloPressionada);
};

#endif