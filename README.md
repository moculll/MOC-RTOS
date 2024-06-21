# MOC-RTOS User Guide
- Environment: Ubuntu22.04
## 1. Clone MOC-RTOS
```
git clone https://github.com/moculll/MOC-RTOS.git
cd MOC-RTOS
```
## 2. Get cross compile tools
#### 2.1 decide which board you're going to use

```
vim ./config/boards.cfg


# boards.cfg
BOARDS=[nrf52840dk]
```

or
```
BOARDS=[nrf52840dk, esp32h2]
```

#### 2.2 bootstarp for board dependency
```
./scripts/bootstrap.sh
```

## 3. Compile the project

```
./scripts/build.sh BOARD=nrf52840dk
```

## 4. Flash the executable file into your Board and Enjoy!

```
nrfjprog -f nrf52 --sectorerase --program out/nrf52840dk/nrf52840dk.bin --reset
```



# Contribute
- #### Commit Fomat
```
[date][author](type): changes
```
e.g. 
```
[2024-06-22][mocul](optimize/fix/addFunc): 
1. remove useless files
2. fix build issue
3. add more makefiles to board nrf52840dk
```