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

#ifndef OPTIONS_H
#define OPTIONS_H

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

#endif
