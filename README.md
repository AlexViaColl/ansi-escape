# ANSI escape codes

**ANSI escape sequences** start with an ASCII escape character and a bracket
character.

The ASCII escape character, octal: `\033`, hexadecimal: `\x1b`, decimal: `27`,
also represented as `^[` or `\e` is used to start a control sequence.

Tip: `cat -v` is extremely useful for debugging non-printable characters:
```shell
$ ls --color | cat -v
```

```c
printf("\x1b[%uA", n); // Move the cursor n cells up
printf("\x1b[%uB", n); // Move the cursor n cells down
printf("\x1b[%uC", n); // Move the cursor n cells forward
printf("\x1b[%uD", n); // Move the cursor n cells back

printf("\x1b[%u;%uH", row, col); // Move the cursor to row, col
```

## References
- https://en.wikipedia.org/wiki/ANSI_escape_code
- https://en.wikipedia.org/wiki/Escape_character#ASCII_escape_character
- https://github.com/aristocratos/btop
- https://github.com/nicolargo/glances
- https://github.com/ranger/ranger
- https://github.com/jarun/nnn
