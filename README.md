# ANSI escape codes

**ANSI escape sequences** start with an ASCII escape character and a bracket
character.

The ASCII escape character, octal: `\033`, hexadecimal: `\x1b`, decimal: `27`,
also represented as `^[` or `\e` is used to start a control sequence.

💡Tip: `cat -v` is extremely useful for debugging non-printable characters:
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

## Function and Extended key escape sequences
| Key    | Sequence    | Bytes (hex)    |
| ------ | ----------- | -------------- |
| F1     | ESC O P     | 1B 4F 50       |
| F2     | ESC O Q     | 1B 4F 51       |
| F3     | ESC O R     | 1B 4F 52       |
| F4     | ESC O S     | 1B 4F 53       |
| F5     | ESC [ 1 5 ~ | 1B 5B 31 35 7E |
| F6     | ESC [ 1 7 ~ | 1B 5B 31 37 7E |
| F7     | ESC [ 1 8 ~ | 1B 5B 31 38 7E |
| F8     | ESC [ 1 9 ~ | 1B 5B 31 39 7E |
| F9     | ESC [ 2 0 ~ | 1B 5B 32 30 7E |
| F10    | ESC [ 2 1 ~ | 1B 5B 32 31 7E |
| F11    | ESC [ 2 3 ~ | 1B 5B 32 33 7E |
| F12    | ESC [ 2 4 ~ | 1B 5B 32 34 7E |
| ↑      | ESC [ A     | 1B 5B 41       |
| ↓      | ESC [ B     | 1B 5B 42       |
| →      | ESC [ C     | 1B 5B 43       |
| ←      | ESC [ D     | 1B 5B 44       |
| Home   | ESC [ H     | 1B 5B 48       |
| End    | ESC [ F     | 1B 5B 49       |
| Insert | ESC [ 2 ~   | 1B 5B 32 7E    |
| Delete | ESC [ 3 ~   | 1B 5B 33 7E    |
| PgUp   | ESC [ 5 ~   | 1B 5B 35 7E    |
| PgDown | ESC [ 6 ~   | 1B 5B 36 7E    |

## References
- https://en.wikipedia.org/wiki/ANSI_escape_code
- https://en.wikipedia.org/wiki/Escape_character#ASCII_escape_character
- https://github.com/aristocratos/btop
- https://github.com/nicolargo/glances
- https://github.com/ranger/ranger
- https://github.com/jarun/nnn
