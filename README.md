```asm
#main // package name

@io // imports

[+++++] => x // means x has 5 bits, all on (11111, so, 31)
|x 4 -> - // toggle the 5th bit thereby making 11110, so 30

[++] => y // 3

// Arrays can be formed via
<x, y> => z

// output of the basic unicode characters can be done via
<< z // where z is an array of integers representing the corresponding unicode character 

; if conditions
[+] => TRUE
[-] => FALSE
[---+] => i

(:TRUE ${i = i + 1}) // if TRUE is bigger than 0, loop TRUE times, each time run ${i = i + 1}
// $ run operator in loops
```

```asm
#main

@io

[+++++] => x
|x 4 -> - 

<x, y> => z
<< z

[+] => TRUE
[-] => FALSE
[---+] => i
(:TRUE ${i = i + 1})

[--------] => toggle_cout

&toggle
    |x 4 -> +
    |toggle_cout ^+
.

&print
    |<< toggle_cout
    |<< x
.
    

(:5 ${
      $toggle
      $print
})
```

### Create collections, which is a variable that stores a collection of code that will be run if the variable is used