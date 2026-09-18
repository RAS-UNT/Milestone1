#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
#include <vector>

struct Resource {
    int id;             
    std::string name;     
    std::string type;   
    int capacity;       
    bool available;  
};

class ResourceManager {
public:
    int loadFromFile(const std::string& filename);

    bool addResource(int id, const std::string& name,
                     const std::string& type, int capacity);

    // Display
    void displayAll() const;
    void displayAvailability() const;       
    void displayAvailableOnly() const;

    const Resource* findById(int id) const;   
    bool exists(int id) const;
    bool isAvailable(int id) const;           
    bool markReserved(int id);            
    bool markAvailable(int id);           

    int count() const { return (int)resources.size(); }

private:
    std::vector<Resource> resources;
    int indexOf(int id) const;          
    static std::string trim(const std::string& s);
};

#endif
