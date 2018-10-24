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
#include <cstdio>

#include "STB/Matrix_4x4.h"

#include "Angle.h"
#include "Vector.h"

struct KeplerianElements
{
   double   a{0.0};   // Semi-major axis (Au)
   Angle    e{0.0};   // Eccentricity
   Angle    I{0.0};   // Inclination
   double   L{0.0};   // Mean longitude (degs)
   double   w{0.0};   // Longitude of perihelion (degs)
   double   W{0.0};   // Longitude of ascending node (degs)

   KeplerianElements() = default;

   KeplerianElements(double a_, Angle e_, Angle I_, double L_, double w_, double W_)
      : a(a_)
      , e(e_)
      , I(I_)
      , L(L_)
      , w(w_)
      , W(W_)
   {
   }

   void debug()
   {
      printf("a = %g Au\n", a);
      printf("e = %g rad\n", double(e.rad()));
      printf("I = %g deg\n", double(I.deg()));
      printf("L = %g deg\n", L);
      printf("w = %g deg\n", w);
      printf("W = %g deg\n", W);
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

   void computePosition(Vector& pos)
   {
      double argument_of_perihelion = w - W;
      // printf("argument of perihelion = %g\n", argument_of_perihelion);

      // Mean anomaly
      double M = L - w;
      // printf("M = %g\n", M);

      // Ensure mean anomoly is between -180 and +180 degs
      M = fmod(fmod(M + 180.0, 360.0) - 180 - 180, 360.0) + 180.0;
      // printf("M = %g deg\n", M);

      // Compute the eccentric anomoly
      double E = M + e.deg()*sin_deg(M);
      // printf("E0 = %g deg\n", E);

      while(true)
      {
         double delta_M = M - (E - e.deg() * sin_deg(E));
         double delta_E = delta_M / (1.0 - e.rad() * cos_deg(E));
         E = E + delta_E;
         if (fabs(delta_E) < 1E-6) break;
         // printf("E%d = %g deg\n", i, E);
      }

      // Compute the planet's heliocentric co-ordinates
      pos.x = a * (cos_deg(E) - e.rad());
      pos.y = a * sqrt(1.0 - e.rad() * e.rad())*sin_deg(E);
      pos.z = 0.0;

      // Transform to co-ordinates in the ecliptic plane
      STB::Matrix_4x4<double> trans;
      trans.rotateZ(Angle::deg(-W).rad());
      trans.rotateX(-I.rad());
      trans.rotateZ(Angle::deg(-argument_of_perihelion).rad());
      pos = trans.transformPos(pos);

      // Transform to equatorial co-ordinates
      trans.setIdentity();
      trans.rotateX(-getObliquity().rad());
      pos = trans.transformPos(pos);
   }

   static double sin_deg(double angle)
   {
      return sin(Angle::deg(angle).rad());
   }

   static double cos_deg(double angle)
   {
      return cos(Angle::deg(angle).rad());
   }

   static unsigned getEpoch();
   static Angle    getObliquity();
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
