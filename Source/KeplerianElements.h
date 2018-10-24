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

#ifndef KEPLERIAN_ELEMENTS_H
#define KEPLERIAN_ELEMENTS_H

#include <string>

struct KeplerianElements
{
   double   a{0.0};   // Semi-major axis (Au)
   double   e{0.0};   // Eccentricity
   double   I{0.0};   // inclination (degs)
   double   L{0.0};   // mean longitude (degs)
   double   w{0.0};   // longitude of perihelion (degs)
   double   W{0.0};   // longitude of ascending node (degs)

   KeplerianElements() = default;

   KeplerianElements(double a_, double e_, double I_, double L_, double w_, double W_)
      : a(a_)
      , e(e_)
      , I(I_)
      , L(L_)
      , w(w_)
      , W(W_)
   {
   }

   const KeplerianElements& operator=(const KeplerianElements& that)
   {
      a = that.a;
      e = that.e;
      I = that.I;
      L = that.L;
      w = that.w;
      W = that.W;

      return that;
   }

   bool isValid() const { return a != 0.0; }

   void operator*=(double t)
   {
      a *= t;
      e *= t;
      I *= t;
      L *= t;
      w *= t;
      W *= t;
   }

   void operator+=(const KeplerianElements& that)
   {
      a += that.a;
      e += that.e;
      I += that.I;
      L += that.L;
      w += that.w;
      W += that.W;
   }

   static unsigned getEpoch();
};


struct KeplerianElementsInit : public KeplerianElements
{
   KeplerianElementsInit(const std::string& planet);
};


struct KeplerianElementsRate : public KeplerianElements
{
   KeplerianElementsRate(const std::string& planet);
};

#endif
