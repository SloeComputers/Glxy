//-------------------------------------------------------------------------------
// Copyright (c) 2018 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <ctime>
#include <cstdio>
#include <cassert>

#include <string>

class Time
{
public:
   Time(bool utc_)
      : utc(utc_)
   {
      setNow();
   }

   //! Is time Universal Co-oridinated time
   bool isUTC() const { return utc; }

   //! Returns the Julian year (e.g. 2018)
   unsigned getYear() const { return year; }

   //! Returns day of the year (0-365)
   signed getDayOfYear() const { return yday; }

   //! Returns day of the month (1-31)
   unsigned getDayOfMonth() const { return mday; }

   //! Returns the month (1-12)
   unsigned getMonth() const { return month; }

   //! Returns Time hours (0-23)
   signed getHour() const { return hour; }

   //! Returns minutes (0-59)
   signed getMinute() const { return minute; }

   //! Returns seconds (0-60)
   signed getSecond() const { return second; }

   //!
   double getYearsSinceEpoch(unsigned epoch) const
   {
      double days_since_j2000 = (getYear() - epoch) * 365.25 + getDayOfYear() + (getHour()/24.0);
      return days_since_j2000 / 365.25;
   }

   std::string getDate() const
   {
      static const char* months[] = {"Jan", "Feb", "Mar", "Apr",
                                     "May", "Jun", "Jul", "Aug",
                                     "Sep", "Oct", "Nov", "Dec"};

      std::string text = std::to_string(getDayOfMonth());

      text += ' ';
      text += months[getMonth() - 1];
      text += ' ';
      text += std::to_string(getYear());

      return text;
   }

   std::string getTime() const
   {
      // TODO don't do it like this it will scare people

      char text[32];

      snprintf(text, sizeof(text), "%02u:%02u:%02u", getHour(), getMinute(), getSecond());

      return text;
   }

   bool isLeapYear() const
   {
      if ((year % 4)   != 0) return false;
      if ((year % 100) != 0) return true;
      if ((year % 400) != 0) return false;
      return true;
   }

   signed getDaysPerYear() const
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
         signed end = start + days_in_month[m];
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

   //! Set the time
   void setTime(signed hour_, signed minute_, signed second_ = 0)
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
      time_t    t = ::time(nullptr);
      struct tm tm;

      if (utc)
      {
          ::gmtime_r(&t, &tm);
          assert(tm.tm_isdst == 0);
      }
      else
      {
          ::localtime_r(&t, &tm);
      }

      setDate(1900 + tm.tm_year, tm.tm_yday);
      setTime(tm.tm_hour, tm.tm_min, tm.tm_sec);
   }

   static const unsigned SECONDS_PER_MINUTE = 60;
   static const unsigned MINUTES_PER_HOUR   = 60;
   static const unsigned HOURS_PER_DAY      = 24;
   static const unsigned SECONDS_PER_DAY    = HOURS_PER_DAY * MINUTES_PER_HOUR * SECONDS_PER_MINUTE;

private:
   const bool utc{false};
   unsigned   year{1970};
   signed     yday{0};
   unsigned   mday{0};
   unsigned   month{0};
   signed     hour{0};
   signed     minute{0};
   signed     second{0};
};

