# CCRipt - Smycka WHILE

[zpet](../readme.md)

CCRipt nabizi opakjici se smycky. K dispozici je smycka `while`. K rizeni stavu smycky jsou tez k dispozici klicova slova `continue` a `break`.



## WHILE 

**ccript** umi jednoduche opakovani za pomoci prikazu `while`.

Smycky muzou byt **zanorene do sebe**, mohou se **predcasne ukoncit** (`break`) a mohou se predcasne **vratit na 'svuj zacatek'** (`continue`).

### Prototyp

```
while(<cond>) {
  // do work here ...
  [continue]
  [break]
}
```



## Zanoreni

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

## Kondicni podminky

Pro praci s kondicni podminkou `while` je pouzita [pravdivostni logika](syntax.logic.md).
 

#### typicky

Nekonecna smycka.
*Pozor, nekonecna smycka muze zpusobit 'zamrznuti' programu!*

```
while(1) {
  // do work here ...
}
```

### Promenna jako podminka

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


## Rizeni opakovani
 
Smycky je mozne retezit s pomoci techto operatoru:
 - `continue` - vrati se na zacatek smycky a zacne ji zpracovani
 - `break` - okamzite ukonci zpracovani bloku
 
### prikaz 'BREAK'

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

### prikaz 'CONTINUE'

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



