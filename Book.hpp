#pragma once
#include <iostream>
#include <string>
#include <cstring>
using std::cin, std::cout, std::endl, std::string;

struct Book {
    char isbn[21];
    char book_name[61];
    char author[61];
    char* keywords[30];
    int keyword_cnt;
    long double price;
    long long stock;

    Book(char idx[21]) {
        strcpy(isbn, idx);
        book_name[0] = '\0';
        author[0] = '\0';
        keyword_cnt = 0;
        price = 0;
        stock = 0;
    }
};

struct BookStorage {
    Book books_[2000];    // ISBN排列。    // todo：块链
    int book_num_;
    std::streampos book_name_[2000];    // 以书名排列。

    BookStorage() {
        book_num_ = 0;
    }

    void QueryBook();    // todo

    bool Buy(int priv = 0) {
        string idx, quantity;
        cin >> idx >> quantity;
        if (priv < 1) {    // 权限不足。
            cout << "Invalid\n";
            return false;
        }
        if (idx.length() > 20) {
            cout << "Invalid\n";
            return false;
        }
        // 二分查找ISBN。
        int l = 0, r = book_num_ - 1, mid = 0;
        while (l <= r) {
            mid = (l + r) / 2;
            long long isbn_cmp = strcmp(books_[mid].isbn, idx.c_str());
            if (isbn_cmp == 0) {
                break;
            }
            if (isbn_cmp > 0) {
                r = mid - 1;
            }
            else {
                l = mid + 1;
            }
        }
        if (strcmp(books_[mid].isbn, idx.c_str()) != 0) {    // isbn不存在。
            cout << "Invalid\n";
            return false;
        }
        long long quant = 0;
        for (int i = 0; i < quantity.length(); i++) {
            if (quantity[i] - '0' < 0 || quantity[i] - '9' > 0) {
                cout << "Invalid\n";
                return false;
            }
            quant *= 10;
            quant += quantity[i] - '0';
        }
        if (quant > books_[mid].stock) {    // 库存不足。
            cout << "Invalid\n";
            return false;
        }
        books_[mid].stock -= quant;
        cout << quant * books_[mid].price << endl;
        return true;
    }

    bool Select(int priv = 0) {
        string idx;
        cin >> idx;
        if (priv < 3) {
            cout << "Invalid\n";
            return false;
        }
        if (idx.length() > 20) {
            cout << "Invalid\n";
            return false;
        }
        // 二分查找ISBN。
        int l = 0, r = book_num_ - 1, mid = 0;
        while (l <= r) {
            mid = (l + r) / 2;
            long long isbn_cmp = strcmp(books_[mid].isbn, idx.c_str());
            if (isbn_cmp == 0) {
                break;
            }
            if (isbn_cmp > 0) {
                r = mid - 1;
            }
            else {
                l = mid + 1;
            }
        }
        if (strcmp(books_[mid].isbn, idx.c_str()) != 0) {    // isbn不存在。
            cout << "Invalid\n";
            return false;
        }
        return true;    // todo：在主控程序的登陆状况中保存选中情况。
    }

    bool Modify(int priv = 0, char idx []) {}    // todo：修改图书信息。从类外传入选中情况。
    bool Restock(int priv = 0, char idx []) {
        string quantity, total_cost;
        // todo
    }
};