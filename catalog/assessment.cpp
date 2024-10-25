#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;

// Function to compute modular inverse (Fermat's Little Theorem)
int modInverse(int a, int p = 1000000007) {
    int result = 1, exponent = p - 2; // p is prime
    while (exponent) {
        if (exponent % 2) result = (result * a) % p;
        a = (a * a) % p;
        exponent /= 2;
    }
    return result;
}

// Function to decode a number from a given base
int decodeValue(const std::string& baseStr, const std::string& valueStr) {
    int base = std::stoi(baseStr);
    int value = 0;
    
    // Decode the value from the specified base
    for (char digit : valueStr) {
        value = value * base + (digit - '0');
    }
    return value;
}

// Function to calculate the constant term (c) using Lagrange interpolation
int calculateConstantTerm(const std::vector<int>& x, const std::vector<int>& y) {
    int n = x.size();
    int secret = 0;

    for (int i = 0; i < n; ++i) {
        int li = 1; // Lagrange basis polynomial
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                li *= (0 - x[j]) * modInverse(x[i] - x[j]);
            }
        }
        secret += li * y[i];
    }

    return secret;
}

int main() {
    // Read JSON input from a file
    std::ifstream inputFile("input.json");
    json inputData;
    inputFile >> inputData;

    // Get n and k values
    int n = inputData["keys"]["n"];
    int k = inputData["keys"]["k"];

    std::vector<int> x(n);
    std::vector<int> y(n);

    // Decode the Y values
    for (int i = 1; i <= n; ++i) {
        x[i - 1] = i; // Using keys as x values (1 to n)
        y[i - 1] = decodeValue(inputData[std::to_string(i)]["base"], inputData[std::to_string(i)]["value"]);
    }

    // Calculate the constant term c
    int constantTerm = calculateConstantTerm(x, y);

    // Output the constant term
    std::cout << "The constant term (c) is: " << constantTerm << std::endl;

    return 0;
}
