#include <memory>
#include <string>

class Bucket {
    private:
        std::unique_ptr<int []> data;
        int localDepth;
        int occupancy;
        int capacity;

        void setData(int index, int value);
        void incrementOccupancy();
        void decrementOccupancy();

    public:
        /**
            Creates an empty bucket with specified capacity and depth (determines how many hash-bits are used).
            @param capacity - the fixed size of the bucket
            @return a new instance of a bucket.
        */
        Bucket(int capacity, int localDepth);

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
};