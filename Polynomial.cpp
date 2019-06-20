#include <iostream>
#include <algorithm> // For std::erase AND std::remove_if
#include <regex>
#include "Polynomial.h"

using namespace std;

//-------------------------------------------------------------------
//operator<<
ostream& operator<<(ostream& output, const Polynomial& rhs) {
   for (auto it {rhs.polyTerms.crbegin()}; it != rhs.polyTerms.crend(); ++it) {
      int exp_i {it->first};
      double coef_i {it->second};

      output << ' ';
      if (coef_i != 1 && coef_i != -1) {
         output << showpos << coef_i;
      }
      else if (exp_i != 0) {//Terms like +1x or -1x
         output << (coef_i > 0 ? '+' : '-');
      }
      else {
         output << showpos << coef_i;
      }

      if (exp_i != 0) {
         output << "X";
         if (exp_i != 1) {
            output << "^" << noshowpos << exp_i ;
         }
      }
   }
   return output;
}//end of operator<<

//-------------------------------------------------------------------
//constructor
Polynomial::Polynomial(string input) {
   preprocessing(input);

   //tokenize input string
   tokenize(input);
}//end of constructor

//-------------------------------------------------------------------
//operator+
Polynomial Polynomial::operator+(const Polynomial& rhs) const {
   Polynomial temp {*this};

   for (auto const& i : rhs.polyTerms) {
      temp.polyTerms[i.first] += i.second;
      //check if coefficient is zero and if so, delete from polyTerms
      if (temp.polyTerms[i.first] == 0) {
         temp.polyTerms.erase(i.first);
      }
   }

   return temp;
}//end of operator+

//-------------------------------------------------------------------
//operator-
Polynomial Polynomial::operator-(const Polynomial& rhs) const {
   Polynomial temp {*this};

   for (auto const& i : rhs.polyTerms) {
      temp.polyTerms[i.first] -= i.second;
      //check if coefficient is zero and if so, delete from polyTerms
      if (temp.polyTerms[i.first] == 0) {
         temp.polyTerms.erase(i.first);
      }
   }

   return temp;
}//end of operator-

//-------------------------------------------------------------------
//operator*
Polynomial Polynomial::operator*(const Polynomial& rhs) const {
   Polynomial temp;

   for (auto const& term1 : this->polyTerms) {
      for (auto const& term2 : rhs.polyTerms) {
         int exp {term1.first + term2.first};
         double coef {term1.second * term2.second};
         //add result to the term with same exponent
         temp.polyTerms[exp] += coef;
         //check if coefficient is zero and if so, delete from polyTerms
         if (temp.polyTerms[exp] == 0) {
            temp.polyTerms.erase(exp);
         }
      }
   }

   return temp;
}//end of operator*

//-------------------------------------------------------------------
//operator/
//https://en.wikipedia.org/wiki/Polynomial_long_division
pair<Polynomial, Polynomial> Polynomial::operator/(const Polynomial& d) const {
   Polynomial q;//quotient
   Polynomial r {*this};//remainder

   //At each step n = d × q + r
   while (!r.isEmpty() && degree(r) >= degree(d)) {
      //t ← lead(r)/lead(d)     # Divide the leading terms
      Polynomial t {divLeads(lead(r), lead(d))};

      //q ← q + t
      q = q + t;

      //r ← r − t * d
      r = r - t * d;
   }

   return make_pair(q, r);
}//end of operator/

//-------------------------------------------------------------------
//integral
Polynomial Polynomial::integral(const Polynomial& p) {
   Polynomial temp;
   for (auto const& i : p.polyTerms) {
      int exp {i.first};
      double coef {i.second};
      temp.polyTerms[exp + 1] += coef / (exp + 1);
   }

   return temp;
}//end of integral

//-------------------------------------------------------------------
//derivative
Polynomial Polynomial::derivative(const Polynomial& p) {
   Polynomial temp;
   for (auto const& i : p.polyTerms) {
      int exp {i.first};
      double coef {i.second};

      if (exp != 0) {
         temp.polyTerms[exp - 1] += coef * (exp);
      }
   }

   return temp;
}//end of derivative

//-------------------------------------------------------------------
//pre-processing
void Polynomial::preprocessing(string& str) const {
   //append '+' to the beginning of string
   if (str[ 0 ] != '+' && str[ 0 ] != '-') {
      str = '+' + str;
   }

   //convert terms like '+x' and '-x^2'
   //to '+1 x' and '-1 x^2' respectively
   regex e(R"(([+-])\s*([xX]))");
   str = regex_replace(str, e, "$1 1$2");

   //erase spaces
   str.erase(
      remove_if(str.begin(), str.end(), ::isspace), str.end());
}//end of pre-processing

//-------------------------------------------------------------------
//tokenize
void Polynomial::tokenize(const string& str) {

   //tokenize input string
   regex e(R"([-+]?((\d*[xX]\^\d+)|(\d*[xX])|(\d+)))");
   sregex_token_iterator first(str.begin(), str.end(), e), last;

   //convert tokenized string terms to exponent and coefficient
   //and then insert them to map<int, double> polyTerms
   while (first != last) {
      string token {*first++};
      smatch sm;

      // Terms like "-5x^2" , "+2x^3" , "x^5"
      if (regex_match(token, sm, regex(R"(([-+]?\d+)([xX](\^(\d+))?)?)"))) {
         double coef {stod(sm[1])};
         int exp {0};

         string expToken {sm[4].str()};
         if (!sm[2].str().empty()) {
            if (!expToken.empty()) {// "3x^2" , "x^5"
               exp = stoi(expToken);
            }
            else {// "4x" , "-2X" , "1x"
               exp = 1;
            }
         }
         else {// "9" , "-2"
            exp = 0;
         }

         polyTerms[exp] += coef;
         //check if coefficient is zero and if and so, delete from polyTerms
         if (polyTerms[exp] == 0) {
            polyTerms.erase(exp);
         }
      }
   }
}//end of tokenize

//-------------------------------------------------------------------
//degree
int Polynomial::degree(const Polynomial& p) {
   if (p.isEmpty()) {
      return 0;
   }
   else {
      return p.maxTerm().first;
   }
}//end of degree

//-------------------------------------------------------------------
//divLeads calculate lead(r)/lead(d)
Polynomial Polynomial::divLeads(const Polynomial& op1, const Polynomial& op2) {
   int exp_t {op1.maxTerm().first - op2.maxTerm().first};
   double coef_t {op1.maxTerm().second / op2.maxTerm().second};
   map<int, double> map_t {make_pair(exp_t, coef_t)};
   return Polynomial{map_t};
}//end of divLeads
