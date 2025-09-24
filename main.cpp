#include <iostream>
#include <cstdlib>
#include <ctime>
#include <map>
#include <unordered_map>
#include <string>
#include <algorithm>    
#include <vector>

int generateRandom(int min, int max){
    
    int randomNumberInRange = min + (std::rand() % (max - min + 1)); 
    return randomNumberInRange;
}

std::string createKey(std::string plainText, char characters[37]){
    std::string plaintext;
    std::string temporaryKey;

    for (int i=0; i <=plaintext.length()-1; i++){
        int randomNumber = generateRandom(0, 46);
        temporaryKey.push_back(characters[randomNumber]);
    }

    std::cout << "Key: " << temporaryKey << std::endl;

    return temporaryKey;
}

std::string createCipher(std::string temporaryKey, std::unordered_map<char,int> charToIndex,std::string plaintext,char characters[37]){
    std::string cipher;
    for (int i=0; i <=plaintext.length()-1; i++){

        int textVal = charToIndex[plaintext[i]];
        int keyVal = charToIndex[temporaryKey[i]]; 

        int sum = (textVal + keyVal);

        if (sum > 46){
            sum = sum - 47;
        }

        cipher.push_back(characters[sum]);
    }

    std::cout << "Cipher: " << cipher << std::endl;
    return cipher;
}

std::string transposition(std::string key, std::string cipher){
    std::string transposedCipher;
    int keyLength = key.length();
    int rows = (cipher.length() + keyLength - 1) / keyLength; 

    std::vector<std::vector<char>> cipherText(rows, std::vector<char>(keyLength, '_')); 

    int idx = 0;
    for (int c = 0; c < keyLength && idx < cipher.length(); c++){
        for (int r = 0; r < rows && idx < cipher.length(); r++){
            cipherText[r][c] = cipher[idx++];
        }
    }

    for (int r = 0; r < rows; r++){
        for (int c = 0; c < keyLength; c++){
            transposedCipher.push_back(cipherText[r][c]);
        }
    }
    std::cout << "transposedCipher: " << transposedCipher << std::endl;
    return transposedCipher;   
}

std::string shiftKey(std::string key) {
    int len = key.length();
    int half = len / 2;
    std::string newKey = key.substr(half) + key.substr(0, half);
    std::cout << "shifted key: " << newKey << std::endl;
    return newKey;
}

std::string xorCipher(const std::string &plaintext, const std::string &key) {
    std::string cipher;
    cipher.reserve(plaintext.size());

    for (size_t i = 0; i < plaintext.size(); i++) {
        char p = plaintext[i];
        char k = key[i % key.size()]; 
        char c = p ^ k;               
        cipher.push_back(c);
    }
    std::cout << "xor: " << cipher << std::endl;
    return cipher;
}

std::string decryptCipher(std::string temporaryKey, std::unordered_map<char,int> charToIndex, std::string cipher, char characters[47]) {
    std::string plaintext;

    for (int i = 0; i < cipher.length(); i++) {

        int cipherVal = charToIndex[cipher[i]];
        int keyVal = charToIndex[temporaryKey[i]];

        int diff = cipherVal - keyVal;
        if(characters[diff] != '_'){
            
            if (diff < 0) {
                diff += 47;  
            }

            plaintext.push_back(characters[diff]);
        }
    }

    std::cout << "Cipher-PlainText: " << plaintext << std::endl;
    return plaintext;
}

std::string inverseTransposition(std::string key, std::string transposedCipher){
    std::string cipher;
    int keyLength = key.length();
    int rows = (transposedCipher.length() + keyLength - 1) / keyLength; 

    std::vector<std::vector<char>> cipherText(rows, std::vector<char>(keyLength, '_')); 

    int idx = 0;
    for (int r = 0; r < rows && idx < transposedCipher.length(); r++) {
        for (int c = 0; c < keyLength && idx < transposedCipher.length(); c++) {
            cipherText[r][c] = transposedCipher[idx++];
        }
    }

    for (int c = 0; c < keyLength; c++) {
        for (int r = 0; r < rows; r++) {
            //if (cipherText[r][c] != '_') {
                cipher.push_back(cipherText[r][c]);
            //}
        }
    }
    std::cout << "Inverse-transposedCipher: " << cipher << std::endl;
    return cipher;   
}

std::string inverseShiftKey(std::string key) {
    int len = key.length();
    int half = len / 2;
    std::string newKey = key.substr(len - half) + key.substr(0, len - half);
    std::cout << "shifted key: " << newKey << std::endl;
    return newKey;
}

int main (){

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    char characters[47] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
        'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 
        'Y', 'Z', '.', '_', '-', '?', '!', '*', '/', '#', '%', '$',
        '&', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};   
    
    std::unordered_map<char,int> charToIndex;
    for (int i = 0; i < 47; i++) {
        charToIndex[characters[i]] = i;
    }
    
    std::string plaintext;
    std::string key;

    std::cout << "Write the alphanumeric text to be encrypted: ";
    std::cin >> plaintext;
    std::transform(plaintext.begin(), plaintext.end(), plaintext.begin(), ::toupper);

    std::cout << "Write the alphanumeric key for encryption (or '-' to auto-generate): ";
    std::cin >> key;
    std::transform(key.begin(), key.end(), key.begin(), ::toupper);

    if (key == "-") {
        key = createKey(plaintext, characters);
    }

    std::string cipher = createCipher(key, charToIndex, plaintext, characters);
    cipher = transposition(key, cipher);
    key = shiftKey(key);
    cipher = transposition(key, cipher);
    cipher = transposition(key, cipher);
    cipher = xorCipher(cipher, key);

    std::string keyToDecrypt;

    std::cout << "ciphered text to be decrypted: " << cipher << "\n";

    std::cout << "Write the key to decrypt the chiper: ";
    std::cin >> keyToDecrypt;

    keyToDecrypt = shiftKey(keyToDecrypt);
    std::string cipherDetransposotioned = xorCipher(cipher, keyToDecrypt);
    cipherDetransposotioned = inverseTransposition(keyToDecrypt, cipherDetransposotioned);
    cipherDetransposotioned = inverseTransposition(keyToDecrypt, cipherDetransposotioned);
    keyToDecrypt = inverseShiftKey(keyToDecrypt);
    cipherDetransposotioned = inverseTransposition(keyToDecrypt, cipherDetransposotioned);
    std::string decyptedPlaintext = decryptCipher(keyToDecrypt, charToIndex, cipherDetransposotioned, characters);

    return 0;
}
