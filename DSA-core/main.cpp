#include <iostream>
using namespace std;

struct Job{
  int id = 0;
  string title;
  string company;
  string location;
  double salary = 0.0;
  string category;
  string status = "open";
  string description;
};

struct Applicant{
  int id = 0;
  string name;
  string email;
  int jobId = 0;
  string jobTitle;
  int experience = 0;
  string status = "pending";
  string resumePath = "";
  string phone = "";
};

class JobNode{
  friend class JobLinkedList;
private:
  Job data;
  JobNode* next;
  JobNode* prev;
public:
  JobNode():next(nullptr), prev(nullptr) {}
  JobNode(Job j): data(j), next(nullptr), prev(nullptr) {}
};

class JobLinkedList{
  private:
    JobNode* head;
    JobNode* tail;
    int size;
  public:
    JobLinkedList(): head(nullptr), tail(nullptr), size(0) {}

    void addJob(Job job){
      JobNode* newNode = new JobNode(job);

      if(head == NULL){
        head = tail = newNode;
      }else {
        tail -> next = newNode;
        newNode -> prev = tail;
        tail = newNode;
      }
      size++;
    }

    void displayAll(){
      if(head == NULL){
        cout << "No job vacancies available." << endl;
        return;
      }
      JobNode* current = head;
      while(current != NULL){
        cout << "----------------------------" << endl;
        cout << "ID       : " << current->data.id          << endl;
        cout << "Title    : " << current->data.title       << endl;
        cout << "Company  : " << current->data.company     << endl;
        cout << "Location : " << current->data.location    << endl;
        cout << "Salary   : " << current->data.salary      << endl;
        cout << "Category : " << current->data.category    << endl;
        cout << "Status   : " << current->data.status      << endl;
        current = current->next;
      }
      cout << "----------------------------" << endl;
      cout << "Total Jobs: " << size << endl;
    }
};


int main(){

  cout << "Smart Job Vacancy Management System" << endl;
   JobLinkedList jobList;

    Job j1;
    j1.id       = 1;
    j1.title    = "Software Engineer";
    j1.company  = "TechCorp";
    j1.location = "Colombo";
    j1.salary   = 85000;
    j1.category = "IT";

    Job j2;
    j2.id       = 2;
    j2.title    = "Data Analyst";
    j2.company  = "DataFirm";
    j2.location = "Kandy";
    j2.salary   = 60000;
    j2.category = "IT";

    jobList.addJob(j1);
    jobList.addJob(j2);
    jobList.displayAll();

  return 0;
}

