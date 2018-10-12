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

#ifndef STAR_DB_H
#define STAR_DB_H

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

      document.readHeader();

      document.requireField("proper", Attr::NAME);
      document.requireField("mag", Attr::MAG);

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

#endif // STAR_DB_H
