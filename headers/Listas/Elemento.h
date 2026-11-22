#ifndef ELEMENTO_H
#define ELEMENTO_H

namespace Listas
{
    template <class TE>
    class Elemento
    {
    private:
        TE* pInfo_;
        Elemento<TE>* pProx_;

    public:
        explicit Elemento(TE* pInfo)
            : pInfo_(pInfo),
              pProx_(0)
        {
        }

        void incluir(TE* pInfo)
        {
            pInfo_ = pInfo;
        }

        void setProx(Elemento<TE>* pProx)
        {
            pProx_ = pProx;
        }

        Elemento<TE>* getProximo() const
        {
            return pProx_;
        }

        TE* getInfo() const
        {
            return pInfo_;
        }
    };
}

#endif
