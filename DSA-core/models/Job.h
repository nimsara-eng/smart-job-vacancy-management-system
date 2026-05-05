#pragma once
#include <vector>
#include <string>


struct Job{
  int id = 0;
  std::string title;
  std::string company;
  std::vector<std::string> requiredSkills;
  float salary = 0.0;
  bool isOpen = true;
  bool isRemote = false;
  std::string description;
  std::string location;
  std::string category;
};