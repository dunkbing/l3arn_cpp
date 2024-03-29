#include "Setting.h"
#include "Utils.h"
#include <iomanip>

using namespace std;

Setting::Setting() {
    _odo = 0;
    _serviceRemind = 0;
}

Setting::Setting(const Setting& s) {
    _carName = s.getCarName();
    _personalKey = s.getPersonalKey();
    _email = s.getEmail();
    _odo = s.getOdo();
    _serviceRemind = s.getServiceRemind();
}

Setting::~Setting() = default;

string Setting::getCarName() const {
    return _carName;
}

string Setting::getPersonalKey() const {
    return _personalKey;
}

int Setting::getOdo () const {
    return _odo;
}

int Setting::getServiceRemind() const {
    return _serviceRemind;
}

void Setting::setCarName(const string& name) {
    this->_carName = name;
}

void Setting::setPersonalKey(const string key) {
    this->_personalKey = key;
}

void Setting::setOdo(int odo) {
    this->_odo = odo;
}

void Setting::setServiceRemind(const int service) {
    this->_serviceRemind = service;
}

string Setting::getEmail() const {
    return this->_email;
}

void Setting::setEmail(const string& email) {
    this->_email = email;
}

void Setting::inputInfo(const std::set<std::string>& keys)
{
    const string emailReg = R"((\w{3,})(\.|_)?(\w*)@(\w{3,})(\.(\w{3,}))+)";
    setCarName(Utils::getLine("Owner name: "));
    setEmail(Utils::getLine("Email: ", emailReg, "Email must be a string in format abc@xyz.def: "));
    setOdo(Utils::getInt(1, INT_MAX, "Odo: "));
    setServiceRemind(Utils::getInt(1, 10000, "Service remind: "));
    const string key = Utils::getLine("Personal key: ", "^[0-9]{8}$", "Personal key must be 8 digits: ");
    if (keys.find(key) != keys.end()) {
        cout << "    -> This car already existed, data will be overwritten" << endl;
    }
    else {
        cout << "    -> This car is new, data will be appended to your list" << endl;
    }
    setPersonalKey(key);
}

void Setting::outputInfo(){
    cout << setw(20) << _carName << setw(25) << _email << setw(10) << _personalKey << setw(10) << _odo << setw(10) << _serviceRemind;
}

bool operator==(const Setting& s1, const Setting& s2)
{
    return s1.getPersonalKey() == s2.getPersonalKey();
}

bool operator<(const Setting& s1, const Setting& s2)
{
    return s1.getPersonalKey() < s2.getPersonalKey();
}

bool operator>(const Setting& s1, const Setting& s2)
{
    return s1.getPersonalKey() > s2.getPersonalKey();
}
