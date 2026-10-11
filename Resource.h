
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
    int timesReserved;
};

enum SortKey {
    SORT_BY_ID,
    SORT_BY_NAME,
    SORT_BY_TYPE,
    SORT_BY_CAPACITY,
    SORT_BY_POPULARITY
};

class ResourceManager {
public:
    int loadFromFile(const std::string& filename);

    bool addResource(int id, const std::string& name,
                     const std::string& type, int capacity);

    void displayAll() const;
    void displayAvailability() const;
    void displayAvailableOnly() const;

    const Resource* findById(int id) const;
    bool exists(int id) const;
    bool isAvailable(int id) const;

    bool markReserved(int id, bool countAsNewReservation = true);
    bool markAvailable(int id);

    int count() const { return (int)resources.size(); }

    void sortResources(SortKey key, bool ascending);

    std::vector<Resource> sortedCopy(SortKey key, bool ascending) const;

    static const char* sortKeyName(SortKey key);

private:
    std::vector<Resource> resources;
    int indexOf(int id) const;
    static std::string trim(const std::string& s);
    static void printTable(const std::vector<Resource>& list);
};

#endif
