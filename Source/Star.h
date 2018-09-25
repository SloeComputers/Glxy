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

#include "CSV.h"
#include "STB/Vector3.h"


enum class Attr
{
   MAG,
   X,
   Y,
   Z,
   VX,
   VY,
   VZ
};


class Star : public CSV::Document<Attr>::AttrCallBack
{
public:
   Star() = default;

   using Vector = STB::Vector3<double>;

   void setAttr(Attr attr, const char* value) override
   {
      switch(attr)
      {
      case Attr::MAG: mag   = strtod(value, nullptr); break;
      case Attr::X:   pos.x = strtod(value, nullptr); break;
      case Attr::Y:   pos.y = strtod(value, nullptr); break;
      case Attr::Z:   pos.z = strtod(value, nullptr); break;
      case Attr::VX:  vel.x = strtod(value, nullptr); break;
      case Attr::VY:  vel.y = strtod(value, nullptr); break;
      case Attr::VZ:  vel.z = strtod(value, nullptr); break;
      }
   }

   float getMagnitude() const { return mag; }

   const Vector& getPosition() const { return pos; }

   const Vector& getVelocity() const { return vel; }

private:
   float  mag{0.0};
   Vector pos{0.0, 0.0, 0.0};
   Vector vel{0.0, 0.0, 0.0};
};

#endif // STAR_H
