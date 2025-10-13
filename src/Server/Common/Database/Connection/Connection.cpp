#include "Connection.h"

/**
 * @brief Acquire a connection from the pool, creating one if the pool is below capacity.
 *
 * If a pooled connection is available, returns it immediately. If the pool has not
 * reached its maximum size, attempts to create a new connection and return it.
 * If the pool is at maximum size and no connection is available, blocks until
 * another thread releases a connection.
 *
 * @return std::shared_ptr<pqxx::connection> A pointer to an active connection from the pool,
 * or `nullptr` if connection creation failed.
 */
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

/**
 * @brief Returns a database connection to the pool and wakes one waiting acquirer.
 *
 * The provided shared pointer is placed back into the pool of available connections
 * and one thread waiting in acquire() is notified.
 *
 * @param connection Shared pointer to the `pqxx::connection` to return to the pool.
 */
void Connection::release(std::shared_ptr<pqxx::connection> connection) {
    std::unique_lock<std::mutex> lock(mutex);
    connections.push(connection);
    lock.unlock();
    condition.notify_one();
}

/**
 * @brief Retrieves the current number of connections managed by the pool.
 *
 * Returns the total number of connections currently allocated by the pool, including those that are checked out.
 *
 * @return size_t The current connection count.
 */
size_t Connection::size() const {
    std::lock_guard<std::mutex> lock(mutex);
    return _cSize;
}

/**
 * @brief Initialize the connection pool and pre-create initial connections.
 *
 * Builds the connection string via the pool's builder, creates `_iSize`
 * connections and pushes them into the pool, sets the current size `_cSize`
 * to `_iSize`, and logs an initialization message.
 */
Connection::Connection() {
    const std::string temp = builder.build();

    for (size_t i = 0; i < _iSize; ++i) {
        connections.push(create(temp));
    }

    _cSize = _iSize;

    std::cout << "Connection pool initialized with " << _iSize <<  " connections." << std::endl;
}

/**
 * @brief Destroys the connection pool.
 *
 * Removes all remaining connections from the internal pool and logs the pool destruction.
 */
Connection::~Connection() {
    while (!connections.empty())
        connections.pop();
    LOG_INFO(PROTOCOLS::DATABASE, "Connection pool destroyed.");
}

/**
 * @brief Creates a new PostgreSQL connection from a connection string.
 *
 * Attempts to construct a pqxx::connection using the provided connection string.
 * If the connection is successfully opened, returns a shared pointer to it.
 * On failure (connection not open or an exception during construction) logs an error and returns `nullptr`.
 *
 * @param temp Connection string used to initialize the pqxx::connection.
 * @return std::shared_ptr<pqxx::connection> Shared pointer to an open connection on success, `nullptr` on failure.
 */
std::shared_ptr<pqxx::connection> Connection::create(const std::string& temp) {
    try {
        auto str = std::make_shared<pqxx::connection>(temp);
        if (!str->is_open()) {
            LOG_ERROR(PROTOCOLS::DATABASE, "Failed to open pooled connection.");
            return nullptr;
        }
        return str;
    }
    catch (const std::exception& e) {
        LOG_ERROR(PROTOCOLS::DATABASE, "{}", e.what());
        return nullptr;
    }
}