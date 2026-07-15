//-------------------------------------------------------------------------------
// Copyright (c) 2018 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "GUI/Frame.h"
#include "GUI/Font/Teletext.h"

#include "SolarSystem.h"

class ViewPlanets
{
private:
   void drawPlanet(GUI::Frame& frame, const Planet& planet, unsigned x, unsigned y, double scale)
   {
      Vector pos = planet.getPositionAu();

      unsigned px = x + pos.x * scale;
      unsigned py = y - pos.y * scale;

      frame.fillCircle(STB::RED, px, py, 3);
      frame.drawText(STB::RED, STB::BLACK, px + 4, py + 4, &GUI::font_teletext18, planet.getName().c_str());
   }

public:
   ViewPlanets() = default;

   void render(GUI::Frame&        frame,
               unsigned           x,
               unsigned           y,
               unsigned           width,
               unsigned           height,
               const SolarSystem& system,
               bool               reticule,
               bool               names)
   {
      frame.fillCircle(STB::YELLOW, x, y, 10);

      double scale = width / 20.0;

      for(const auto& planet : system.planet_list)
      {
         drawPlanet(frame, planet, x, y, scale);
      }

      drawPlanet(frame, system.earth, x, y, scale);
   }
};

