#include <bits/stdc++.h>
using namespace std;

#pragma once
#ifndef SJTU_BIGINTEGER
#define SJTU_BIGINTEGER

// Integer 1:
// Implement a signed big integer class that only needs to support simple addition and subtraction

// Integer 2:
// Implement a signed big integer class that supports addition, subtraction, multiplication, and division, and overload related operators

// Do not use any header files other than the following
#include <complex>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

// Do not use "using namespace std;"

namespace sjtu {
class int2048 {
  // Internal representation: base 1000, little-endian digits
  static const int BASE = 1000;
  static const int BASE_DIGS = 3;
  std::vector<int> a; // digits, a[0] is least significant
  bool neg;

  // Helpers
  void trim();
  static int cmp_abs(const int2048 &, const int2048 &);
  static void add_abs_to(std::vector<int> &dst, const std::vector<int> &src);
  static void sub_abs_from(std::vector<int> &dst, const std::vector<int> &src); // assumes |dst|>=|src|
  static int2048 mul_simple(const int2048 &, const int2048 &);
  static int2048 mul_fft(const int2048 &, const int2048 &);
  static void divmod_abs(const int2048 &x, const int2048 &y, int2048 &q, int2048 &r);
  static void mul_scalar(std::vector<int> &v, int m);
  static void add_scalar_shifted(std::vector<int> &v, const std::vector<int> &w, long long m, size_t shift);
public:
  // Constructors
  int2048();
  int2048(long long);
  int2048(const std::string &);
  int2048(const int2048 &);

  // The parameter types of the following functions are for reference only, you can choose to use constant references or not
  // If needed, you can add other required functions yourself
  // ===================================
  // Integer1
  // ===================================

  // Read a big integer
  void read(const std::string &);
  // Output the stored big integer, no need for newline
  void print();

  // Add a big integer
  int2048 &add(const int2048 &);
  // Return the sum of two big integers
  friend int2048 add(int2048, const int2048 &);

  // Subtract a big integer
  int2048 &minus(const int2048 &);
  // Return the difference of two big integers
  friend int2048 minus(int2048, const int2048 &);

  // ===================================
  // Integer2
  // ===================================

  int2048 operator+() const;
  int2048 operator-() const;

  int2048 &operator=(const int2048 &);

  int2048 &operator+=(const int2048 &);
  friend int2048 operator+(int2048, const int2048 &);

  int2048 &operator-=(const int2048 &);
  friend int2048 operator-(int2048, const int2048 &);

  int2048 &operator*=(const int2048 &);
  friend int2048 operator*(int2048, const int2048 &);

  int2048 &operator/=(const int2048 &);
  friend int2048 operator/(int2048, const int2048 &);

  int2048 &operator%=(const int2048 &);
  friend int2048 operator%(int2048, const int2048 &);

  friend std::istream &operator>>(std::istream &, int2048 &);
  friend std::ostream &operator<<(std::ostream &, const int2048 &);

  friend bool operator==(const int2048 &, const int2048 &);
  friend bool operator!=(const int2048 &, const int2048 &);
  friend bool operator<(const int2048 &, const int2048 &);
  friend bool operator>(const int2048 &, const int2048 &);
  friend bool operator<=(const int2048 &, const int2048 &);
  friend bool operator>=(const int2048 &, const int2048 &);
};
} // namespace sjtu

#endif



namespace sjtu {

// Helpers
void int2048::trim() {
  while (!a.empty() && a.back() == 0) a.pop_back();
  if (a.empty()) neg = false;
}

int int2048::cmp_abs(const int2048 &x, const int2048 &y) {
  if (x.a.size() != y.a.size()) return x.a.size() < y.a.size() ? -1 : 1;
  for (int i = (int)x.a.size() - 1; i >= 0; --i) {
    if (x.a[i] != y.a[i]) return x.a[i] < y.a[i] ? -1 : 1;
  }
  return 0;
}

void int2048::add_abs_to(std::vector<int> &dst, const std::vector<int> &src) {
  int carry = 0; size_t n = std::max(dst.size(), src.size());
  if (dst.size() < n) dst.resize(n, 0);
  for (size_t i = 0; i < n || carry; ++i) {
    if (i == dst.size()) dst.push_back(0);
    long long sum = dst[i] + carry + (i < src.size() ? src[i] : 0);
    carry = int(sum / BASE);
    dst[i] = int(sum % BASE);
  }
}

void int2048::sub_abs_from(std::vector<int> &dst, const std::vector<int> &src) {
  // assumes |dst| >= |src|
  int carry = 0;
  for (size_t i = 0; i < src.size() || carry; ++i) {
    long long cur = dst[i] - (long long)(i < src.size() ? src[i] : 0) - carry;
    if (cur < 0) { cur += BASE; carry = 1; } else carry = 0;
    dst[i] = int(cur);
  }
  while (!dst.empty() && dst.back() == 0) dst.pop_back();
}

void int2048::mul_scalar(std::vector<int> &v, int m) {
  if (m == 0 || v.empty()) { v.clear(); return; }
  long long carry = 0;
  for (size_t i = 0; i < v.size(); ++i) {
    long long cur = carry + 1LL * v[i] * m;
    v[i] = int(cur % BASE);
    carry = cur / BASE;
  }
  while (carry) { v.push_back(int(carry % BASE)); carry /= BASE; }
}

void int2048::add_scalar_shifted(std::vector<int> &v, const std::vector<int> &w, long long m, size_t shift) {
  if (m == 0 || w.empty()) return;
  if (v.size() < w.size() + shift + 2) v.resize(w.size() + shift + 2, 0);
  long long carry = 0;
  for (size_t i = 0; i < w.size(); ++i) {
    long long cur = v[i + shift] + carry + m * w[i];
    v[i + shift] = int(cur % BASE);
    carry = cur / BASE;
  }
  size_t idx = w.size() + shift;
  while (carry) {
    long long cur = v[idx] + carry;
    v[idx] = int(cur % BASE);
    carry = cur / BASE;
    ++idx;
    if (idx >= v.size()) v.push_back(0);
  }
}

int2048 int2048::mul_simple(const int2048 &x, const int2048 &y) {
  int2048 res; res.neg = x.neg ^ y.neg;
  if (x.a.empty() || y.a.empty()) { res.neg = false; return res; }
  res.a.assign(x.a.size() + y.a.size(), 0);
  for (size_t i = 0; i < x.a.size(); ++i) {
    long long carry = 0;
    for (size_t j = 0; j < y.a.size() || carry; ++j) {
      long long cur = res.a[i + j] + carry + 1LL * x.a[i] * (j < y.a.size() ? y.a[j] : 0);
      res.a[i + j] = int(cur % BASE);
      carry = cur / BASE;
    }
  }
  res.trim();
  return res;
}

namespace {
using cd = std::complex<long double>;
const long double PI = acosl(-1.0L);

void fft(std::vector<cd> &a, bool invert) {
  int n = (int)a.size();
  for (int i = 1, j = 0; i < n; ++i) {
    int bit = n >> 1;
    for (; j & bit; bit >>= 1) j ^= bit;
    j ^= bit;
    if (i < j) std::swap(a[i], a[j]);
  }
  for (int len = 2; len <= n; len <<= 1) {
    long double ang = 2 * PI / len * (invert ? -1 : 1);
    cd wlen(cosl(ang), sinl(ang));
    for (int i = 0; i < n; i += len) {
      cd w(1);
      for (int j = 0; j < len / 2; ++j) {
        cd u = a[i + j];
        cd v = a[i + j + len / 2] * w;
        a[i + j] = u + v;
        a[i + j + len / 2] = u - v;
        w *= wlen;
      }
    }
  }
  if (invert) {
    for (int i = 0; i < n; ++i) a[i] /= n;
  }
}
}

int2048 int2048::mul_fft(const int2048 &x, const int2048 &y) {
  int2048 res; res.neg = x.neg ^ y.neg;
  if (x.a.empty() || y.a.empty()) { res.neg = false; return res; }
  std::vector<cd> fa(x.a.begin(), x.a.end()), fb(y.a.begin(), y.a.end());
  int n = 1;
  while (n < (int)x.a.size() + (int)y.a.size()) n <<= 1;
  fa.resize(n); fb.resize(n);
  fft(fa, false); fft(fb, false);
  for (int i = 0; i < n; ++i) fa[i] *= fb[i];
  fft(fa, true);
  std::vector<long long> tmp(n);
  for (int i = 0; i < n; ++i) tmp[i] = (long long) llround(fa[i].real());
  long long carry = 0;
  res.a.assign(n, 0);
  for (int i = 0; i < n; ++i) {
    long long cur = tmp[i] + carry;
    res.a[i] = int(cur % BASE);
    carry = cur / BASE;
  }
  while (carry) { res.a.push_back(int(carry % BASE)); carry /= BASE; }
  res.trim();
  return res;
}

void int2048::divmod_abs(const int2048 &X, const int2048 &Y, int2048 &Q, int2048 &R) {
  // assumes X, Y are non-negative and Y != 0
  Q.a.clear(); Q.neg = false;
  R.a.clear(); R.neg = false;
  if (cmp_abs(X, Y) < 0) { R = X; Q.trim(); R.trim(); return; }
  Q.a.assign(X.a.size(), 0);
  int2048 cur; cur.neg = false;
  for (int i = (int)X.a.size() - 1; i >= 0; --i) {
    // cur = cur * BASE + X.a[i]
    if (!cur.a.empty()) {
      long long carry = 0;
      for (size_t k = 0; k < cur.a.size(); ++k) {
        long long val = 1LL * cur.a[k] * BASE + carry;
        cur.a[k] = int(val % BASE);
        carry = val / BASE;
      }
      if (carry) cur.a.push_back((int)carry);
    }
    if (X.a[i]) {
      if (cur.a.empty()) cur.a.push_back(X.a[i]);
      else {
        long long carry = X.a[i];
        size_t k = 0;
        while (carry) {
          if (k == cur.a.size()) cur.a.push_back(0);
          long long val = cur.a[k] + carry;
          cur.a[k] = int(val % BASE);
          carry = val / BASE;
          ++k;
        }
      }
    }
    // binary search t in [0, BASE-1] such that Y * t <= cur < Y * (t+1)
    int l = 0, r = BASE - 1, best = 0;
    while (l <= r) {
      int mid = (l + r) >> 1;
      // compute Y * mid
      std::vector<int> t = Y.a;
      mul_scalar(t, mid);
      // compare t with cur
      int cmp;
      int2048 tmpY; tmpY.a = std::move(t); tmpY.neg = false;
      cmp = cmp_abs(tmpY, cur);
      if (cmp <= 0) { best = mid; l = mid + 1; }
      else r = mid - 1;
    }
    Q.a[i] = best;
    if (best) {
      std::vector<int> t = Y.a; mul_scalar(t, best);
      sub_abs_from(cur.a, t);
    }
  }
  Q.trim();
  R = cur; R.trim();
}

// Constructors
int2048::int2048() : neg(false) {}

int2048::int2048(long long v) {
  neg = false; a.clear();
  if (v < 0) { neg = true; v = -v; }
  while (v) { a.push_back(int(v % BASE)); v /= BASE; }
}

int2048::int2048(const std::string &s) { read(s); }

int2048::int2048(const int2048 &other) : a(other.a), neg(other.neg) {}

// Integer1
void int2048::read(const std::string &s) {
  a.clear(); neg = false;
  // parse sign and contiguous digits
  size_t i = 0; while (i < s.size() && isspace((unsigned char)s[i])) ++i;
  if (i < s.size() && (s[i] == '+' || s[i] == '-')) { neg = (s[i] == '-'); ++i; }
  while (i < s.size() && isspace((unsigned char)s[i])) ++i;
  size_t start = i;
  while (i < s.size() && isdigit((unsigned char)s[i])) ++i;
  size_t end = i;
  // strip leading zeros in [start,end)
  while (start < end && s[start] == '0') ++start;
  if (start == end) { neg = false; a.clear(); return; }
  // group by BASE_DIGS from end
  for (size_t j = end; j > start; ) {
    size_t k = (j >= start + BASE_DIGS) ? j - BASE_DIGS : start;
    int val = 0;
    for (size_t p = k; p < j; ++p) val = val * 10 + (s[p] - '0');
    a.push_back(val);
    j = k;
  }
  trim();
}

void int2048::print() {
  if (a.empty()) { std::cout << '0'; return; }
  if (neg) std::cout << '-';
  int n = (int)a.size();
  std::cout << a.back();
  for (int i = n - 2; i >= 0; --i) {
    int x = a[i];
    if (BASE_DIGS == 3) {
      char buf[4]; std::snprintf(buf, sizeof(buf), "%03d", x);
      std::cout << buf;
    } else {
      // generic padding
      std::string s = std::to_string(x);
      if ((int)s.size() < BASE_DIGS) std::cout << std::string(BASE_DIGS - s.size(), '0');
      std::cout << s;
    }
  }
}

int2048 &int2048::add(const int2048 &other) {
  if (other.a.empty()) return *this;
  if (a.empty()) { *this = other; return *this; }
  if (neg == other.neg) {
    add_abs_to(a, other.a);
  } else {
    int cmp = cmp_abs(*this, other);
    if (cmp == 0) { a.clear(); neg = false; return *this; }
    if (cmp > 0) {
      sub_abs_from(a, other.a);
      // sign stays as this.neg
    } else {
      std::vector<int> res = other.a;
      sub_abs_from(res, a);
      a.swap(res);
      neg = other.neg;
    }
  }
  trim();
  return *this;
}

int2048 add(int2048 x, const int2048 &y) { return x.add(y); }

int2048 &int2048::minus(const int2048 &other) {
  int2048 tmp = other;
  tmp.neg = !tmp.neg;
  return add(tmp);
}

int2048 minus(int2048 x, const int2048 &y) { return x.minus(y); }

// Integer2
int2048 int2048::operator+() const { return *this; }
int2048 int2048::operator-() const { int2048 r(*this); if (!r.a.empty()) r.neg = !r.neg; return r; }

int2048 &int2048::operator=(const int2048 &o) { a = o.a; neg = o.neg; return *this; }

int2048 &int2048::operator+=(const int2048 &o) { return add(o); }
int2048 operator+(int2048 x, const int2048 &y) { return x.add(y); }

int2048 &int2048::operator-=(const int2048 &o) { return minus(o); }
int2048 operator-(int2048 x, const int2048 &y) { return x.minus(y); }

int2048 &int2048::operator*=(const int2048 &o) {
  size_t n = a.size(), m = o.a.size();
  if (n == 0 || m == 0) { a.clear(); neg = false; return *this; }
  if (std::min(n, m) < 256) *this = mul_simple(*this, o);
  else *this = mul_fft(*this, o);
  return *this;
}
int2048 operator*(int2048 x, const int2048 &y) { x *= y; return x; }

int2048 &int2048::operator/=(const int2048 &o) {
  // floor division
  if (o.a.empty()) return *this; // undefined
  if (a.empty()) return *this;
  int2048 A = *this; A.neg = false;
  int2048 B = o; B.neg = false;
  int2048 q, r; divmod_abs(A, B, q, r); // q >= 0, 0<=r<|B|
  bool diff = (neg != o.neg);
  if (diff) {
    if (!r.a.empty()) { // need to round toward -inf
      int2048 one(1);
      q.add(one);
    }
    q.neg = !q.a.empty();
  } else {
    q.neg = false;
  }
  q.trim();
  *this = q;
  return *this;
}
int2048 operator/(int2048 x, const int2048 &y) { x /= y; return x; }

int2048 &int2048::operator%=(const int2048 &o) {
  // r = x - (x / y) * y, with floor division
  if (o.a.empty() || a.empty()) { a.clear(); neg = false; return *this; }
  int2048 q = *this / o;
  int2048 prod = q * o;
  *this = *this - prod;
  return *this;
}
int2048 operator%(int2048 x, const int2048 &y) { x %= y; return x; }

std::istream &operator>>(std::istream &is, int2048 &v) {
  std::string s; is >> s; v.read(s); return is;
}

std::ostream &operator<<(std::ostream &os, const int2048 &v) {
  if (v.a.empty()) { os << '0'; return os; }
  if (v.neg) os << '-';
  os << v.a.back();
  for (int i = (int)v.a.size() - 2; i >= 0; --i) {
    int x = v.a[i];
    if (int2048::BASE_DIGS == 3) {
      char buf[4]; std::snprintf(buf, sizeof(buf), "%03d", x);
      os << buf;
    } else {
      std::string s = std::to_string(x);
      if ((int)s.size() < int2048::BASE_DIGS) os << std::string(int2048::BASE_DIGS - s.size(), '0');
      os << s;
    }
  }
  return os;
}

bool operator==(const int2048 &x, const int2048 &y) { return x.neg == y.neg && x.a == y.a; }
bool operator!=(const int2048 &x, const int2048 &y) { return !(x == y); }
bool operator<(const int2048 &x, const int2048 &y) {
  if (x.neg != y.neg) return x.neg;
  int cmp = int2048::cmp_abs(x, y);
  return x.neg ? (cmp > 0) : (cmp < 0);
}
bool operator>(const int2048 &x, const int2048 &y) { return y < x; }
bool operator<=(const int2048 &x, const int2048 &y) { return !(y < x); }
bool operator>=(const int2048 &x, const int2048 &y) { return !(x < y); }

} // namespace sjtu
