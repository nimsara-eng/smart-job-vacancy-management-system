#pragma once
#include <string>

struct Application{
  int applicationId = 0;
  int userId = 0;
  int jobId = 0;
  std::string status = "pending"; // pending, approved, rejected
  std::string appliedDate;
};