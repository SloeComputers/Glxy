//------------------------------------------------------------------------------
// Copyright (c) 2018 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

#ifndef PLANET_H
#define PLANET_H

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

#endif // PLANET_H
