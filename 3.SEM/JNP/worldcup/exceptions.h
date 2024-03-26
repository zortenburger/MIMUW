#ifndef WORLDCUP_EXCEPTIONS_H
#define WORLDCUP_EXCEPTIONS_H
#include <exception>
class TooManyDiceException : public std::exception {
};

class TooFewDiceException : public std::exception {
};

class TooManyPlayersException : public std::exception {
};

class TooFewPlayersException : public std::exception {
};

#endif //WORLDCUP_EXCEPTIONS_H
