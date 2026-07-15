//-------------------------------------------------------------------------------
// Copyright (c) 2018 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

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

   //! Ensure angle between -180 and +180 degs
   static double modulus180(double angle)
   {
      return fmod(fmod(angle + 180.0, 360.0) - 180 - 180, 360.0) + 180.0;
   }

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
      printf("Eccentricity (e)        = %g rad\n", double(e.rad()));
      printf("Semi-major axis (a)     = %g Au\n", a);
      printf("Inclination (i)         = %g deg\n", double(I.deg()));
      printf("Long. of asc. node ( )  = %g deg\n", W);
      printf("Long. of perihelion (w) = %g deg\n", w);
      printf("Longitude (L)           = %g deg [%g}\n", L, modulus180(L));
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

   void computePosition(Vector& pos, bool debug = false)
   {
      double argument_of_perihelion = w - W;
      if(debug) printf("Arg. of perihelion      = %g deg\n", argument_of_perihelion);

      // Mean anomaly
      double M = L - w;
      if (debug) printf("Mean anomaly            = %g deg\n", M);

      // Ensure mean anomoly is between -180 and +180 degs
      M = modulus180(M);
      if (debug) printf("Mean anomaly            = %g deg\n", M);

      // Compute the eccentric anomoly
      double E = M + e.deg()*sin_deg(M);
      if (debug) printf("Eccentric anomaly (E0)  = %g deg\n", E);

      for(unsigned i = 1; true; i++)
      {
         double delta_M = M - (E - e.deg() * sin_deg(E));
         double delta_E = delta_M / (1.0 - e.rad() * cos_deg(E));
         E = E + delta_E;
         if (debug) printf("Eccentric anomaly (E%u)  = %g deg\n", i, E);
         if (fabs(delta_E) < 1E-6) break;
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
      if (debug) printf("Position (equitorial)   = (%.4f,%.4f,%.4f) Au\n", pos.x, pos.y, pos.z);
      if (debug) printf("Longitude (equitorial)  = %g deg\n",
                        (double)Angle::rad(atan2(pos.y, pos.x)).deg());
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

