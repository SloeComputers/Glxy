//-------------------------------------------------------------------------------
// Copyright (c) 2018 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdlib>

#include "Angle.h"
#include "STB/CSV.h"
#include "Object.h"


enum class Attr
{
   CON,
   SPECT,
   BAYER,
   NAME,
   MAG,
   ABSMAG,
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

   void setAttr(Attr attr, const std::string& value) override
   {
      switch(attr)
      {
      case Attr::CON:
         constelation = value;
         break;

      case Attr::SPECT:
         spectrum = value;
         break;

      case Attr::BAYER:
              if (value == "Alp") bayer = 'a';
         else if (value == "Bet") bayer = 'b';
         else if (value == "Gam") bayer = 'c';
         else if (value == "Del") bayer = 'd';
         else if (value == "Eps") bayer = 'e';
         else if (value == "Zet") bayer = 'f';
         else if (value == "Eta") bayer = 'g';
         else if (value == "The") bayer = 'h';
         else if (value == "Iot") bayer = 'i';
         else if (value == "Kap") bayer = 'j';
         else if (value == "Lam") bayer = 'k';
         else if (value == "Mu")  bayer = 'l';
         else if (value == "Nu")  bayer = 'm';
         else if (value == "Xi")  bayer = 'n';
         else if (value == "Omi") bayer = 'o';
         else if (value == "Pi")  bayer = 'p';
         else if (value == "Rho") bayer = 'q';
         else if (value == "Sig") bayer = 'r';
         else if (value == "Tau") bayer = 's';
         else if (value == "Ups") bayer = 't';
         else if (value == "Phi") bayer = 'u';
         else if (value == "Chi") bayer = 'v';
         else if (value == "Psi") bayer = 'w';
         else if (value == "Ome") bayer = 'x';
         break;

      case Attr::NAME:
         setName(value);
         is_the_sun = getName() == "Sol";
         break;

      case Attr::MAG:  setAparentMagnitude(strtod(value.c_str(), nullptr)); break;
      case Attr::VX:   vel.x  = strtod(value.c_str(), nullptr); break;
      case Attr::VY:   vel.y  = strtod(value.c_str(), nullptr); break;
      case Attr::VZ:   vel.z  = strtod(value.c_str(), nullptr); break;
      case Attr::DECL: decl   = Angle::rad(strtod(value.c_str(), nullptr)); break;
      case Attr::RA:   ra     = Angle::rad(strtod(value.c_str(), nullptr)); break;

      case Attr::ABSMAG:
         abs_mag = strtod(value.c_str(), nullptr);
         break;

      case Attr::X:
         {
            Vector pos = getPositionParsec();
            pos.x  = strtod(value.c_str(), nullptr);
            setPositionParsec(pos);
         }
         break;

      case Attr::Y:
         {
            Vector pos = getPositionParsec();
            pos.y  = strtod(value.c_str(), nullptr);
            setPositionParsec(pos);
         }
         break;

      case Attr::Z:
         {
            Vector pos = getPositionParsec();
            pos.z  = strtod(value.c_str(), nullptr);
            setPositionParsec(pos);
         }
         break;
      }
   }

   bool isTheSun() const { return is_the_sun; }

   const Vector& getVelocity() const { return vel; }

   const std::string& getConstelation() const { return constelation; }

   const std::string& getSpectrum() const { return spectrum; }

   float getAbsMag() const { return abs_mag; }

   char getBayer() const { return bayer; }

   void debug(FILE* fp = stdout) const
   {
      Vector pos = getPositionParsec();
      printf("%s: pos = {%g, %g, %g} vel = {%g, %g, %g} ra=%.15f decl=%.15f ap_mag=%f\n",
             getName().c_str(),
             pos.x, pos.y, pos.z,
             vel.x, vel.y, vel.z,
             double(ra.deg()), double(decl.deg()), getAparentMagnitude());
   }

private:
   std::string constelation;
   std::string spectrum;
   Vector      vel{0.0, 0.0, 0.0};
   Angle       decl;
   Angle       ra;
   bool        is_the_sun{false};
   char        bayer;
   float       abs_mag;
};

