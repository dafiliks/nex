# Documentation

So far, the BNF for Nex Lang is as follows: 
```bnf
<program> ::= <funcdecl>
<funcdecl> ::= "int" <name> "(" ")" "{" <stmt> "}"
<stmt> ::= "return" <expr> ";"
<expr> ::= <int>
```

A simple application that returns 22 would look like:
```c
int main() {
    return 22; // unary ops can be used on return value 
}
```

<sub> © David Filiks </sub>
