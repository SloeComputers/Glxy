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

class Planet
{
public:
   Planet(const char* name_)
      : name(name_)
      , ke_init(name_)
      , ke_rate(name_)
   {
      assert(ke_init.isValid());
      assert(ke_rate.isValid());
   }

   void compute(const Time& time)
   {
      // Calculate the number of centries since the data set epoch
      double t_cy = time.getYearsSinceEpoch(KeplerianElements::getEpoch()) / 100.0;

      // Compute Keplerian elements for the given time
      KeplerianElements elem = ke_rate;
      elem *= t_cy;
      elem += ke_init;

      //argument_of_perihelion = elem.w - elem.W;
      //mean_anomaly           = elem.L - elem.w + 
   }

private:
   const std::string     name;
   KeplerianElementsInit ke_init;
   KeplerianElementsRate ke_rate;
};

#endif // PLANET_H
