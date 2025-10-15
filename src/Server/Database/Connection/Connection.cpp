#include "Connection.h"

std::shared_ptr<pqxx::connection> Connection::acquire() {
    std::unique_lock<std::mutex> lock(mutex);
    const std::string str = builder.build();

    if (connections.empty()) {
        if (_cSize < _mSize) {
            auto temp = create(str);
            ++_cSize;
            return temp;
        }
        else {
            condition.wait(
                lock,
                [this] {
                    return !connections.empty();
            });
        }
    }

    auto temp = connections.front();
    connections.pop();
    return temp;
}

void Connection::release(std::shared_ptr<pqxx::connection> connection) {
    std::unique_lock<std::mutex> lock(mutex);
    connections.push(connection);
    lock.unlock();
    condition.notify_one();
}

size_t Connection::size() const {
    std::lock_guard<std::mutex> lock(mutex);
    return _cSize;
}

Connection::Connection() {
    const std::string temp = builder.build();

    for (size_t i = 0; i < _iSize; ++i) {
        connections.push(create(temp));
    }

    _cSize = _iSize;

    std::cout << "Connection pool initialized with " << _iSize <<  " connections." << std::endl;
}

Connection::~Connection() {
    while (!connections.empty())
        connections.pop();
    LOG_INFO(Protocol::Database, "Connection pool destroyed.");
}

std::shared_ptr<pqxx::connection> Connection::create(const std::string& temp) {
    try {
        auto str = std::make_shared<pqxx::connection>(temp);
        if (!str->is_open()) {
            LOG_ERROR(Protocol::Database, "Failed to open pooled connection.");
            return nullptr;
        }
        return str;
    }
    catch (const std::exception& e) {
        LOG_ERROR(Protocol::Database, "{}", e.what());
        return nullptr;
    }
}