//-------------------------------------------------------------------------------
// Copyright (c) 2018 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "STB/CSV.h"

#include "Star.h"

class StarDB : public std::vector<Star>
{
public:
   StarDB() = default;

   void load(const std::string& filename)
   {
      printf("Loading \"%s\"\n", filename.c_str());

      STB::CSV::Document<Attr> document(filename);;

      if (!document.isOpen())
      {
         fprintf(stderr, "ERR: Failed to open \"%s\"\n", filename.c_str());
         return;
      }

      document.readHeader();

      document.requireField("bayer", Attr::BAYER);
      document.requireField("con", Attr::CON);
      document.requireField("spect", Attr::SPECT);
      document.requireField("proper", Attr::NAME);
      document.requireField("mag", Attr::MAG);
      document.requireField("absmag", Attr::ABSMAG);

      document.requireField("x", Attr::X);
      document.requireField("y", Attr::Y);
      document.requireField("z", Attr::Z);

      document.requireField("vx", Attr::VX);
      document.requireField("vy", Attr::VY);
      document.requireField("vz", Attr::VZ);

      document.requireField("decrad", Attr::DECL);
      document.requireField("rarad",  Attr::RA);

      while(true)
      {
          emplace_back();

          if (!document.readRecord(back()))
          {
             pop_back();
             break;
          }
      }
   }
};

