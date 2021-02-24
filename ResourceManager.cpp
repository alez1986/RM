#include "ResourceManager.h"

// STL
#include <string>


ResourceManager::ResourceManager(size_t capacity)
  : m_capacity(capacity)
{}


string ResourceManager::last_error()
{
  return m_last_error;
}


void ResourceManager::set_capacity(size_t capacity)
{
  m_capacity = capacity;
}


void ResourceManager::set_personal_limit(const string &user_name, size_t limit)
{
  m_personal_limit[user_name] = limit;
}


void ResourceManager::set_personal_limits(const unordered_map<string, size_t> &limits)
{
  m_personal_limit = limits;
}


string ResourceManager::allocate(const string& user_name) {
  m_last_error.clear();

  if (m_resources.size() >= m_capacity) {
    m_last_error = string("RM has reached its maximum capacity");
    return string();
  }

  if (m_personal_limit.count(user_name)) {
    size_t user_max_count = m_personal_limit.at(user_name);

    if (m_personal_counter.count(user_name) && user_max_count <= m_personal_counter.at(user_name)) {
      m_last_error = user_name + string(" has reached their maximum capacity");
      return string();
    }
  }

  string resource_id = std::to_string(m_id_gen);
  ++m_id_gen;

  ++(m_personal_counter[user_name]);
  m_resources[resource_id] = user_name;
  m_resources_by_users[user_name].push_back(resource_id);

  return resource_id;
}


bool ResourceManager::deallocate(const string& resource_id) {
  m_last_error.clear();

  if (!m_resources.count(resource_id)) {
    m_last_error = string("No resource with given ID") + resource_id;
    return false;
  }

  auto user_name = m_resources.at(resource_id);
  m_resources.erase(resource_id);
  auto iterator = std::find_if(m_resources_by_users[user_name].begin(),
               m_resources_by_users[user_name].end(),
               [resource_id](const string& s) { return resource_id == s;});
  m_resources_by_users[user_name].erase(iterator);
  --(m_personal_counter[user_name]);

  return true;
}


vector<string> ResourceManager::list(const string& user_name) {
  return m_resources_by_users.at(user_name);
}


vector<pair<string,string>> ResourceManager::list() {
  vector<pair<string,string>> data;
  for (auto const& [key, value] : m_resources) {
    data.push_back({value, key});
  }

  return data;
}
