#include "Bucket.h"
#include <memory>

class ExtensibleHashTable {
    private:
        std::unique_ptr< std::shared_ptr<Bucket> []> directory;
        int globalDepth;

        /**
            Hash function used for indexing. k -> H(k).
            If there are 2^i total buckets where i represents the global depth
            than only the right i bits of the hash are used.
            @param key - the key to be hashed
            @return int - the hash value
        */
        //int hash(int key);

        /**
            Splits a bucket, returning a new bucket with containi
        */
        void splitBucket(int bucketIndex);

        void incrementGlobalDepth();

    public:
        /**
            Creates an empty hash table, initially with 2 entries/buckets,
            each bucket contains 4 keys. and uses only the single right-most bit.
            @return a new ExtensibleHashTable instance.
        */
        ExtensibleHashTable();
        
        /**
            Creates an empty hash table, initially with 2 entries/buckets
            and uses only the single right-most bit.
            @param keysPerBucket - the number of keys contained in each bucket.
            @return a new ExtensibleHashTable instance.
        */
        ExtensibleHashTable(int keysPerBucket);

        /**
            Destructor
        */
        ~ExtensibleHashTable();

        /**
            Searches the hash index for a key.
            @param key - the search key
            @return bool - whether or not the search key was found.
        */
        bool find(int key); 

        /**
            Inserts a new key into the appropiate bucket.
            @param key - the new key
            @return void
        */
        void insert(int key);

        /**
            Searchs the hash table and deletes the key if found.
            @param key - the key to be removed.
            @return bool - whether or not the key was found and removed.
        */
        bool remove(int key);

        /**
            Prints the contents of the directory and buckets
            @return void
        */
        void print();

        /**
            Returns only the right-most bits, all other bits set to 0
            @param hashedKey - a key that has been hashed
            @param numBits - number of bits considered
            @return int - in which only right numBits are let through
        */
        static int maskBits(int hashedKey, int numBits);
};
