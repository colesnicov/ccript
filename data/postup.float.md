float prvorade2;


- if isalpha(ch)
 |- parseSymbolIdentifier(&parser, phrase_type)
 |- if phrase_type == "float"
   |- ParseDefineTypeFloat(&parser) =>
     |- if isalpha(ch)
          |- parseSymbolIdentifier(_parser, _name, &_len)
          |- if ch == ';'
            |- varCreate(_parser, phrase_name, phrase_name_len, _type)


float prvorade1 = 22.2;


- if isalpha(ch)
 |- parseSymbolIdentifier(&parser, phrase_type)
 |- if phrase_type == "float"
   |- ParseDefineTypeFloat(&parser) =>
   |- if isalpha(ch)
     |- parseSymbolIdentifier(_parser, _name, &_len)
      |- if ch == '='
        |- parseVarArgsFloat(_parser, ';', &fval) =>
        |- while bufferValid():
          |- if isdigit(ch) || ch == '-'|| ch == '.'
            |- parseValueFloat(_parser, value_name, &value_len, &is_float)
            |- if ch == _symbol_end // ';'
                |- varCreate(_parser, phrase_name, phrase_name_len _type)
                |- varVarValueSetFloat(_parser, var, fval)

prvorade2 = prvorade1;


- if isalpha(ch)
 |- parseSymbolIdentifier(&parser, phrase_type)
 |- if phrase_type == "float"
   |- ParseDefineTypeFloat(&parser) =>
   |- if isalpha(ch)
     |- parseSymbolIdentifier(_parser, _name, &_len)
      |- if ch == '='
        |- parseVarArgsFloat(_parser, ';', &fval) =>
        |- while bufferValid():
          |- if isdigit(ch) || ch == '-'|| ch == '.'
            |- parseValueFloat(_parser, value_name, &value_len, &is_float)
            |- if ch == _symbol_end // ';'
                |- varCreate(_parser, phrase_name, phrase_name_len _type)
                |- varVarValueSetFloat(_parser, var, fval)



float prvorade1 = 10 + 10;

- if isalpha(ch)
 |- parseSymbolIdentifier(&parser, phrase_type)
 |- if phrase_type == "int"
   |- ParseDefineTypeNumeric(&parser, phrase_type, VAR_TYPE_INT)  [numeric.c] =>
   |- if isalpha(ch)
     |- parseSymbolIdentifier(_parser, phrase_name)
     |- if ch == '='
       |- while bufferValid():
         |- if isdigit(ch) || ch == '-'
           |- skipIntValue33(_parser, value_name)
             |- if ch == '+'
               | // prace s promennou
               |- continue
             |- if ch == ';''
               |- varCreateAssign2(_parser, phrase_name, phrase_name_len, value_name, value_len, _type)



int prvorade2 = prvorade1; 

- if (isalpha(ch)
 |- parseSymbolIdentifier(&parser, phrase_type)
 |- if phrase_type == "int"
   |- if ch == '='
     |- while bufferValid():
       |- if !isdigit(ch) && ch != '-'
         |- parseSymbolIdentifier(_parser, value_name)
           |- if VarGetValue(value_name)
             |- if ch == ';''
               |- varCreateAssign2(_parser, phrase_name, phrase_name_len, value_name, value_len, _type)


prvorade2 = prvorade1; 

- if (isalpha(ch)
 |- parseSymbolIdentifier(&parser, phrase_type)
 |- if ch == '='
   |- parseVarAssignment33(&parser, phrase_type) => 
     |- if varGetNamed(_var_name, &var)
       |- while bufferValid():
         |- if ParseIntTypeAssign33(_parser, _var_name, VAR_TYPE_INT) ->
       |- while bufferValid():
         |- if !isdigit(ch) && ch != '-'
           |- parseSymbolIdentifier(_parser, value_name)
             |- if VarGetValue(value_name)
               |- if ch == ';''
                 |- varSet(_type_name, value_name)

prvorade2= prvorade1 + 4

- if (isalpha(ch)
 |- parseSymbolIdentifier(&parser, phrase_type)
 |- if ch == '='
   |- parseVarAssignment33(&parser, phrase_type) => 
     |- if varGetNamed(_var_name, &var)
       |- if ParseIntTypeAssign33(_parser, _var_name, VAR_TYPE_INT) ->
         |- while bufferValid():
           |- if !isdigit(ch) && ch != '-'
             |- parseSymbolIdentifier(_parser, value_name)
             |- if VarGetValue(value_name)
               |- if ch == '+''
                 |- continue
               |- if ch == ';'
                   |- varSet(_type_name, value_name)

 


prvorade1 = 8;


- if (isalpha(ch)
 |- parseSymbolIdentifier(&parser, phrase_type);
   |- if ch == '=' // next ch
     |- parseVarAssignment33(&parser, phrase_type) => 
     |- if varGetNamed(_var_name, &var)
       |- if var.type == INT
         |- ParseIntTypeAssign33(_parser, _var_name, VAR_TYPE_INT) =>
         |- while bufferValid():
           |- if isdigit(ch) || ch == '-'
             |- SkipIntValue33(_parser, phrase_name)
             |- if ch == ';'
                |- if varSet(var.name, phrase_name)




delete prvorade1;


- if (isalpha(ch)
 |- parseSymbolIdentifier(&parser, phrase_type);
 |- if phrase_type == "delete"
   |- parseVarDelete33(&parser, phrase_type) =>
   |- if isalpha(ch)
     |- parseSymbolIdentifier(_parser, var_name)
     |- if ch == ';'
       |- if varGetNamed(value_name, &var)
         |- varDelete(var.name, strlen(var.name))



 







