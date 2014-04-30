/* 
 * File:   util.h
 * Author: acordeon
 *
 * Created on April 7, 2014, 9:53 AM
 */

#ifndef UTIL_H
#define	UTIL_H

#include <string.h>
#include <fstream>

static bool ReadFile(const char* filename, std::string &text){
  std::ifstream in(filename);
  if(!in.is_open()) return false;
  std::string line;
  while(getline(in,line)){
    text+=line+"\n";
  }
  return true;
  
}

#endif	/* UTIL_H */

