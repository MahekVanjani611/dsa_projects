#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <fstream>

using namespace std;

class PhoneRecord {
private:
    string name;
    string organisation;
    vector<string> phoneNumbers;

public:
    PhoneRecord(const string& n, const string& org, const vector<string>& numbers)
        : name(n), organisation(org), phoneNumbers(numbers) {}

    string getName() const {
        return name;
    }

    string getOrganisation() const {
        return organisation;
    }

    vector<string> getPhoneNumbers() const {
        return phoneNumbers;
    }
};

class HashTableRecord {
private:
    int key;
    PhoneRecord* element;
    HashTableRecord* next;

public:
    HashTableRecord(int k, PhoneRecord* rec)
        : key(k), element(rec), next(nullptr) {}

    int getKey() const {
        return key;
    }

    PhoneRecord* getRecord() const {
        return element;
    }

    HashTableRecord* getNext() const {
        return next;
    }

    void setNext(HashTableRecord* nxt) {
        next = nxt;
    }
};

class PhoneBook {
private:
    static const int HASH_TABLE_SIZE = 263;
    unordered_map<string, vector<HashTableRecord*>> hashTable;
    
    long long computeHash(const string& str) {
        const long long p = 1000000007;
        const long long x = 263;
        long long hashValue = 0;

        for (int i = static_cast<int>(str.size()) - 1; i >= 0; --i) {
            hashValue = (hashValue * x + str[i]) % p;
        }

        return hashValue;
    }

    int getKey(const string& str, int index) {
        const long long p = 1000000007;
        const long long x = 263;
        long long key = 0;
        for (char c : str) {
            key = (key * x + c) % p;
        }
        return static_cast<int>((key + index * index) % HASH_TABLE_SIZE);
    }

public:
    PhoneBook() {}

    void addContact(const PhoneRecord* record) {
        const string& fullName = record->getName();
        const string& organisation = record->getOrganisation();
        const vector<string>& phoneNumbers = record->getPhoneNumbers();

        // Split the full name into words
        vector<string> words;
        size_t start = 0, end = fullName.find(' ');
        while (end != string::npos) {
            words.push_back(fullName.substr(start, end - start));
            start = end + 1;
            end = fullName.find(' ', start);
        }
        words.push_back(fullName.substr(start));

        for (int i = 0; i < words.size(); ++i) {
            int key = getKey(words[i], i);
            HashTableRecord* newRecord = new HashTableRecord(key, const_cast<PhoneRecord*>(record));

            // Insert the new record at the beginning of the linked list
            hashTable[words[i]].push_back(newRecord);
        }
    }

    bool deleteContact(const string* searchName) {
    string searchString = *searchName; // Create a temporary string object

    // Fetch contacts based on the searchName
    vector<PhoneRecord*> contacts = fetchContacts(&searchString);

    if (contacts.empty()) {
        return false;
    }

    // Iterate through the contacts and delete them from the hash table
    for (PhoneRecord* contact : contacts) {
        vector<string> words;
        size_t start = 0, end = contact->getName().find(' ');
        while (end != string::npos) {
            words.push_back(contact->getName().substr(start, end - start));
            start = end + 1;
            end = contact->getName().find(' ', start);
        }
        words.push_back(contact->getName().substr(start));

        for (int i = 0; i < words.size(); ++i) {
            int key = getKey(words[i], i);
            vector<HashTableRecord*>& records = hashTable[words[i]];

            for (int j = 0; j < records.size(); ++j) {
                if (records[j]->getRecord() == contact) {
                    delete records[j];
                    records.erase(records.begin() + j);
                    break;
                }
            }
        }
    }

    return true;
}


    vector<PhoneRecord*> fetchContacts(const string* query) {
        string queryString = *query; // Create a temporary string object
        vector<string> queryWords;
        size_t start = 0, end = queryString.find(' ');
        while (end != string::npos) {
            queryWords.push_back(queryString.substr(start, end - start));
            start = end + 1;
            end = queryString.find(' ', start);
        }
        queryWords.push_back(queryString.substr(start));

        vector<PhoneRecord*> result;
        vector<PhoneRecord*> commonRecords;

        for (const string& queryWord : queryWords) {
            int key = getKey(queryWord, 0);
            vector<HashTableRecord*>& records = hashTable[queryWord];

            for (HashTableRecord* record : records) {
                PhoneRecord* contact = record->getRecord();

                // Check if this contact is already in the commonRecords vector
                bool found = false;
                for (PhoneRecord* commonRecord : commonRecords) {
                    if (commonRecord == contact) {
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    result.push_back(contact);
                    commonRecords.push_back(contact);
                }
            }
        }

        // Sort the result vector by the number of common words
        sort(result.begin(), result.end(), [&queryWords](PhoneRecord* a, PhoneRecord* b) {
            int countA = 0, countB = 0;
            for (const string& queryWord : queryWords) {
                if (a->getName().find(queryWord) != string::npos) {
                    countA++;
                }
                if (b->getName().find(queryWord) != string::npos) {
                    countB++;
                }
            }
            return countA > countB;
        });

        return result;
    }

    void readRecordsFromFile(const string& filename) {
        ifstream inputFile(filename);

        if (!inputFile.is_open()) {
            cout << "Error: Unable to open file " << filename << endl;
            return;
        }

        string line;
        while (getline(inputFile, line)) {
            // Parse the line and create a PhoneRecord
            // Format: "Name, PhoneNumber1, PhoneNumber2, Organisation"
            size_t pos = line.find(',');
            if (pos != string::npos) {
                string name = line.substr(0, pos);
                line = line.substr(pos + 1);

                vector<string> phoneNumbers;
                while ((pos = line.find(',')) != string::npos) {
                    phoneNumbers.push_back(line.substr(0, pos));
                    line = line.substr(pos + 1);
                }

                string organisation = line;

                // Create a PhoneRecord and add it to the PhoneBook
                PhoneRecord* record = new PhoneRecord(name, organisation, phoneNumbers);
                addContact(record);
            }
        }

        inputFile.close();
    }
};
