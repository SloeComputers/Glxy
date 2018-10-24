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

#include <cassert>

#include "Time.h"
#include "Planet.h"

class Earth : public Planet
{
public:
   Earth()
      : Planet{"Earth"}
   {}

   Time utc{/* is_utc */ true};
   Time local_time{/* is_utc */ false};

   //! Set the date and time to now
   void setNow()
   {
      utc.setNow();
      local_time.setNow();
   }

   //! Adjust the date
   void adjustDate(signed delta_year, signed delta_day)
   {
      utc.setDate(       utc.getYear()        + delta_year, utc.getDayOfYear()        + delta_day);
      local_time.setDate(local_time.getYear() + delta_year, local_time.getDayOfYear() + delta_day);
   }

   //! Adjust the time
   void adjustTime(signed delta_hour, signed delta_minute)
   {
      utc.setTime(       utc.getHour()        + delta_hour, utc.getMinute()        + delta_minute);
      local_time.setTime(local_time.getHour() + delta_hour, local_time.getMinute() + delta_minute);
   }

   //! Get the current position of the earth relative to the sun and the
   //  vernal equinox
   Angle getEclipticLongitude()
   {
      // XXX  this will drift as we move further from 2018
      // TODO should calculate days since the reference date
      (void) ref_year;
      (void) ref_day;
 
      return ref_ecliptic_longitude + Angle::turn(double(utc.getDayOfYear()) / utc.getDaysPerYear());
   }

   //! Get the current rotation of the Greenwich Meridian
   Angle getRotationOfPrimeMeridian()
   {
      unsigned seconds_since_midnight = utc.getSecond() +
                                        Time::SECONDS_PER_MINUTE * (utc.getMinute()  +
                                                              Time::MINUTES_PER_HOUR * utc.getHour());

      return Angle::turn(double(seconds_since_midnight) / Time::SECONDS_PER_DAY);
   }

private:
   // Reference for 1st Jan 2018 at 00:00
   const unsigned ref_year = 2018;
   const unsigned ref_day  = 0;
   const Angle    ref_ecliptic_longitude{280.265, Angle::Unit::DEG};

   void setDate(unsigned year_, signed yday_)
   {
      utc.setDate(year_, yday_);
      local_time.setDate(year_, yday_);
   }

   void setTime(signed hour_, signed minute_)
   {
      utc.setTime(hour_, minute_);
      local_time.setTime(hour_, minute_);
   }
};

#endif // EARTH_H
