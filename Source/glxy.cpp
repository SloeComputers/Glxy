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

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "STB/ConsoleApp.h"
#include "STB/Vector2.h"
#include "GUI/Frame.h"
#include "GUI/Font/Teletext.h"
#include "PLT/Event.h"

#include "Angle.h"
#include "Earth.h"
#include "PolarPlot.h"
#include "Star.h"
#include "StarDB.h"

#define  PROGRAM         "Glxy"
#define  DESCRIPTION     "Star database viewer"
#define  LINK            "https://github.com/AnotherJohnH/"
#define  COPYRIGHT_YEAR  "2018"
#define  AUTHOR          "John D. Haughton"
#define  VERSION         PROJ_VERSION


class GlxyApp : public STB::ConsoleApp
{
private:
   using Pixel = PolarPlot::Pixel;

   const Angle MIN_DECL{-30.0, Angle::Unit::DEG};

   STB::Option<bool>     debug{         'd', "debug", "Enable debug", false};
   STB::Option<unsigned> size{          's', "size",  "Size (pixels)", 756};
   STB::Option<double>   latitude_degs{ 'a', "lat",   "Latitude (degs)", 51.5};
   STB::Option<double>   longitude_degs{'o', "lon",   "Longitude (degs)", 0.0};
   std::string           filename;

   Angle                 latitude;
   Angle                 longitude;
   Earth                 earth;
   bool                  dynamic{true};

   StarDB                star_db;
   GUI::Frame            frame{PROGRAM, 0, 0, PLT::Frame::FULL_SCREEN};
   PolarPlot             plot{frame};

   //! 
   void drawReticule(unsigned step_ra, Angle outer_decl, Angle inner_decl, unsigned step_decl)
   {
      // Draw the compass points
      Angle save_rotation = plot.getRotation();
      plot.setRotation(0.0);
      plot.setFont(GUI::font_teletext18, /* horz_align= */ 0, /* vert_align= */ 0);
      plot.drawText(STB::RED, Angle::deg(  0), outer_decl - Angle::deg(4), "N");
      plot.drawText(STB::RED, Angle::deg( 90), outer_decl - Angle::deg(4), "W");
      plot.drawText(STB::RED, Angle::deg(180), outer_decl - Angle::deg(4), "S");
      plot.drawText(STB::RED, Angle::deg(270), outer_decl - Angle::deg(4), "E");
      plot.setRotation(save_rotation);

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

      plot.drawCircleOfDeclination(STB::RGB(0xA0, 0x00, 0x00), 0.0);
      plot.drawCircleOfDeclination(STB::RGB(0xC0, 0x00, 0x00), inner_decl);
      plot.drawCircleOfDeclination(STB::RGB(0xC0, 0x00, 0x00), outer_decl);
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

      sprintf(text, "Time : %02u:%02u:%02u UTC",
              earth.getUTCHour(), 
              earth.getMinute(), 
              earth.getSecond());
      frame.drawText(STB::RED, STB::BLACK, 8, 26, &GUI::font_teletext18, text);

      sprintf(text, "Lat  : %c%02d\x7f%02d'%04.1f\"",
              latitude.degrees() >= 0 ? 'N' : 'S',
              ::abs((int)latitude.degrees()),
              latitude.minutes(),
              latitude.seconds());
      frame.drawText(STB::RED, STB::BLACK, 8, 48, &GUI::font_teletext18, text);

      sprintf(text, "Lon  : %c%03d\x7f%02d'%04.1f\"",
              longitude.degrees() >= 0 ? 'W' : 'E',
              ::abs((int)longitude.degrees()),
              longitude.minutes(),
              longitude.seconds());
      frame.drawText(STB::RED, STB::BLACK, 8, 70, &GUI::font_teletext18, text);
   }

   void doPlot()
   {
      frame.clear(STB::BLACK);

      drawState();

      plot.setRotation(earth.getEclipticLongitude() + earth.getRotationOfPrimeMeridian());
      plot.setView(size, size, size - 40);
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

      for(const auto& star : star_db)
      {
         if (!star.isTheSun())
         {
            float mag = star.getMagnitude();
            if (mag > 7)     mag = 7;
            mag = pow(1.112, -mag);
            float m = (mag - min_mag) / (max_mag - min_mag);

            STB::Colour col = STB::GREY(m*255);

            const Star::Vector& pos = star.getPosition();
            double proj_radius      = sqrt(pos.x * pos.x + pos.y * pos.y);
            Angle  declination      = Angle::rad(atan2(pos.z, proj_radius));
            Angle  right_ascension  = Angle::rad(atan2(pos.y, pos.x));

            plot.fillCircle(col, right_ascension, declination, m * 8);

            if (debug)
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
               return 0;

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
               earth.setUTC(earth.getUTCHour() + (shift ? -1 : +1), earth.getMinute());
               doPlot();
               break;

            case 'q':
               dynamic = false;
               earth.setUTC(earth.getUTCHour(), earth.getMinute() + (shift ? -15 : +15));
               doPlot();
               break;

            case 'm':
               dynamic = false;
               earth.setUTC(earth.getUTCHour(), earth.getMinute() + (shift ? -1 : +1));
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

   virtual int startConsoleApp() override
   {
      latitude  = Angle::deg(latitude_degs);
      longitude = Angle::deg(longitude_degs);

      filename = "/Users/johnh/Data/Astronomy/Data/hygdata_v3.csv";

      star_db.load(filename);

      if (debug) printf("Number of stars = %lu\n", star_db.size());

      frame.resize(size * 2, size * 2);

      return eventLoop();
   }

public:
   GlxyApp(int argc, const char* argv[])
      : ConsoleApp(PROGRAM, DESCRIPTION, LINK, AUTHOR, VERSION, COPYRIGHT_YEAR)
   {
      parseArgsAndStart(argc, argv);
   }
};


int main(int argc, const char* argv[])
{
   GlxyApp(argc, argv);
}
