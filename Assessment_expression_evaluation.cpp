#include <cctype>
#include <stdexcept>
#include <string>
#include <iostream>
#include <variant>
#include <functional>
#include <vector>

using namespace std;

using value = variant<int, string, vector<int>>;
using token = variant<string, value, char>;

token consumeNextToken(string& s) {
    token r;
    size_t tokenLen;

    size_t i = 0;
    while (i < s.length() && isspace(s[i])) ++i;
    s.erase(0, i);
    if (s.empty()) throw runtime_error("Unexpected end of input");

    if (s[0] == ')' || s[0] == '(' || s[0] == ',') {
        r = s[0];
        tokenLen = 1;
    } else if (s[0] == '"') {
        size_t i = 1;
        while (i < s.length() && s[i] != '"') ++i;
        if (i >= s.length()) throw runtime_error("Unterminated string literal");
        r = value(s.substr(1, i - 1));
        tokenLen = i + 1;
    } else if (s[0] == '-' || isdigit(s[0])) {
        size_t i = (s[0] == '-') ? 1 : 0;
        while (i < s.length() && isdigit(s[i])) ++i;
        if (i == 0 || (s[0] == '-' && i == 1)) throw runtime_error("Invalid number format");
        r = value(stoi(s.substr(0, i)));
        tokenLen = i;
    } else {
        size_t i = 0;
        while (i < s.length() && !isspace(s[i]) && s[i] != '(' && s[i] != ',' && s[i] != ')') ++i;
        r = s.substr(0, i);
        tokenLen = i;
    }

    while (tokenLen < s.length() && isspace(s[tokenLen])) ++tokenLen;
    s.erase(0, tokenLen);
    return r;
}

int main() {
    string expr;
    getline(cin, expr);

    function<value(string&)> eval;

    eval = [&](string& s) -> value {
        token t = consumeNextToken(s);

        if (holds_alternative<value>(t)) {
            return get<value>(t);
        } else if (holds_alternative<string>(t)) {
            string func = get<string>(t);
            token open = consumeNextToken(s);
            if (!holds_alternative<char>(open) || get<char>(open) != '(')
                throw runtime_error("Expected '(' after function name");

            vector<value> args;
            while (true) {
                size_t i = 0;
                while (i < s.size() && isspace(s[i])) ++i;
                s.erase(0, i);

                if (!s.empty() && s[0] == ')') {
                    consumeNextToken(s);
                    break;
                }

                args.push_back(eval(s));

                token next = consumeNextToken(s);
                if (holds_alternative<char>(next)) {
                    char ch = get<char>(next);
                    if (ch == ')') break;
                    else if (ch == ',') continue;
                }
                throw runtime_error("Expected ',' or ')' after argument");
            }

            // Function implementations
            if (func == "add") {
                int sum = 0;
                for (auto& arg : args) sum += get<int>(arg);
                return sum;
            } else if (func == "sub") {
                if (args.size() != 2) throw runtime_error("sub expects 2 args");
                return get<int>(args[0]) - get<int>(args[1]);
            } else if (func == "concat") {
                string result;
                for (auto& arg : args) result += get<string>(arg);
                return result;
            } else if (func == "len") {
                if (args.size() != 1) throw runtime_error("len expects 1 arg");
                return static_cast<int>(get<string>(args[0]).size());
            } else if (func == "parse") {
                if (args.size() != 1) throw runtime_error("parse expects 1 arg");
                return stoi(get<string>(args[0]));
            } else if (func == "vector") {
                vector<int> vec;
                for (auto& arg : args) vec.push_back(get<int>(arg));
                return vec;
            } else if (func == "range") {
                if (args.size() != 2) throw runtime_error("range expects 2 args");
                int start = get<int>(args[0]), end = get<int>(args[1]);
                vector<int> vec;
                for (int i = start; i < end; ++i) vec.push_back(i);
                return vec;
            } else if (func == "addv") {
                if (args.size() != 2) throw runtime_error("addv expects 2 args");
                auto v1 = get<vector<int>>(args[0]);
                auto v2 = get<vector<int>>(args[1]);
                if (v1.size() != v2.size()) throw runtime_error("Vector sizes must match");
                vector<int> result(v1.size());
                for (size_t i = 0; i < v1.size(); ++i) result[i] = v1[i] + v2[i];
                return result;
            } else if (func == "subv") {
                if (args.size() != 2) throw runtime_error("subv expects 2 args");
                auto v1 = get<vector<int>>(args[0]);
                auto v2 = get<vector<int>>(args[1]);
                if (v1.size() != v2.size()) throw runtime_error("Vector sizes must match");
                vector<int> result(v1.size());
                for (size_t i = 0; i < v1.size(); ++i) result[i] = v1[i] - v2[i];
                return result;
            } else if (func == "reduce_add") {
                if (args.size() != 1) throw runtime_error("reduce_add expects 1 arg");
                auto vec = get<vector<int>>(args[0]);
                int sum = 0;
                for (int x : vec) sum += x;
                return sum;
            } else if (func == "if_ge") {
                if (args.size() != 4) throw runtime_error("if_ge expects 4 arguments");
                const value& val   = args[0];
                const value& comp  = args[1];
                const value& left  = args[2];
                const value& right = args[3];

                if (holds_alternative<int>(val)) {
                    int v = get<int>(val);
                    int c = get<int>(comp);
                    return (v >= c) ? left : right;
                } else if (
                    holds_alternative<vector<int>>(val) &&
                    holds_alternative<vector<int>>(comp) &&
                    holds_alternative<vector<int>>(left) &&
                    holds_alternative<vector<int>>(right)) {

                    const auto& vvec = get<vector<int>>(val);
                    const auto& cvec = get<vector<int>>(comp);
                    const auto& lvec = get<vector<int>>(left);
                    const auto& rvec = get<vector<int>>(right);

                    if (vvec.size() != cvec.size() || vvec.size() != lvec.size() || vvec.size() != rvec.size())
                        throw runtime_error("All vectors in if_ge must be the same length");

                    vector<int> result;
                    for (size_t i = 0; i < vvec.size(); ++i)
                        result.push_back(vvec[i] >= cvec[i] ? lvec[i] : rvec[i]);
                    return result;
                }
                throw runtime_error("Unsupported types for if_ge");
            } else {
                throw runtime_error("Unknown function: " + func);
            }
        } else {
            throw runtime_error("Unexpected token");
        }
    };

    value result = eval(expr);

    if (holds_alternative<int>(result)) {
        cout << get<int>(result) << endl;
    } else if (holds_alternative<string>(result)) {
        cout << get<string>(result) << endl;
    } else if (holds_alternative<vector<int>>(result)) {
        const auto& vec = get<vector<int>>(result);
        cout << "[";
        for (size_t i = 0; i < vec.size(); ++i) {
            if (i > 0) cout << ", ";
            cout << vec[i];
        }
        cout << "]" << endl;
    }

    return 0;
}
