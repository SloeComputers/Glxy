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

#ifndef POLAR_PLOT_H
#define POLAR_PLOT_H

#include <cmath>

#include "Angle.h"
#include "STB/Vector2.h"
#include "GUI/Frame.h"

//! Maths support for polar plots
class PolarPlot
{
public:
   using Pixel = STB::Vector2<unsigned>;

   PolarPlot(GUI::Frame& frame_)
      : frame(frame_)
   {}

   //! Configure the position and size of the plot in pixels
   void setView(unsigned org_x, unsigned org_y, unsigned radius_)
   {
      radius   = radius_;
      centre.x = org_x;
      centre.y = org_y;
   }

   void setMinDeclination(Angle min_decl_)
   {
      min_decl = min_decl_;
   }

   void setRotation(Angle rotation_)
   {
      rotation = rotation_;
   }

   void setZenith(Angle zenith_)
   {
      zenith = zenith_;
   }

   Angle getRotation() const
   {
      return rotation;
   }

   //! Convert polar co-ordinates in radians to screen pixels
   bool polarToXY(Angle right_ascension, Angle declination, Pixel& pixel) const
   {
      right_ascension -= rotation;

      double r = radius * ((max_decl - declination) / (max_decl - min_decl));
      pixel.x  = centre.x + sin(right_ascension.rad()) * r;
      pixel.y  = centre.y - cos(right_ascension.rad()) * r;

      return declination > min_decl;
   }

   //! Draw a circle of declination
   void drawCircleOfDeclination(STB::Colour rgb, Angle decl)
   {
      Pixel origin;
      polarToXY(0.0, Angle::deg(90.0), origin);

      Pixel point_on_circle;
      polarToXY(0.0, decl, point_on_circle);

      unsigned radius = (point_on_circle - origin).getMagnitude();
      frame.drawCircle(rgb, origin.x, origin.y, radius);
   }

   void drawRadial(STB::Colour rgb, Angle right_ascension, Angle from_decl, Angle to_decl)
   {
      Pixel outer, inner;
      polarToXY(right_ascension, from_decl, outer); 
      polarToXY(right_ascension, to_decl,   inner);

      frame.drawLine(rgb, outer.x, outer.y, inner.x, inner.y);
   }

   void fillCircle(STB::Colour rgb, Angle right_ascension, Angle declination, unsigned radius)
   {
      Pixel pixel;

      if (polarToXY(right_ascension, declination, pixel))
      {
         frame.fillCircle(rgb, pixel.x, pixel.y, radius);
      }
   }

   void setFont(const GUI::Font& font_, signed horz_align_ = -1, signed vert_align_ = -1)
   {
      font       = &font_;
      horz_align = horz_align_;
      vert_align = vert_align_;
   }

   void drawText(STB::Colour rgb, Angle right_ascension, Angle declination, const std::string& text)
   {
      Pixel pos;
      polarToXY(right_ascension, declination, pos);

      if (horz_align >= 0)
      {
         unsigned width = text.size() * font->getWidth();
         pos.x -= horz_align == 0 ? width/2 : width;
      }

      if (vert_align >= 0)
      {
         unsigned height = font->getHeight();
         pos.y -= vert_align == 0 ? height/2 : height;
      }

      frame.drawText(rgb, STB::BLACK, pos.x, pos.y, font, text.c_str());
   }

private:
   GUI::Frame&      frame;
   const GUI::Font* font{nullptr};
   signed           horz_align{-1};
   signed           vert_align{-1};

   //!< Centre of the plot(pixels)
   Pixel        centre;

   //!< Radius of the plot (pixels)
   unsigned     radius{0};

   //!< Minimum declination to plot (declination on the circumferance)
   Angle        min_decl{0.0};

   //!< Maximum declination to plot
   Angle        max_decl{90.0, Angle::Unit::DEG};

   Angle        rotation;
   Angle        zenith;
};

#endif // POLAR_PLOT_H
