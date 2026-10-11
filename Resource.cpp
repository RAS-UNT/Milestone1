#include "Resource.h"
#include "Sorter.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cctype>

std::string ResourceManager::trim(const std::string& s) {
    size_t a = 0, b = s.size();
    while (a < b && std::isspace((unsigned char)s[a])) a++;
    while (b > a && std::isspace((unsigned char)s[b - 1])) b--;
    return s.substr(a, b - a);
}

static bool parseInt(const std::string& s, int& out) {
    if (s.empty()) return false;
    char* end = nullptr;
    long v = std::strtol(s.c_str(), &end, 10);
    if (*end != '\0') return false;
    out = (int)v;
    return true;
}

int ResourceManager::indexOf(int id) const {
    for (size_t i = 0; i < resources.size(); i++)
        if (resources[i].id == id) return (int)i;
    return -1;
}

int ResourceManager::loadFromFile(const std::string& filename) {
    std::ifstream in(filename.c_str());
    if (!in) {
        std::cout << "Error: cannot open resource file '" << filename << "'.\n";
        return -1;
    }
    std::string line;
    int lineNo = 0, loaded = 0;
    while (std::getline(in, line)) {
        lineNo++;
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;

        std::vector<std::string> f;
        std::stringstream ss(line);
        std::string tok;
        while (std::getline(ss, tok, ',')) f.push_back(trim(tok));

        int id, cap;
        if (f.size() != 4 || !parseInt(f[0], id) || !parseInt(f[3], cap)) {
            std::cout << "Warning: skipping malformed line " << lineNo << ": " << line << "\n";
            continue;
        }
        if (addResource(id, f[1], f[2], cap)) loaded++;
        else std::cout << "Warning: skipping invalid/duplicate resource on line " << lineNo << "\n";
    }
    return loaded;
}

bool ResourceManager::addResource(int id, const std::string& name,
                                  const std::string& type, int capacity) {
    if (id <= 0 || name.empty() || type.empty() || capacity <= 0) return false;
    if (indexOf(id) != -1) return false;
    Resource r;
    r.id = id; r.name = name; r.type = type; r.capacity = capacity;
    r.available = true;
    r.timesReserved = 0;
    resources.push_back(r);
    return true;
}

void ResourceManager::printTable(const std::vector<Resource>& list) {
    std::cout << std::left << std::setw(6) << "ID" << std::setw(24) << "Name"
              << std::setw(12) << "Type" << std::setw(10) << "Capacity"
              << std::setw(11) << "Status" << "Times Reserved\n";
    std::cout << std::string(77, '-') << "\n";
    for (size_t i = 0; i < list.size(); i++) {
        const Resource& r = list[i];
        std::cout << std::left << std::setw(6) << r.id << std::setw(24) << r.name
                  << std::setw(12) << r.type << std::setw(10) << r.capacity
                  << std::setw(11) << (r.available ? "Available" : "Reserved")
                  << r.timesReserved << "\n";
    }
}

void ResourceManager::displayAll() const {
    if (resources.empty()) { std::cout << "No resources loaded.\n"; return; }
    std::cout << "\n=== All Resources ===\n";
    printTable(resources);
}

void ResourceManager::displayAvailability() const {
    if (resources.empty()) { std::cout << "No resources loaded.\n"; return; }
    std::cout << "\n=== Resource Availability ===\n";
    for (size_t i = 0; i < resources.size(); i++)
        std::cout << "[" << resources[i].id << "] " << resources[i].name << ": "
                  << (resources[i].available ? "AVAILABLE" : "RESERVED") << "\n";
}

void ResourceManager::displayAvailableOnly() const {
    std::cout << "\n=== Available Resources ===\n";
    bool any = false;
    for (size_t i = 0; i < resources.size(); i++)
        if (resources[i].available) {
            std::cout << "[" << resources[i].id << "] " << resources[i].name
                      << " (" << resources[i].type << ", cap " << resources[i].capacity << ")\n";
            any = true;
        }
    if (!any) std::cout << "None available.\n";
}

const Resource* ResourceManager::findById(int id) const {
    int i = indexOf(id);
    return i == -1 ? nullptr : &resources[i];
}

bool ResourceManager::exists(int id) const {
    return indexOf(id) != -1;
}

bool ResourceManager::isAvailable(int id) const {
    int i = indexOf(id);
    return i != -1 && resources[i].available;
}

bool ResourceManager::markReserved(int id, bool countAsNewReservation) {
    int i = indexOf(id);
    if (i == -1 || !resources[i].available) return false;
    resources[i].available = false;
    if (countAsNewReservation) resources[i].timesReserved++;
    return true;
}

bool ResourceManager::markAvailable(int id) {
    int i = indexOf(id);
    if (i == -1 || resources[i].available) return false;
    resources[i].available = true;
    return true;
}

static std::string toLower(const std::string& s) {
    std::string out(s);
    for (size_t i = 0; i < out.size(); i++)
        out[i] = (char)std::tolower((unsigned char)out[i]);
    return out;
}

static int compareOn(const Resource& a, const Resource& b, SortKey key) {
    switch (key) {
        case SORT_BY_NAME:       return toLower(a.name).compare(toLower(b.name));
        case SORT_BY_TYPE:       return toLower(a.type).compare(toLower(b.type));
        case SORT_BY_CAPACITY:   return a.capacity - b.capacity;
        case SORT_BY_POPULARITY: return a.timesReserved - b.timesReserved;
        case SORT_BY_ID:
        default:                 return a.id - b.id;
    }
}

struct ResourceBefore {
    SortKey key;
    bool ascending;

    ResourceBefore(SortKey k, bool asc) : key(k), ascending(asc) {}

    bool operator()(const Resource& a, const Resource& b) const {
        int c = compareOn(a, b, key);
        if (c != 0) return ascending ? (c < 0) : (c > 0);
        return a.id < b.id;
    }
};

void ResourceManager::sortResources(SortKey key, bool ascending) {
    Sorter::mergeSort(resources, ResourceBefore(key, ascending));
}

std::vector<Resource> ResourceManager::sortedCopy(SortKey key, bool ascending) const {
    std::vector<Resource> copy(resources);
    Sorter::mergeSort(copy, ResourceBefore(key, ascending));
    return copy;
}

const char* ResourceManager::sortKeyName(SortKey key) {
    switch (key) {
        case SORT_BY_NAME:       return "Name";
        case SORT_BY_TYPE:       return "Type";
        case SORT_BY_CAPACITY:   return "Capacity";
        case SORT_BY_POPULARITY: return "Popularity";
        case SORT_BY_ID:
        default:                 return "ID";
    }
}
