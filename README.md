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

## Walkthroughs

https://github.com/sphynx/c/blob/master/synacor-challenge/walkthrough
https://github.com/sphynx/c/blob/master/synacor-challenge/walkthrough2
