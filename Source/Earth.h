//-------------------------------------------------------------------------------
// Copyright (c) 2018 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cassert>

#include "Time.h"
#include "Planet.h"

class Earth : public Planet
{
public:
   Earth()
      : Planet{"Earth"}
   {}

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
   Angle getEclipticLongitude() const
   {
      // XXX  this will drift as we move further from 2018
      // TODO should calculate days since the reference date
      (void) REF_YEAR;
      (void) REF_DAY;
 
      return REF_ECLIPTIC_LONGITUDE + Angle::turn(double(utc.getDayOfYear()) / utc.getDaysPerYear());
   }

   //! Get the current rotation of the Greenwich Meridian
   Angle getRotationOfPrimeMeridian() const
   {
      unsigned seconds_since_midnight = utc.getSecond() +
                                        Time::SECONDS_PER_MINUTE * (utc.getMinute()  +
                                                              Time::MINUTES_PER_HOUR * utc.getHour());

      return Angle::turn(double(seconds_since_midnight) / Time::SECONDS_PER_DAY);
   }

   Time utc{/* is_utc */ true};
   Time local_time{/* is_utc */ false};

private:
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

   // Reference for 1st Jan 2018 at 00:00
   const unsigned REF_YEAR = 2018;
   const unsigned REF_DAY  = 0;
   const Angle    REF_ECLIPTIC_LONGITUDE{280.265, Angle::Unit::DEG};
};

