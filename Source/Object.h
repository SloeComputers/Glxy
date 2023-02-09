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

#ifndef OBJECT_H
#define OBJECT_H

#include "Vector.h"

class Object
{
public:
   Object() = default;

   Object(const std::string& name_)
      : name(name_)
   {}

   void setName(const std::string& name_) { name = name_; }

   void setAparentMagnitude(float ap_mag_) { ap_mag = ap_mag_; }

   void setPositionAu(const Vector& pos_au_) { pos_au = pos_au_; }

   void setPositionParsec(const Vector& pos_ps_) { pos_au = pos_ps_ * AUS_PER_PARSEC; }

   const std::string& getName() const { return name; }

   float getAparentMagnitude() const { return ap_mag; }

   const Vector& getPositionAu() const { return pos_au; }

   const Vector getPositionParsec() const { return pos_au / AUS_PER_PARSEC; }

private:
   std::string  name{};
   float        ap_mag{0.0};
   Vector       pos_au{0.0, 0.0, 0.0};
   const double AUS_PER_PARSEC {2.06265e5};
};

#endif // OBJECT_H
