# SynacorVM

* Build:
```sh
make
```

* Run:
```sh
make run
```

* Debug:
```sh
make debug
```

Running `./vm <binary>` in debug mode will create a log file (`synacorVM.log`).

## Download Synacor Challenge

```sh
curl -o synacor-challenge.tgz -XPOST -d "action=signup&email=$RANDOM@example.com&password=x&twitter=&firstname=x&lastname=x" https://challenge.synacor.com/
```

Contains `arch-spec` and a unique `challenge.bin` (in regards to the codes that can be found inside the binary, i.e. playing the game).

## Walkthroughs

* https://github.com/sphynx/c/blob/master/synacor-challenge/walkthrough
* https://github.com/sphynx/c/blob/master/synacor-challenge/walkthrough2
