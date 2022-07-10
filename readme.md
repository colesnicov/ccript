#CCRipt <sub>*An Embeddable Interpreter for a 'C' Like Scripting Language*</sub>

###### *version* v.1b0


**Beta verze. Nutno testovat. Pripominky jsou vitany!**


**ccript** vznikl jako volnocasovy maraton za snahou uspokojit svou potrebu po prostredku jak s pomoci, textovych a tudyz i snadno upravitelnych, skript upravovat chovani programu a intereagovat s HW vrstvou zarizeni. Byl vyvijen s vedomim ze na poli s *'embedded'* zarizenimi, v dobe vyvoje tohoto nastroje, neni k dispozici nic co by plne vyhovelo mym potrebam.

**ccript** je napsan na cistem 'C' bez vnejsich zavislosti. S moznosti nekterych nastaveni a sadou predpripravenych funkci pro praci s peti datovymi typy.

**ccript** umoznuje pracovat s peti datovymi typy:
 - [int](syntax.numeric.md)
 - [float](syntax.numeric.md)
 - [bool](syntax.bool.md)
 - [char](syntax.char.md)
 - [string](syntax.string.md)
 
 smycky:
 - [while](syntax.while.md)
 
 podminky:
 - [if](syntax.if.md)
 
 a navesovat vlastni 'C' [funkce](syntax.c_functions.md):
 
 definice and registerace 'C' funkce jako `led`
 
 ```
 var_s* cb_Led(parser_s* parser, var_s **_vars, uint8_t _vars_count){
	 ...
 }
 
 cc_registerFunction(&parser, "led", 3, cb_Led);
 ```

volani funkce `led` ve skriptu

```
int pin = 3;
led(pin, 1); // led on
led(pin, 0); // led off
```

#### TODO:
 - `do...while`
 - `for`
 - praci s poli. Napr. `int var[5] = 3;` inicializuje pole s peti prvky typu `int` a priradi jim hodnotu '3'.
 - definici funkci uvnitr skript

