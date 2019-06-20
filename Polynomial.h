#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <string>
#include <map>
#include <utility> // for std::pair

class Polynomial {
   friend std::ostream& operator<<(std::ostream&, const Polynomial&);
 public:
   //constructors
   Polynomial() {};
   explicit Polynomial(const std::map<int, double>& n) : polyTerms{n} {};
   explicit Polynomial(std::string);

   //delegating constructor
   explicit Polynomial(const char* s) : Polynomial{std::string{s}} {};

   //arithmetic operators
   Polynomial operator+(const Polynomial&) const;
   Polynomial operator-(const Polynomial&) const;
   Polynomial operator*(const Polynomial&) const;
   std::pair<Polynomial, Polynomial> operator/(const Polynomial&) const;

   static Polynomial integral(const Polynomial&);
   static Polynomial derivative(const Polynomial&);

 private:
   std::map<int, double> polyTerms;

   //utility functions
   void preprocessing(std::string&) const;
   void tokenize(const std::string&);
   bool isEmpty() const {return polyTerms.empty();}
   std::pair<int, double> maxTerm() const {return *polyTerms.crbegin();}
   static int degree(const Polynomial&);
   static Polynomial lead(const Polynomial& p) {return Polynomial{std::map<int, double> {p.maxTerm()}};}
   static Polynomial divLeads(const Polynomial&, const Polynomial&);
};

#endif /* POLYNOMIAL_H */
