#include <bits/stdc++.h>

using namespace std;

// ==================== Minimal BigInt using strings ====================
// We only need: add, multiply by small int, subtract 1, compare, print

string add(string a, string b) {
    if (a.length() < b.length()) swap(a, b);
    string result = "";
    int len1 = a.size(), len2 = b.size();
    int carry = 0;
    for (int i = 0; i < len1; i++) {
        int sum = carry + (a[len1 - 1 - i] - '0');
        if (i < len2) sum += (b[len2 - 1 - i] - '0');
        carry = sum / 10;
        result.push_back(sum % 10 + '0');
    }
    if (carry) result.push_back(carry + '0');
    reverse(result.begin(), result.end());
    return result;
}

string mul_small(string num, unsigned long long x) {
    if (x == 0) return "0";
    string result;
    unsigned long long carry = 0;
    for (int i = num.size() - 1; i >= 0; i--) {
        unsigned long long temp = (num[i] - '0') * x + carry;
        carry = temp / 10;
        result.push_back(temp % 10 + '0');
    }
    while (carry) {
        result.push_back(carry % 10 + '0');
        carry /= 10;
    }
    reverse(result.begin(), result.end());
    return result;
}

// Simple multiply for two big numbers (sufficient for our needs)
string multiply(string a, string b) {
    int len1 = a.size();
    int len2 = b.size();
    if (len1 == 0 || len2 == 0) return "0";

    vector<int> result(len1 + len2, 0);

    for (int i = len1 - 1; i >= 0; i--) {
        for (int j = len2 - 1; j >= 0; j--) {
            int mul = (a[i] - '0') * (b[j] - '0');
            int sum = mul + result[i + j + 1];
            result[i + j + 1] = sum % 10;
            result[i + j] += sum / 10;
        }
    }

    string s = "";
    for (int num : result) {
        if (!(s.empty() && num == 0)) s += char(num + '0');
    }
    return s.empty() ? "0" : s;
}

string subtract_one(string num) {
    int n = num.size();
    string result = num;
    int i = n - 1;
    while (i >= 0) {
        if (result[i] > '0') {
            result[i]--;
            break;
        } else {
            result[i] = '9';
            i--;
        }
    }
    // Remove leading zeros
    if (result[0] == '0' && result.size() > 1) result = result.substr(1);
    return result;
}

// Compare two positive big numbers as strings ("123" > "99" → true)
bool is_greater(string a, string b) {
    if (a.size() != b.size()) return a.size() > b.size();
    return a > b;
}

bool is_equal(string a, string b) {
    return a == b;
}

void print_big(const string& s) {
    cout << s;
}

// ==================== Main program ====================

struct Pair {
    string n, m, dim;
};

int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <max_dimension>\n";
        return 1;
    }

    string MAX_DIM = argv[1];

    // Remove leading zeros from input
    size_t start = MAX_DIM.find_first_not_of('0');
    if (start != string::npos) MAX_DIM = MAX_DIM.substr(start);
    else MAX_DIM = "0";

    vector<Pair> results;

    // ========== Chain 1: start from n=4, m=6 ==========
    {
        string k = "11";
        string n = "4";

        for(int step = 0; step < 200; ++step) {
            string dim = subtract_one(multiply(n, n));
            if (is_greater(dim, MAX_DIM) || dim == "0") break;

            // m = (1 + k) / 2
            string temp = add("1", k);
            string m = "";
            int carry = 0;
            for (char c : temp) {
                int d = c - '0';
                int new_d = (carry * 10 + d) / 2;
                carry = (carry * 10 + d) % 2;
                if (!m.empty() || new_d != 0) m += char(new_d + '0');
            }
            if (m.empty()) m = "0";

            results.push_back({n, m, dim});

            string next_k = add(mul_small(k, 3), mul_small(n, 8));
            string next_n = add(k, mul_small(n, 3));
            k = next_k;
            n = next_n;
        }
    }

    // ========== Chain 2: start from n=2, m=3 ==========
    {
        string k = "5";
        string n = "2";

        for(int step = 0; step < 200; ++step) {
            string dim = subtract_one(multiply(n, n));
            if (is_greater(dim, MAX_DIM) || dim == "0") break;

            string temp = add("1", k);
            string m = "";
            int carry = 0;
            for (char c : temp) {
                int d = c - '0';
                int new_d = (carry * 10 + d) / 2;
                carry = (carry * 10 + d) % 2;
                if (!m.empty() || new_d != 0) m += char(new_d + '0');
            }
            if (m.empty()) m = "0";

            results.push_back({n, m, dim});

            string next_k = add(mul_small(k, 3), mul_small(n, 8));
            string next_n = add(k, mul_small(n, 3));
            k = next_k;
            n = next_n;
        }
    }

    // Sort by dimension (as big numbers)
    sort(results.begin(), results.end(), [](const Pair& a, const Pair& b) {
        if (a.dim.size() != b.dim.size()) return a.dim.size() < b.dim.size();
        return a.dim < b.dim;
    });

    // Print ONLY the table in English
    cout << left 
         << setw(14) << "n (SU(n))" 
         << setw(14) << "m (SO(m))" 
         << "Dimension\n";
    cout << string(42, '-') << "\n";

    for (const auto& p : results) {
        cout << left << setw(14) << p.n 
             << setw(14) << p.m 
             << p.dim << "\n";
    }

    return 0;
}