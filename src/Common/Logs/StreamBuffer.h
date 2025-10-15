#pragma once
#include <iostream>
#include <regex>

// Capture std::cout's
class StreamBuffer : public std::streambuf {
public:
    StreamBuffer(std::streambuf* sb1, std::streambuf* sb2)
        : sb1_(sb1), sb2_(sb2) { }

    ~StreamBuffer() {
        flush();
    }

protected:
    int overflow(int c) override {
        if (c == EOF)
            return !EOF;
        if (sb1_->sputc(c) == EOF)
            return EOF;
        buffer_.push_back(static_cast<char>(c));
        if(c == '\n')
            flush();
        return c;
    }

    int sync() override {
        flush();
        return (sb1_->pubsync() == 0) ? 0 : -1;
    }

private:
    std::streambuf* sb1_; // console
    std::streambuf* sb2_; // file
    std::string buffer_;

    void flush() {
        if (buffer_.empty())
            return;
        static const std::regex ansi_regex(R"(\x1B\[[0-9;]*[A-Za-z])");
        std::string clean = std::regex_replace(buffer_, ansi_regex, "");
        sb2_->sputn(clean.data(), clean.size());
        sb2_->pubsync();
        buffer_.clear();
    }
};