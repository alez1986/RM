## Resource Manager

Resource Manager (RM) allocates resources by user and deallocates them by resource name

### Usage

First, create an object and set capacity

``` cpp
// in constructor
ResourceManager rm(100);

// in method
ResourceManager rm_new;
rm_new.set_capacity(100);
```

Set limits for users. For users without limit any amount of resources could be allocated

``` cpp
// set for all users. Previous configuration would be lost
rm.set_personal_limits({{"Alex", 5}, {"Wlad", 5}});

// set individually or add a new limit
void set_personal_limit("Serg", 7);
```

Allocate new resource

``` cpp
std::string resource_id = rm.allocate("Alex");
```

Deallocate resource

``` cpp
rm.deallocate(resource_id);
```

Get resource for concrete user

``` cpp
rm.list("Alex");
```

Get all resources

``` cpp
rm.list();
```

## Build
Cmake
