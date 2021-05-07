#include "polynom.h"
#include <QDebug>

std::string Polynom::print() {
  std::string res;
  auto elem = this->back();
  while (elem != nullptr) {
    if (elem->coef >= 0 && elem != tail) {
      res+="+";
    }
    std::stringstream ss;
    ss << elem->coef;
    std::string str = ss.str();
    res += str;
    if (elem->power != 0) {

      res = res + std::string(1, elem->Var);
      res+="^";
      res+=std::to_string(int(elem->power));
    }
    elem = elem->prev;
  }
  return res;
}
bool Polynom::empty() { return size == 0; }

int Polynom::lenght() { return size; }

void Polynom::push_front(block* elem) {
  block* nde = elem;
  if (head == nullptr) {
    head = tail = nde;
    nde->next = nullptr;
  } else {
    nde->next = head;
    head->prev = nde;
    head = nde;
  }
  if (nde->power == 0) {
    nde->Var = 0;
  }
  ++size;
}

void Polynom::push_back(block* elem) {
  block* nde = elem;
  if (tail == nullptr) {
    head = tail = nde;
    nde->prev = nullptr;
  } else {
    nde->prev = tail;
    tail->next = nde;
    tail = nde;
  }
  if (nde->power == 0) {
    nde->Var = 0;
  }
  ++size;
}

void Polynom::pop_front() {
  if (head == nullptr) {
    std::cout << "error\n";
    return;
  } else {
    block* nde;
    nde = head;
    head = nde->next;
    delete nde;
    if (head == nullptr) {
      tail = nullptr;
    } else {
      head->prev = nullptr;
    }
  }
  --size;
}

void Polynom::pop_back() {
  if (tail == nullptr) {
    std::cout << "error\n";
    return;
  } else {
    block* nde;
    nde = tail;
    tail = nde->prev;
    delete nde;
    if (tail == nullptr) {
      head = nullptr;
    } else {
      tail->next = nullptr;
    }
  }
  --size;
}

block* Polynom::back() { return tail; }

block* Polynom::front() { return head; }

Polynom& Polynom::deriv() {
  Polynom* ans = new Polynom;
  block* blk = this->tail;
  while (blk != nullptr) {
    if (blk->power != 0) {
      ans->push_front(new block(blk->coef * blk->power, this->base, blk->power - 1));
    }
    blk = blk->prev;
  }
  ans->sort();
  ans->simplify();
  ans->base = this->base;
  return *ans;
}

Polynom& Polynom::operator=(Polynom& inp) {
  this->clear();
  block* pointer = inp.front();
  while (pointer != nullptr) {
    block* add = new block(pointer->coef, pointer->Var, pointer->power);
    this->push_back(add);
    pointer = pointer->next;
  }
  this->base = inp.base;
  return *this;
}

Polynom& Polynom::operator+(Polynom& inp) {
  Polynom* poly = new Polynom;
  block* elem = this->head;
  while (elem != nullptr) {
    poly->push_back(new block(elem->coef, elem->Var, elem->power));
    elem = elem->next;
  }
  elem = inp.head;
  while (elem != nullptr) {
    poly->push_back(new block(elem->coef, elem->Var, elem->power));
    elem = elem->next;
  }
  poly->sort();
  poly->simplify();
  return *poly;
}

Polynom& Polynom::operator-(Polynom& inp) {
  Polynom* poly = new Polynom;
  block* elem = this->head;
  while (elem != nullptr) {
    poly->push_back(new block(elem->coef, elem->Var, elem->power));
    elem = elem->next;
  }
  elem = inp.head;
  while (elem != nullptr) {
    poly->push_back(new block(-elem->coef, elem->Var, elem->power));
    elem = elem->next;
  }
  poly->sort();
  poly->simplify();
  return *poly;
}

Polynom& Polynom::operator*(Polynom& inp) {
  if (!(this->base == inp.base || this->base == 0 || inp.base == 0)) {
    throw std::invalid_argument("Impossible operation");
  }
  Polynom* poly = new Polynom;
  block* elem = this->head;
  while (elem != nullptr) {
    block* elem2 = inp.head;
    while (elem2 != nullptr) {
      block* to_add =
          new block(elem->coef * elem2->coef,
                    (elem->Var > elem2->Var) ? elem->Var : elem2->Var,
                    elem->power + elem2->power);

      poly->push_back(to_add);
      elem2 = elem2->next;
    }
    elem = elem->next;
  }
  poly->sort();
  poly->simplify();
  return *poly;
}


std::pair<Polynom*, Polynom*>& Polynom::operator/(Polynom& inp) {
  if (inp.lenght() == 1 && inp.head->coef == 0) {
    throw std::invalid_argument("Division by zero");
  }

  if (!(inp.base == this->base || inp.base == 0 || this->base == 0)) {
    throw std::invalid_argument("Different base`s");
  }

  Polynom* quot = new Polynom;
  Polynom* rem = new Polynom;

  std::vector<double> divided(this->tail->power + 1, 0), devider(inp.tail->power + 1, 0);
  block* iter = this->tail;
  while (iter != nullptr){
    divided[divided.size() - 1 - iter->power] = iter->coef;
    iter = iter->prev;
  }
  iter = inp.tail;
  while (iter != nullptr) {
    devider[devider.size() - 1 - iter->power] = iter->coef;
    iter = iter->prev;
  }
  std::vector<std::pair<double, int>> ans;
  for (int it = 0; it < int(divided.size()) - int(devider.size()) + 1; ++it) {
    double n_coef = divided[it] / devider[0];
    int d_pow = divided.size() - devider.size() - it;
    for (int tm = 0; tm < devider.size(); ++tm) {
      divided[it + tm] -= devider[tm] * n_coef;
    }
    divided[it] = 0;
    char Var = this->base;
    if (it == divided.size() - devider.size()) {
      Var = 0;
    }
    block* to_add = new block(n_coef, Var, d_pow);
    quot->push_back(to_add);
  }
  for (int i = 0; i < divided.size(); ++i) {
    rem->push_back(new block(divided[i], this->base, divided.size() - i - 1));
  }

  if (quot->size == 0) {
    quot->push_back(new block(0, 0, 0));
  }

  quot->sort();
  quot->simplify();

  rem->sort();
  rem->simplify();

  return *(new std::pair<Polynom*, Polynom*>(quot, rem));
}

void Polynom::clear() {
  block* nde;
  while (head != nullptr) {
    nde = head->next;
    delete head;
    head = nde;
  }
  tail = head;
  size = 0;
  base = 0;
}

double Polynom::_sign(std::vector<double>& poly, double x_cord) {
  double ans = 0;
  for (int i = 0; i < poly.size(); ++i) {
    ans += poly[i] * std::pow(x_cord, i);
  }
  if (ans > rEps) {
    return 1;
  }
  if (fabs(ans) < rEps) {
    return 0;
  }
  return -1;
}

std::vector<double> Polynom::_roots(std::vector<double>& poly) {
  if (poly.size() == 2) {
    return std::vector<double>{-poly[0] / poly[1]};
  }
  std::vector<double> new_pol(poly.size() - 1);
  for (int i = 1; i < poly.size(); ++i) {
    new_pol[i - 1] = poly[i] * i;
  }

  std::vector<double> deriv = _roots(new_pol);
  deriv.push_back(Rgr);
  std::vector<double> ans;
  double left = Lgr;
  for (int iter = 0; iter < deriv.size(); ++iter) {
    double right = deriv[iter];
    if (_sign(poly, left) * _sign(poly, right) > rEps) {
      continue;
    } else if (_sign(poly, left) * _sign(poly, right) == 0) {
      if (!_sign(poly, left)) {
        right = left;
      } else {
        left = right;
      }
    }
    while (right - left > eps) {
      double mid = (right + left) / 2;
      if (_sign(poly, mid) * _sign(poly, left) > eps) {
        left = mid;
      } else if (!_sign(poly, mid)) {
        left = right = mid;
      } else {
        right = mid;
      }
    }
    ans.push_back(right);
    left = deriv[iter];
  }
  return ans;
}

double Polynom::value_in(double dot) {
  block* iter = head;
  double res = 0;
  while (iter != nullptr) {
    res += iter->coef * pow(dot, iter->power);
    iter = iter->next;
  }
  return res;
}

std::pair<char, std::vector<double>> Polynom::roots() {
  block* elem = this->back();
  char verd;
  verd = 1;
  if (this->size == 1 && this->front()->power == 0) {
    if (this->front()->coef == 0) {
      verd = 0;
      return std::pair<char, std::vector<double>>{verd, std::vector<double>(0)};
    } else {
      verd = -1;
      return std::pair<char, std::vector<double>>{verd, std::vector<double>(0)};
    }
  }
  std::vector<double> poly(tail->power + 1);
  for (int i = 0; i < poly.size(); ++i) {
    poly[i] = 0;
  }
  poly[elem->power] = elem->coef;
  for (int it = 1; it < this->size; ++it) {
    elem = elem->prev;
    poly[elem->power] = elem->coef;
  }
  std::vector<double> ans = _roots(poly);
  for (int it = 0; it < ans.size(); ++it) {
    if (fabs(ans[it]) < (rEps * 10)) {
      ans[it] = 0;
    }
  }
  return std::pair<char, std::vector<double>>{verd, ans};
}

std::pair<char, std::vector<int>> Polynom::Int_roots() {

  auto roots = this->roots();
  if(roots.first == -1){
    return std::pair<char, std::vector<int>>{-1, std::vector<int>(0)};
  }
  if(roots.first == 0){
    return std::pair<char, std::vector<int>>{0, std::vector<int>(0)};
  }
  std::vector<int> int_roots;
  for (int it = 0; it < roots.second.size(); ++it) {
    if ( fabs(round(roots.second[it]) - roots.second[it]) < 1e-8) {
      int_roots.push_back(round(roots.second[it]));
    }
  }
  return std::pair<char, std::vector<int>>{1, int_roots};;
}

void Polynom::sort() {
  _sort(head);
  block* findTail = head;
  while (findTail != nullptr) {
    if (findTail->next == nullptr) {
      tail = findTail;
    }

    findTail = findTail->next;
  }
}

void Polynom::_sort(block*& theHead) {
  block* a = nullptr;
  block* b = nullptr;

  // Base case
  if (theHead == nullptr || theHead->next == nullptr) {
    return;
  }
  _frontBackSplit(theHead, a, b);

  _sort(a);
  _sort(b);

  theHead = _sortedMerge(a, b);
}

block* Polynom::_sortedMerge(block* a, block* b) {
  block* headOfMerged;

  // If one of the block is nullptr, return the other block
  // No merging occurs this this case
  if (a == nullptr) {
    return b;
  } else if (b == nullptr) {
    return a;
  }

  // First element in list, a, is less than the first element in b
  if (a->power <= b->power) {
    headOfMerged = a;

    while (b != nullptr) {
      if (a->power <= b->power) {
        if (a->next == nullptr) {
          a->next = b;
          b->prev = a;
          break;
        }
        a = a->next;
      } else {
        block* toAdd = b;
        b = b->next;
        toAdd->prev = a->prev;
        a->prev->next = toAdd;
        toAdd->next = a;
        a->prev = toAdd;
      }
    }
  }
  // First element in list, b, is less than the first element in a
  else {
    headOfMerged = b;

    while (a != nullptr) {
      if (b->power <= a->power) {
        if (b->next == nullptr) {
          b->next = a;
          a->prev = b;
          break;
        }
        b = b->next;
      } else {
        block* toAdd = a;
        a = a->next;
        toAdd->prev = b->prev;
        b->prev->next = toAdd;
        toAdd->next = b;
        b->prev = toAdd;
      }
    }
  }

  return headOfMerged;
}

/**
 *  Utility function for merge sort
 *  @param frontRef block* reference pointer that will point to the head of
 * first sorted half
 *  @param backRef block* reference pointer that will point to the head of
 * second sorted half
 */
void Polynom::_frontBackSplit(block* theHead, block*& frontRef,
                              block*& backRef) {
  block* fast;
  block* slow;

  if (theHead == nullptr || theHead->next == nullptr) {
    frontRef = theHead;
    backRef = nullptr;
  } else {
    slow = theHead;
    fast = theHead->next;

    // Fast advances 2 blocks while slow advances 1 block
    while (fast != nullptr) {
      fast = fast->next;

      if (fast != nullptr) {
        slow = slow->next;
        fast = fast->next;
      }
    }

    // slow should be pointing right before midpoint. Split at this point
    frontRef = theHead;
    backRef = slow->next;

    // Update the prev and next pointers
    backRef->prev = nullptr;
    slow->next = nullptr;
  }
}

void Polynom::simplify() {
  Polynom* pol = new Polynom;
  block* temp = this->head;
  int cur_p = temp->power;
  double cur_coef = 0;
  char cur_var = temp->Var;
  char base = 0;
  while (temp != nullptr) {
    if (temp->power == cur_p) {
      cur_coef += temp->coef;
      if (temp->next == nullptr) {
        if (cur_coef != 0) {
          base = std::fmax(base, cur_var);
          block* add = new block(cur_coef, cur_var, cur_p);
          pol->push_back(add);
        }
      }
    } else {
      if (cur_coef != 0) {
        base = std::fmax(base, cur_var);
        block* add = new block(cur_coef, cur_var, cur_p);
        pol->push_back(add);
      }
      cur_var = temp->Var;
      cur_coef = temp->coef;
      cur_p = temp->power;
      if (temp->next == nullptr) {
        if (cur_coef != 0) {
          base = std::fmax(base, temp->Var);
          block* add = new block(cur_coef, temp->Var, cur_p);
          pol->push_back(add);
        }
      }
    }
    temp = temp->next;
  }

  (*this) = *pol;
  this->base = base;
  if (this->lenght() == 0) {
    block* to_add = new block(0, 0, 0);
    this->push_back(to_add);
    this->base = 0;
  }
}

bool issign(char inp) { return (inp == '+' || inp == '-'); }

Polynom& parse(std::string& inp) {
  int pos = 0;
  Polynom* polynom = new Polynom;
  int tact, cur_pow, p_len, c_len;
  long long sign;
  double cur_coef = 0;
  char Var;
  sign = tact = cur_coef = cur_pow = p_len = c_len = 0;
  // tact = 0 - Знак
  // tact = 1 - коеф
  // tact = 2 - переменная
  // tact = 3 - знак степень
  // tact = 4 - степень
  for (pos = 0; pos < inp.length();) {
    if (!isalnum(inp[pos]) && inp[pos] != '+' && inp[pos] != '-' &&
        inp[pos] != '^') {
      std::string message = "Unexpected character '" + std::string(1, inp[pos]) + "'  on position " + std::to_string(pos);
      throw std::invalid_argument(message);
    }
    if (tact == 0) {
      // sign
      if (sign) {
        std::string message =
            "Expected  coefficient, found sign '" + std::string(1, inp[pos]) + "'  on position " + std::to_string(pos);
        throw std::invalid_argument(message);
      }
      if (inp[pos] == '+' || inp[pos] == '-') {
        sign = (inp[pos] == '+') ? 1 : -1;
        if (pos == inp.length() - 1) {
          std::string message =
              "Expected  coefficient, found sign '" + std::string(1, inp[pos]) + "'  on position " + std::to_string(pos);
          throw std::invalid_argument(message);
        }
        ++tact;
        ++pos;
        continue;
      } else {
        sign = 1;
        ++tact;
      }
    }

    // coef
    if (tact == 1) {
      if (!isdigit(inp[pos])) {
        if (!c_len) {
          std::string message = "Expected  coefficient, found '" +
                                std::string(1, inp[pos]) + "'  on position " +
                                std::to_string(pos);
          throw std::invalid_argument(message);
        }
        ++tact;
      } else {
        ++c_len;
        cur_coef = cur_coef * 10 + inp[pos] - '0';
        if (pos == inp.length() - 1) {
          block* to_add = new block(cur_coef * sign, 0, 0);
          cur_coef = 0;
          sign = 0;
          cur_pow = 0;
          c_len = 0;
          polynom->push_front(to_add);
        }
        ++pos;
        continue;
      }
    }

    // Var
    if (tact == 2) {
      if (!isalpha(inp[pos])) {
        if (!issign(inp[pos])) {
          std::string message = "Expected  coefficient or sign, found '" +
                                std::string(1, inp[pos]) + "'  on position " +
                                std::to_string(pos);
          throw std::invalid_argument(message);
        }
        block* to_add = new block(cur_coef * sign, 0, 0);
        cur_coef = 0;
        sign = 0;
        tact = 0;
        c_len = 0;
        polynom->push_front(to_add);
        continue;
      } else {
        Var = inp[pos];
        if (polynom->base != 0 && polynom->base != Var) {
          std::string message = "Expected  the same variable, found '" +
                                std::string(1, inp[pos]) + "'  on position " +
                                std::to_string(pos);
          throw std::invalid_argument(message);
        }
        polynom->base = std::fmax(polynom->base, Var);
        ++tact;
        if (pos == inp.length() - 1) {
          block* to_add = new block(cur_coef * sign, Var, 1);
          Var = 0;
          cur_coef = 0;
          sign = 0;
          cur_pow = 0;
          c_len = 0;
          polynom->push_front(to_add);
        }
        ++pos;
        continue;
      }
    }

    // ^ sym
    if (tact == 3) {
      if (issign(inp[pos])) {
        block* to_add = new block(cur_coef * sign, Var, 1);
        sign = 0;
        cur_coef = 0;
        Var = 0;
        cur_pow = 0;
        tact = 0;
        c_len = 0;
        polynom->push_front(to_add);
        continue;
      }
      if (inp[pos] != '^') {
        std::string message = "Expected  ^, found '" +
                              std::string(1, inp[pos]) + "'  on position " +
                              std::to_string(pos);
        throw std::invalid_argument(message);
      } else {
        ++tact;
        ++pos;
        continue;
      }
    }

    // power
    if (tact == 4) {
      if (!isdigit(inp[pos])) {
        if (issign(inp[pos]) && p_len) {
          Var = cur_pow ? Var : 0;
          block* to_add = new block(cur_coef * sign, Var, cur_pow);
          cur_coef = 0;
          sign = 0;
          cur_pow = 0;
          p_len = 0;
          c_len = 0;
          polynom->push_front(to_add);
          tact = 0;
          continue;
        }
        std::string message = "Expected  power, found '" +
                              std::string(1, inp[pos]) + "'  on position " +
                              std::to_string(pos);
        throw std::invalid_argument(message);
      } else {
        ++p_len;
        cur_pow = cur_pow * 10 + inp[pos] - '0';
        if (pos == inp.length() - 1) {
          Var = cur_pow ? Var : 0;
          block* to_add = new block(cur_coef * sign, Var, cur_pow);
          cur_coef = 0;
          sign = 0;
          cur_pow = 0;
          p_len = 0;
          c_len = 0;
          polynom->push_front(to_add);
        }
        ++pos;
        continue;
      }
    }
  }

  if (polynom->lenght() == 0) {
    std::string message = "zero polynom lenght";
    throw std::invalid_argument(message);
  }

  polynom->sort();
  polynom->simplify();
  return *polynom;
}
