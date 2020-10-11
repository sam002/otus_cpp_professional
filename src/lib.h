#pragma once
#include <cstdint>
#include <string>
#include <vector>

//Optimization enable, 30~40% faster
#define IP_AS_UINT32

class IpAddr
{
private:
#ifdef IP_AS_UINT32
    uint32_t addr = 0;
    void  setByte(uint position, u_int8_t value);
    uint8_t getByte(uint n) const;
#else
    struct bytes {
        uint8_t byte0;
        uint8_t byte1;
        uint8_t byte2;
        uint8_t byte3;
    } ipBytes;
#endif
public:
    explicit IpAddr(std::string ipOrig);
    friend bool operator>(const IpAddr &a, const IpAddr &b);
    std::string to_string() const;
    bool filter(u_int8_t byte0, u_int8_t ibyte1, u_int8_t ibyte2, u_int8_t ibyte3) const;
    bool filter(u_int8_t byte0, u_int8_t ibyte1, u_int8_t ibyte2) const;
    bool filter(u_int8_t byte0, u_int8_t ibyte1) const;
    bool filter(u_int8_t byte0) const;
    bool filter_any(u_int8_t byte0) const;
};

std::vector<std::string> split(const std::string &str, char d);