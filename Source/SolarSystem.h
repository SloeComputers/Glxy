//-------------------------------------------------------------------------------
// Copyright (c) 2018 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

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

