#pragma once
#include <string>
#include <vector>

struct User{
  int id = 0;
  std::string username;
  std::string password;
  std::string email;
  std::vector<std::string> skills;
  std::string role = "applicant"; // employer or applicant
  std::string name;
  std::string phone;
};