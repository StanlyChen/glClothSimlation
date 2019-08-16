#pragma once
#include <glm.hpp>
#include "cstsequence.h"



using REAL = float;
using vect3 = glm::tvec3<REAL>;

using CstInt32Sequence = CstSequence<int>;
using CstPointSequence = CstSequence<vect3>;