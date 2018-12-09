# glpkxx

A cxx interface for glpk liner programming library.

⚠️: In construction! Not ready to use!

## How to use

Take glpk's example:

-----

maximize

z = 10x<sub>1</sub>+6x<sub>2</sub>+4x<sub>3</sub>

subject to

x<sub>1</sub>+x<sub>2</sub>+x<sub>3</sub> ≤ 100

10x<sub>1</sub>+4x<sub>2</sub>+5x<sub>3</sub> ≤ 600

2x<sub>1</sub>+2x<sub>2</sub>+6x<sub>3</sub> ≤ 300

where

x<sub>1</sub> ≥ 0, x<sub>2</sub> ≥ 0, x<sub>3</sub> ≥ 0

----

In glpkxx:
```c++
LinerProblem problem;
Variable x1("x1"), x2("x2"), x3("x3");
x1 >= 0;
x2 >= 0;
x3 >= 0;
problem.addConstraint(x1 + x2 + x3 <= 100);
problem.addConstraint(10 * x1 + 4 * x2 + 5 * x3 <= 600);
problem.addConstraint(2 * x1 + 2 * x2 + 6 * x3 <= 100);
pair<double, map<Variable,double>> result = problem.minimize(10 * x1 + 6 * x2 + 4 * x3);
cout << result.first << endl; // 733.333
cout << result.second[x1] << endl; // 33.333
cout << result.second[x2] << endl; // 66.667
cout << result.second[x3] << endl; // 0
```