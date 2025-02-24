// sha256.cpp
// Implementation of our SHA256 hashing algorithm

#include "sha256.h"
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <stdexcept>


const uint32_t SHA256::K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

std::string hashFile(const std::string& file_path) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Could not open file for reading: " + file_path);
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    std::string content = ss.str();

    SHA256 sha256;
    return sha256.generateHash(content);
}

std::string shortHashFile(const std::string& file_path) {
    std::string full_hash = hashFile(file_path);
    return full_hash.substr(0, 8);  // Return the first 8 characters
}

std::string SHA256::generateHash(const std::string &message) {
    // Padding
    std::vector<uint8_t> msg(message.begin(), message.end());
    uint64_t length = msg.size() * 8;
    msg.push_back(0x80);
    while ((msg.size() * 8 + 64) % 512 != 0) {
        msg.push_back(0x00);
    }
    for (int i = 7; i >= 0; --i) {
        msg.push_back((length >> (i * 8)) & 0xff);
    }

    // Parsing
    std::vector<std::vector<uint8_t> > blocks;
    for (size_t i = 0; i < msg.size(); i += 64) {
        blocks.push_back(std::vector<uint8_t>(msg.begin() + i, msg.begin() + i + 64));
    }

    // Setting Initial Hash Value
    uint32_t h0 = 0x6a09e667;
    uint32_t h1 = 0xbb67ae85;
    uint32_t h2 = 0x3c6ef372;
    uint32_t h3 = 0xa54ff53a;
    uint32_t h4 = 0x510e527f;
    uint32_t h5 = 0x9b05688c;
    uint32_t h6 = 0x1f83d9ab;
    uint32_t h7 = 0x5be0cd19;

    // SHA-256 Hash Computation
    for (const auto &message_block : blocks) {
        std::vector<uint32_t> message_schedule(64);
        for (int t = 0; t < 16; ++t) {
            message_schedule[t] = (message_block[t * 4] << 24) | (message_block[t * 4 + 1] << 16) |
                                  (message_block[t * 4 + 2] << 8) | message_block[t * 4 + 3];
        }
        for (int t = 16; t < 64; ++t) {
            uint32_t term1 = sigma1(message_schedule[t - 2]);
            uint32_t term2 = message_schedule[t - 7];
            uint32_t term3 = sigma0(message_schedule[t - 15]);
            uint32_t term4 = message_schedule[t - 16];
            message_schedule[t] = term1 + term2 + term3 + term4;
        }

        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;
        uint32_t e = h4;
        uint32_t f = h5;
        uint32_t g = h6;
        uint32_t h = h7;

        for (int t = 0; t < 64; ++t) {
            uint32_t t1 = h + capsigma1(e) + ch(e, f, g) + K[t] + message_schedule[t];
            uint32_t t2 = capsigma0(a) + maj(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + t1;
            d = c;
            c = b;
            b = a;
            a = t1 + t2;
        }

        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
        h5 += f;
        h6 += g;
        h7 += h;
    }

    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    ss << std::setw(8) << h0 << std::setw(8) << h1 << std::setw(8) << h2 << std::setw(8) << h3;
    ss << std::setw(8) << h4 << std::setw(8) << h5 << std::setw(8) << h6 << std::setw(8) << h7;

    return ss.str();
}

uint32_t SHA256::sigma0(uint32_t num) {
    return rotateRight(num, 7) ^ rotateRight(num, 18) ^ (num >> 3);
}

uint32_t SHA256::sigma1(uint32_t num) {
    return rotateRight(num, 17) ^ rotateRight(num, 19) ^ (num >> 10);
}

uint32_t SHA256::capsigma0(uint32_t num) {
    return rotateRight(num, 2) ^ rotateRight(num, 13) ^ rotateRight(num, 22);
}

uint32_t SHA256::capsigma1(uint32_t num) {
    return rotateRight(num, 6) ^ rotateRight(num, 11) ^ rotateRight(num, 25);
}

uint32_t SHA256::ch(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ (~x & z);
}

uint32_t SHA256::maj(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ (x & z) ^ (y & z);
}

uint32_t SHA256::rotateRight(uint32_t num, uint32_t shift, uint32_t size) {
    return (num >> shift) | (num << (size - shift));
}
