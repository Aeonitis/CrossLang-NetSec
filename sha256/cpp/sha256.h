#ifndef SHA256_H
#define SHA256_H

#include <vector>
#include <cstdint>
#include <string>

class SHA256 {
public:
    static std::string generateHash(const std::string &message);

private:
    static const uint32_t K[64];
    static uint32_t sigma0(uint32_t num);
    static uint32_t sigma1(uint32_t num);
    static uint32_t capsigma0(uint32_t num);
    static uint32_t capsigma1(uint32_t num);
    static uint32_t ch(uint32_t x, uint32_t y, uint32_t z);
    static uint32_t maj(uint32_t x, uint32_t y, uint32_t z);
    static uint32_t rotateRight(uint32_t num, uint32_t shift, uint32_t size = 32);
};

// Function to return the truncated 8-character hash for a given file
extern std::string shortHashFile(const std::string &file_path);

// Function to return the full hash for a given file
extern std::string hashFile(const std::string &file_path);


#endif
