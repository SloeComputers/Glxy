//-------------------------------------------------------------------------------
// Copyright (c) 2018 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "STB/ConsoleApp.h"

#include "Glxy.h"

#define  DESCRIPTION     "Star database viewer"
#define  COPYRIGHT_YEAR  "2018"
#define  AUTHOR          "John D. Haughton"

class GlxyApp : public STB::ConsoleApp
{
public:
   GlxyApp()
      : ConsoleApp(PDK_PROJ_NAME, DESCRIPTION, PDK_PROJ_URL, AUTHOR, COPYRIGHT_YEAR)
   {
   }

private:
   virtual int startConsoleApp() override
   {
      options.filename = "Data/hygdata_v3.csv";

      Glxy glxy{options, PDK_PROJ_NAME};

      return glxy.eventLoop();
   }

   Options options;
};

int main(int argc, const char* argv[])
{
   return GlxyApp().parseArgsAndStart(argc, argv);
}
