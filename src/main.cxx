// main.cxx - Minimal Professional CRUD Contact Manager
// Compile: g++ -std=c++17 -O2 -o crud_app.exe main.cxx

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cstdint>

// ─── Model ────────────────────────────────────────────────────────────────────

struct Contact {
    uint32_t    id;
    std::string name;
    std::string email;
    std::string phone;
};

// ─── Repository ───────────────────────────────────────────────────────────────

class ContactRepository {
public:
    explicit ContactRepository(const std::string& path) : path_(path) {
        load();
    }

    Contact& create(const std::string& name, const std::string& email, const std::string& phone) {
        Contact c;
        c.id    = nextId_++;
        c.name  = name;
        c.email = email;
        c.phone = phone;
        contacts_.push_back(c);
        save();
        return contacts_.back();
    }

    const std::vector<Contact>& all() const {
        return contacts_;
    }

    Contact* find(uint32_t id) {
        for (size_t i = 0; i < contacts_.size(); ++i)
            if (contacts_[i].id == id) return &contacts_[i];
        return NULL;
    }

    bool update(uint32_t id, const std::string& name, const std::string& email, const std::string& phone) {
        Contact* c = find(id);
        if (!c) return false;
        c->name  = name;
        c->email = email;
        c->phone = phone;
        save();
        return true;
    }

    bool remove(uint32_t id) {
        for (std::vector<Contact>::iterator it = contacts_.begin(); it != contacts_.end(); ++it) {
            if (it->id == id) {
                contacts_.erase(it);
                save();
                return true;
            }
        }
        return false;
    }

private:
    std::string          path_;
    std::vector<Contact> contacts_;
    uint32_t             nextId_;

    void load() {
        nextId_ = 1;
        std::ifstream f(path_.c_str());
        if (!f) return;
        std::string line;
        while (std::getline(f, line)) {
            if (line.empty()) continue;
            std::istringstream ss(line);
            Contact c;
            std::string id;
            std::getline(ss, id,     ','); c.id = (uint32_t)std::stoul(id);
            std::getline(ss, c.name, ',');
            std::getline(ss, c.email,',');
            std::getline(ss, c.phone,',');
            contacts_.push_back(c);
            if (c.id >= nextId_) nextId_ = c.id + 1;
        }
    }

    void save() const {
        std::ofstream f(path_.c_str(), std::ios::trunc);
        for (size_t i = 0; i < contacts_.size(); ++i)
            f << contacts_[i].id    << ','
              << contacts_[i].name  << ','
              << contacts_[i].email << ','
              << contacts_[i].phone << '\n';
    }
};

// ─── UI Helpers ───────────────────────────────────────────────────────────────

namespace ui {

const int W = 60;

void line(char ch = '-') {
    std::cout << std::string(W, ch) << '\n';
}

void header(const std::string& title) {
    line('=');
    int pad = (W - (int)title.size()) / 2;
    std::cout << std::string(pad, ' ') << title << '\n';
    line('=');
}

void table(const std::vector<Contact>& contacts) {
    if (contacts.empty()) {
        std::cout << "  (no records)\n";
        return;
    }
    std::cout << std::left
              << std::setw(5)  << "ID"
              << std::setw(18) << "Name"
              << std::setw(22) << "Email"
              << std::setw(15) << "Phone" << '\n';
    line();
    for (size_t i = 0; i < contacts.size(); ++i) {
        std::cout << std::left
                  << std::setw(5)  << contacts[i].id
                  << std::setw(18) << contacts[i].name
                  << std::setw(22) << contacts[i].email
                  << std::setw(15) << contacts[i].phone << '\n';
    }
}

std::string prompt(const std::string& label) {
    std::cout << "  " << label << ": ";
    std::string val;
    std::getline(std::cin, val);
    return val;
}

uint32_t promptId(const std::string& label) {
    std::string raw = prompt(label);
    try { return (uint32_t)std::stoul(raw); }
    catch (...) { return 0; }
}

void ok(const std::string& msg)  { std::cout << "  [OK] " << msg << '\n'; }
void err(const std::string& msg) { std::cout << "  [!!] " << msg << '\n'; }

} // namespace ui

// ─── Application ──────────────────────────────────────────────────────────────

class App {
public:
    App() : repo_("contacts.csv") {}

    void run() {
        while (true) {
            ui::header("CONTACT MANAGER");
            std::cout << "  1. List all\n"
                      << "  2. Create\n"
                      << "  3. Update\n"
                      << "  4. Delete\n"
                      << "  5. Find by ID\n"
                      << "  0. Exit\n";
            ui::line();

            std::string choice = ui::prompt("Choice");

            if      (choice == "1") doList();
            else if (choice == "2") doCreate();
            else if (choice == "3") doUpdate();
            else if (choice == "4") doDelete();
            else if (choice == "5") doFind();
            else if (choice == "0") { std::cout << "\n  Goodbye.\n"; break; }
            else    ui::err("Invalid option.");

            std::cout << '\n';
        }
    }

private:
    ContactRepository repo_;

    void doList() {
        ui::line();
        ui::table(repo_.all());
    }

    void doCreate() {
        ui::line();
        std::string name  = ui::prompt("Name");
        std::string email = ui::prompt("Email");
        std::string phone = ui::prompt("Phone");
        if (name.empty()) { ui::err("Name is required."); return; }
        const Contact& c = repo_.create(name, email, phone);
        ui::ok("Created contact #" + std::to_string(c.id));
    }

    void doUpdate() {
        ui::line();
        uint32_t id = ui::promptId("ID to update");
        if (!id) { ui::err("Invalid ID."); return; }
        Contact* c = repo_.find(id);
        if (!c) { ui::err("Contact not found."); return; }
        std::cout << "  Leave blank to keep current value.\n";
        std::string name  = ui::prompt("Name  [" + c->name  + "]");
        std::string email = ui::prompt("Email [" + c->email + "]");
        std::string phone = ui::prompt("Phone [" + c->phone + "]");
        if (name.empty())  name  = c->name;
        if (email.empty()) email = c->email;
        if (phone.empty()) phone = c->phone;
        repo_.update(id, name, email, phone);
        ui::ok("Updated contact #" + std::to_string(id));
    }

    void doDelete() {
        ui::line();
        uint32_t id = ui::promptId("ID to delete");
        if (!id) { ui::err("Invalid ID."); return; }
        if (repo_.remove(id)) ui::ok("Deleted contact #" + std::to_string(id));
        else                  ui::err("Contact not found.");
    }

    void doFind() {
        ui::line();
        uint32_t id = ui::promptId("ID to find");
        if (!id) { ui::err("Invalid ID."); return; }
        Contact* c = repo_.find(id);
        if (!c) { ui::err("Contact not found."); return; }
        std::cout << "  ID   : " << c->id    << '\n'
                  << "  Name : " << c->name  << '\n'
                  << "  Email: " << c->email << '\n'
                  << "  Phone: " << c->phone << '\n';
    }
};

// ─── Entry Point ──────────────────────────────────────────────────────────────

int main() {
    App{}.run();
    return 0;
}