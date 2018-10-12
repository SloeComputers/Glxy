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

#ifndef TIME_H
#define TIME_H

#include <ctime>
#include <cassert>

class Time
{
public:
   static const unsigned SECONDS_PER_MINUTE = 60;
   static const unsigned MINUTES_PER_HOUR   = 60;
   static const unsigned HOURS_PER_DAY      = 24;

   static const unsigned SECONDS_PER_DAY    = HOURS_PER_DAY * MINUTES_PER_HOUR * SECONDS_PER_MINUTE;

   Time()
   {
      setNow();
   }

   //! Returns the Julian year (e.g. 2018)
   unsigned getYear() const { return year; }

   //! Returns day of the year (0-365)
   signed getDayOfYear() const { return yday; }

   //! Returns day of the month (1-31)
   unsigned getDayOfMonth() const { return mday; }

   //! Returns the month (1-12)
   unsigned getMonth() const { return month; }

   //! Returns Co-oridinated Universal Time hours (0-23)
   signed getUTCHour() const { return hour; }

   //! Returns minutes (0-59)
   signed getMinute() const { return minute; }

   //! Returns seconds (0-60)
   signed getSecond() const { return second; }

   bool isLeapYear() const
   {
      if ((year % 4)   != 0) return false;
      if ((year % 100) != 0) return true;
      if ((year % 400) != 0) return false;
      return true;
   }

   unsigned getDaysPerYear() const
   {
      return isLeapYear() ? 366 : 365;
   }

   //! Set the date
   void setDate(unsigned year_, signed yday_)
   {
      year = year_;
      yday = yday_;

           if (yday >= getDaysPerYear()) { yday -= getDaysPerYear(); year++; }
      else if (yday < 0)                 { year--; yday += getDaysPerYear(); }

      static const unsigned days_in_month[12] =
      {
         31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
      };

      unsigned start = 0;
      for(unsigned m=0; m<12; m++)
      {
         unsigned end = start + days_in_month[m];
         if ((m == 1) && isLeapYear()) end++;
         if (yday < end)
         {
            month = m + 1;
            mday  = yday - start + 1;
            break;
         }
         start = end;
      }
   }

   //! Set the Universal Co-ordinated Time
   void setUTC(signed hour_, signed minute_, signed second_ = 0)
   {
      hour   = hour_;
      minute = minute_;
      second = second_;

           if (second >= 60) { second -= 60; minute++; }
      else if (second < 0)   { second += 60; minute--; }

           if (minute >= 60) { minute -= 60; hour++; }
      else if (minute < 0)   { minute += 60; hour--; }

           if (hour >= 24)   { hour -= 24; setDate(year, yday+1); }
      else if (hour < 0)     { hour += 24; setDate(year, yday-1); }
   }

   //! Update to the current time
   void setNow()
   {
      time_t t = ::time(nullptr);
      struct tm tm;
      ::gmtime_r(&t, &tm);
      assert(tm.tm_isdst == 0);

      setDate(1900 + tm.tm_year, tm.tm_yday);
      setUTC(tm.tm_hour, tm.tm_min, tm.tm_sec);
   }

private:
   unsigned year{1970};
   signed   yday{0};
   unsigned mday{0};
   unsigned month{0};
   signed   hour{0};
   signed   minute{0};
   signed   second{0};
};

#endif // TIME_H
