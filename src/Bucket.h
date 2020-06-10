class Bucket {
    private:
        unique_ptr<int []> data;
        int localDepth;

    public:
        /**
            Creates an empty bucket with a capacity for 4 keys.
            @return a new instance of a bucket.
        */
        Bucket();

        /**
            Creates an empty bucket with specified capacity
            @param capacity - the fixed size of the bucket
            @return a new instance of a bucket.
        */
        Bucket(int capacity);

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
            @return bool - success/failure dependent on capacity of the bucket.
        */
        bool insert(int hashedKey);

        /**
            Removes a key from the bucket.
            @param hashedKey - the search key that has been hashed.
            @return bool - whether or not the key was found and deleted.
        */
        bool remove(int hashedKey);

        /**
            Prints the contents of the bucket and it's local depth.
        */
        void print();
}