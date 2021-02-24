#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

// STL
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

using namespace std;


class ResourceManager {

public:
  ResourceManager(size_t capacity = 0);
  string last_error();
  void set_capacity(size_t capacity);

  // Add person with limit or rewrite current value
  void set_personal_limit(const string& user_name, size_t limit);

  // Set person's limits, overwrite the current one
  void set_personal_limits(const unordered_map<string,size_t>& limits);

  string allocate(const string& user_name);

  bool deallocate(const string& resource_id);

  vector<string> list(const string& user_name);

  vector<pair<string,string>> list();

private:
  size_t m_capacity = 0;
  unordered_map<string,size_t> m_personal_limit;
  unordered_map<string,size_t> m_personal_counter;

  // resource id generator
  int m_id_gen = 1;
  string m_last_error;

  map<string, vector<string>> m_resources_by_users;   // { user_name, [resource_id] }
  map<string,string> m_resources;                     // { resource_id, user_name }
};

#endif // RESOURCEMANAGER_H
