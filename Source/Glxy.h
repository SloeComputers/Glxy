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

#ifndef GLXY_H
#define GLXY_H

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <array>

#include "STB/ConsoleApp.h"
#include "STB/Vector2.h"
#include "STB/Matrix_4x4.h"
#include "GUI/Frame.h"
#include "GUI/Font/Teletext.h"
#include "PLT/Event.h"

#include "Angle.h"
#include "Options.h"
#include "PolarPlot.h"
#include "SolarSystem.h"

class Glxy
{
private:
   using Pixel = PolarPlot::Pixel;

   const Angle     MIN_DECL{-0.0, Angle::Unit::DEG};

   const Options&  options;
   GUI::Frame      frame;
   SolarSystem     system;
   Angle           latitude;
   Angle           longitude;
   PolarPlot       plot{frame};

   bool            dynamic{true};
   bool            reticule{true};
   bool            names{true};

   //!
   void drawReticule(unsigned step_ra, Angle outer_decl, Angle inner_decl, unsigned step_decl)
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
               plot.drawText(STB::RED, ra, outer_decl - Angle::deg(2), "N");
               break;

            case 90:
               plot.setFont(GUI::font_teletext18, /* horz_align= */ 0, /* vert_align= */ 0);
               plot.drawText(STB::RED, ra, outer_decl - Angle::deg(2), "E");
               break;

            case 180:
               plot.setFont(GUI::font_teletext18, /* horz_align= */ 0, /* vert_align= */ 0);
               plot.drawText(STB::RED, ra, outer_decl - Angle::deg(2), "S");
               break;

            case 270:
               plot.setFont(GUI::font_teletext18, /* horz_align= */ 0, /* vert_align= */ 0);
               plot.drawText(STB::RED, ra, outer_decl - Angle::deg(2), "W");
               break;

            default:
               plot.setFont(GUI::font_teletext12, /* horz_align= */ 0, /* vert_align= */ 0);
               plot.drawText(STB::RED, ra, outer_decl - Angle::deg(2), std::to_string(i));
               break;
            }

            plot.drawRadial(STB::RED, ra, outer_decl - Angle::deg(1), outer_decl);
         }
         else
         {
            plot.drawRadial(STB::RED, ra, outer_decl - Angle::deg(0.5), outer_decl);
         }
      }

      plot.drawCircleOfDeclination(STB::RED, outer_decl);

      if (!reticule) return;

      // Draw right ascension radials
      for(unsigned ra = 0; ra < 360; ra += step_ra)
      {
         plot.drawRadial(STB::RGB(0x60, 0x00, 0x00), Angle::deg(ra), outer_decl, inner_decl);
      }

      plot.setFont(GUI::font_teletext12, /* horz_align= */ 0, /* vert_align= */ 0);
      for(unsigned i=0; i<360; i += 90)
      {
         Angle ra = Angle::deg(i);

         plot.drawRadial(STB::RGB(0x40, 0x00, 0x00), ra,  outer_decl, inner_decl);

         for(signed decl = 90-10; decl > outer_decl.deg(); decl -= 10)
         {
            plot.drawText(STB::RGB(0xC0, 0x00, 0x00),
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
         plot.drawCircleOfDeclination(STB::RGB(0x60, 0x00, 0x00), Angle::deg(decl));
      }

      plot.drawCircleOfDeclination(STB::RGB(0x60, 0x00, 0x00), inner_decl);
   }

   void drawState()
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
      frame.drawText(STB::RED, STB::BLACK, options.width - 250, 4, &GUI::font_teletext18, txt);

      sprintf(txt, "Lon : %c%03d\x7f%02d'%04.1f\"",
              longitude.deg() >= 0 ? 'W' : 'E',
              ::abs((int)longitude.degrees()),
              longitude.minutes(),
              longitude.seconds());
      frame.drawText(STB::RED, STB::BLACK, options.width - 250, 26, &GUI::font_teletext18, txt);
   }

   void doPlot()
   {
      frame.clear(STB::BLACK);

      drawState();

      plot.setView(options.width / 2, options.height / 2, (options.height / 2) - 40);

      plot.setMinDeclination(MIN_DECL);

      float min_mag = +99.0;
      float max_mag = -99.0;

      for(const auto& star : system.star_db)
      {
         if (!star.isTheSun())
         {
            float mag = star.getMagnitude();
            if (mag > 7)     mag = 7;
            mag = pow(1.112, -mag);
            if (mag > max_mag) max_mag = mag;
            if (mag < min_mag) min_mag = mag;
         }
      }

      // printf("min = %g max = %g\n", min_mag, max_mag);

      STB::Matrix_4x4<double> trans;

      Angle rot_y = Angle::deg(90.0) - latitude;
      trans.rotateY(rot_y.rad());

      Angle rot_z = system.earth.getEclipticLongitude() +
                    system.earth.getRotationOfPrimeMeridian() -
                    longitude;
      trans.rotateZ(rot_z.rad());

      plot.setFont(GUI::font_teletext12, /* horz_align= */ -1, /* vert_align= */ -1);

      for(const auto& star : system.star_db)
      {
         if (!star.isTheSun())
         {
            float mag = star.getMagnitude();
            if (mag > 7)     mag = 7;
            mag = pow(1.112, -mag);
            float m = (mag - min_mag) / (max_mag - min_mag);

            STB::Colour col = STB::GREY(m*255);

            const Star::Vector& abs_pos = star.getPosition();
            Star::Vector pos = trans.transformPos(abs_pos);

            double proj_radius     = sqrt(pos.x * pos.x + pos.y * pos.y);
            Angle  declination     = Angle::rad(atan2(pos.z, proj_radius));
            Angle  right_ascension = Angle::rad(-atan2(pos.y, pos.x));

            if (plot.fillCircle(col, right_ascension, declination, m * 8))
            {
               if (names) plot.drawText(STB::RED, right_ascension, declination, star.getName());
            }

            if (options.debug)
            {
               printf("ra=%.15f decl=%.15f m=%f ::: ",
                      double(right_ascension.deg()), double(declination.deg()), m);
               star.debug();
            }
         }
      }

      drawReticule(30, MIN_DECL, Angle::deg(85.0), 30);

      frame.refresh();
  }

public:
   Glxy(const Options& options_, const std::string& title_)
      : options(options_)
      , frame{title_.c_str(),
              options.width, options.height,
              options.full_screen ? PLT::Frame::FULL_SCREEN : 0}
      , system((const char*)options.filename)
   {
      latitude  = Angle::deg(options.latitude_degs);
      longitude = Angle::deg(options.longitude_degs);

      if (options.debug) system.debug();
   }

  int eventLoop()
  {
      static bool shift = false;

      PLT::Event::setTimer(1000);

      doPlot();

      while(true)
      {
         PLT::Event::Message event;

         switch(PLT::Event::wait(event))
         {
         case PLT::Event::TIMER:
            if (dynamic)
            {
               system.setNow();
               doPlot();
            }
            break;

         case PLT::Event::KEY_DOWN:
            switch(event.code)
            {
            case 'x':
            case PLT::ESCAPE:
               return 0;

            case 'r':
               reticule = !reticule;
               doPlot();
               break;

            case 't':
               names = !names;
               doPlot();
               break;

            case 'n':
               dynamic = true;
               system.setNow();
               doPlot();
               break;

            case 'y':
               dynamic = false;
               system.adjustDate(shift ? -1 : +1, 0);
               doPlot();
               break;

            case 'd':
               dynamic = false;
               system.adjustDate(0, shift ? -1 : +1);
               doPlot();
               break;

            case 'h':
               dynamic = false;
               system.adjustTime(shift ? -1 : +1, 0);
               doPlot();
               break;

            case 'q':
               dynamic = false;
               system.adjustTime(0, shift ? -15 : +15);
               doPlot();
               break;

            case 'm':
               dynamic = false;
               system.adjustTime(0, shift ? -1 : +1);
               doPlot();
               break;

            case PLT::LSHIFT:
            case PLT::RSHIFT:
               shift = true;
               break;

            default:
               break;
            }
            break;

         case PLT::Event::KEY_UP:
            switch(event.code)
            {
            case PLT::LSHIFT:
            case PLT::RSHIFT:
               shift = false;
               break;
            }
            break;

         case PLT::Event::QUIT:
            return 0;

         default:
            break;
         }
      }
   }
};

#endif
