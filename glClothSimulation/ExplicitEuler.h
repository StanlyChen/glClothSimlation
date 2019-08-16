#pragma once
#include "Simulator.h"

struct ExplictEulerClothParticle
{
    float mass;
    vect3 acc;
    vect3 position;
    vect3 vecolity;
    vect3 force;
};

class ExplicitEuler
{
public:
    ExplicitEuler();
    ~ExplicitEuler();

    void setPartciles(const CstPointSequence& particleInitPos, const CstInt32Sequence& indices  );


};

