# CCRipt - Pravdivostni logika

[zpet](../readme.md)

CCRipt disponuje operatory pro praci s jednoduchou pravdivostni logikou. Tato logika je pouzita pri praci s typem `bool` se smyckou `while` a podminkou `if`. 
 

## Kondicni podminky

Pravdivostni logika umi pracovat s temito typy:
  
 - `bool`
 - `int`
 - `float`
 - funkci ktera vraci jeden z vyse uvedenych typu

Zpracovava porovnavaci operatory:
 - `==` - rovna se
 - `!=` - lisi se
 - `>`  - vetsi
 - `<`  - mensi
 - `>=` - vetsi nebo rona se
 - `<=` - mensi nebo rona se
 
Podminky je mozne retezit s pomoci techto operatoru:
 - `&&` - logicke 'AND'
 - `||` - logicke 'OR'
 
A seskupovat s pomoci zavorek `(` a `)`. 

```
number1 == number2; // rovna se
number1 < number2; // mensi nez
number1 >= number2; // vetsi nez nebo rovna se
number1 >= number2; // mensi nez nebo rona se
number1 && number2; // a take
number1 || number2; // nebo
number1 && (number2 || number3); // a take nebo
```

### aritmetika
 
V pravdivostne logice je mozne provadet jednoduche matematicke vypocty.
 - `+` - scitani
 - `-` - odecitani
 - `*` - nasobeni
 - `/` - deleni

Napriklad tuto je v poradku:

```
bool pravdivost1 = a - b > 0; // spravne
```

ale tuto jiz selze:

```
bool pravdivost1 = a--; // spatne
```
   
### retezeni podminek

Ukonci se az 'number1' bude vetsi nez '0' a 'number2' bude mensi nebo rovnat se '5'.

```
int number1 = 15;
int number2 = 10;

while(number1 && (number2 <= 5)) {
  number1--;
  number2--;
}
```
   

## Priorita logickych operaci

Protoze zpracovani skript probiha postupnym ctenim *zleva doprava*, je dulezite si dobre naplanovat prioritu vypoctu.

Na rozdil od plnehodnotneho C, kde je priorita operatoru urcovana na zaklade priority operatoru, ve skriptach *ccript* **se poradi vypoctu urcuje s pomoci zavorek '('** a **')'**!

To znamena ze pro vyhodnoceni podminke jestli je **A je `true` a zaroven je take `true` B nebo C** tak musime pouzit zavorek nebo spravne poradi operaci.

#### spatne

```
pravdivost1 = A && B || C;
```

#### spravne

```
pravdivost1 = A && (B || C);
```

nebo

```
pravdivost1 = B || C && A;
```

**Je tedy** <u>nutno pouzit zavorky</u>: 

```
pravdivost1 = A && (B || C);
                   ^      ^
```

nebo <u>usporadat poradi operaci</u> tak aby vysledek byl spravny:

```
pravdivost1 = A && B || C; // vysledek je spatny

pravdivost1 = A && (B || C); // vysledek je spravny

pravdivost1 = B || C && A; // vysledek je spravny
```



