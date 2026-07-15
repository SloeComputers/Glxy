//-------------------------------------------------------------------------------
// Copyright (c) 2018 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
// IMPORTANT NOTE:
//
// The license status of the data values used in the table below is unknown. The
// full header in the text file used as the source for these data values is as
// follows...
//
//    "These data are to be used as described in the related document
//     titled "Keplerian Elements for Approximate Positions of the
//     Major Planets" by E.M. Standish (JPL/Caltech) available from
//     the JPL Solar System Dynamics web site (http://ssd.jpl.nasa.gov/)."
//
//------------------------------------------------------------------------------


#include "KeplerianElements.h"

#include <array>
#include <cassert>


struct DataSet
{
   const char*       name;
   KeplerianElements init;
   KeplerianElements rate;
};

// Keplerian elements and their rates, with respect to the mean ecliptic
// and equinox of J2000. Valid for the time-interval 1800 AD - 2050 AD.
//
//             a                  e                 I                          L            long.peri       long.node
//         Au, Au/Cy        rad, rad/Cy        deg, deg/Cy                deg, deg/Cy       deg, deg/Cy     deg, deg/Cy

const std::array<DataSet,8> data_set =
{
   {
      {"Mercury",
         { 0.38709927, Angle::rad(+0.20563593), Angle::deg( 7.00497902),   252.25032350,   77.45779628,   48.33076593 },
         { 0.00000037, Angle::rad(+0.00001906), Angle::deg(-0.00594749),149472.67411175,    0.16047689,   -0.12534081 }},
      {"Venus",
         { 0.72333566, Angle::rad(+0.00677672), Angle::deg( 3.39467605),   181.97909950,  131.60246718,   76.67984255 },
         { 0.00000390, Angle::rad(-0.00004107), Angle::deg(-0.00078890), 58517.81538729,    0.00268329,   -0.27769418 }},
      {"Earth",
         { 1.00000261, Angle::rad(+0.01671123), Angle::deg(-0.00001531),   100.46457166,  102.93768193,    0.0        },
         { 0.00000562, Angle::rad(-0.00004392), Angle::deg(-0.01294668), 35999.37244981,    0.32327364,    0.0        }},
      {"Mars",
         { 1.52371034, Angle::rad(+0.09339410), Angle::deg( 1.84969142),    -4.55343205,  -23.94362959,   49.55953891 },
         { 0.00001847, Angle::rad(+0.00007882), Angle::deg(-0.00813131), 19140.30268499,    0.44441088,   -0.29257343 }},
      {"Jupiter",
         { 5.20288700, Angle::rad(+0.04838624), Angle::deg( 1.30439695),    34.39644051,   14.72847983,  100.47390909 },
         {-0.00011607, Angle::rad(-0.00013253), Angle::deg(-0.00183714),  3034.74612775,    0.21252668,    0.20469106 }},
      {"Saturn",
         { 9.53667594, Angle::rad(+0.05386179), Angle::deg( 2.48599187),    49.95424423,   92.59887831,  113.66242448 },
         {-0.00125060, Angle::rad(-0.00050991), Angle::deg( 0.00193609),  1222.49362201,   -0.41897216,   -0.28867794 }},
      {"Uranus",
         {19.18916464, Angle::rad(+0.04725744), Angle::deg( 0.77263783),   313.23810451,  170.95427630,   74.01692503 },
         {-0.00196176, Angle::rad(-0.00004397), Angle::deg(-0.00242939),   428.48202785,    0.40805281,    0.04240589 }},
      {"Neptune",
         {30.06992276, Angle::rad(+0.00859048), Angle::deg( 1.77004347),   -55.12002969,   44.96476227,  131.78422574 },
         { 0.00026291, Angle::rad(+0.00005105), Angle::deg( 0.00035372),   218.45945325,   -0.32241464,   -0.00508664 }}
   }
};

unsigned KeplerianElements::getEpoch()
{
   return 2000; // J2000
}

Angle KeplerianElements::getObliquity()
{
   return Angle::deg(23.43928); // J2000
}

KeplerianElementsInit::KeplerianElementsInit(const std::string& planet)
{
   for(const auto& data : data_set)
   {
      if (planet == data.name)
      {
         *static_cast<KeplerianElements*>(this) = data.init;
         return;
      }
   }

   assert(!"Should not be here");
}

KeplerianElementsRate::KeplerianElementsRate(const std::string& planet)
{
   for(const auto& data : data_set)
   {
      if (planet == data.name)
      {
         *static_cast<KeplerianElements*>(this) = data.rate;
         return;
      }
   }

   assert(!"Should not be here");
}

