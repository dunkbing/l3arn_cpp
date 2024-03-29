#include <fstream>
#include <iomanip>
#include "General.h"
#include "Utils.h"

using namespace std;

std::vector<General::CommonInfo> General::_timeZones;  // NOLINT(clang-diagnostic-exit-time-destructors)
std::vector<General::CommonInfo> General::_languages;  // NOLINT(clang-diagnostic-exit-time-destructors)

General::General() {
    _language = "N/A";
    _timeZone = "N/A";
}

General::General(const Setting& s) : Setting(s)
{
    _language = "N/A";
    _timeZone = "N/A";
}

General::~General() = default;

void General::inputInfo(const std::set<std::string>& keys){
    Setting::inputInfo(keys);
    const CommonInfo language = _languages[selectLanguage()];
    const CommonInfo timezone = _timeZones[selectTimeZone()];
    setLanguage(language.name);
    setTimeZone(timezone.name);
}

void General::outputInfo() {
    Setting::outputInfo();
    cout << setw(30) << _timeZone << setw(20) << _language << endl;
}

void General::outputInfoLabel()
{
    cout << setw(20) << "TEN CHU XE" << setw(25) << "Email" << setw(10) << "MSC" << setw(10) << "ODO" << setw(10) << "SERVICES" << setw(30) << "TimeZone" << setw(20) << "Language" << endl;
}

string General::getLanguage() const {
    return _language;
}

string General::getTimeZone() const {
    return _timeZone;
}

void General::setTimeZone(const string& timeZone) {
    this->_timeZone = timeZone;
}

void General::setLanguage(const string& language) {
    this->_language = language;
}

void General::loadTimeZones() {
    const string path = "timezones.txt";
    try
    {
        ifstream f;
        f.open(path, ios::in);

        if (f.is_open())
        {
            string line;
            while (std::getline(f, line)) {
                vector<string> timeZone = Utils::splitString(line, "/");
                CommonInfo common;
                common.name = timeZone[1];
                common.number = timeZone[0];
                _timeZones.push_back(common);
            }
            f.close();
            std::sort(_timeZones.begin(), _timeZones.end(), [](const CommonInfo& c1, const CommonInfo& c2) {
                int c1Num = std::stoi(c1.number.substr(4, 3));
                int c2Num = std::stoi(c2.number.substr(4, 3));
                return c1Num < c2Num;
            });
        }
    }
    catch (exception& e)
    {
        cout << e.what() << endl;
    }
}

void General::loadLanguages() {
    const string path = "languages.txt";
    try {
        ifstream f(path, ios::in);
        if (f.is_open()) {
            string line;
            while (std::getline(f, line)) {
                vector<string> language = Utils::splitString(line, "/");
                CommonInfo common;
                common.name = language[1];
                common.number = language[0];
                _languages.push_back(common);
            }
            f.close();
            std::sort(_languages.begin(), _languages.end(), [](const CommonInfo& c1, const CommonInfo& c2) {
                return c1.name < c2.name;
            });
            for (size_t i = 0; i < _languages.size(); i++) {
                auto number = to_string(i + 1);
                _languages[i].number = number;
            }
        }
    }
    catch (exception& e) {
        cout << e.what() << endl;
    }
}

void General::release()
{
    std::vector<CommonInfo>().swap(_languages);
    std::vector<CommonInfo>().swap(_timeZones);
}

int General::selectTimeZone() {
    cout << "--- Select timezone data ---" << endl;
    for (unsigned int i = 0; i < _timeZones.size(); i++) {
        cout << i + 1 << ": " << _timeZones[i].number << " " << _timeZones[i].name << endl;
    }
    const int size = static_cast<int>(_timeZones.size());
    return Utils::getInt(1, size) - 1;
}

int General::selectLanguage() {
    cout << "--- Select language data ---" << endl;
    for (auto& language : _languages) {
        cout << language.number << ": " << language.name << endl;
    }
    const int size = static_cast<int>(_languages.size());
    return Utils::getInt(1, size) - 1;
}