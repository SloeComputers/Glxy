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

#include "STB/ConsoleApp.h"
#include "STB/Vector2.h"
#include "STB/Matrix_4x4.h"
#include "GUI/Frame.h"
#include "GUI/Font/Teletext.h"
#include "PLT/Event.h"

#include "Angle.h"
#include "Earth.h"
#include "Options.h"
#include "PolarPlot.h"
#include "Star.h"
#include "StarDB.h"

class Glxy
{
private:
   using Pixel = PolarPlot::Pixel;

   const Angle     MIN_DECL{-0.0, Angle::Unit::DEG};

   const Options&  options;
   GUI::Frame      frame;
   StarDB          star_db;
   Angle           latitude;
   Angle           longitude;
   Earth           earth;
   PolarPlot       plot{frame};

   bool            dynamic{true};
   bool            reticule{true};
   bool            names{true};

   //! 
   void drawReticule(unsigned step_ra, Angle outer_decl, Angle inner_decl, unsigned step_decl)
   {
      // Draw the compass points
      plot.setFont(GUI::font_teletext18, /* horz_align= */ 0, /* vert_align= */ 0);
      plot.drawText(STB::RED, Angle::deg(  0), outer_decl - Angle::deg(4), "N");
      plot.drawText(STB::RED, Angle::deg( 90), outer_decl - Angle::deg(4), "W");
      plot.drawText(STB::RED, Angle::deg(180), outer_decl - Angle::deg(4), "S");
      plot.drawText(STB::RED, Angle::deg(270), outer_decl - Angle::deg(4), "E");

      plot.drawCircleOfDeclination(STB::RGB(0xC0, 0x00, 0x00), outer_decl);

      if (!reticule) return;

      plot.drawCircleOfDeclination(STB::RGB(0xA0, 0x00, 0x00), 0.0);
      plot.drawCircleOfDeclination(STB::RGB(0xC0, 0x00, 0x00), inner_decl);

      // Draw right ascension radials
      plot.setFont(GUI::font_teletext12, /* horz_align= */ 0, /* vert_align= */ 0);
      for(unsigned ra = 0; ra < 360; ra += step_ra)
      {
         plot.drawText(STB::RED, Angle::deg(ra), outer_decl - Angle::deg(3), std::to_string(ra));
         plot.drawRadial(STB::RGB(0x60, 0x00, 0x00), Angle::deg(ra), outer_decl, inner_decl);
      }

      plot.setFont(GUI::font_teletext12, /* horz_align= */ 0, /* vert_align= */ 0);
      for(unsigned i=0; i<360; i += 90)
      {
         Angle ra = Angle::deg(i);

         plot.drawRadial(STB::RGB(0xC0, 0x00, 0x00), ra,  outer_decl, inner_decl);

         for(signed decl = 90-10; decl > outer_decl.deg(); decl -= 10)
         {
            plot.drawText(STB::RED, ra, Angle::deg(decl) - Angle::deg(1), std::to_string(decl));

            Pixel a, b;
            plot.polarToXY(ra - Angle::deg(1), Angle::deg(decl), a);
            plot.polarToXY(ra + Angle::deg(1), Angle::deg(decl), b);
            frame.drawLine(STB::RED, a.x, a.y, b.x, b.y);
         }
      }

      // Draw circles of declination
      for(signed decl = 90 - step_decl; decl > outer_decl.deg(); decl -= step_decl)
      {
         plot.drawCircleOfDeclination(STB::RGB(0x60, 0x00, 0x00), Angle::deg(decl));
      }
   }

   void drawState()
   {
      static const char* months[] = {"Jan", "Feb", "Mar", "Apr",
                                     "May", "Jun", "Jul", "Aug",
                                     "Sep", "Oct", "Nov", "Dec"};
      char text[128];

      sprintf(text, "Date : %u %s %4u",
              earth.getDayOfMonth(),
              months[earth.getMonth() - 1],
              earth.getYear());
      frame.drawText(STB::RED, STB::BLACK, 8, 4, &GUI::font_teletext18, text);

      sprintf(text, "UTC  : %02u:%02u:%02u",
              earth.getHour(), 
              earth.getMinute(), 
              earth.getSecond());
      frame.drawText(STB::RED, STB::BLACK, 8, 26, &GUI::font_teletext18, text);

      Time local_time{/* utc */ false};
      sprintf(text, "Local: %02u:%02u:%02u",
              local_time.getHour(), 
              local_time.getMinute(), 
              local_time.getSecond());
      frame.drawText(STB::RED, STB::BLACK, 8, 48, &GUI::font_teletext18, text);

      sprintf(text, "Lat :  %c%02d\x7f%02d'%04.1f\"",
              latitude.degrees() >= 0 ? 'N' : 'S',
              ::abs((int)latitude.degrees()),
              latitude.minutes(),
              latitude.seconds());
      frame.drawText(STB::RED, STB::BLACK, options.width - 250, 4, &GUI::font_teletext18, text);

      sprintf(text, "Lon : %c%03d\x7f%02d'%04.1f\"",
              longitude.degrees() >= 0 ? 'W' : 'E',
              ::abs((int)longitude.degrees()),
              longitude.minutes(),
              longitude.seconds());
      frame.drawText(STB::RED, STB::BLACK, options.width - 250, 26, &GUI::font_teletext18, text);
   }

   void doPlot()
   {
      frame.clear(STB::BLACK);

      drawState();

      plot.setView(options.width / 2, options.height / 2, (options.height / 2) - 40);

      plot.setMinDeclination(MIN_DECL);

      float min_mag = +99.0;
      float max_mag = -99.0;

      for(const auto& star : star_db)
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

      Angle rot_z = earth.getEclipticLongitude() +
                    earth.getRotationOfPrimeMeridian() -
                    longitude;
      trans.rotateZ(rot_z.rad());

      for(const auto& star : star_db)
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
            Angle  right_ascension = Angle::rad(atan2(pos.y, pos.x));

            plot.fillCircle(col, right_ascension, declination, m * 8);

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
   {
      latitude  = Angle::deg(options.latitude_degs);
      longitude = Angle::deg(options.longitude_degs);

      star_db.load((const char*)options.filename);

      if (options.debug) printf("Number of stars = %lu\n", star_db.size());
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
               earth.setNow();
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
               earth.setNow();
               doPlot();
               break;

            case 'y':
               dynamic = false;
               earth.setDate(earth.getYear() + (shift ? -1 : +1), earth.getDayOfYear());
               doPlot();
               break;

            case 'd':
               dynamic = false;
               earth.setDate(earth.getYear(), earth.getDayOfYear() + (shift ? -1 : +1));
               doPlot();
               break;

            case 'h':
               dynamic = false;
               earth.setTime(earth.getHour() + (shift ? -1 : +1), earth.getMinute());
               doPlot();
               break;

            case 'q':
               dynamic = false;
               earth.setTime(earth.getHour(), earth.getMinute() + (shift ? -15 : +15));
               doPlot();
               break;

            case 'm':
               dynamic = false;
               earth.setTime(earth.getHour(), earth.getMinute() + (shift ? -1 : +1));
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
