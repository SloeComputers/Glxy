//-------------------------------------------------------------------------------
// Copyright (c) 2018 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <string>

#include "STB/Option.h"

struct Options
{
   STB::Option<bool>        debug{         'd', "debug",    "Enable debug",      false};
   STB::Option<unsigned>    width{         'w', "width",    "Width (pixels)",    1500};
   STB::Option<unsigned>    height{        'h', "height",   "Height (pixels)",   1500};
   STB::Option<bool>        full_screen{   'f', "full",     "Full screen",       false};
   STB::Option<double>      latitude_degs{ 'a', "lat",      "Latitude (degs)",   51.5};
   STB::Option<double>      longitude_degs{'o', "lon",      "Longitude (degs)",  0.0};
   STB::Option<const char*> filename{      'D', "database", "Database CSV file", "Data/hygdata_v3.csv"};
};

