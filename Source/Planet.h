//-------------------------------------------------------------------------------
// Copyright (c) 2018 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cassert>

#include "KeplerianElements.h"
#include "Object.h"
#include "Time.h"

class Planet : public Object
{
public:
   Planet(const char* name_)
      : Object(name_)
      , ke_init(name_)
      , ke_rate(name_)
   {
      assert(ke_init.isValid());
      assert(ke_rate.isValid());

           if (getName() == "Mercury") { setAparentMagnitude(-2.48); }
      else if (getName() == "Venus")   { setAparentMagnitude(-2.98); }
      else if (getName() == "Mars")    { setAparentMagnitude(-2.94); }
      else if (getName() == "Jupiter") { setAparentMagnitude(-2.94); }
      else if (getName() == "Saturn")  { setAparentMagnitude(-0.55); }
      else if (getName() == "Uranus")  { setAparentMagnitude(+5.38); }
      else if (getName() == "Neptune") { setAparentMagnitude(+7.67); }
   }

   void computePosition(const Time& time, bool debug = false)
   {
      // Calculate the number of centries since the data set epoch
      double t_cy = time.getYearsSinceEpoch(KeplerianElements::getEpoch()) / 100.0;

      // Compute Keplerian elements for the given time
      KeplerianElements elem = ke_rate;
      elem *= t_cy;
      elem += ke_init;

      if (debug)
         elem.debug();

      // Calculate a position from the Keplerian element
      Vector pos;
      elem.computePosition(pos, debug);

      setPositionAu(pos);
   }

private:
   KeplerianElementsInit ke_init;
   KeplerianElementsRate ke_rate;
};

