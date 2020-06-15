#include <memory>
#include <string>

class Bucket {
    private:
        std::unique_ptr<int []> data;
        int localDepth;
        int occupancy;
        int capacity;
        bool recentlyPrinted;

        void setData(int index, int value);
        void incrementOccupancy();
        void decrementOccupancy();
        bool removeByIndex(int index);

    public:
        /**
            Creates an empty bucket with specified capacity and depth (determines how many hash-bits are used).
            @param capacity - the fixed size of the bucket
            @return a new instance of a bucket.
        */
        Bucket(int capacity, int localDepth);

        Bucket(const Bucket &copy);

        /**
            Destructor
        */
        ~Bucket();

        /**
            Linearly searches the bucket for a key
            @param hashedKey - search key that has been hashed.
            @return int - index of the hash key.
        */
        int find(int hashedKey);

        /**
            Inserts a new key, if the bucket has reached capacity, it will warn the hash
            table through the return value.
            @param hashedKey - new hashed key to be inserted.
            @return int - returns -1 on failure, index if found.
        */
        bool insert(int hashedKey);

        /**
            Removes a key from the bucket.
            @param hashedKey - the search key that has been hashed.
            @return bool - whether or not the key was found and deleted.
        */
        bool remove(int hashedKey);

        /**
            formatted string with contents of bucket and localDepth
            @return string - [<hash1>, <hash2>, -, -, ..., -] (<localDepth>)
        */
        std::string print();

        /**
            splits the bucket into an into 2 buckets, the if localDepth is i bits
            than the existing bucket will contain ...0<old localDepth bits> keys and
            the new bucket will return ...1<old localDepth bits> keys
            @param maskBits function from an ExtensibleHashTable class
            @return Bucket* - the new instance containing ...1<old localDepth bits> keys
        */
        std::shared_ptr<Bucket> splitBucket(int naughtKeyPattern, int oneKeyPattern, int(*maskBits)(int, int));

        /**
            returns local depth
        */
        int getLocalDepth();

        void resetPrintStatus();
};