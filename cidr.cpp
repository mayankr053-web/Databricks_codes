#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <cstdint>
#include <iomanip>

//
// ---------- Utility Functions ----------
//

// Convert "192.168.1.10" → 32-bit unsigned integer
uint32_t ipToInt(const std::string& ip) {
    std::stringstream ss(ip);
    std::string segment;
    uint32_t result = 0;

    for (int i = 0; i < 4; ++i) {
        if (!std::getline(ss, segment, '.'))
            throw std::invalid_argument("Invalid IP format: " + ip);
        int octet = std::stoi(segment);
        if (octet < 0 || octet > 255)
            throw std::invalid_argument("Invalid IP octet: " + ip);
        result = (result << 8) | static_cast<uint32_t>(octet);
    }
    return result;
}

// Convert back (for debugging)
std::string intToIp(uint32_t val) {
    std::ostringstream os;
    for (int i = 3; i >= 0; --i) {
        os << ((val >> (i * 8)) & 0xFF);
        if (i > 0) os << '.';
    }
    return os.str();
}

//
// ---------- CIDRBlock ----------
//
class CIDRBlock {
    uint32_t networkAddress;
    uint32_t mask;
    int prefixLength;

public:
    CIDRBlock(const std::string& cidr) {
        size_t slashPos = cidr.find('/');
        if (slashPos == std::string::npos)
            throw std::invalid_argument("Invalid CIDR: " + cidr);

        std::string ip = cidr.substr(0, slashPos);
        prefixLength = std::stoi(cidr.substr(slashPos + 1));
        if (prefixLength < 0 || prefixLength > 32)
            throw std::invalid_argument("Invalid prefix length in: " + cidr);

        networkAddress = ipToInt(ip);
        mask = prefixLength == 0 ? 0 : 0xFFFFFFFF << (32 - prefixLength);
    }

    bool contains(const std::string& ip) const {
        uint32_t ipInt = ipToInt(ip);
        return (networkAddress & mask) == (ipInt & mask);
    }

    void debug() const {
        std::cout << "Network: " << intToIp(networkAddress)
                  << " Mask: " << intToIp(mask)
                  << " Prefix: /" << prefixLength << "\n";
    }
};

//
// ---------- Firewall ----------
//
class Firewall {
    struct Rule {
        CIDRBlock block;
        std::string mode; // "ALLOW" or "DENY"
        Rule(const std::string& cidr, const std::string& m)
            : block(cidr), mode(m) {}
    };

    std::vector<Rule> rules;

public:
    void addRule(const std::string& cidr, const std::string& mode) {
        rules.emplace_back(cidr, mode);
    }

    bool allow(const std::string& ip) const {
        for (const auto& rule : rules) {
            if (rule.block.contains(ip)) {
                return rule.mode == "ALLOW";
            }
        }
        return false; // default deny
    }
};

//
// ---------- Example Usage ----------
//
int main() {
    try {
        Firewall fw;
        fw.addRule("192.168.1.0/24", "ALLOW");
        fw.addRule("10.0.0.0/8", "DENY");

        std::cout << std::boolalpha;
        std::cout << "192.168.1.50 → " << fw.allow("192.168.1.50") << "\n"; // true
        std::cout << "10.1.2.3 → " << fw.allow("10.1.2.3") << "\n";         // false
        std::cout << "8.8.8.8 → " << fw.allow("8.8.8.8") << "\n";           // false
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
    }
}
