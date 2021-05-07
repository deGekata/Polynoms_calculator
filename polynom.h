#ifndef POLYNOM_H
#define POLYNOM_H

#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <utility>

struct block {
  double coef = 0, power = 0;
  char Var = 0;
  block* next;
  block* prev;
  block(double coef, int Var, int power) {
    this->coef = coef;
    this->Var = Var;
    this->power = power;
    this->next = this->prev = NULL;
  };
  void print() {
    // std::cout << "coef=" << coef << "   var=" << Var << "  pow=" << power <<
    // "\n";
    std::cout << "" << coef << "";
    if (Var) {
      std::cout << Var << power;
    }
    std::cout << " ";
  }
};

class Polynom {
 private:
  int size = 0;
  block* head = nullptr;
  block* tail = nullptr;

  const double Lgr = -1e6, Rgr = 1e6;
  const double eps = 1e-12, rEps = 1e-10;

  std::vector<double> _roots(std::vector<double>&);
  double _sign(std::vector<double>&, double);
  void _sort(block*&);
  block* _sortedMerge(block*, block*);
  void _frontBackSplit(block* theHead, block*& frontRef, block*& backRef);

 public:
  char base = 0;
  bool empty();
  int lenght();
  void push_front(block* elem);
  void push_back(block* elem);
  void pop_front();
  void pop_back();
  block* front();
  block* back();
  void sort();
  void simplify();
  std::string print();
  Polynom& deriv();
  Polynom& operator=(Polynom&);
  Polynom& operator+(Polynom&);
  Polynom& operator-(Polynom&);
  Polynom& operator*(Polynom&);
  std::pair<Polynom*, Polynom*>& operator/(Polynom&);
  friend Polynom& parse(std::string& inp);
  // Polynom& operator/(Polynom&);
  // bool sign(double);
  double value_in(double);
  std::pair<char, std::vector<double>> roots();
  std::pair<char, std::vector<int>> Int_roots();
  // std::vector<double> int_roots();
  void clear();
};



bool issign(char inp);

Polynom& parse(std::string& inp);

#endif // POLYNOM_H
