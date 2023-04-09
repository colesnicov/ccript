# CCRipt - Vlastni funkcni bloky

[zpet](../readme.md)

**ccript** dovoluje definovat si sve vlastni funkcni bloky primo uvnitr script a nasledne je volat.

## definice 

Funkcni blok muze ale nemusi prijimat argument a vracet nejake vypocty.

definice funkcniho bloku jako `nop` ktery neprijima zadny argument a nic nevraci:

 ```
int nop() {
    int a = 2;
    while(a){
        a--;
    }
}
 ```
 
 
definice funkcniho bloku jako `sum` ktery prijima dva argument a jeden vraci:
 
 ```
int sum(int a, int b) {
	int res = a + b;

	return res;
}
 ```


## volani

volani funkcniho bloku:

```
nop(); // volani funkcniho bloku

int ret = sum(1, 2); // funkcni blok neco vraci tak to ziskame a zpracujeme
if(3 == ret){
    ...
}

```

## Omezeni

**Funkcni blok musi byt prede definovany nez se pouzije!**


