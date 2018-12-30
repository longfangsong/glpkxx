#include "glpkxx.h"
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <glpk.h>
#include <utility>


FormulaBase::FormulaBase() {
    hasHighBnd = false;
    hasLowBnd = false;
    highBnd = 0;
    lowBnd = 0;
}

Variable::Variable(string name) {
    this -> name = name;
}

bool Variable::operator==(const Variable& v) const {
    if (name.compare(v.getName()) == 0) {
        return true;
    }
    
    return false;
}

Formula Variable::operator+(const Variable& v) {
    Formula formula;
    formula.add(1, *this);
    formula.add(1, v);
    return formula;
}

void Variable::operator<=(const double& v) {
    hasHighBnd = true;
    highBnd = v;
}

void Variable::operator>=(const double& v) {
    hasLowBnd = true;
    lowBnd = v;
}

string Variable::getName() const {
    return name;
}

Formula::Formula() {
}

void Formula::add(double f, Variable v) {
    factors.push_back(f);
    variables.push_back(v);
}

int Formula::size() {
    return factors.size();
}

const vector<double>& Formula::getFactors() const {
    return factors;
}

const vector<Variable>& Formula::getVariables() const {
    return variables;
}

void Formula::print() {
    stringstream s;
    for (int i = 0; i < size(); i ++) {
        s << factors[i] << " * " << variables[i].getName();
        if (i != size() - 1) 
            s << " + ";
    }
    // s << " <= " << rhs;
    cout << s.str() << endl;
}

Formula& Formula::operator+(const Formula& f) {
    factors.push_back(f.getFactors()[0]);
    variables.push_back(f.getVariables()[0]);
    return *this;
}

Formula& Formula::operator+(const Variable& v) {
    factors.push_back(1);
    variables.push_back(v);
    return *this;
}

Formula& Formula::operator<=(const double& v) {
    hasHighBnd = true;
    highBnd = v;
    return *this;
}

Formula& Formula::operator>=(const double& v) {
    hasLowBnd = true;
    lowBnd = v;
    return *this;
}

void LinearProblem::addConstraint(Formula f) {
    subjects.push_back(f);
}

void LinearProblem::maximize(Formula f) {
    return simplex(GLP_MAX, f);
}

int LinearProblem::getBoundMode(const FormulaBase& base) {
    int bnd = GLP_FR;
    if (base.hasHighBnd && base.hasLowBnd) {
        bnd = GLP_DB;
    } else if (base.hasHighBnd) {
        bnd = GLP_UP;
    } else if (base.hasLowBnd) {
        bnd = GLP_LO;
    }
    return bnd;
}

void LinearProblem::simplex(int mode, Formula& f) {
    glp_prob* lp;
    int size = f.size() * subjects.size();
    int* ia = new int[size];
    int* ja = new int[size];
    double* ar = new double[size];

    lp = glp_create_prob();
    glp_set_obj_dir(lp, mode);
    
    glp_add_rows(lp, subjects.size());
    for (int i = 0; i < subjects.size(); i ++) {
        int bnd = getBoundMode(subjects[i]);
        glp_set_row_bnds(lp, i + 1, bnd, subjects[i].lowBnd, subjects[i].highBnd);
    }

    const vector<Variable>& variables = f.getVariables();
    
    glp_add_cols(lp, f.size());
    for (int i = 0; i < f.size(); i ++) {
        int bnd = getBoundMode(variables[i]);
        glp_set_col_bnds(lp, i + 1, bnd, variables[i].lowBnd, variables[i].highBnd);
        glp_set_obj_coef(lp, i + 1, f.getFactors()[i]);
    }

    for (int i = 0; i < subjects.size(); i ++) {
        for (int j = 0; j < variables.size(); j ++) {
            int idx = i * variables.size() + j + 1;
            ia[idx] = i + 1;
            ja[idx] = j + 1;
            ar[idx] = 0;
            
            // find factor with the variable in current cell
            for (int k = 0; k < variables.size(); k ++) {
                if ((variables[j]) == (subjects[i].getVariables()[k])) {
                    ar[idx] = subjects[i].getFactors()[k];
                    // cout << ar[idx] << endl;
                    break;
                }
            }
        }
    }
    
    glp_load_matrix(lp, size, ia, ja, ar);
    glp_simplex(lp, NULL);
    
    double z = glp_get_obj_val(lp);
    cout << z << endl;
    for (int i = 0; i < variables.size(); i ++) {
        cout << glp_get_col_prim(lp, i + 1) << endl;
    }
    
    glp_delete_prob(lp);
    glp_free_env();
    
    delete [] ia;
    delete [] ja;
    delete [] ar;
}
