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

#ifndef STAR_H
#define STAR_H

#include <cstdlib>

#include "Angle.h"
#include "STB/CSV.h"
#include "Object.h"


enum class Attr
{
   NAME,
   MAG,
   X,
   Y,
   Z,
   VX,
   VY,
   VZ,
   DECL,
   RA
};


class Star : public STB::CSV::Document<Attr>::AttrCallBack
           , public Object
{
public:
   Star() = default;

   void setAttr(Attr attr, const char* value) override
   {
      switch(attr)
      {
      case Attr::NAME:
         name = value;
         is_the_sun = name == "Sol";
         break;

      case Attr::MAG:  ap_mag = strtod(value, nullptr); break;
      case Attr::X:    pos.x  = strtod(value, nullptr); break;
      case Attr::Y:    pos.y  = strtod(value, nullptr); break;
      case Attr::Z:    pos.z  = strtod(value, nullptr); break;
      case Attr::VX:   vel.x  = strtod(value, nullptr); break;
      case Attr::VY:   vel.y  = strtod(value, nullptr); break;
      case Attr::VZ:   vel.z  = strtod(value, nullptr); break;
      case Attr::DECL: decl   = Angle::rad(strtod(value, nullptr)); break;
      case Attr::RA:   ra     = Angle::rad(strtod(value, nullptr)); break;
      }
   }

   bool isTheSun() const { return is_the_sun; }

   const Vector& getVelocity() const { return vel; }

   void debug(FILE* fp = stdout) const
   {
      printf("%s: pos = {%g, %g, %g} vel = {%g, %g, %g} ra=%.15f decl=%.15f ap_mag=%f\n",
             name.c_str(),
             pos.x, pos.y, pos.z,
             vel.x, vel.y, vel.z,
             double(ra.deg()), double(decl.deg()), ap_mag);
   }

private:
   Vector      vel{0.0, 0.0, 0.0};
   Angle       decl;
   Angle       ra;
   bool        is_the_sun{false};
};

#endif // STAR_H
