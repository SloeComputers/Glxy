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

#ifndef VIEW_PLANISPEHER_H
#define VIEW_PLANISPEHER_H

#include <cmath>
#include <cstdio>

#include "STB/Vector2.h"
#include "STB/Matrix_4x4.h"
#include "GUI/Frame.h"
#include "GUI/Font/Teletext.h"

#include "Angle.h"
#include "PolarPlot.h"
#include "SolarSystem.h"

class ViewPlanisphere
{
private:
   using Pixel = PolarPlot::Pixel;

   const Angle MIN_DECL{-0.0, Angle::Unit::DEG};

   Angle       latitude;
   Angle       longitude;
   PolarPlot   plot;

   //!
   void drawReticule(GUI::Frame& frame,
                     bool reticule, unsigned step_ra, Angle outer_decl, Angle inner_decl, unsigned step_decl)
   {
      // Draw heading scale
      for(signed i = 0; i < 360; i += 5)
      {
         Angle ra = Angle::deg(i);

         if ((i % 10) == 0)
         {
            switch(i)
            {
            case 0:
               plot.setFont(GUI::font_teletext18, /* horz_align= */ 0, /* vert_align= */ 0);
               plot.drawText(frame, STB::RED, ra, outer_decl - Angle::deg(2), "N");
               break;

            case 90:
               plot.setFont(GUI::font_teletext18, /* horz_align= */ 0, /* vert_align= */ 0);
               plot.drawText(frame, STB::RED, ra, outer_decl - Angle::deg(2), "E");
               break;

            case 180:
               plot.setFont(GUI::font_teletext18, /* horz_align= */ 0, /* vert_align= */ 0);
               plot.drawText(frame, STB::RED, ra, outer_decl - Angle::deg(2), "S");
               break;

            case 270:
               plot.setFont(GUI::font_teletext18, /* horz_align= */ 0, /* vert_align= */ 0);
               plot.drawText(frame, STB::RED, ra, outer_decl - Angle::deg(2), "W");
               break;

            default:
               plot.setFont(GUI::font_teletext12, /* horz_align= */ 0, /* vert_align= */ 0);
               plot.drawText(frame, STB::RED, ra, outer_decl - Angle::deg(2), std::to_string(i));
               break;
            }

            plot.drawRadial(frame, STB::RED, ra, outer_decl - Angle::deg(1), outer_decl);
         }
         else
         {
            plot.drawRadial(frame, STB::RED, ra, outer_decl - Angle::deg(0.5), outer_decl);
         }
      }

      plot.drawCircleOfDeclination(frame, STB::RED, outer_decl);

      if (!reticule) return;

      // Draw right ascension radials
      for(unsigned ra = 0; ra < 360; ra += step_ra)
      {
         plot.drawRadial(frame, STB::RGB(0x60, 0x00, 0x00), Angle::deg(ra), outer_decl, inner_decl);
      }

      plot.setFont(GUI::font_teletext12, /* horz_align= */ 0, /* vert_align= */ 0);
      for(unsigned i=0; i<360; i += 90)
      {
         Angle ra = Angle::deg(i);

         plot.drawRadial(frame, STB::RGB(0x40, 0x00, 0x00), ra,  outer_decl, inner_decl);

         for(signed decl = 90-10; decl > outer_decl.deg(); decl -= 10)
         {
            plot.drawText(frame, STB::RGB(0xC0, 0x00, 0x00),
                          ra, Angle::deg(decl) - Angle::deg(1), std::to_string(decl));

            Pixel a, b;
            plot.polarToXY(ra - Angle::deg(1), Angle::deg(decl), a);
            plot.polarToXY(ra + Angle::deg(1), Angle::deg(decl), b);
            frame.drawLine(STB::RGB(0x40, 0x00, 0x00), a.x, a.y, b.x, b.y);
         }
      }

      // Draw circles of declination
      for(signed decl = 90 - step_decl; decl > outer_decl.deg(); decl -= step_decl)
      {
         plot.drawCircleOfDeclination(frame, STB::RGB(0x60, 0x00, 0x00), Angle::deg(decl));
      }

      plot.drawCircleOfDeclination(frame, STB::RGB(0x60, 0x00, 0x00), inner_decl);
   }

   void drawState(GUI::Frame& frame, unsigned width, const SolarSystem& system)
   {
      std::string text;

      text = "Date : ";
      text += system.getDate(/* is_utc */ true);
      frame.drawText(STB::RED, STB::BLACK, 8, 4, &GUI::font_teletext18, text.c_str());

      text = "UTC  : ";
      text += system.getTime(/* is_utc */ true);
      frame.drawText(STB::RED, STB::BLACK, 8, 26, &GUI::font_teletext18, text.c_str());

      text = "Local: ";
      text += system.getTime(/* is_utc */ false);
      frame.drawText(STB::RED, STB::BLACK, 8, 48, &GUI::font_teletext18, text.c_str());

      char txt[128];

      sprintf(txt, "Lat :  %c%02d\x7f%02d'%04.1f\"",
              latitude.deg() >= 0 ? 'N' : 'S',
              ::abs((int)latitude.degrees()),
              latitude.minutes(),
              latitude.seconds());
      frame.drawText(STB::RED, STB::BLACK, width - 250, 4, &GUI::font_teletext18, txt);

      sprintf(txt, "Lon : %c%03d\x7f%02d'%04.1f\"",
              longitude.deg() >= 0 ? 'W' : 'E',
              ::abs((int)longitude.degrees()),
              longitude.minutes(),
              longitude.seconds());
      frame.drawText(STB::RED, STB::BLACK, width - 250, 26, &GUI::font_teletext18, txt);
   }

   void drawStars(GUI::Frame& frame, const STB::Matrix_4x4<double>& trans, bool names, const SolarSystem& system)
   {
      plot.setFont(GUI::font_teletext12, /* horz_align= */ -1, /* vert_align= */ -1);

      float min_mag = +99.0;
      float max_mag = -99.0;

      // printf("min = %g max = %g\n", min_mag, max_mag);

      for(const auto& star : system.star_db)
      {
         if (!star.isTheSun())
         {
            float mag = star.getAparentMagnitude();
            if (mag > 7)     mag = 7;
            mag = pow(1.112, -mag);
            if (mag > max_mag) max_mag = mag;
            if (mag < min_mag) min_mag = mag;
         }
      }

      for(const auto& star : system.star_db)
      {
         if (!star.isTheSun())
         {
            float mag = star.getAparentMagnitude();
            if (mag > 7)     mag = 7;
            mag = pow(1.112, -mag);
            float m = (mag - min_mag) / (max_mag - min_mag);

            STB::Colour col = STB::GREY(m*255);

            const Vector& abs_pos = star.getPosition();
            Vector pos = trans.transformPos(abs_pos);

            double proj_radius     = sqrt(pos.x * pos.x + pos.y * pos.y);
            Angle  declination     = Angle::rad(atan2(pos.z, proj_radius));
            Angle  right_ascension = Angle::rad(-atan2(pos.y, pos.x));

            if (plot.fillCircle(frame, col, right_ascension, declination, m * 8))
            {
               if (names) plot.drawText(frame, STB::RED, right_ascension, declination, star.getName());
            }
         }
      }
   }

   void drawPlanets(GUI::Frame& frame, const STB::Matrix_4x4<double>& trans, bool names, const SolarSystem& system)
   {
      plot.setFont(GUI::font_teletext12, /* horz_align= */ -1, /* vert_align= */ -1);

      for(const auto& planet : system.planet_list)
      {
         const Vector& abs_pos = planet.getPosition();
         Vector pos = trans.transformPos(abs_pos);

         double proj_radius     = sqrt(pos.x * pos.x + pos.y * pos.y);
         Angle  declination     = Angle::rad(atan2(pos.z, proj_radius));
         Angle  right_ascension = Angle::rad(-atan2(pos.y, pos.x));

         if (plot.fillCircle(frame, STB::YELLOW, right_ascension, declination, 10))
         {
            if (names) plot.drawText(frame, STB::YELLOW, right_ascension, declination, planet.getName());
         }
      }
   }

public:
   ViewPlanisphere(double latitude_degs, double longitude_degs)
      : latitude(Angle::deg(latitude_degs))
      , longitude(Angle::deg(longitude_degs))
   {
   }

   void render(GUI::Frame&        frame,
               unsigned           x,
               unsigned           y,
               unsigned           width,
               unsigned           height,
               const SolarSystem& system,
               bool               reticule,
               bool               names)
   {
      plot.setView(x, y, (height / 2) - 40);
      plot.setMinDeclination(MIN_DECL);

      STB::Matrix_4x4<double> trans;

      Angle rot_y = Angle::deg(90.0) - latitude;
      trans.rotateY(rot_y.rad());

      Angle rot_z = system.earth.getEclipticLongitude() +
                    system.earth.getRotationOfPrimeMeridian() -
                    longitude;
      trans.rotateZ(rot_z.rad());

      drawState(   frame, width, system);
      drawStars(   frame, trans, names, system);
      drawPlanets( frame, trans, names, system);
      drawReticule(frame, reticule, 30, MIN_DECL, Angle::deg(85.0), 30);
   }
};

#endif // VIEW_PLANISPHERE_H
