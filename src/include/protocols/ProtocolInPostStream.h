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
#include <system_error>

namespace RocksServer {

    class ProtocolInPostStream
    {
    public:
        using size_type = std::string_view::size_type;

        enum class Error : unsigned char
        {
            None = 0,
            ExpectedLengthLineEof,
            InvalidLengthLine,
            NotEnoughBytesForDeclaredLength,
        };

        explicit ProtocolInPostStream(std::string_view raw) noexcept
            : _raw(raw)
        {}

        void reset() noexcept
        {
            _pos = 0;
            _err = Error::None;
        }

        bool eof() const noexcept
        {
            return _pos >= _raw.size();
        }

        size_type position() const noexcept
        {
            return _pos;
        }

        Error errorCode() const noexcept
        {
            return _err;
        }

        std::string_view error() const noexcept
        {
            return errorMessage(_err);
        }

        std::optional<rocksdb::Slice> key() noexcept
        {
            return readLine();
        }

        std::optional<rocksdb::Slice> value() noexcept
        {
            auto lenOpt = readLengthLine();
            if (!lenOpt) return std::nullopt;

            const size_t len = *lenOpt;

            if (_raw.size() - _pos < len) {
                setError(Error::NotEnoughBytesForDeclaredLength);
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

        std::optional<std::pair<rocksdb::Slice, rocksdb::Slice>> pair() noexcept
        {
            auto k = key();
            if (!k) return std::nullopt;

            auto v = value();
            if (!v) return std::nullopt;

            return std::make_pair(*k, *v);
        }

    private:
        static constexpr std::string_view errorMessage(Error e) noexcept
        {
            switch (e) {
                case Error::None:
                    return {};
                case Error::ExpectedLengthLineEof:
                    return "value(): expected length line, got EOF";
                case Error::InvalidLengthLine:
                    return "value(): invalid length line";
                case Error::NotEnoughBytesForDeclaredLength:
                    return "value(): not enough bytes for declared length";
            }
            return "unknown error";
        }

        std::optional<rocksdb::Slice> readLine() noexcept
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

        std::optional<size_t> readLengthLine() noexcept
        {
            auto line = readLine();
            if (!line) {
                setError(Error::ExpectedLengthLineEof);
                return std::nullopt;
            }

            size_t len = 0;
            const char* b = line->data();
            const char* e = b + line->size();

            auto [ptr, ec] = std::from_chars(b, e, len);
            if (ec != std::errc{} || ptr != e) {
                setError(Error::InvalidLengthLine);
                return std::nullopt;
            }

            return len;
        }

        void setError(Error e) noexcept
        {
            _err = e;
        }

        std::string_view _raw;
        size_type _pos = 0;
        Error _err = Error::None;
    };

} // namespace RocksServer
