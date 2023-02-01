#include "include/utils/config/read_val.hpp"

char *left_trim(char *src) {

    if (!src)
        return src;

    char *dst = src;                  // pour savoir si on a ecrit quelque chose
    char *pStart = src;               // memorise debut

    while (*src && isspace(*src))     // ignorer les blancs devant
        src++;
    int iShift = src - dst;             // combien on en a passe ?
    if (iShift > 0) {                  // y'en avait
        while (*src)                   // donc faut recopier en decalant
            *dst++ = *src++;            // jusqu'e la fin
        *dst = 0;
    }
    return pStart;
}

char *right_trim(char *src) {

    if (!src)
        return src;

    char *pLastNonSpc = nullptr;           // dernier char NON space
    char *pStart = src;              // memorise debut

    while (*src) {
        if (!isspace(*src)) pLastNonSpc = src;
        src++;
    }
    if (!pLastNonSpc)                      // aucun caractere interressant
        *pStart = '\0';
    else
        *(pLastNonSpc + 1) = '\0';
    return pStart;
}

char *trim(char *src) {
    return left_trim(right_trim(src));
}

std::string &left_trim(std::string &s) {
    std::string::size_type firstNonSpaceLoc = s.find_first_not_of(SPACE_STR);

    if (firstNonSpaceLoc != std::string::npos)
        s.erase(0, firstNonSpaceLoc);
    else
        s.clear();

    return s;
}

std::string &right_trim(std::string &s) {
    std::string::size_type lastNonSpaceLoc = s.find_last_not_of(SPACE_STR);

    if (lastNonSpaceLoc != std::string::npos)
        s.erase(lastNonSpaceLoc + 1, std::string::npos);
    else
        s.clear();

    return s;
}

std::string &trim(std::string &s) {

    std::string::size_type firstNonSpaceLoc = s.find_first_not_of(SPACE_STR);
    std::string::size_type lastNonSpaceLoc = s.find_last_not_of(SPACE_STR);

    if (lastNonSpaceLoc != std::string::npos)           // d'abord derriere
        s.erase(lastNonSpaceLoc + 1, std::string::npos);
    if (firstNonSpaceLoc != std::string::npos)
        s.erase(0, firstNonSpaceLoc);
    else
        s.clear();

    return s;
}

bool getKeyVal(std::string &line, char sep, std::string &name, std::string &value, char comment) {
    std::string::size_type iPos = line.find_first_not_of(SPACE_STR);
    // au moins un caractere significatif different du commentaire
    if (iPos != std::string::npos && line.at(iPos) != comment) {
        // on recherche la position du separateur
        iPos = line.find(sep);
        if (iPos != std::string::npos) {
            name.assign(line, 0, iPos);
            right_trim(left_trim(name));
            value.assign(line, iPos + 1, std::string::npos);
            // y avait-il un commentaire derriere ?
            iPos = value.find_first_of(comment);
            if (iPos != std::string::npos)                  // il y en avait un, on le vire
                value.erase(iPos, std::string::npos);
            right_trim(left_trim(value));
            return true;
        }
    }
    name.clear();
    value.clear();
    return false;
}
