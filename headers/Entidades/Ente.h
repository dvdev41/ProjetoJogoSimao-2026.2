#ifndef ENTE_H
#define ENTE_H

#include <SFML/Graphics/Drawable.hpp>

namespace Gerenciadores
{
    class Gerenciador_Grafico;
}

namespace Entidades
{
    class Ente
    {
    protected:
        static int proximoId_;
        static Gerenciadores::Gerenciador_Grafico* pGG_;
        int id_;
        const sf::Drawable* pFig_;

        explicit Ente(const sf::Drawable* figura = 0);
        void setFigura(const sf::Drawable* figura);

    public:
        virtual ~Ente();
        virtual void executar() = 0;
        virtual void desenhar();

        static void setGG(Gerenciadores::Gerenciador_Grafico* pG);
        int getId() const;
    };
}

#endif
