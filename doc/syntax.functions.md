# CCRipt - Vlastni 'C' funkce

[zpet](../readme.md)

**ccript** dovoluje pridavat sve vlastni 'C' funkce ktere budou dostupne ve skriptach.


## definice 

definice a registrace 'C' funkce jako `cb_led`
 
 ```
 var_s* cb_Led(parser_s* parser, var_s **_vars, uint8_t _vars_count){
	int pin = 0;
	int state = 0;
	
	// tady ziskame prvni predany argument
	if(false == VarValueGetInt(_parser, _vars[0], &pin)){
	    // error
	    return NULL;
	}
	
	// tady ziskame druhy predany argument
	if(false == VarValueGetInt(_parser, _vars[1], &state)){
	    // error
	    return NULL;
	}
	
	// tady volame svoje API ...
	bool ret = api_write_pin(pin, state);
	
	// toto je nepovine pokud funkce nevraci nic staci vratit NULL
	// vratime neco. V nasem pripade je to `bool`
	var_s *var = VarCreate("@", 1, CC_TYPE_BOOL, _parser->depth);

	if (var == NULL)
	{
		return NULL;
	}

	if (!VarValueSetBool(_parser, var, ret))
	{
		VarDestroy(var);
		return NULL;
	}

	return var;
 }
 ```
 
Funkce prijima tri parametry

- `parser_s* parser` - *je ukazatel na parser*.
- `var_s **_vars` - *je pole ukazatelu na promenne ktere byli predany funkci pri jejim volani*.
- `uint8_t _vars_count` - *je pocet promennych ktere byli predany funkci pri jejim volani*.

a **vraci ukazatel na promennou** ktera bude pristupna ve skripte nebo NULL.
 
## registrace
 
registrace 'C' funkce `cb_led` jako `led` aby byla pristupna ve skriptach.
 
 ```
 cc_registerFunction(&parser, "led", 3, cb_Led);
 ```

## volani

volani funkce `led` ve skriptu

```
int pin = 3;
led(pin, 1); // led on
led(pin, 0); // led off

bool ret = led(pin, 1); // funkce neco vraci tak to ziskame a zpracujeme
if(true == ret){
    ...
}

```

 

