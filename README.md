# simple text-art player

`stap` *height* [xoffset] [yoffset]

a little c program to play animated text art

## example: ascii starwars

this can play [asciimation](https://www.asciimation.co.nz/)
'ascii starwars', also known from
`telnet towel.blinkenlights.nl`.

simply download the 'run-length encoded' text file from the
website and play it by piping the file into stap:
```sh
make
./stap 13 7 5 <starwars.txt
```

you can also store it compressed and feed it to stap via a
program like zstdcat or xzcat.

an x and y offset of `7` and `5` roughly center it on a
80x24 terminal. if yours is a different size, experiment
with those numbers.

note that we use 13 as the height, despite the website
claiming frames are 14 tall, since we only count the actual
displayed portion

## bugs

- the maximum supported frame delay is 999 (66.6 seconds),
  or 9999 (666.6 seconds) with slightly broken parsing

- the terminal clear sequence is hard-coded rather than
  supporting terminfo. if the included one does not work,
  piping various options of `tput` into `cat -v`, with any
  `^[` replaced with `\033`, may help in finding correct
  values for `CLEAR` `HOME` and `CLEAREOL`

- there is no limit to offset-size, obscenely large offsets
  may cause you to run out of memory

