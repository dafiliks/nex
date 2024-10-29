# Nex Lang
> [!CAUTION]
> This project is currently in WIP.

# Backus-Naur Form
So far, the Backus-Naur Form for Nex Lang is as follows: 
```
<program> ::= <funcdecl>
<funcdecl> ::= "int" <name> "(" ")" "{" <stmt> "}"
<stmt> ::= "return" <expr> ";"
<expr> ::= <unop> <expr> | <int>
<unop> ::= "!" | "~" | "-"
```

<br> </br>
<sub> © David Filiks </sub>