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

#include <cstdio>

#include "STB/ConsoleApp.h"

#include "CSV.h"
#include "Star.h"

#define  PROGRAM         "Glxy"
#define  DESCRIPTION     "Star database viewer"
#define  LINK            "https://github.com/AnotherJohnH/"
#define  COPYRIGHT_YEAR  "2018"
#define  AUTHOR          "John D. Haughton"
#define  VERSION         PROJ_VERSION


class GlxyApp : public STB::ConsoleApp
{
private:
   std::string        filename;
   std::vector<Star>  star_list;

   virtual int startConsoleApp() override
   {
      filename = "/Users/johnh/Data/Astronomy/Data/hygdata_v3.csv";

      printf("Loading \"%s\"\n", filename.c_str());

      CSV::Document<Attr> document(filename);;

      document.readHeader();

      document.requireField("absmag", Attr::MAG);

      document.requireField("x",  Attr::X);
      document.requireField("y",  Attr::Y);
      document.requireField("z",  Attr::Z);

      document.requireField("vx", Attr::VX);
      document.requireField("vy", Attr::VY);
      document.requireField("vz", Attr::VZ);

      // document.debugFields();

      while(true)
      {
          star_list.emplace_back();
          Star& star = star_list.back();

          if (!document.readRecord(star))
          {
             star_list.pop_back();
             break;
          }
      }

      printf("Number of stars = %lu\n", star_list.size());

      return 0;
   }

public:
   GlxyApp(int argc, const char* argv[])
      : ConsoleApp(PROGRAM, DESCRIPTION, LINK, AUTHOR, VERSION, COPYRIGHT_YEAR)
   {
      parseArgsAndStart(argc, argv);
   }
};


int main(int argc, const char* argv[])
{
   GlxyApp(argc, argv);
}
