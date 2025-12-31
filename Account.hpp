#pragma once

#include <iostream>
#include <string>
#include <cstring>
using std::cin, std::cout, std::endl, std::string;

struct Accout {    // 单个账户。
    char userid [31];
    char username [31];
    char password [31];
    int privilege;

    Accout(const char id [31], const char pw [31], const char name [31], int priv = 1) {
        privilege = priv;
        strcpy(userid, id);
        strcpy(password, pw);
        strcpy(username, name);
    }

    Accout& operator=(const Accout& other) {
        if (this != &other) {  // 防止自赋值
            strcpy(userid, other.userid);
            strcpy(username, other.username);
            strcpy(password, other.password);
            privilege = other.privilege;
        }
        return *this;
    }

    const char* QueryID() const {
        return userid;
    }

    void AccoutChangePW(const char new_pw [31]) {
        strcpy(password, new_pw);
    }
};

struct LogIn {    // todo：用于实现在主控中的登陆情况管理。
    char userid [31];
    int privilege;
    char select_book [21];
};

struct User {    // todo：文件读写。
    Accout accounts_[2000];    // todo：块链
    long long account_num_;    // 账户个数。
    Accout crt_account_[2000];    // todo：当前登录的账户（个数？）
    long long crt_account_num_;    // 当前登录的账户个数。

    User() : account_num_(0), crt_account_num_(0) {}

    // 另外还需实现一个Parser./ 目前实现的是，在类外读出指令类型，然后在类内继续读取cin。

    // 登入。
    bool LogIn(int priv = 0) {    // 传入当前账户权限。
        string id, pw;
        cin >> id;
        getline(cin, pw);
        if (id.length() > 30 || pw.length() > 30) {
            return false;
        }
        if (account_num_ == 0) {
            return false;
        }
        // 二分查找id。
        int l = 0, r = account_num_ - 1, mid = 0;
        while (l <= r) {
            mid = (l + r) / 2;
            long long id_cmp = strcmp(accounts_[mid].userid, id.c_str());
            if (id_cmp == 0) {
                break;
            }
            if (id_cmp > 0) {
                r = mid - 1;
            }
            else {
                l = mid + 1;
            }
        }
        if (strcmp(accounts_[mid].userid, id.c_str()) != 0) {    // id不存在。
            cout << "Invalid\n";
            return false;
        }
        if (priv > accounts_[mid].privilege) {
            crt_account_[crt_account_num_] = accounts_[mid];    // 登入。
            return true;
        }
        if (strcmp(accounts_[mid].password, pw.c_str()) != 0) {    // 密码错误。
            cout << "Invalid\n";
            return false;
        }
        crt_account_[crt_account_num_] = accounts_[mid];    // 登入。
        return true;    // 密码正确。
    }

    // 登出。    // todo:可以迁移到主控文件。
    bool LogOut() {
        if (crt_account_num_ < 1) {
            cout << "Invalid\n";
            return false;
        }
        crt_account_num_--;
        return true;
    }

    // 游客注册。
    bool SignUp() {
        string id, pw, name;
        cin >> id >> pw;
        getline(cin, name);
        if (id.length() > 30 || pw.length() > 30 || name.length() > 30) {
            cout << "Invalid\n";
            return false;
        }
        // 二分查找id。
        int l = 0, r = account_num_ - 1, mid = 0, last = -1;
        while (l <= r) {
            mid = (l + r) / 2;
            long long id_cmp = strcmp(accounts_[mid].userid, id.c_str());
            if (id_cmp == 0) {
                last = mid - 1;
                break;
            }
            if (id_cmp > 0) {
                r = mid - 1;
            }
            else {
                last = mid;
                l = mid + 1;
            }
        }
        if (strcmp(accounts_[last + 1].userid, id.c_str()) == 0) {    // id已存在。
            cout << "Invalid\n";
            return false;
        }
        for (int i = account_num_ - 1; i > last; i--) {
            accounts_[i + 1] = accounts_[i];
        }
        accounts_[last + 1] = Accout(id.c_str(), pw.c_str(), name.c_str());
        return true;
    }

    bool ChangePassword(int priv = 0) {
        string id, pw_1, pw_2;
        cin >> id >> pw_1;
        getline(cin, pw_2);
        if (priv < 1) {
            cout << "Invalid\n";
            return false;
        }
        if (id.length() > 30 || pw_1.length() > 30) {
            cout << "Invalid\n";
            return false;
        }
        // 二分查找id。
        int l = 0, r = account_num_ - 1, mid = 0;
        while (l <= r) {
            mid = (l + r) / 2;
            long long id_cmp = strcmp(accounts_[mid].userid, id.c_str());
            if (id_cmp == 0) {
                break;
            }
            if (id_cmp > 0) {
                r = mid - 1;
            }
            else {
                l = mid + 1;
            }
        }
        if (strcmp(accounts_[mid].userid, id.c_str()) != 0) {    // id不存在。
            cout << "Invalid\n";
            return false;
        }
        if (priv == 7) {    // 高权限。
            accounts_[mid].AccoutChangePW(pw_1.c_str());
            return true;
        }
        if (strcmp(accounts_[mid].password, pw_1.c_str()) != 0) {    // 密码错误。
            cout << "Invalid\n";
            return false;
        }
        if (pw_2.length() > 30) {
            cout << "Invalid\n";
            return false;
        }
        accounts_[mid].AccoutChangePW(pw_2.c_str());
        return true;
    }

    bool AddUser(int priv = 0) {
        string id, pw, name;
        int privi;
        cin >> id >> pw >> privi;
        getline(cin, name);
        if (priv < 3 || priv < privi) {     // 权限不足。
            cout << "Invalid\n";
            return false;
        }
        if (id.length() > 30 || pw.length() > 30 || name.length() > 30) {
            cout << "Invalid\n";
            return false;
        }
        // 二分查找id。
        int l = 0, r = account_num_ - 1, mid = 0, last = -1;
        while (l <= r) {
            mid = (l + r) / 2;
            long long id_cmp = strcmp(accounts_[mid].userid, id.c_str());
            if (id_cmp == 0) {
                last = mid - 1;
                break;
            }
            if (id_cmp > 0) {
                r = mid - 1;
            }
            else {
                last = mid;
                l = mid + 1;
            }
        }
        if (strcmp(accounts_[last + 1].userid, id.c_str()) == 0) {    // id已存在。
            cout << "Invalid\n";
            return false;
        }
        for (int i = account_num_ - 1; i > last; i--) {
            accounts_[i + 1] = accounts_[i];
        }
        accounts_[last + 1] = Accout(id.c_str(), pw.c_str(), name.c_str(), privi);
        return true;
    }

    bool DeleteUser(int priv = 0) {    // todo:可以迁移到主控文件。
        string id;
        cin >> id;
        if (priv < 7) {
            cout << "Invalid\n";
            return false;
        }
        if (id.length() > 30) {
            cout << "Invalid\n";
            return false;
        }
        // 二分查找id。
        int l = 0, r = account_num_ - 1, mid = 0;
        while (l <= r) {
            mid = (l + r) / 2;
            long long id_cmp = strcmp(accounts_[mid].userid, id.c_str());
            if (id_cmp == 0) {
                break;
            }
            if (id_cmp > 0) {
                r = mid - 1;
            }
            else {
                l = mid + 1;
            }
        }
        if (strcmp(accounts_[mid].userid, id.c_str()) != 0) {    // id不存在。
            cout << "Invalid\n";
            return false;
        }
        for (int i = 0; i < crt_account_num_; i++) {
            if (strcmp(crt_account_[crt_account_num_].userid, id.c_str()) == 0) {    // 登陆中。
                cout << "Invalid\n";
                return false;
            }
        }
        for (int i = mid; i < account_num_ - 1; i++) {
            accounts_[i] = accounts_[i + 1];
            account_num_--;
        }
    }
};