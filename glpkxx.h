#include <string>
#include <vector>
#include <map>

using namespace std;


#ifndef GLPKXX_LIBRARY_H
#define GLPKXX_LIBRARY_H

class Variable;
class Formula;
class LinearProblem;

class FormulaBase {
    public:
    bool hasHighBnd;
    bool hasLowBnd;
    double lowBnd;
    double highBnd;
    
    FormulaBase();
};

class Variable: public FormulaBase {
    private: 
    string name;
    
    public:
    Variable(string name);
    string getName() const;
    bool operator==(const Variable& v) const;
    Formula operator+(const Variable& v);
    void operator<=(const double& v);
    void operator>=(const double& v);
    // Formula operator<=(const Variable& v);
};

class Formula: public FormulaBase {
    private:
    vector<double> factors;
    vector<Variable> variables;
    // double rhs;

    public:
    Formula();
    void add(double f, Variable v);
    int size();
    const vector<double>& getFactors() const;
    const vector<Variable>& getVariables() const;
    void print();
    Formula& operator+(const Formula& f);
    Formula& operator+(const Variable& v);
    Formula& operator<=(const double& v);
    Formula& operator>=(const double& v);
    // Formula& operator<=(const double r);
};

static Formula operator*(double f, const Variable& v) {
    Formula formula;
    formula.add(f, v);
    return formula;
}

class LinearProblem {
    private: 
    vector<Formula> subjects;
    void simplex(int mode, Formula& f);
    int getBoundMode(const FormulaBase& base);

    public: 
    void addConstraint(Formula f);
    void maximize(Formula f);
    void minimize(Formula f);
};

#endif
