/**
 *  Int64Incrementor.h
 *  Helper class for implementing counter
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer 
{
    class Int64Incrementor : public rocksdb::AssociativeMergeOperator {
    public:
        virtual bool Merge(
                            const rocksdb::Slice& key,
                            const rocksdb::Slice* existing_value,
                            const rocksdb::Slice& value,
                            std::string* new_value,
                            rocksdb::Logger* logger
                          ) const override 
        {
            
            // assuming 0 if no existing value
            int64_t existing = 0;
            if (existing_value) {
                existing = std::strtoll(existing_value->ToString().c_str(), nullptr, 10);
            }
            
            int64_t incr = std::strtoll(value.ToString().c_str(), nullptr, 10);
            int64_t newval = existing + incr;
            *new_value = std::to_string(newval);
            return true; // always return true for this, since we treat all errors as "zero".
        }
        
        virtual const char* Name() const override {
            return "Int64Incrementor";
        }
    };

}