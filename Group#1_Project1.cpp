/*
  CSC 275 – Project #1
  Group members: Jhester Buenaventura, Katlyn Winegardner, Trevion Morrison, Ferrisa Boodram

 }
*/

#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>
#include <algorithm>
#include <limits>

// ----- Types to mirror the handout -----
using String = std::string;
using Int    = int;

// ----- App class: as required (public fields, no ctor/mutators) -----
class App {
public:
    String appName;
    Int    appSize;
    // default constructor used implicitly (per spec) 
    // no accessors/mutators (per spec)             
};

// ----- helper utilities -----
static std::string trim_copy(const std::string& s) {
    auto b = s.begin();
    while (b != s.end() && std::isspace(static_cast<unsigned char>(*b))) ++b;
    auto e = s.end();
    do { if (e == b) break; --e; } while (std::isspace(static_cast<unsigned char>(*e)));
    return std::string(b, e == s.end() ? e : e + 1);
}

static std::string lower_copy(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return static_cast<char>(std::tolower(c)); });
    return s;
}

// ----- Smartphone class (per spec) -----
class Smartphone {
private:
    Int capacity;         // how much TOTAL space the phone has (GB)
    String name;          // smartphone name
    App* apps_array;      // pointer to array of Apps (dynamic)
    Int nbAppsInst;       // number of installed apps

    // compute used storage by summing app sizes
    Int used_storage() const {
        Int sum = 0;
        for (Int i = 0; i < nbAppsInst; ++i) sum += apps_array[i].appSize;
        return sum;
    }

    // find index of an app by case-insensitive name (returns -1 if not found)
    Int find_app_idx_ci(const String& appname) const {
        const std::string needle = lower_copy(trim_copy(appname));
        for (Int i = 0; i < nbAppsInst; ++i) {
            if (lower_copy(apps_array[i].appName) == needle) return i;
        }
        return -1;
    }

public:
    // Method #1: constructor with two parameters (capacity, name).
    Smartphone(Int cap, const String& phoneName)
        : capacity(cap), name(phoneName), apps_array(nullptr), nbAppsInst(0) {}

    // Method #2: add_app(appname, appsize)
    void add_app(const String& appname, Int appsize) {
        String nm = trim_copy(appname);
        if (nm.empty() || appsize <= 0) {
            std::cout << "Invalid app data.\n";
            return;
        }

        // reject duplicates
        if (find_app_idx_ci(nm) != -1) {
            std::cout << "App already installed.\n";
            return;
        }

        // reject if not enough capacity
        if (get_available_space() < appsize) {
            std::cout << "Cannot install app, no available space\n";
            return;
        }

        // grow array by one (manual reallocation per spec’s pointer-array requirement)
        App* newArr = new App[nbAppsInst + 1];
        for (Int i = 0; i < nbAppsInst; ++i) newArr[i] = apps_array[i];
        newArr[nbAppsInst].appName = nm;
        newArr[nbAppsInst].appSize = appsize;

        delete[] apps_array;
        apps_array = newArr;
        ++nbAppsInst;
    }

    // Method #3: remove_app(appname)
    void remove_app(const String& appname) {
        Int idx = find_app_idx_ci(appname);
        if (idx == -1) {
            std::cout << "App not installed.\n";
            return;
        }

        String removedName = apps_array[idx].appName;

        // shrink array by one
        App* newArr = (nbAppsInst - 1 > 0) ? new App[nbAppsInst - 1] : nullptr;
        for (Int i = 0, j = 0; i < nbAppsInst; ++i) {
            if (i == idx) continue;
            newArr[j++] = apps_array[i];
        }
        delete[] apps_array;
        apps_array = newArr;
        --nbAppsInst;

        std::cout << "App removed: " << removedName << "\n";
    }

    // Method #4: has_app(appname) -> bool
    bool has_app(const String& appname) const {
        return find_app_idx_ci(appname) != -1;
    }

    // Method #5: get_available_space() -> int
    Int get_available_space() const {
        return capacity - used_storage();
    }

    // Method #6: report() – prints detailed report (per spec) :contentReference[oaicite:6]{index=6}
    void report() const {
        const Int used = used_storage();
        std::cout << "Name: " << name << "\n";
        std::cout << "Capacity: " << used << " out of " << capacity << " GB\n";
        std::cout << "Available space: " << (capacity - used) << "\n";
        std::cout << "Apps installed: " << nbAppsInst << "\n";
        if (nbAppsInst > 0) {
            for (Int i = 0; i < nbAppsInst; ++i) {
                std::cout << "* " << apps_array[i].appName
                          << " is using " << apps_array[i].appSize << " GB\n";
            }
        }
    }
};

// ----- Robust input helpers for a nicer UX -----
static int read_menu_choice() {
    // returns one of: 'r' (report), 'a' (add), 'e' (remove), 'q' (quit)
    while (true) {
        std::cout << "\n(r)eport, (a)dd app, r(e)move app or (q)uit: ";
        std::string s; std::getline(std::cin, s);
        s = lower_copy(trim_copy(s));
        if (s == "r" || s == "a" || s == "e" || s == "q") return s[0];
        std::cout << "Please enter r, a, e, or q.\n";
    }
}

static int read_int_with_prompt(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        int v;
        if (std::cin >> v) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return v;
        }
        std::cout << "Please enter a valid integer.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

static std::string read_line(const std::string& prompt) {
    std::cout << prompt;
    std::string s; std::getline(std::cin, s);
    return trim_copy(s);
}

int main() {
    // Greeting
    std::cout << "Let’s set up your new smartphone.\n";

    // The sample run suggests 32, 64, or 128 GB. We guide the user to these values. :contentReference[oaicite:7]{index=7}
    int cap = 0;
    while (true) {
        cap = read_int_with_prompt("Size of your new smartphone (32, 64 or 128 GB): ");
        if (cap == 32 || cap == 64 || cap == 128) break;
        std::cout << "For this exercise, please choose 32, 64, or 128.\n";
    }

    std::string phoneName = read_line("Smartphone name: ");
    if (phoneName.empty()) phoneName = "My Phone";

    Smartphone phone(cap, phoneName);
    std::cout << "\nSmartphone created!\n\n";
    phone.report();

    // Main command loop  :contentReference[oaicite:8]{index=8}
    while (true) {
        const int choice = read_menu_choice();
        if (choice == 'q') {
            std::cout << "Goodbye!\n";
            break;
        } else if (choice == 'r') {
            phone.report();
        } else if (choice == 'a') {
            std::string appName = read_line("App name to add: ");
            int appSize = read_int_with_prompt("App size in GB: ");
            phone.add_app(appName, appSize);
        } else if (choice == 'e') {
            std::string appName = read_line("App name to remove: ");
            phone.remove_app(appName);
        }
    }

    return 0;
}
