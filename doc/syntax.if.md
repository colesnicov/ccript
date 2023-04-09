# CCRipt - Podminka IF

[zpet](../readme.md)

CCRipt umi pracovat s podminky. K dispozici je prikaz `if`. Doplneny prikazy pro vetveni `elif` a `else`.

Podminky muzou byt **zanorene do sebe**, mohou se **vetvit do nekolika** (`elif`) **a/nebo** (`else`).


## IF

**ccript** umi jednoduche podminky vetveni programu za pomoci prikazu [IF](https://en.wikipedia.org/wiki/While_loop).


#### Prototyp

```
if(<cond1>) {
  ...
} elif(<cond2>) {
  ...
} elif(<cond3>) {
  ...
} else {
  ...
}
```

## Zanoreni

Podminka se zanorenou podminkou

```
if(cond1) {

// zanorena podminka
  if(cond2) {
    ...
  } else{
    ...
  }
}
```

## Kondicni podminky

Pro praci s kondicni podminkou v prikazech `if` a `elif` je pouzita [pravdivostni logika](syntax.logic.md).



### Promenna jako podminka

```
bool number1 = true;

if(var == true) {
    ...
}
```

*nebo*

```
bool var = true;

if(var) {
  ....
}
```

### retezeni podminek

Podminka se vykona jen v prpade ze 'number1' bude vetsi nez '0' a 'number2' bude mensi nebo rovnat se '5'.

```
int number1 = 15;
int number2 = 10;

if(number1 && (number2 <= 5)) {
  ...
}
```

## Rizeni
 
Smycky je mozne retezit s pomoci techto operatoru:
 - `elif` - Tento blok se provede jen v pripade ze predchozi podminka nebyla splnena a tato podminka nabyva pravdivostni hodnoty.
 - `else` - Tento blok se provede jen v pripade ze zadna z predchozich podminek nebyla splnena.
 

### prikaz 'ELIF'

V pripade ze potrebujeme otestovat vice podminek, treba kdyz promenna muze nabyvat vice nez jen dvou hodnot muzeme pouzit prikaz `elif`.

```
int number1 = 2;

if(number1 == 0){
    ...
} elif(number1 == 1){
    ...
}
} elif(number1 == 2){
    ...
```

### prikaz 'ELSE'

V pripade ze ani jedna z podminek nebude provdivostni a my o tom potrebujeme vedet je prikaz `else`

```
if(cond1){
    ...
} else{
    ...
}
```

nebo
    
```
if(cond1){
    ...
} elif(cond2){
    ...
} elif(cond3){
    ...
} else{
    ...
}
```


