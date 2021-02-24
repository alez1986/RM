// Local
#include "ResourceManager.h"

// STL
#include <iostream>
#include <sstream>


using namespace std;


template <class K, class V>
ostream &operator<<(ostream &os, const pair<K, V> &m) {
  return os << "(" << m.first << ", " << m.second << ")";
}


template <class T>
ostream &operator<<(ostream &os, const vector<T> &s) {
  os << "[";
  bool first = true;
  for (const auto &x : s) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << x;
  }
  return os << "]";
}


template <class K, class V>
ostream &operator<<(ostream &os, const map<K, V> &m) {
  os << "{";
  bool first = true;
  for (const auto &kv : m) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << kv.first << ": " << kv.second;
  }
  return os << "}";
}


template <class T, class U>
void AssertEqual(const T &t, const U &u, const string &hint = {}) {
  if (t != u) {
    ostringstream os;
    os << "Assertion failed: " << t << " != " << u;
    if (!hint.empty()) {
      os << " hint: " << hint;
    }
    throw runtime_error(os.str());
  }
}


void Assert(bool b, const string &hint) { AssertEqual(b, true, hint); }

class TestRunner {
public:
  template <class TestFunc>
  void RunTest(TestFunc func, const string &test_name) {
    try {
      func();
      cerr << test_name << " OK" << endl;
    }
    catch (exception &e) {
      ++fail_count;
      cerr << test_name << " fail: " << e.what() << endl;
    }
    catch (...) {
      ++fail_count;
      cerr << "Unknown exception caught" << endl;
    }
  }

  ~TestRunner() {
    if (fail_count > 0) {
      cerr << fail_count << " unit tests failed. Terminate" << endl;
      exit(1);
    }
  }

private:
  int fail_count = 0;
};


void test_allocate() {
  ResourceManager rm(10);
  rm.set_personal_limits({{"Alex", 5}, {"Wlad", 5}});

  Assert(rm.allocate("Alex").size(), "Allocation Alex 1");
  Assert(rm.allocate("Alex").size(), "Allocation Alex 2");
  Assert(rm.allocate("Alex").size(), "Allocation Alex 3");
  Assert(rm.allocate("Alex").size(), "Allocation Alex 4");
  Assert(rm.allocate("Alex").size(), "Allocation Alex 5");

  Assert(rm.allocate("Alex").size() == 0, "Allocation Alex 6: personal limit");
  cout << rm.last_error() << endl;

  Assert(rm.allocate("Serg").size(), "Allocation Serg 1: no limits");
  cout << rm.last_error() << endl;

  Assert(rm.allocate("Wlad").size(), "Allocation Wlad 1");
  Assert(rm.allocate("Wlad").size(), "Allocation Wlad 2");
  Assert(rm.allocate("Wlad").size(), "Allocation Wlad 3");
  Assert(rm.allocate("Wlad").size(), "Allocation Wlad 4");

  Assert(rm.allocate("Alex").size() == 0, "Allocation Alex: capacity limitation");
  cout << rm.last_error() << endl;

  Assert (rm.list("Alex").size() + rm.list("Wlad").size() == rm.list().size(), "Compare resource count");
}


void test_deallocate() {
  ResourceManager rm(10);
  rm.set_personal_limits({{"Alex", 3}});

  auto id1 = rm.allocate("Alex");
  auto id2 = rm.allocate("Alex");
  auto id3 = rm.allocate("Alex");
  Assert(rm.deallocate(id2), "Deallocation id2");
  auto id4 = rm.allocate("Alex");
  Assert(id4.size(), "Allocation Alex 4");
  Assert(rm.allocate("Alex").size() == 0, "Allocation Alex 5: personal limit");
  cout << rm.last_error() << endl;
  Assert(!rm.deallocate(id2), "Deallocation id2: unknown resource ID");
}


void test_list() {
  ResourceManager rm(10);
  rm.set_personal_limits({{"Alex", 5}, {"Wlad", 5}});

  Assert(rm.allocate("Alex").size(), "Allocation Alex 1");
  Assert(rm.allocate("Alex").size(), "Allocation Alex 2");
  Assert(rm.allocate("Wlad").size(), "Allocation Wlad 1");
  Assert(rm.allocate("Wlad").size(), "Allocation Wlad 2");
  Assert(rm.allocate("Wlad").size(), "Allocation Wlad 3");

  auto list = rm.list();
  cout << list << endl;
}


void test_list_by_name() {
  ResourceManager rm(10);
  rm.set_personal_limits({{"Alex", 5}, {"Wlad", 5}});

  Assert(rm.allocate("Alex").size(), "Allocation Alex 1");
  Assert(rm.allocate("Alex").size(), "Allocation Alex 2");
  Assert(rm.allocate("Wlad").size(), "Allocation Wlad 1");
  Assert(rm.allocate("Wlad").size(), "Allocation Wlad 2");
  Assert(rm.allocate("Wlad").size(), "Allocation Wlad 3");

  auto list = rm.list("Wlad");
  cout << list << endl;
}


int test_rm() {
  TestRunner runner;
  runner.RunTest(test_allocate, "test_allocate");
  runner.RunTest(test_deallocate, "test_deallocate");
  runner.RunTest(test_list, "test_list");
  runner.RunTest(test_list_by_name, "test_list_by_name");
  return 0;
}

