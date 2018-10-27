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

#ifndef VIEW_PLANETS_H
#define VIEW_PLANETS_H

#include "GUI/Frame.h"
#include "GUI/Font/Teletext.h"

#include "SolarSystem.h"

class ViewPlanets
{
private:
   void drawPlanet(GUI::Frame& frame, const Planet& planet, unsigned x, unsigned y, double scale)
   {
      Vector pos = planet.getPosition();

      unsigned px = x + pos.x * scale;
      unsigned py = y + pos.y * scale;

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

#endif // VIEW_PLANETS_H
