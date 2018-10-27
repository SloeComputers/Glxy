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

#ifndef SOLAR_SYSTEM_H
#define SOLAR_SYSTEM_H

#include <array>

#include "StarDB.h"
#include "Earth.h"
#include "Planet.h"

class SolarSystem
{
public:
   StarDB star_db;
   Earth  earth;

   std::array<Planet,7> planet_list =
   {
      "Mercury", "Venus", "Mars", "Jupiter", "Saturn", "Neptune", "Uranus"
   };

private:
   void update()
   {
      for(auto& planet : planet_list)
      {
         planet.computePosition(earth.utc);
      }

      earth.computePosition(earth.utc);
   }

public:
   SolarSystem(const std::string& filename)
   {
      star_db.load(filename);
   }

   void debug() const
   {
      for(const auto& star : star_db)
      {
         star.debug();
      }

      printf("Number of stars = %lu\n", star_db.size());
   }

   //! Get date formatted into a string
   std::string getDate(bool is_utc) const
   {
      const Time& time = is_utc ? earth.utc : earth.local_time;
      return time.getDate();
   }

   //! Get time formatted into a string
   std::string getTime(bool is_utc) const
   {
      const Time& time = is_utc ? earth.utc : earth.local_time;
      return time.getTime();
   }

   //! Set the date and time to now
   void setNow()
   {
      earth.setNow();
      update();
   }

   //! Adjust the date
   void adjustDate(signed delta_year, signed delta_day)
   {
      earth.adjustDate(delta_year, delta_day);
      update();
   }

   //! Adjust the time
   void adjustTime(signed delta_hour, signed delta_minute)
   {
      earth.adjustTime(delta_hour, delta_minute);
      update();
   }
};

#endif
