#include "Date.hpp"
#include <string>
#include <sstream>
#include <stdio.h>


Date::Date() {
  m_year = 0;
  m_month = 0;
  m_day = 0;
  m_hour = 0;
  m_minute = 0;
}

Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute)
         : m_year(t_year), m_month(t_month), m_day(t_day), m_hour(t_hour), m_minute(t_minute) {}

Date::Date(const std::string &dateString) {
  bool flag = false;
  if(dateString.size() != 16 || dateString[4] != '-' || 
                dateString[7] != '-' || dateString[10] != '/' || dateString[13] != ':') {
    m_year = 0;
    m_month = 0;
    m_day = 0;
    m_hour = 0;
    m_minute = 0;
  }
  else {
   for(int i = 0; i < dateString.size(); i++) {
      if(i != 4 && i != 7 && i != 10 && i != 13) {
        if(dateString[i] < '0' || dateString[i] > '9') {
          flag = true;
          break;
        }
      }
    }
    if(flag) {
     m_year = 0;
      m_month = 0;
      m_day = 0;
      m_hour = 0;
      m_minute = 0;
    }
    else {
      std::string s = dateString;
      for(char & c : s) {
        if(!isdigit(c))
          c = ' ';
      }
      std::stringstream ss;
      ss << s;
      ss >> m_year >> m_month >> m_day >> m_hour >> m_minute;
    }
  }
}

int Date::getYear(void) const {
  return m_year;
}

void Date::setYear(const int t_year) {
  m_year = t_year;
}

int Date::getMonth(void) const {
  return m_month;
}

void Date::setMonth(const int t_month) {
  m_month = t_month;
}

int Date::getDay(void) const {
 return m_day;
}

void Date::setDay(const int t_day) {
  m_day = t_day;
}

int Date::getHour(void) const {
  return m_hour;
}

void Date::setHour(const int t_hour) {
  m_hour = t_hour;
}

int Date::getMinute(void) const {
  return m_minute;
}

void Date::setMinute(const int t_minute) {
  m_minute = t_minute;
}

bool Date::isValid(const Date &t_date) {
  if(t_date.m_year >= 10000 || t_date.m_year < 1000)
   return false;
  if(t_date.m_month <= 0 || t_date.m_month > 12)
   return false;
  int a[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if(t_date.m_year % 4 == 0 && t_date.m_year % 100 != 0 || t_date.m_year % 400 == 0)
   a[1] = 29;
  if(t_date.m_day > a[t_date.m_month - 1] || t_date.m_day <= 0)
   return false;
  if(t_date.m_hour < 0 || t_date.m_hour >= 24)
   return false;
  if(t_date.m_minute < 0 || t_date.m_minute >= 60)
   return false;
  return true;
}

Date Date::stringToDate(const std::string &t_dateString) {
  Date date(t_dateString);
  bool flag = isValid(date);
  if(flag)
   return date;
  else
   return Date();
}

std::string Date::dateToString(const Date &t_date) {
  if(!isValid(t_date))
   return "0000-00-00/00:00";
  char a[17];
  snprintf(a, 17, "%04d-%02d-%02d/%02d:%02d",
                t_date.m_year, t_date.m_month, t_date.m_day, t_date.m_hour, t_date.m_minute);
  std::string str = a;
  return str;
}

Date & Date::operator=(const Date &t_date) {
  m_year = t_date.m_year;
  m_month = t_date.m_month;
  m_day = t_date.m_day;
  m_hour = t_date.m_hour;
  m_minute = t_date.m_minute;
  return *this;
}

bool Date::operator==(const Date &t_date) const {
  return m_year == t_date.m_year &&
           m_month == t_date.m_month &&
             m_day == t_date.m_day &&
               m_hour == t_date.m_hour && 
                 m_minute == t_date.m_minute;
}

bool Date::operator>(const Date &t_date) const {
  if(m_year > t_date.m_year)
    return true;
  if(m_year < t_date.m_year)
   return false;
  if(m_month > t_date.m_month)
   return true;
  if(m_month < t_date.m_month)
   return false;
  if(m_day > t_date.m_day)
   return true;
  if(m_day < t_date.m_day)
   return false;
  if(m_hour < t_date.m_hour)
    return false;
  if(m_hour > t_date.m_hour)
   return true;
  if(m_minute <= t_date.m_minute)
   return false;
  return true;
} 

bool Date::operator<(const Date &t_date) const {
  return !(*this > t_date || *this == t_date);
}

bool Date::operator>=(const Date &t_date) const {
  return *this > t_date || *this == t_date;
}

bool Date::operator<=(const Date &t_date) const {
  return *this < t_date || *this == t_date;
}
