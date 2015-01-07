#ifndef eoInitP_h
#define eoInitP_h

#include <eoInit.h>

template <class EOT>
class eoInitP: public eoInit<EOT>
{
public:


    eoInitP(unsigned _combien, unsigned int _p)
            : combien(_combien), p(_p){}

    virtual void operator()(EOT& chrom)
    {
        chrom.resize(combien);
        for (int i = 0; p > 0; ++i) {
            if (i == chrom.size()){
                i = 0;
            }
            bool result = rng.flip(0.5);
            if (result == 1 && chrom[i] != 1){
                chrom[i] = 1;
                --p;
            }
        }
        chrom.invalidate();
    }

private :
    unsigned combien;
    unsigned p;
};

#endif