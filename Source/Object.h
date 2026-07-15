//-------------------------------------------------------------------------------
// Copyright (c) 2018 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

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

