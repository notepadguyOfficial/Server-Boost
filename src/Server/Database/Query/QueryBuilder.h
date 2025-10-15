#pragma once
#include <pqxx/pqxx>
#include <string>
#include <sstream>
#include <vector>
#include <utility>

// ORM Bullshits

class QueryBuilder {
    pqxx::connection& connection;
public:
    explicit QueryBuilder(pqxx::connection& conn) : connection(conn) { }
    
    QueryBuilder& select(const std::vector<std::string>& columns) {
        query << "SELECT ";
        if (columns.empty())
            query << "* ";
        else {
            for (size_t i = 0; i < columns.size(); ++i) {
                query << columns[i];
                if (i < (columns.size() - 1))
                    query << ", ";
            }
            query << " ";
        }

        return *this;
    }

    QueryBuilder& from(const std::string& table) {
        query << "FROM public." << connection.quote_name(table) << " ";
        return *this;
    }

    QueryBuilder& where(const std::string& condition) {
        if (!has_where) {
            query << "WHERE " << condition << " ";
            has_where = true;
        }
        else {
            query << "AND " << condition << " ";
        }

        return *this;
    }

    QueryBuilder& and_where(const std::string& condition) {
        return where(condition);
    }

    QueryBuilder& or_where(const std::string& condition) {
        if (!has_where) {
            query << "WHERE " << condition << " ";
            has_where = true;
        }
        else {
            query << "OR " << condition << " ";
        }

        return *this;
    }

    QueryBuilder& order_by(const std::string& clause) {
        query << "ORDER BY " << clause << " ";
        return *this;
    }

    QueryBuilder& limit(int n) {
        query << "LIMIT " << n << " ";
        return *this;
    }

    QueryBuilder& insert(const std::string& table, const std::vector<std::string>& columns) {
        query << "INSERT INTO " << table << " (";
        for (size_t i = 0; i < columns.size(); ++i) {
            query << columns[i];
            if (i < columns.size() - 1)
                query << ", ";
        }
        query << ") ";
        return *this;
    }

    QueryBuilder& values(const std::vector<std::vector<std::string>>& rows) {
        query << "VALUES ";
        for (size_t r = 0; r < rows.size(); ++r) {
            query << "(";
            for (size_t c = 0; c < rows[r].size(); ++c) {
                query << "'" << rows[r][c] << "'";
                if (c < (rows[r].size() - 1))
                    query << ", ";
            }
            query << ")";
            if (r < (rows.size() - 1))
                query << ", ";
        }
        query << " ";
        return *this;
    }

    QueryBuilder& update(const std::string& table) {
        query << "UPDATE " << table << " ";
        return *this;
    }

    QueryBuilder& set(const std::vector<std::pair<std::string, std::string>>& assignments) {
        query << "SET ";
        for (size_t i = 0; i < assignments.size(); ++i) {
            query << assignments[i].first << "='" << assignments[i].second << "'";
            if (i < (assignments.size() - 1))
                query << ", ";
        }
        query << " ";
        return *this;
    }

    QueryBuilder& delete_from(const std::string& table) {
        query << "DELETE FROM " << table << " ";
        return *this;
    }

    std::string build() const {
        return query.str();
    }

    void flush() {
        query.str("");
        query.clear();
        has_where = false;
    }

private:
    std::ostringstream query;
    bool has_where = false;
};