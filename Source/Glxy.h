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

#include "STB/ConsoleApp.h"
#include "GUI/Frame.h"
#include "PLT/Event.h"

#include "Options.h"
#include "SolarSystem.h"
#include "ViewPlanisphere.h"

class Glxy
{
private:
   using Pixel = PolarPlot::Pixel;

   const Options&   options;
   GUI::Frame       frame;
   SolarSystem      system;
   ViewPlanisphere  view_planisphere;
   bool             dynamic{true};
   bool             reticule{true};
   bool             names{true};
   unsigned         mode{1};

   void doPlot()
   {
      frame.clear(STB::BLACK);

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

#endif
