#include <algorithm>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

class Passport
{
  std::optional<std::string> byr;
  std::optional<std::string> iyr;
  std::optional<std::string> eyr;
  std::optional<std::string> hgt;
  std::optional<std::string> hcl;
  std::optional<std::string> ecl;
  std::optional<std::string> pid;
  std::optional<std::string> cid;

public:
  std::optional<std::string> get_value(std::string key) const;
  bool                       is_valid(void) const;
  void                       set_value(std::string key, std::string value);
};

int
main(void)
{
  std::string           line;
  std::vector<Passport> passports{ {} };
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      passports.emplace_back();
    } else {
      std::stringstream field_stream(line);
      std::string       field;
      while (std::getline(field_stream, field, ' ')) {
        size_t sep_idx = field.find(':');

        std::string key   = field.substr(0, sep_idx);
        std::string value = field.substr(sep_idx + 1);

        passports.back().set_value(key, value);
      }
    }
  }

  size_t valid_count =
    std::count_if(passports.begin(), passports.end(), [](const Passport& p) {
      return p.is_valid();
    });

  std::cout << valid_count << std::endl;

  return EXIT_SUCCESS;
}

std::optional<std::string>
Passport::get_value(std::string key) const
{
  // clang-format off
  if (key == "byr") { return this->byr; };
  if (key == "iyr") { return this->iyr; };
  if (key == "eyr") { return this->eyr; };
  if (key == "hgt") { return this->hgt; };
  if (key == "hcl") { return this->hcl; };
  if (key == "ecl") { return this->ecl; };
  if (key == "pid") { return this->pid; };
  if (key == "cid") { return this->cid; };
  // clang-format on

  std::cerr << "what the fuck" << std::endl;
  return {};
}

void
Passport::set_value(std::string key, std::string value)
{
  // clang-format off
  if (key == "byr") { this->byr = value; return; };
  if (key == "iyr") { this->iyr = value; return; };
  if (key == "eyr") { this->eyr = value; return; };
  if (key == "hgt") { this->hgt = value; return; };
  if (key == "hcl") { this->hcl = value; return; };
  if (key == "ecl") { this->ecl = value; return; };
  if (key == "pid") { this->pid = value; return; };
  if (key == "cid") { this->cid = value; return; };
  // clang-format on

  std::cerr << "what the fuck (" << key << ")" << std::endl;
}

bool
Passport::is_valid(void) const
{
  // clang-format off
  if (!this->byr) { return false; };
  if (!this->iyr) { return false; };
  if (!this->eyr) { return false; };
  if (!this->hgt) { return false; };
  if (!this->hcl) { return false; };
  if (!this->ecl) { return false; };
  if (!this->pid) { return false; };
  // clang-format on

  return true;
}
