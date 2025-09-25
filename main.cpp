#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <vector>
#include <fstream>
#include <cctype>

bool isValidAlphanumeric(const std::string &input) {
    for (char c : input)
        if (!std::isalnum(static_cast<unsigned char>(c))) return false;
    return true;
}

bool validatePlaintext(const std::string &text) {
    if (text.length() > 100) {
        std::cerr << "Text exceeds 100 characters.\n";
        return false;
    }
    if (!isValidAlphanumeric(text)) {
        std::cerr << "Text must be alphanumeric only (A-Z, a-z, 0-9).\n";
        return false;
    }
    return true;
}

bool validateKey(const std::string &key) {
    if (key == "-") return true;
    if (key.length() < 4 || key.length() > 16) {
        std::cerr << "The key must be between 4 and 16 characters.\n";
        return false;
    }
    if (!isValidAlphanumeric(key)) {
        std::cerr << "The key must be alphanumeric only.\n";
        return false;
    }
    return true;
}

int generateRandom(int min, int max){
    return min + (std::rand() % (max - min + 1));
}

std::string createKey(std::string plainText, char characters[47]){
    std::string temporaryKey;
    int maxLength = std::min(16, (int)plainText.length());
    for (int i=0; i < maxLength; i++){
        temporaryKey.push_back(characters[generateRandom(0,46)]);
    }
    std::cout << "Generated key: " << temporaryKey << std::endl;
    return temporaryKey;
}

std::string createCipher(std::string key, std::unordered_map<char,int> charToIndex, std::string plaintext, char characters[47]){
    while (key.length() < plaintext.length()) key += key;
    key = key.substr(0, plaintext.length());

    std::string cipher;
    for (size_t i = 0; i < plaintext.length(); i++){
        int sum = charToIndex[plaintext[i]] + charToIndex[key[i]];
        if (sum > 46) sum -= 47;
        cipher.push_back(characters[sum]);
    }
    std::cout << "Cipher: " << cipher << std::endl;
    return cipher;
}

std::string transposition(const std::string &key, const std::string &cipher, int index) {
    std::string transposedCipher;
    int keyLength = (index == 1) ? key.length() / 2 : key.length();
    if (keyLength == 0) keyLength = 1;

    int rows = (cipher.length() + keyLength - 1) / keyLength;

    std::vector<std::vector<char>> matrix(rows, std::vector<char>(keyLength, ':')); 

    int idx = 0;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < keyLength; c++) {
            if (idx < cipher.size())
                matrix[r][c] = cipher[idx++];
        }
    }

    for (int c = 0; c < keyLength; c++)
        for (int r = 0; r < rows; r++)
            transposedCipher.push_back(matrix[r][c]);

    std::cout << "transposedCipher: " << transposedCipher << std::endl;
    return transposedCipher;
}

std::string shiftKey(std::string key) {
    int len = key.length();
    int half = len/2;
    std::string newKey = key.substr(half) + key.substr(0,half);
    std::cout << "shifted key: " << newKey << std::endl;
    return newKey;
}

std::string xorCipher(const std::string &text, std::string key) {
    while(key.length() < text.length()) key += key;
    key = key.substr(0,text.length());

    std::string cipher;
    cipher.resize(text.size());
    for(size_t i=0; i<text.size(); i++) {
        unsigned char a = static_cast<unsigned char>(text[i]);
        unsigned char b = static_cast<unsigned char>(key[i]);
        unsigned char x = a ^ b;
        cipher[i] = static_cast<char>(x);
    }

    std::cout << "xor: " << cipher << std::endl;
    return cipher;
}

std::string decryptCipher(std::string key, std::unordered_map<char,int> charToIndex, std::string cipher, char characters[47]) {
    while(key.length() < cipher.length()) key += key;
    key = key.substr(0,cipher.length());

    std::string plaintext;
    for(size_t i=0; i<cipher.size(); i++){
        int diff = charToIndex[cipher[i]] - charToIndex[key[i]];
        if(diff<0) diff+=47;
        plaintext.push_back(characters[diff]);
    }
    std::cout << "Cipher-PlainText: " << plaintext << std::endl;
    return plaintext;
}

std::string inverseTransposition(const std::string &key, const std::string &transposedCipher, int index){
    std::string cipher;
    int keyLength = (index==1)? key.length()/2 : key.length();
    if(keyLength==0) keyLength=1;

    int rows = (transposedCipher.length() + keyLength - 1) / keyLength;

    int fullCols = transposedCipher.length() % keyLength;
    if (fullCols == 0) fullCols = keyLength;

    std::vector<int> colHeights(keyLength, rows);
    for (int c = fullCols; c < keyLength; c++) {
        colHeights[c] = rows - 1;
    }

    std::vector<std::vector<char>> matrix(rows, std::vector<char>(keyLength, ':'));

    int idx = 0;
    for (int c = 0; c < keyLength; c++) {
        for (int r = 0; r < colHeights[c]; r++) {
            if (idx < transposedCipher.size())
                matrix[r][c] = transposedCipher[idx++];
        }
    }

    for (int r = 0; r < rows; r++)
        for (int c = 0; c < keyLength; c++)
            cipher.push_back(matrix[r][c]);

    while(!cipher.empty() && cipher.back() == ':')
        cipher.pop_back();

    std::cout << "Inverse-transposedCipher: " << cipher << std::endl;
    return cipher;
}

int main(){
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    char characters[47] = {
        'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X',
        'Y','Z','.','_','-','?','!','*','/','#','%','$','&','0','1','2','3','4','5','6','7','8','9'
    };

    std::unordered_map<char,int> charToIndex;
    for(int i=0;i<47;i++) charToIndex[characters[i]]=i;

    std::string plaintext, key;
    std::cout << "Write the alphanumeric text to be encrypted: ";
    std::cin >> plaintext;
    std::transform(plaintext.begin(), plaintext.end(), plaintext.begin(), ::toupper);

    if(!validatePlaintext(plaintext)) return 1;

    std::cout << "Write the alphanumeric key for encryption (or '-' to auto-generate): ";
    std::cin >> key;
    std::transform(key.begin(), key.end(), key.begin(), ::toupper);

    if(!validateKey(key)) return 1;

    if(key=="-") key=createKey(plaintext,characters);

    key = shiftKey(key);
    std::string cipher = createCipher(key,charToIndex,plaintext,characters);
    cipher = transposition(key,cipher,0);
    cipher = transposition(key,cipher,0);
    cipher = transposition(key,cipher,1);
    cipher = xorCipher(cipher,key);

    std::string loadedCipher = cipher;
    std::string keyToDecrypt;
    std::cout << "ciphered text to be decrypted: " << loadedCipher << "\n";
    std::cout << "Write the key to decrypt the chiper: ";
    std::cin >> keyToDecrypt;
    if(!validateKey(keyToDecrypt)) return 1;

    keyToDecrypt = shiftKey(keyToDecrypt);
    std::string detrans = xorCipher(loadedCipher,keyToDecrypt);
    detrans = inverseTransposition(keyToDecrypt,detrans,1);
    detrans = inverseTransposition(keyToDecrypt,detrans,0);
    detrans = inverseTransposition(keyToDecrypt,detrans,0);
    std::string decryptedPlaintext = decryptCipher(keyToDecrypt,charToIndex,detrans,characters);

    return 0;
}
