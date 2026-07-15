//-------------------------------------------------------------------------------
// Copyright (c) 2018 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "STB/ConsoleApp.h"
#include "GUI/Frame.h"
#include "PLT/Event.h"

#include "Options.h"
#include "SolarSystem.h"

#include "ViewPlanisphere.h"
#include "ViewPlanets.h"

class Glxy
{
private:
   using Pixel = PolarPlot::Pixel;

   const Options&   options;
   GUI::Frame       frame;
   SolarSystem      system;
   ViewPlanisphere  view_planisphere;
   ViewPlanets      view_planets;
   bool             dynamic{true};
   bool             reticule{true};
   bool             names{true};
   unsigned         mode{1};

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

      Angle latitude = Angle::deg(options.latitude_degs);
      snprintf(txt, sizeof(txt), "Lat :  %c%02d\x7f%02d'%04.1f\"",
              latitude.deg() >= 0 ? 'N' : 'S',
              ::abs((int)latitude.degrees()),
              latitude.minutes(),
              latitude.seconds());
      frame.drawText(STB::RED, STB::BLACK, options.width - 250, 4, &GUI::font_teletext18, txt);

      Angle longitude = Angle::deg(options.longitude_degs);
      snprintf(txt, sizeof(txt), "Lon : %c%03d\x7f%02d'%04.1f\"",
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

      switch(mode)
      {
      default:
         mode = 1;
      case 1:
         view_planisphere.render(frame,
                                 options.width / 2, options.height / 2,
                                 options.width, options.height,
                                 system, reticule, names);
         break;

      case 2:
         view_planets.render(frame,
                             options.width / 2, options.height / 2,
                             options.width, options.height,
                             system, reticule, names);
         break;
      }

      frame.refresh();
   }

public:
   Glxy(const Options& options_, const std::string& title_)
      : options(options_)
      , frame{title_.c_str(),
              options.width, options.height,
              options.full_screen ? PLT::Frame::FULL_SCREEN : 0}
      , system((const char*)options.filename)
      , view_planisphere(options.latitude_degs, options.longitude_degs)
   {
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

            case '1': mode = 1; break;
            case '2': mode = 2; break;
            case '3': mode = 3; break;
            case '4': mode = 4; break;

            case 'r': reticule = !reticule; break;

            case 't': names = !names; break;

            case 'n':
               dynamic = true;
               system.setNow();
               break;

            case 'y':
               dynamic = false;
               system.adjustDate(shift ? -1 : +1, 0);
               break;

            case 'd':
               dynamic = false;
               system.adjustDate(0, shift ? -1 : +1);
               break;

            case 'h':
               dynamic = false;
               system.adjustTime(shift ? -1 : +1, 0);
               break;

            case 'q':
               dynamic = false;
               system.adjustTime(0, shift ? -15 : +15);
               break;

            case 'm':
               dynamic = false;
               system.adjustTime(0, shift ? -1 : +1);
               break;

            case PLT::LSHIFT:
            case PLT::RSHIFT:
               shift = true;
               break;

            default:
               break;
            }
            doPlot();
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

