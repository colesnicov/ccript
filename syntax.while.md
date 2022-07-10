#CCRipt <sub>*An Embeddable Interpreter for a 'C' Like Scripting Language*</sub>

###### *version* v.1b0

[NAHORU](readme.md)

### Smycky

CCRipt nabizi smycky. K dispozici je smycka `while` a `do...while //(under construction)`. K rizeni stavu smycky jsou tez k dispozici klicova slova `continue` a `break`.

Smycky muzou byt **zanorene do sebe**, mohou se **predcasne ukoncit** (`break`) a mohou se predcasne **vratit na zacatek bloku** (`continue`).

V podstate se prace se smyckami nelisi od smycek v jazyce 'C'.

##### WHILE a DO...WHILE

Jednou z moznych konstrukci smycek kterou **ccript** zpracovava je [smycka WHILE](https://en.wikipedia.org/wiki/While_loop) a druhou je [smycka DO...WHILE](https://en.wikipedia.org/wiki/Do_while_loop).

K vykonani bloku smycky `while` dojde, na rozdil od `do...while` pouze pri splneni podminky kdez to v pripade smycky `do...while` nejdriv probehne vykonani bloku smycky `do...while` a az pote k overeni jestli je podminka splena.

-----------------------

##### *Prototyp*:

```
while(<cond>) {
  // do work here ...
  [continue]
  [break]
}
```

nebo

```
do {
  // do work here ...
  [continue]
  [break]
} while(<cond>);
```

-----------------------

_Nadale jako priklad se bude pouzivat smycka_  `while`  _ale to same plati i pro smycku_  `do...while`.

##### *Kondicni podminky*:

Stejne jako pominka `if`)

Jako podminku, smycka `while` prijima tyto typy argumentu:
  
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
 - `&` - logicke 'AND'
 - `|` - logicke 'OR'
 
A seskupovat s pomoci zavorek `(` a `)`. 
  
**Pozor**, na rozdil od "C", v pominkach smycky `WHILE` neni mozne provadet matematicke vypocty. **Smycka s podminkou** `while(a--)` **tedy selze_!**
  
##### *Rizeni*:
 
Smycky je mozne retezit s pomoci techto operatoru:
 - `continue` - vrati se na zacatek smycky a zacne ji zpracovani
 - `break` - okamzite ukonci zpracovani bloku
  

------------------------------------------------------


##### *Typicky*:

Nekonecna smycka.
*Pozor, nekonecna smycka muze zpusobit 'zamrznuti' programu!*

```
while(1) {
  // do work here ...
}
```

##### *Promenna jako podminka*:

Ukonci se az 'number1' bude mensi nebo rovnat se '0'.

```
int number1 = 10;

while(number1) {
  number1--;
}
```
*nebo*

Ukonci se az 'number1' bude mensi nebo rovnat se '0'.

```
int number1 = 10;

while(number1 > 0) {
  number1--;
}
```

##### *retezeni podminek*:

Ukonci se az 'number1' bude vetsi ne '0' a 'number2' bude mensi nebo rovnat se '5'.

```
int number1 = 15;
int number2 = 10;

while(number1 & (number2 <= 5)) {
  number1--;
  number2--;
}
```

##### *prikaz 'BREAK'*:

Ukonci se az 'number1' bude '0' a zpracuje se prikaz `BREAK`.

```
int number1 = 10;

while(1) {
  if(number1 == 0){
    break;
  }
  number1--;
}
```

##### *prikaz 'CONTINUE'*:

Ukonci se az 'number1' bude '0' a zpracuje se prikaz `BREAK`, do te doby prikaz `CONTINUE` zpusobi skok na zacatek smycky

```
int number1 = 10;

while(1) {
  if(number1 > 0){
    number1--;
    continue;
  }
  break;
}
```

##### *Zanoreni*:

Smycka se zanorenou smyckou

```
while(1) {

// zanorena smycka
  while(1) {
    // do work here ...
    break;
  }
  
  break;
}
```
