#include <print>
#include <string>
#include <string_view>

void log(std::string_view s) { 
  std::println("{}", s);
};

int main() {
  std::string s = "string";
  std::string_view sv = s;
  const std::string& cr = s;
  const char* cs = s.c_str();
  
  log(sv);
  log(s);
  log(cr);
  log(cs);
}