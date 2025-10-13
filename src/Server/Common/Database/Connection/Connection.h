#pragma once
#include "ConnectionStringBuilder.h"
#include "Logs.h"
#include <pqxx/pqxx>
#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>

/**
     * Get the singleton Connection instance.
     *
     * @returns Reference to the unique Connection singleton.
     */
    class Connection {
public:
    static Connection& instance() {
        static Connection instance;
        return instance;
    }

    std::shared_ptr<pqxx::connection> acquire();
    void release(std::shared_ptr<pqxx::connection> connection);
    size_t size() const;

private:
    Connection();
    ~Connection();
    std::shared_ptr<pqxx::connection> create(const std::string& temp);
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

private:
    ConnectionStringBuilder builder;
    std::queue<std::shared_ptr<pqxx::connection>> connections;
    mutable std::mutex mutex;
    std::condition_variable condition;
    size_t _cSize = 0; // current size
    const size_t _iSize = 2; // initial size
    const size_t _mSize = 10; // max size
};