#include "Bucket.h"
#include <memory>
#include <fstream>

class ExtensibleHashTable {
    private:
        std::unique_ptr< std::shared_ptr<Bucket> []> directory;
        int globalDepth;
        int numDirectories;

        /**
            Initializes 2 empty buckets and sets the global depth to 1.
            @param keyPerBucket - size of each bucket.
        */
        void initialize(int keysPerBucket);

        /**
            Splits a bucket, the bucket at bucketIndex will be split into 2 buckets;
            ...0<oldBucketIndex> & ...1<oldBucketIndex>. This function has the potential
            to call incrementGlobalDepth() if the localDepth is the same as the globalDepth.
            @param bucketIndex - hash-value/directory-index to be split.
        */
        void splitBucket(int bucketIndex);

        /**
            Increments globalDepth and expands the directory to account for the change. Does
            not create any new buckets but wires new directories to existing buckets.
        */
        void incrementGlobalDepth();

        /**
            In response to bucket splitting, directory that are dependent on the bucket being split
            will be redirected to either the new bucket or old bucket.
            @param newBucketIndex - the bucket that was recently created.
            @param oldLocalDepth - the localDepth of the bucket pre-split.
        */
        void updateDirectory(int newBucketIndex, int oldLocalDepth);

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
        void print(std::ofstream& outFile);

        /**
            Returns only the right-most bits, all other bits set to 0
            @param hashedKey - a key that has been hashed
            @param numBits - number of bits considered
            @return int - in which only right numBits are let through
        */
        static int maskBits(int hashedKey, int numBits);
};
