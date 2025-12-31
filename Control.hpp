#pragma once
#include "Account.hpp"
#include "Book.hpp"
#include "Log.hpp"

struct Control {
    User users_;
    LogIn logins_[2000];    // todo
    BookStorage book_storage_;
    Log log_;
};