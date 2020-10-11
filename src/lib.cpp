#include <tuple>
#include "lib.h"

#include <cstdlib>

IpAddr::IpAddr(std::string ipOrig) {
    auto ipSplited = split(ipOrig, '.');
    if(ipSplited.size() != 4){
        throw "Invalid ip, should be need 4 bytes \"" + ipOrig +"\"";
    }

    auto a0 = atoi(ipSplited[0].c_str());
    auto a1 = atoi(ipSplited[1].c_str());
    auto a2 = atoi(ipSplited[2].c_str());
    auto a3 = atoi(ipSplited[3].c_str());
    if(a0 > 255 || a1 > 255 || a2 > 255 || a3 > 255
       ||  a0 < 0 || a1 < 0 || a2 < 0 || a3 < 0 ) {
        throw "Invalid ip, each byte should be in range [0;255] \"" + ipOrig +"\"";
    }
#ifdef IP_AS_UINT32
    this->setByte(0, a0);
    this->setByte(1, a1);
    this->setByte(2, a2);
    this->setByte(3, a3);
#else
    this->ipBytes = bytes{
            (uint8_t)a0, (uint8_t)a1, (uint8_t)a2, (uint8_t)a3
    };
#endif
}

    bool operator>(const IpAddr &a, const IpAddr &b) {
#ifdef IP_AS_UINT32
        return a.addr > b.addr;
#else
        return std::tie(a.ipBytes.byte0, a.ipBytes.byte1, a.ipBytes.byte2, a.ipBytes.byte3) > std::tie(b.ipBytes.byte0, b.ipBytes.byte1, b.ipBytes.byte2, b.ipBytes.byte3);
#endif
    }

    std::string IpAddr::to_string() const {
        std::string res = "";

#ifdef IP_AS_UINT32
        res += std::to_string(this->getByte(0));
        res += '.';
        res += std::to_string(this->getByte(1));
        res += '.';
        res += std::to_string(this->getByte(2));
        res += '.';
        res += std::to_string(this->getByte(3));
#else
        res = std::to_string(this->ipBytes.byte0) + "."
              + std::to_string(this->ipBytes.byte1) + "."
              + std::to_string(this->ipBytes.byte2) + "."
              + std::to_string(this->ipBytes.byte3);
#endif
        return res;
    }

#ifdef IP_AS_UINT32
void IpAddr::setByte(const uint n, const u_int8_t value)
{
    if (n > 3) {
        throw "Ip have only 4 bytes, setted: " + std::to_string(n);
    }

    //Cat tail by shitt to left; Then get only last 8 bit (by binary and)
    this->addr += ((uint32_t)value) <<8*(3-n);
}

uint8_t IpAddr::getByte(const uint n) const
{
    if (n > 3) {
        throw "Ip have only 4 bytes, asked: " + std::to_string(n);
    }

    //Cat tail by shitt to left; Then get only last 8 bit (by binary and)
    return (this->addr >> (3-n)*8) bitand 255UL;
}
#endif

bool IpAddr::filter(u_int8_t byte0, u_int8_t byte1, u_int8_t byte2, u_int8_t byte3) const {
#ifdef IP_AS_UINT32
    return byte0 == this->getByte(0)
           && byte1 == this->getByte(1)
           && byte2 == this->getByte(2)
           && byte3 == this->getByte(3);
#else
    return byte0 == ipBytes.byte0
           && byte1 == ipBytes.byte1
           && byte2 == ipBytes.byte2
           && byte3 == ipBytes.byte3;
#endif
}

bool IpAddr::filter(u_int8_t byte0, u_int8_t byte1, u_int8_t byte2) const {
#ifdef IP_AS_UINT32
    return byte0 == this->getByte(0)
           && byte1 == this->getByte(1)
           && byte2 == this->getByte(2);
#else
    return byte0 == ipBytes.byte0
           && byte1 == ipBytes.byte1
           && byte2 == ipBytes.byte2;
#endif
}

bool IpAddr::filter(u_int8_t byte0, u_int8_t byte1) const {
#ifdef IP_AS_UINT32
    return byte0 == this->getByte(0)
           && byte1 == this->getByte(1);
#else
    return byte0 == ipBytes.byte0
           && byte1 == ipBytes.byte1;
#endif
}

bool IpAddr::filter(u_int8_t byte0) const {
#ifdef IP_AS_UINT32
    return byte0 == this->getByte(0);
#else
    return byte0 == ipBytes.byte0;
#endif
}

bool IpAddr::filter_any(u_int8_t byte) const {
#ifdef IP_AS_UINT32
    return byte == this->getByte(0)
           || byte == this->getByte(1)
           || byte == this->getByte(2)
           || byte == this->getByte(3);
#else
    return byte == this->ipBytes.byte0
           || byte == this->ipBytes.byte1
           || byte == this->ipBytes.byte2
           || byte == this->ipBytes.byte3;
#endif
}

std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}
