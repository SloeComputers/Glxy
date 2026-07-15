//-------------------------------------------------------------------------------
// Copyright (c) 2018 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "STB/ConsoleApp.h"

#include "Glxy.h"

#define  PROGRAM         "Glxy"
#define  DESCRIPTION     "Star database viewer"
#define  LINK            "https://github.com/AnotherJohnH/"
#define  COPYRIGHT_YEAR  "2018"
#define  AUTHOR          "John D. Haughton"

class GlxyApp : public STB::ConsoleApp
{
private:
   Options options;

   virtual int startConsoleApp() override
   {
      options.filename = "Data/hygdata_v3.csv";

      Glxy glxy{options, PROGRAM};

      return glxy.eventLoop();
   }

public:
   GlxyApp()
      : ConsoleApp(PROGRAM, DESCRIPTION, LINK, AUTHOR, COPYRIGHT_YEAR)
   {
   }
};


int main(int argc, const char* argv[])
{
   return GlxyApp().parseArgsAndStart(argc, argv);
}
