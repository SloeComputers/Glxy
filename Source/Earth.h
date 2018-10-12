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

#ifndef EARTH_H
#define EARTH_H

#include "Time.h"

class Earth : public Time
{
public:
   Earth() = default;

   //! Get the current position of the earth relative to the sun and the
   //  vernal equinox
   Angle getEclipticLongitude()
   {
      // XXX  this will drift as we move further from 2018
      // TODO should calculate days since the reference date
      (void) ref_year;
      (void) ref_day;
 
      return ref_ecliptic_longitude + Angle::turn(double(getDayOfYear()) / getDaysPerYear());
   }

   //! Get the current rotation of the Greenwich Meridian
   Angle getRotationOfPrimeMeridian()
   {
      unsigned seconds_since_midnight = getSecond() +
                                        SECONDS_PER_MINUTE * (getMinute()  +
                                                              MINUTES_PER_HOUR * getUTCHour());

      return Angle::turn(double(seconds_since_midnight) / SECONDS_PER_DAY);
   }

private:
   // Reference for 1st Jan 2018 at 00:00
   const unsigned ref_year = 2018;
   const unsigned ref_day  = 0;
   const Angle    ref_ecliptic_longitude{280.265, Angle::Unit::DEG};
};

#endif // TIME_H
