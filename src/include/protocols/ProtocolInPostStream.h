 /**
 *  ProtocolInPostStream.h
 *
 *  Cursor to walk across post-data 
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


#pragma once

#include <optional>
#include <utility>
#include <string_view>
#include <charconv>

namespace RocksServer {

    class ProtocolInPostStream
    {
    public:
        using size_type = std::string_view::size_type;

        explicit ProtocolInPostStream(std::string_view raw) noexcept
            : _raw(raw)
        {}

        void reset() noexcept
        {
            _pos = 0;
            _err.clear();
        }

        bool eof() const noexcept
        {
            return _pos >= _raw.size();
        }

        size_type position() const noexcept
        {
            return _pos;
        }

        const std::string& error() const
        {
            return _err;
        }

        std::optional<rocksdb::Slice> key()
        {
            return readLine();
        }

        std::optional<rocksdb::Slice> value()
        {
            auto lenOpt = readLengthLine();
            if (!lenOpt) return std::nullopt;

            const size_t len = *lenOpt;

            if (_raw.size() - _pos < len) {
                setError("value(): not enough bytes for declared length");
                return std::nullopt;
            }

            rocksdb::Slice v(_raw.data() + _pos, len);
            _pos += len;

            // съедаем завершающий '\n' если он есть
            if (_pos < _raw.size() && _raw[_pos] == '\n') {
                ++_pos;
            }

            return v;
        }

        std::optional<std::pair<rocksdb::Slice, rocksdb::Slice>> pair()
        {
            auto k = key();
            if (!k) return std::nullopt;

            auto v = value();
            if (!v) return std::nullopt;

            return std::make_pair(*k, *v);
        }

    private:
        std::optional<rocksdb::Slice> readLine()
        {
            if (_pos >= _raw.size()) {
                return std::nullopt;
            }

            const size_type lpos = _pos;
            const size_type rpos = _raw.find('\n', lpos);

            if (rpos == std::string_view::npos) {
                _pos = _raw.size();
                return rocksdb::Slice(_raw.data() + lpos, _raw.size() - lpos);
            }

            _pos = rpos + 1;
            return rocksdb::Slice(_raw.data() + lpos, rpos - lpos);
        }

        std::optional<size_t> readLengthLine()
        {
            auto line = readLine();
            if (!line) {
                setError("value(): expected length line, got EOF");
                return std::nullopt;
            }

            size_t len = 0;
            const char* b = line->data();
            const char* e = b + line->size();

            auto [ptr, ec] = std::from_chars(b, e, len);
            if (ec != std::errc{} || ptr != e) {
                setError("value(): invalid length line");
                return std::nullopt;
            }

            return len;
        }

        void setError(const char* msg)
        {
            _err = msg;
        }

        std::string_view _raw;
        size_type _pos = 0;
        std::string _err;
    };

} // namespace RocksServer
