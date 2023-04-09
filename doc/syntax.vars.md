# CCRipt - VARS

[zpet](../readme.md)

## Promenne

CCRipt umi pracovat s promennymi. Umi je vytvaret, ukladat/ziskavat/menit data v nich a odstranovat je.

CCript <u>implementuje spravce pameti pro spravu zivotnosti promennych</u>.
Promenna ma svou zivotnost jen v ramci skripty nebo bloku ve kterem byla vytvorena a **pri 'vystupu' z bloku je automaticky odstranena**.

### Prototyp

```
<type> <name> [ = <data>];
```

- Typ muze byt jakykoliv z podporovanych.
- Nazev promenne musi zacinat pismenem!
- Data se mohou prirazovat k promenne kdykoliv v prubehu programu.


## Definice a prirazeni

Pred prvnim pouzitim se musi promena definovat/vytvorit.

### Definice

```
int number1;
int number2;
```

### Definice a prirazeni

```
int number1 = 5;
int number2 = 2;
```


### Prirazeni

```
number1 = 10;
number2 = number1;
```

### Duplicitni nazvy

Promenne museji mit jedinecne nazvy jinak program selze:

```
int number1 = 0;
// int number1 = 2; // error, duplicate variable 'number1'
int number2 = 2; // Je treba pouzit jiny nazev
```

## Mazani 

I zde plati pravidlo zivotnosti a viditelnosti promenne v ramci bloku.
 
```
int number1 = 0;
{
  int number2 = number1;
}
number2 += 1; // error, undefined variable 'number2'
```

### Odstranit muzu kdykoliv: 

```
delete number1;
delete number2;
 ```
 
### Nekdy take musim
 
```
int number1 = 0;
// int number1 = 2; // error, duplicate variable 'number1'

delete number1;

float number1 = 2.3; // ok
 ```

 